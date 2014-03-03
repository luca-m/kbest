/**
 * KBest.cpp
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#include <solver/KBest.h>

#if DEBUG == 1 
  #define D(x) x
  #define DEBUG_STDERR(x) std::cerr<<x<<std::endl;
#else 
  #define D(x)
  #define DEBUG_STDERR(x)
#endif

namespace kbest{

  KBestSolver::KBestSolver(){
    this->prob=NULL;
    this->M=NULL;
    this->L=NULL;
  }

  KBestSolver::~KBestSolver(){
    // Cleanup memory once you have finished to use these objects
    this->prob=NULL;
    this->M=NULL;
    this->L=NULL;
    this->fwd_start=0;
    this->fwd_end=0;
    this->bwd_start=0;
    this->bwd_end=0;
  }

  float KBestSolver::getLastForwardTime(){
    return ((float)(fwd_end-fwd_start))/CLOCKS_PER_SEC;
  }

  float KBestSolver::getLastBackwardTime(){
    return ((float)(bwd_end-bwd_start))/CLOCKS_PER_SEC;
  }

  float KBestSolver::getLastTotalTime(){
    return this->getLastForwardTime()+this->getLastBackwardTime();
  }

  SolutionList & KBestSolver::kbest(Problem & prob, int k){

    this->k=k;
    this->prob=&prob;
    this->M=&this->prob->getAssociatedMatrix();
    this->L = new SolutionList(k*2);
    if ( this->prob == NULL || this->M == NULL){
      cerr<<"ERR: Null pointer in kbest function"<<endl;
      throw 40;
    }

    this->fwd_start = clock();
    this->forward();
    this->fwd_end = clock();

    DEBUG_STDERR("kbest: matrix after forward:"<<endl<<*this->M)

    this->bwd_start = clock();
    this->backward();
    this->bwd_end = clock();

    delete this->M;

    if ( this->L == NULL){
      cerr<<"ERR: Solution list is still NULL after kbest calculation"<<endl;
      throw 40;
    }

    return *this->L;
  }

  void KBestSolver::forward(){
    int z=0;

    //for (int c=0; c < this->M->getNCols(); c++){
    //  this->M->set(0,c,0);
    //}

    for (int j=1; j < this->prob->getNVar()+1; j++){
      this->M->set1Based(this->prob->getA1Based(j), j, this->prob->getC1Based(j));
    }

    DEBUG_STDERR("forward: matrix after initialization"<<endl<<*this->M)

    for (int t=this->prob->getA1Based(1); t < this->prob->getCapacity()-this->prob->getA1Based(1)+1 ; t++){

      for (int m=1; m < this->M->getNCols()+1 ; m++){
        if (this->M->get1Based(t, m) >= 0) {      // t is 1-based, m is 1-based
          z=this->M->get1Based(t,m);
        
          for (int i=m; i < this->prob->getNVar()+1; i++ ){
            if (t+this->prob->getA1Based(i) <= this->prob->getCapacity() ){
              if ( this->M->get1Based(t,i) > z ){
                z=this->M->get1Based(t,i);
              }
              this->M->set1Based(t+this->prob->getA1Based(i), i, z+this->prob->getC1Based(i));
            }
          }
          break;
        }
      }

    }
    return;
  }

  void KBestSolver::backward(){
    this->builtInitKBest();
    DEBUG_STDERR("backward: initial solution found"<<endl<<*this->L)
    this->recoverSol();
  }

  void KBestSolver::builtInitKBest(){
    Solution * sol;
    Solution * sol1;
    int counter=0;
    int i=this->prob->getCapacity()+1;
    int j=0;
    int i1=0;
    int j1=0;
    int p1=0;
    bool fim=false;
    bool moreleft=false;
    
    while (i > this->prob->getA1Based(1)+1){
      i--;                        // i is 1-based
      j=this->prob->getNVar()+1;
      while (j>1) {
        j--;                      // j is 1-based

        if (this->M->get1Based(i,j) >= 0){

          DEBUG_STDERR("builtInitKBest: (L) i="<<i<<", j="<<j<<", counter="<<counter<<", k="<<this->k)
          sol = new Solution(*this->prob);
          sol->setV(this->M->get1Based(i,j));
          sol->setT(i);
          sol->setJ(j);
          if (!this->L->isIn(*sol)) {
            DEBUG_STDERR("builtInitKBest: (L) found solution "<<*sol<<"")
            counter++;
            this->L->addSolution(*sol);
          }else{
            DEBUG_STDERR("builtInitKBest: (L) found duplicate solution "<<*sol<<"")
            delete sol;
          }

          if (counter==this->k){
            i1=i; j1=j;
            moreleft=true;
            i=0; j=0;
          }
        }

      }
    }

    this->p=counter;
    //DEBUG_STDERR("builtInitKBest: L:\n"<<*this->L<<"")
    this->L->sortNonIncreasing();
    //DEBUG_STDERR("builtInitKBest: sorted L:\n"<<*this->L<<"")

    if (this->p==this->k && (i1>this->prob->getA(0) || j1>1)){
      fim=true;
    }

    while (fim){
      SolutionList L1 = SolutionList(this->k);
      counter=0;
      i=i1+1;
      fim=false;
      while (i>this->prob->getA1Based(1)){
        i--;                          // i is 1-based
        j=this->prob->getNVar()+1;
        if (moreleft){
          j=j1; moreleft=false;
        }
        while (j>1){
          j--;                        // j is 1-based
          
          if (this->M->get1Based(i,j) > this->L->get1Based(this->p).getV()) {    // index of L is 1-based

            DEBUG_STDERR("builtInitKBest: (L1) i="<<i<<", j="<<j<<", counter="<<counter<<", k="<<this->k)
            sol = new Solution(*this->prob);
            sol->setV(this->M->get1Based(i,j));
            sol->setT(i);
            sol->setJ(j);
            if (!this->L->isIn(*sol) && !L1.isIn(*sol)){
              DEBUG_STDERR("builtInitKBest: (L1) found solution "<<*sol<<"")
              L1.addSolution(*sol);
              counter++;
            }else{
              DEBUG_STDERR("builtInitKBest: (L1) duplicate solution "<<*sol<<"")
              delete sol;
            }
            if (counter==this->k){
              i1=i; j1=j;
              moreleft=true;
              i=0; j=0;
            }
          }
        }
      }
      p1=counter;
      //DEBUG_STDERR("builtInitKBest: L1:\n"<<L1)
      L1.sortNonIncreasing();
      //DEBUG_STDERR("builtInitKBest: sorted L1:\n"<<L1)
      
      if (L1.size()>0 && L1.get1Based(1).getV() > this->L->get1Based(this->k).getV()){
        this->L->merge(L1, this->k);
        if (i1 > this->prob->getA1Based(1) || j1 > 1){
          fim=true;
        }
      }
      L1.~SolutionList();
    }
  }

  void KBestSolver::recoverSol(){
    int i=0;
    while (i < this->p){
      i++;
      if (!this->L->get1Based(i).getC()) {
        this->backtracking(this->L->get1Based(i), i, true);
      }
    }
  }

  void KBestSolver::backtracking(Solution & current_sol, int sol_index, bool alternative){
    int t=current_sol.getT();    // t and j have been setted during builtInitKBest
    int j=current_sol.getJ();    // so, these indexes are 1-based.
    int j1=j;
    int z=current_sol.getV();
    int zcum=0;

    //DEBUG_STDERR("backtracking: sol_index="<<sol_index<<", current_sol="<<current_sol)
    DEBUG_STDERR("backtracking: current solution t="<<t<<", j="<<j<<", z="<<z<<", M[t,j]="<<this->M->get1Based(t, j))
      

    while (t>0){
      t-=this->prob->getA1Based(j);
      z-=this->prob->getC1Based(j);
      zcum+=this->prob->getC1Based(j);
      current_sol.incDecisionVar1Based(j);
      DEBUG_STDERR("backtracking current_sol: j="<<j<<", current_sol.X="<<current_sol.getDecisionVars())
      
      if ( z<=0 ) {
        break;
      }
      
      // j = {s: M[t,s] = z , 1 <= s <= j}
      for (int s=1; s < j+1; s++){
        if (this->M->get1Based(t,s)==z){
          j=s;
          DEBUG_STDERR("backtracking sol: z="<<z<<" found at "<<j<<" in M["<<t<<",:"<<j<<"]=")
          break;
        }
      }
      
      if (t>0 && alternative){
        this->searchAltSol(t, j, zcum, j1, sol_index);
      }
      j1=j;
    }
    current_sol.setC(true);
  }

  void KBestSolver::searchAltSol(int t, int j, int zcum, int j1, int sol_index){
    Solution * alt_sol;
    Solution * current_sol;
    // t, j j1 are 1-based
    for (int s=1; s < j1+1; s++){
      if (s!=j){
        if (this->M->get1Based(t,s) >= 0) {
          if (this->M->get1Based(t,s)+zcum >= this->L->get1Based(this->p).getV()) {
            // Determine position g: g>i to insert this alternative 
            // solution in ordered list L
            alt_sol= new Solution(*this->prob);
            alt_sol->setV(this->M->get1Based(t,s)+zcum);
            alt_sol->setT(t);
            alt_sol->setJ(s);

            int g=this->L->getInsertionIndex1Based(*alt_sol, sol_index);     // old: findInsertionIndex(this->L, alt_sol, startfrom=sol_index)
            this->L->insertAt1Based(g, *alt_sol);    // alternative solution is in pos g, now compute its value.

            //DEBUG_STDERR("searchAltSol: g="<<g<<", alt_sol="<<*alt_sol)

            if (this->p < this->k){
              this->p++;
            }
            
            current_sol = new Solution(*this->prob);
            current_sol->setV(this->M->get1Based(t,s));
            current_sol->setJ(s);
            current_sol->setT(t);

            this->backtracking(*current_sol, g, false);

            this->L->get1Based(g).setC(true);
            vector<int> dvar1= this->L->get1Based(sol_index).getDecisionVars();
            vector<int> dvar2= current_sol->getDecisionVars();
            this->L->get1Based(g).setDecisionVars(dvar1 + dvar2);
            
            DEBUG_STDERR("searchAltSol:  alternative sol!"<<endl<<"  current_sol="<<*current_sol<<endl<<"  L[sol_index="<<sol_index<<"]="<<this->L->get1Based(sol_index)<<endl<<"  L[g="<<g<<"]="<<this->L->get1Based(g))

            if (this->M->get1Based(t,s) >= this->L->get1Based(this->p).getV()) {
              // Determine position kk : kk>g to insert this alternative 
              // solution in ordered list L
              int kk = this->L->getInsertionIndex1Based(*current_sol, g); //Solution.findInsertionIndex(this->L, current_sol, startfrom=g)
              if (this->p < this->k){
                this->p++;
              }
              this->L->insertAt1Based(kk, *current_sol);
            }
          }
        }
      }
    }
  }

}

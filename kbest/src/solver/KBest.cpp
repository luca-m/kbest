/**
 * KBest.cpp
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#include "KBest.h"


namespace kbest{

  KBestSolver::KBestSolver(){
    this->matrix=NULL;
    this->prob=NULL;
  }

  KBestSolver::~KBestSolver(){
    this->matrix.~Matrix();
    this->L.~SolutionList();
  }

  SolutionList & KBestSolver::kbest(Problem & prob, int k){
    this->k=k;
    this->prob=prob;
    this->M=prob.getAssociatedMatrix();

    // TODO: time
    this->forward();
    // TODO: time

    // TODO: time
    this->backtrack()
    // TODO: time

    return this->L;
  }

  void KBestSolver::forward(){
    for (int c=0; c < this->M.getNCols(); c++){
      this->M.set(0,c,0);
    }

    for (int c=1; c < this->prob.getNVar()+1; c++){
      this->M.set1Based(this->prob.getA1Based(c), c, this->prob.getC1Based(c));
    }

    for (int t=this->prob.getA1(0); t < this->prob.getCapacity()-this->prob.getA1(0)+1 ; t++){
      
      for (int m=0; m < this->M.getNCols() ; m++){
        if (this->M.get(t-1, m) >=0) {      // t is 1-based, m is 0-based
          int z=this->M.get(t-1,m);
        
          for (int i=m; i < this->prob.getNVar(); i++ ){
            if (t+this->prob.getA(i) <= this->prob.getCapacity() ){
              if ( this->M.get(t-1,i) > z ){
                z=this->M.get(t-1,i);
              }
              this->M.set(t+this->prob.getA(i)-1, i) = z+this->prob.getC(i);
            }
          }

        }
      }

    }
    return;
  }

  void KBestSolver::backward(){
    this->builtInitKBest();
    this->recoverSol();
  }

  void KBestSolver::builtInitKBest(){
    int counter=0;
    int i=this->prob.getCapacity()+1;
    int j=0;
    int i1=0;
    int j1=0;
    int p1=0;
    bool fim=false;
    bool moreleft=false;
    
    while (i > this->prob.getA(0)){
      i--;                        // i is 1-based
      j=this->prob.getNVar()+1;
      while (j>1) {
        j-=1                      // j is 1-based
        if (this->M.get1Based(i,j) >= 0){
          
          sol=Solution(this->prob);
          sol.V=this->M.get1Based(i,j);
          sol.J=j
          sol.T=i
          if (!this->L.isIn(sol)) {
            counter++;
            this->L.addSolution(sol);
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
    this->L.sortNonIncreasing();
    
    if (this->p==this->k && (i1>this->prob.getA(0) || j1>1)){
      fim=true;
    }

    while (fim){
      SolutionList L1=SolutionList();
      counter=0;
      i=i1+1;
      fim=false;
      while (i>this->prob.getA(0)){
        i--;                          // i is 1-based
        j=this->prob.getNVar()+1;
        if (moreleft){
          j=j1; moreleft=false;
        }
        while (j>1){
          j--;                        // j is 1-based
          if (this->M.get1Based(i,j) > this->L.get1Based(this->p).V) {    // index of L is 1-based
            
            sol=Solution(this->prob);
            sol.V=this->M.get1Based(i,j);
            sol.J=j;
            sol.T=i;
            if !this->L.isIn(sol) && !L1.isIn(sol){
              L1.addSolution(sol);
              counter++;
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
      L1.sortNonIncreasing();

      if (L1.size()>0 && L1.get(0).V > this->L.get1Based(this->k).V){
        L.merge(L1);
        if (i1 > this->prob.getA(0) || j1 > 1){
          fim=true;
        }
      }
    }

  }

  void KBestSolver::recoverSol(){
    int i=0;
    while (i < this->p){
      i++;
      if (!this->L.get1Based(i).C) {
        this->backtracking(this->L.get1Based(i), i, true);
      }
    }
  }

  void KBestSolver::backtracking(Solution & current_sol, int sol_index, bool alternative){
    int t=current_sol.T;    // t and j have been setted during builtInitKBest
    int j=current_sol.J;    // so, these indexes are 1-based.
    int j1=j;
    int z=current_sol.V;
    int zcum=0;
    while (t>0){
      t-=this->prob.getA1Based(j);
      z-=this->prob.getC1Based(j);
      zcum+=this->prob.getC1Based(j);
      current_sol.X.at(j)+=1;

      if (z==0) {
        break;
      }
      
      // j = {s: M[t,s] = z , 1 <= s <= j}
      for (int s=1, s < j+1; s++){
        if (this->M.get1Based(t,s)==z){
          j=s;
        }
      }
      if (t>0 && alternative){
        this->searchAltSol(t, j, zcum, j1, sol_index)
      }
      j1=j;
    }
    current_sol.C=true;
  }

  void KBestSolver::searchAltSol(int t, int j, int zcum, int j1, int sol_index){
    // t, j j1 are 1-based
    for (int s=1; s < j1+1; s++){
      if (s!=j){
        if (this->M.get1Based(t,s) >= 0) {
          if (this->M.get1Based(t,s)+zcum >= this->L.get1Based(this->p).V) {
            // Determine position g: g>i to insert this alternative 
            // solution in ordered list L
            Solution alt_sol=Solution(this->prob);
            alt_sol.V=this->M.get1Based(t,s)+zcum;
            alt_sol.T=t;
            alt_sol.J=s;

            int g=this->L.getInsertionIndex(alt_sol);     // old: findInsertionIndex(this->L, alt_sol, startfrom=sol_index)
            this->L.insertAt(g, alt_sol);    // alternative solution is in pos g, now compute its value.

            if (this->p < this->k){
              this->p++;
            }
            
            Solution current_sol=Solution(this->prob);
            current_sol.V=this->M.get1Based(t,s);
            current_sol.J=s;
            current_sol.T=t;

            this->backtracking(current_sol, g, false);
            this->L.get1Based(g).C=true;
            this->L.get1Based(g).setDecisionVars(this->L.get1Based(sol_index).getDecisionVars() + current_sol.getDecisionVars());
            
            if (this->M.get1Based(t,s) >= this->L.get1Based(this->p).V) {
              // Determine position kk : kk>g to insert this alternative 
              // solution in ordered list L
              int kk = this->L.getInsertionIndex(current_sol); //Solution.findInsertionIndex(this->L, current_sol, startfrom=g)
              if (this->p < this->k){
                this->p++;
              }
              this->L.insertAt(kk, current_sol);
            }
          }
        }
      }
    }
  }

}

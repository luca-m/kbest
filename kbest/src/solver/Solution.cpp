/**
 * Solution.cpp
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#include <solver/Solution.h>

#if DEBUG == 1 
  #define D(x) x
  #define DEBUG_STDERR(x) std::cerr<<x<<std::endl;
#else 
  #define D(x)
  #define DEBUG_STDERR(x)
#endif

namespace kbest{

  Solution::Solution(kbest::Problem & problem){
    this->C=false;
    this->T=0;
    this->J=0;
    this->V=0;
    this->X=std::vector<int>(problem.getNVar(),0);
  }
  
  Solution::~Solution(){
    this->X.clear();
  }

  void Solution::setDecisionVars(std::vector<int> dvars){
    if (this->X.size() != dvars.size()){
      cerr<<"ERR: Cannot set decision vars, seems to belong to another problem (dvars="<<dvars<<")!!"<<endl;
      throw 50;
    }
    for(unsigned int i=0;i<this->X.size();i++){
      this->X.at(i)=dvars.at(i);
    }
  }

  std::vector<int> & Solution::getDecisionVars(){
    return this->X;
    //std::vector<int> dvars(this->X.size());
    //for (int i=0; i < this->X.size() ; i++){
    //  dvars.push_back(this->X.at(i));
    //}
    //return dvars;
  }
    /**
    *
    */
    void Solution::addDecisionVars(std::vector<int> dvars){
      cerr<<"ERR: addDecisionVars not yet implemented!"<<endl;
      throw 99;
    }
    /**
    *
    */
    void Solution::incDecisionVar(int j){
     if ( j >= 0 && j < this->X.size()){
        this->X.at(j)++;
      } else {
        cerr<<"ERR: Cannot inc decision vars, out of range (j="<<j<<",nvars="<<this->X.size()<<")!!"<<endl;
        throw 50;
      }
    }
    /**
    *
    */
    void Solution::incDecisionVar1Based(int j){
      this->incDecisionVar(j-1);
    }


  void Solution::setV(int v){this->V=v;}
  
  int Solution::getV(){return this->V;}

  void Solution::setJ(int j) {this->J=j;}
  
  int Solution::getJ(){return this->J;}

  void Solution::setT(int t){this->T=t;}
  
  int Solution::getT(){return this->T;}

  void Solution::setC(bool c){this->C=c;}

  bool Solution::getC(){return this->C;}


  SolutionList::SolutionList(){
    this->slist = new std::vector<Solution>();
    this->slist->reserve(100);
  }

  SolutionList::SolutionList(int estimatedElements){
    this->slist = new std::vector<Solution>();
    this->slist->reserve(estimatedElements);
  }


  SolutionList::~SolutionList(){
    if (this->slist!=NULL){
      this->slist->clear();
      //delete this->slist;
    }
  }

  void SolutionList::sortNonIncreasing(){
    std::sort(this->slist->begin(), this->slist->end());
    std::reverse(this->slist->begin(), this->slist->end());
  }

  void SolutionList::sortNonIncreasing(int begin, int end){
    std::sort(this->slist->begin()+begin, this->slist->begin()+end);
    std::reverse(this->slist->begin()+begin, this->slist->begin()+end);
  }

  Solution & SolutionList::get(int i){
    if (i>=0 && i<this->slist->size()){
      return this->slist->at(i);
    } else {
      cerr<<"ERR: variable index out of valid range: i="<<i<<endl;
      throw 10;
    }
  }

  Solution & SolutionList::get1Based(int i){
    return this->get(i-1);
  }

  Solution & SolutionList::getLast(){
    return this->get1Based(this->size());
  } 

  bool SolutionList::isIn(Solution & sol){
    if (this->slist==NULL){
      cerr<<"ERR: solution list is NULL !"<<endl;
      throw 40;
    }
    return std::find( this->slist->begin(), this->slist->end(), sol) != this->slist->end();
  }

  void SolutionList::addSolution(Solution & sol){
    if (this->slist==NULL){
      cerr<<"ERR: solution list is NULL !"<<endl;
      throw 40;
    }
    this->slist->push_back(sol);
  }

  void SolutionList::insertSorted(Solution & sol){
    this->insertAt(this->getInsertionIndex(sol, 0), sol);
  }

  void SolutionList::insertAt(int i, Solution & sol){
    if (this->slist==NULL){
      cerr<<"ERR: solution list is NULL !"<<endl;
      throw 40;
    }
    DEBUG_STDERR("SolutionList insert at i="<<i<<" of "<<this->slist->size())
    std::vector<Solution>::iterator it;
    it = this->slist->begin() + i;
    it = this->slist->insert(it, sol);
  }

  void SolutionList::insertAt1Based(int i, Solution & sol){
    this->insertAt(i-1, sol);
  }

  int SolutionList::getInsertionIndex(Solution & sol, int startFrom){
    if (this->slist==NULL){
      cerr<<"ERR: solution list is NULL !"<<endl;
      throw 40;
    }
    DEBUG_STDERR("SolutionList finding insertion index ");
    int mid=-1;
    int lo=0;
    int hi=this->size();
    
    if (startFrom>0 && startFrom < hi){
      lo=startFrom;
    }
    
    while (lo < hi) {
        mid=(lo+hi)/2;
        if (sol.getV() > this->slist->at(mid).getV()) { 
          hi=mid;
        } else {
          lo=mid+1; 
        } 
    }
    return lo;
  }

  int SolutionList::getInsertionIndex1Based(Solution & sol, int startFrom){
    return this->getInsertionIndex(sol, startFrom -1) +1;
  }


  void SolutionList::merge(SolutionList & solList, int maxElements){
    int x=0;
    int y=0; 
    int z=0;
    DEBUG_STDERR("SolutionList merge");
    while ( x<this->size() && y<solList.size() && z<maxElements){
      if ( this->get(x).getV() > solList.get(y).getV() ){
        x++;
      } else {
        this->insertAt(x, solList.get(y));
        y++;
      }
      z++;
    }
    while (y<solList.size() && z<maxElements){
      this->addSolution(solList.get(y));
      z++;
      y++;
    }
    DEBUG_STDERR("SolutionList end of merge");
  }

  int SolutionList::size(){
    if (this->slist==NULL){
      cerr<<"ERR: solution list is NULL !"<<endl;
      throw 40;
    }
    return this->slist->size();
  }



  bool operator < (const Solution & sol0, const Solution & sol){
      return sol0.V < sol.V;
  }

  bool operator == (const Solution & sol0, const Solution sol){
      return sol0.V==sol.V && sol0.J==sol.J && sol0.T==sol.T;
  }

  ostream & operator <<(ostream & os, Solution & sol){
      os<<"{ V="<<sol.getV()<<", J="<<sol.getJ()<<", T="<<sol.getT()<<", C="<<sol.getC()<<", X=[ ";
      for (int i=0; i < sol.X.size();i++){
        if( sol.X.at(i)>0 ){
          os<<"x"<<i+1<<"="<<sol.X.at(i)<<", ";
        }
      }
      os<<"]}";
      return os;
  }

  ostream & operator << (ostream & os, SolutionList & sl){
      os<<"[ ";
      for (int i=0; i<sl.size(); i++){
        os<<sl.get(i);
        if (i<sl.size()-1){
          os<<", "<<endl;
        }
      }
      os<<" ]";
     return os;
  }

  std::vector<int> operator + ( std::vector<int> & v1, std::vector<int> & v2 ){  
    if (v1.size()!= v2.size()) {
      cerr<<"ERR: Cannot sum two vecto of different size!"<<endl;
      throw 40;
    }
    int sz=v1.size();
    std::vector<int> result= std::vector<int>(sz);
    for (unsigned int i = 0; i<sz; i++){
      result[i]=v1[i]+v2[i];
    }
    return result;
  }


}


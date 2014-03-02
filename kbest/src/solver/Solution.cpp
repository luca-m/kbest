/**
 * Solution.cpp
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#include "Solution.h"

#include <algorithm>

namespace kbest{

  Solution::Solution(kbest::Problem & problem){
    this->C=false;
    this->T=0;
    this->J=0;
    this->V=0;
    this->X=std::vector<int>(problem.nvars,0);
  }
  
  Solution::~Solution(){
    this->X.clear();
  }

  void Solution::setDecisionVars(std::vector<int> & dvars){
    if (this->X.size() != dvars.size()){
      cerr<<"ERR: Cannot set decision vars, seems to belong to another problem (dvars="<<dvars<<")!!"<<endl;
      throw 50;
    }
    for(unsigned int i=0;i<this->X.size();i++){
      this.X.at(i)=dvars.at(i);
    }
  }

  SolutionList::SolutionList(){
    this->slist.reserve(100);
  }

  SolutionList::SolutionList(int estimatedElements){
    this->slist.reserve(estimatedElements);
  }


  SolutionList::~SolutionList(){
    this->slist.clear();
  }

  void SolutionList::sortNonIncreasing(){
    std::sort(this->slist.begin(), this->slist.end());
    std::reverse(this->slist.begin(), this->slist.end());
  }

  void SolutionList::sortNonIncreasing(int begin, int end){
    std::sort(this->slist.begin()+begin, this->slist.begin()+end);
    std::reverse(this->slist.begin()+begin, this->slist.begin()+end);
  }

  Solution & SolutionList::get(int i){
    if (i>=0 && i<this->slist.size()){
      return this->slist.at(i);
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
    return std::find(this->slist.begin(), this->slist.end(), sol) != this->slist.end();
  }

  void SolutionList::addSolution(Solution & sol){
    this->slist.push_back(sol);
  }

  void SolutionList::insertSorted(Solution & sol){
    this->insertAt(this->getInsertionIndex(sol), sol);
  }

  void SolutionList::insertAt(int i, Solution & sol){
    std::vector<Solution &>::iterator it;
    it = this->slist.begin() + i;
    it = this->slist.insert(it, sol);
  }

  void SolutionList::insertAt1Based(int i, Solution & sol){
    this->insertAt(i-1, sol);
  }

  int SolutionList::getInsertionIndex(Solution & sol){
    int mid=-1
    int lo = 0 ;
    int hi = this->size();
    while (lo < hi) {
        mid=(lo+hi)/2;
        if (sol.V > this->slist.at(mid).V) { 
          hi=mid;
        } else {
          lo=mid+1; 
        } 
    }
    return lo;
  }

  void SolutionList::merge(SolutionList & solList, int maxElements){
    int x=0;
    int y=0; 
    int z=0;
    while ( x<this->size() && y<solList.size() and z<maxElements){
      if ( this->get(x).V > solList.get(y).V ){
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
  }

  int SolutionList::size(){
    return this->slist.size();
  }


}

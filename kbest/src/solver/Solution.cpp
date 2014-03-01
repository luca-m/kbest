/**
 * Solution.cpp
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#include "Solution.h"

namespace kbest{

  Solution::Solution(kbest::Problem & problem){
    this->C=false;
    this->T=0;
    this->J=0;
    this->V=0;
    this->X=std::vector(problem.nvars,0);
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
      this.X.at(i)=dvars.at(i);
    }
  }

}


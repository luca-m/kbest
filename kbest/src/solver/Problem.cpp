/**
 * Problem.cpp
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#include "Problem.h"


namespace kbest{


  Problem::Problem(int nvar, int b){
  
  }
  Problem::Problem(std::string filePath){
  
  }
  ~Problem::Problem(){
  
  }
  
  void Problem::addVariable(int i, int cost, int weight);
  void Problem::addVariable1Based(int i, int cost, int weight);
  int Problem::getC(int i);
  int Problem::getC1Based(int i);
  int Problem::getA(int i);
  int Problem::getA1Based(int i);
  int Problem::getCapacity();
  
  kbest::Matrix Problem::getAssociatedMatrix(){
    kbest::Matrix * mtx = new kbest::Matrix(this->b+1, this->nvar);
    return *mtx;
  }

}


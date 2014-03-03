/**
 * Problem.cpp
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#include "Problem.h"

using namespace std;

namespace kbest{

  Problem::Problem(){
      this->b=15;
      this->nvar=5;
      this->vars=std::vector<std::pair<int,int> >(this->nvar,std::make_pair(std::numeric_limits<int>::max(),std::numeric_limits<int>::max()));
      this->setVariable(0,4,3);
      this->setVariable(1,3,4);
      this->setVariable(2,5,5);
      this->setVariable(3,7,6);
      this->setVariable(4,8,7);
    }


  Problem::Problem(int nvar, int b){
    this->b=b;
    this->nvar=nvar;
    this->vars=std::vector<std::pair<int,int> >(this->nvar,std::make_pair(std::numeric_limits<int>::max(),std::numeric_limits<int>::max()));
  }

  Problem::Problem(std::string filePath){
    std::ifstream infile(filePath.c_str());
    std::string line;
    this->nvar=0;
    this->b=0;
    int dummyVarNum=0;

    if (std::getline(infile, line)){
      std::istringstream iss_numvar(line);
      iss_numvar>>this->nvar;
      this->vars=std::vector<std::pair<int,int> > (this->nvar, std::make_pair(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()));
      int nread=0;
      while (std::getline(infile, line) && nread<this->nvar){
          std::istringstream iss(line);
          if (!(iss >> dummyVarNum >> this->vars.at(nread).second >> this->vars.at(nread).first)) { 
            cerr<<"ERR: Cannot parse var cost and weight (line="<<nread<<")!!"<<endl;
            throw 30;
          }
          nread++;
      }
      if (std::getline(infile, line)){
        std::istringstream iss_capacity(line);
        iss_capacity>>this->b;
      } else {
        cerr<<"ERR: Cannot parse problem capacity !!"<<endl;
        throw 30;
      }
    } else{
      cerr<<"ERR: Cannot parse problem number of variables !!"<<endl;
      throw 30;
    }
    std::sort(this->vars.begin(),this->vars.end());
  }
  
  Problem::~Problem(){
    this->vars.clear();
  }
  
  void Problem::setVariable(int i, int cost, int weight){
    if (i >= 0 && i < this->vars.size() && i < this->vars.size()){
      if (cost < 0 || weight < 0){
        cerr<<"ERR: Cost or Weight of the variable are < 0 !! cost="<<cost<<", weight="<<weight<<endl;
        throw 20;
      }
      this->vars.at(i).second=cost;
      this->vars.at(i).first=weight;
    } else {
      cerr<<"ERR: variable index out of valid range: i="<<i<<endl;
      throw 10;
    }    
  }

  void Problem::setVariable1Based(int i, int cost, int weight){
    this->setVariable(i-1, cost, weight);
  }
  
  int Problem::getC(int i){
    if (i >= 0 && i < this->vars.size()){
      return this->vars.at(i).second;
    } else {
      cerr<<"ERR: variable index out of valid range: i="<<i<<endl;
      throw 10;
    }
  }
  
  int Problem::getC1Based(int i){
    return this->getC(i-1);
  }
  
  int Problem::getA(int i){
    if (i >= 0 && i < this->vars.size()){
      return this->vars.at(i).first;
    } else {
      cerr<<"ERR: variable index out of valid range: i="<<i<<endl;
      throw 10;
    }
  }
  
  int Problem::getA1Based(int i){
    return this->getA(i-1);
  }
  int Problem::getNVar(){
    return this->nvar;
  }
  int Problem::getCapacity(){
    return this->b;
  }

  int Problem::setCapacity(int capacity){
    if (capacity < 0){
      cerr<<"ERR: capacity out of valid range: b="<<capacity<<endl;
      throw 20;
    }
    this->b=capacity;
  }

  kbest::Matrix & Problem::getAssociatedMatrix(){
    kbest::Matrix * mtx = new kbest::Matrix(this->b, this->nvar);
    return *mtx;
  }

  ostream & operator << (ostream & os, Problem & prob){
      os<<"{ nvar="<<prob.getNVar()<<", b="<<prob.getCapacity()<<" }";
      return os;
  }

}


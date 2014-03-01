/**
 * Matrix.cpp
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#include "Matrix.h"

#include <iostream>

using namespace std;

namespace kbest{
      
  Matrix::Matrix(int rows, int cols){
    this->mtx= * new int[rows * cols];
    this->rows=rows;
    this->cols=cols;
  }

  Matrix::~Matrix(){
    delete mtx;
  }
      
  int Matrix::getNRows(){
    return this->rows;
  }

  int Matrix::getNCols(){
    return this->cols;
  }

  int Matrix::get(int r, int c){
    if (r>=0 && r<this.rows && c>=0 && c< this->cols){
      return this->mtx[this->rows * r + c];
    } else {
      cerr<<"ERR: matrix index out of valid range: r="<<r<<",c="<<c<<endl;
      throw 10;
    }
  }
      
  int Matrix::get1Based(int r, int c){
    return this->get(r-1, c-1); 
  }
  
  int Matrix::set(int r, int c, int value){
    if (r>=0 && r<this.rows && c>=0 && c< this->cols){
      this->mtx[this->rows * r + c] = value;
    } else {
      cerr<<"ERR: matrix index out of valid range: r="<<r<<",c="<<c<<endl;
      throw 10;
    }
  }
      
  int Matrix::set1Based(int r, int c, int value){
    return this->set(r-1, c-1, value); 
  }

  std::vector<int> Matrix::getRow(int r){
    std::vector<int> row(this->cols,-1);
    for (int j=0; j<this->cols; j++){
      row.at(j)=this->get(r,j);
    }
    return row;
  }
  
  std::vector<int> Matrix::getRow1Based(int r){
    return this->getRow(r-1);
  }

}


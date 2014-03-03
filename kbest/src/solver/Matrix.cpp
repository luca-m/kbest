/**
 * Matrix.cpp
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#include <solver/Matrix.h>

#if DEBUG == 1 
  #define D(x) x
  #define DEBUG_STDERR(x) std::cerr<<x<<std::endl;
#else 
  #define D(x)
  #define DEBUG_STDERR(x)
#endif

using namespace std;

namespace kbest{
  
  Matrix::Matrix(){
    this->mtx=NULL;
    this->rows=0;
    this->cols=0;
  }

  Matrix::Matrix(int rows, int cols){
    if (rows>0 && cols>0 ){
      this->mtx = new int[rows*cols];
      this->rows=rows;
      this->cols=cols;
      for (int i=0;i<rows*cols;i++){
        this->mtx[i]=-1;
      }
    }else{
      cerr<<"ERR: matrix rows and cols must be >0: rows="<<rows<<",cols="<<cols<<endl;
      throw 10;
    }
  }

  Matrix::~Matrix(){
    if (mtx != NULL){
      delete mtx;
    }
  }
      
  int Matrix::getNRows(){
    return this->rows;
  }

  int Matrix::getNCols(){
    return this->cols;
  }

  int Matrix::get(int r, int c){
    //DEBUG_STDERR("matrix: r "<<r<<", c "<<c)
    if (r>=0 && r < this->rows && c>=0 && c < this->cols){
      return this->mtx[this->cols * r + c];
    } else {
      cerr<<"ERR: matrix index out of valid range: r="<<r<<",c="<<c<<endl;
      throw 10;
    }
  }
      
  int Matrix::get1Based(int r, int c){
    return this->get(r-1, c-1); 
  }
  
  int Matrix::set(int r, int c, int value){
    if (r>=0 && r<this->rows && c>=0 && c< this->cols){
      this->mtx[this->cols * r + c] = value;
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

  ostream & operator << (ostream & os,  std::vector<int> & vec){
      for(int i=0; i < vec.size(); i++){
        os<<vec.at(i);
        if (i!=vec.size()-1){
          os<<", ";
        }
      }
      return os;
  }

  ostream & operator << (ostream & os,  kbest::Matrix & mtx){
    int rows=mtx.getNRows();
      for(int r=0; r < rows; r++){
        vector<int> v = mtx.getRow(r);
        os<<"r="<<r<<" [ "<<v<<" ]"<<endl;
      }
      return os;
  }

}


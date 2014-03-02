/**
 * Matrix.h
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>

using namespace std;

namespace kbest{


  class Matrix{
    public:

      Matrix();

      Matrix(int rows, int cols);

      ~Matrix();
      /**
       * Returns the number of rows
       * */
      int getNRows();
      /**
       * Returns the numbers of coloumns
       * */
      int getNCols();
      /**
       * Retrieve an element using 0-based index
       * @param row index
       * @param col index
       * */
      int get(int r, int c);
      /**
       * Retrieve an element using 1-based index
       * @param row index
       * @param col index
       * */
      int get1Based(int r, int c);
      /**
       * Set an element using 0-based index
       * @param row index
       * @param col index
       * */
      int set(int r, int c, int value);
      /**
       * Set an element using 1-based index
       * @param row index
       * @param col index
       * */
      int set1Based(int r, int c, int value);

      /**
       * Retrieve a row using 0-based index
       * @param row index 
       * */ 
      std::vector<int> getRow(int r);
      /**
       * Retrieve a row using 1-based index
       * @param row index 
       * */ 
      std::vector<int> getRow1Based(int r);

    private:
      /// The matrix
      int mtx *;
      /// Number of rows
      int rows;
      /// Number of coloumns
      int cols;

  };

  /**
  * Pretty printing for Matrix
  */
  ostream & operator << (ostream & os, const kbest::Matrix & mtx){
      for(int r=0; r<mtx.getNRows(); r++){
        os<<"r="<<r<<" [ "<<mtx.getRow(r)<<" ]"<<endl;
      }
      return os;
  }

}

#endif /* !MATRIX_H */


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
      /**
       * Create a void matrix
       * */
      Matrix();
      /**
       * Create a matrix
       * @param rows number of rows
       * @param cols number of coloumns
       * */
      Matrix(int rows, int cols);
      /**
      **/
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
       * @param r row index
       * @param c col index
       * */
      int get(int r, int c);
      /**
       * Retrieve an element using 1-based index
       * @param r row index
       * @param c col index
       * */
      int get1Based(int r, int c);
      /**
       * Set an element using 0-based index
       * @param r Row index
       * @param c Coloumn index
       * @param value Value to set
       * */
      int set(int r, int c, int value);
      /**
       * Set an element using 1-based index
       * @param r Row index
       * @param c Coloumn index
       * @param value Value to set
       * */
      int set1Based(int r, int c, int value);

      /**
       * Retrieve a row using 0-based index
       * @param r Row index 
       * */ 
      std::vector<int> getRow(int r);
      /**
       * Retrieve a row using 1-based index
       * @param r Row index 
       * */ 
      std::vector<int> getRow1Based(int r);

    private:
      /// The matrix
      int * mtx;
      /// Number of rows
      int rows;
      /// Number of coloumns
      int cols;

  };

  /**
  * Pretty printing for Vector Int
  */
  ostream & operator << (ostream & os,  std::vector<int> & vec);
  /**
  * Pretty printing for Matrix
  */
  ostream & operator << (ostream & os,  kbest::Matrix & mtx);
  
}

#endif /* !MATRIX_H */


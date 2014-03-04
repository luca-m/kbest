/**
 * Problem.h
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#ifndef PROBLEM_H
#define PROBLEM_H

#include <solver/Matrix.h>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <limits>
#include <fstream>
#include <sstream>

using namespace std;

namespace kbest{

  /**
   * @class Problem 
   *
   * Knapsak 1-D problem representation
   * */
  class Problem{
  
    public:

      /**
       * Create the sample problem from paper http://dx.doi.org/10.1590/S0101-74382000000100011
       **/
      Problem();
      /**
       * Create a new void problem
       *
       * @param nvar Number of variables in the problem
       * @param b Problem capacity
       * */
      Problem(int nvar, int b);
      /**
       * Create a new problem parsing a problem file
       * @param filePath Problem file path
       * */
      Problem(std::string filePath);
      /**
       * */
      ~Problem();

      /**
       * Set a variable of the problem using 0-based index
       * @param i Index
       * @param cost Cost associated to the variable
       * @param weight Weight of the variable
       * */
      void setVariable(int i, int cost, int weight);
      /** 
       * Set a variable of the problem using 1-based index
       * @param i Index
       * @param cost Cost associated to the variable
       * @param weight Weight of the variable
       */
      void setVariable1Based(int i, int cost, int weight);
      /**
       * Get cost of i-th var using 0-based index
       * @param i Index
       * */
      int getC(int i);
      /**
       * Get cost of i-th var using 1-based index
       * @param i Index
       * */
      int getC1Based(int i);
      /**
       * Get weight of i-th var using 0-based index
       * @param i Index
       * */
      int getA(int i);
      /**
       * Get weight of i-th var using 1-based index
       * @param i Index
       * */
      int getA1Based(int i);
      /**
       * Return the number of variables associated to the problem
       * */
      int getNVar();
      /**
       * Return the capacity associated to the problem
       * */
      int getCapacity();
      /**
       * Set the capacity associated to the problem
       * @param capacity Capacity associated to the Knapsack problem
       * */
      int setCapacity(int capacity);
      /**
       * Returns the matrix associated to the problem
       * */
      kbest::Matrix & getAssociatedMatrix();

    private:
 
      /// Number of variables
      int nvar;
      /// Capacity
      int b;
      /// Variable vector: <Weight,Cost>
      std::vector<std::pair<int,int> > vars;
  
  };

  /**
  * Pretty printing for Problem
  */
  ostream & operator << (ostream & os, Problem & prob);
  
}

#endif /* !PROBLEM_H */


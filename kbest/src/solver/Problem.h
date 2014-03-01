/**
 * Problem.h
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#ifndef PROBLEM_H
#define PROBLEM_H

#include "Matrix.h"
#include <vector>
#include <string>

namespace kbest{

  /**
   * @class Problem 
   *
   * Knapsak 1-D problem representation
   * */
  class Problem{
  
    public:
      /**
       * Create a new void problem
       *
       * @param Number of variables in the problem
       * @param Problem capacity
       * */
      Problem(int nvar, int b);
      /**
       * Create a new problem parsing a problem file
       * @param Problem file path
       * */
      Problem(std::string filePath);
      /**
       * */
      ~Problem();

      /**
       * Set a variable of the problem using 0-based index
       * */
      void addVariable(int i, int cost, int weight);
      /** 
       * Set a variable of the problem using 1-based index
       */
      void addVariable1Based(int i, int cost, int weight);
      /**
       * Get cost of i-th var using 0-based index
       * */
      int getC(int i);
      /**
       * Get cost of i-th var using 1-based index
       * */
      int getC1Based(int i);
      /**
       * Get weight of i-th var using 0-based index
       * */
      int getA(int i);
      /**
       * Get weight of i-th var using 1-based index
       * */
      int getA1Based(int i);
      /**
       * Return the capacity associated to the problem
       * */
      int getCapacity();
      /**
       * Returns the matrix associated to the problem
       * */
      kbest::Matrix getAssociatedMatrix();

    private:
 
      int nvar;
      int b;
      std::vector<int> a;
      std::vector<int> c; 
  
  };

}

#endif /* !PROBLEM_H */


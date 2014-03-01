/**
 * Solution.h
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#ifndef SOLUTION_H
#define SOLUTION_H

#include "Problem.h"
#include <vector>
#include <iostream>

using namespace std;

namespace kbest{

  /**
  * @class Solution 
  *
  * A Solution of a 1-D Knapsack problem containing information about supernodes.
  */
  class Solution{
  
    public:
      /**
      * Create an empty solution for the specified problem
      * @param Problem
      */
      Solution(kbest::Problem & problem);
      /**
      * 
      */
      ~Solution();

      /// Solution Value
      int V;
      /// Supernode index
      int J;
      /// 
      int T;
      /// Has been checked
      bool C;
      std::vector<int> X;
      /**
      * Set the decision variables
      */
      void setDecisionVars(std::vector<int> dvars);

      /**
      * Less operator. This make Solution sortable.
      */
      bool operator < (const Solution & sol) const{
          return this->V < sol.V;
      }
      /**
      * Equal operator.
      */
      bool operator == (const Solution & sol) const{
          return this->V==sol.V && this->J==sol.J && this->T==sol.T;
      }

    private:

  };

  /**
  * @class Solution List
  *
  * A Solution List
  */
  class SolutionList{
    public:
      /**
      *
      */
      SolutionList();
      /**
      *
      */
      ~SolutionList();
      
      /**
      * Sort in non increasing order
      */
      void sortNonIncreasing();
      
      /**
      * @param index of the solution to retrieve
      */
      Solution & get(int index);
      /**
      * @param index of the solution to retrieve
      */
      Solution & get1Based(int index);
      /**
      * @param Solution to add
      */
      int addSolution(Solution sol);
      /**
      *
      * @param Solution to insert
      */
      int insertSorted(Solution sol);
      /**
      *
      * @param insertion index
      */
      int insertAt(int index, Solution sol);
      /**
      *
      * @param insertion index
      */
      int insertAt1Based(int index, Solution sol);
      /**
      *
      * @param Solution to insert
      */
      int getInsertionIndex(Solution sol);
      /**
      *
      * @param Solution List to merge
      */
      int merge(SolutionList solList);
      /**
      * Return the number of Solutions in the list
      */
      int size();

    private:
      /// Solution Vector
      std::vector<Solution> slist;

  };



  /**
  * Definition of "+" operator for vector if integers
  */
  std::vector<int> operator + (const std::vector<int>& v1, const std::vector<int>& v2 ){  
    if (v1.size()!= v2.size()) {
      cerr<<"Cannot sum two vecto of different size !"<<endl;
      throw 40;
    }
    std::vector<int> result(v1.size());
    for (unsigned int i = 0; i<result.size(); i++){
      result[i]=v1[i]+v2[i];
    }
    return result;
  }

}

#endif /* !SOLUTION_H */


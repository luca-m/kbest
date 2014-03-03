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
#include <algorithm>

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

      /**
      * Set the decision variables
      */
      void setDecisionVars(std::vector<int> dvars);
      /**
      *
      */
      std::vector<int> & getDecisionVars();
      /**
      *
      */
      void addDecisionVars(std::vector<int> dvars);
      /**
      *
      */
      void incDecisionVar(int j);
      /**
      *
      */
      void incDecisionVar1Based(int j);

      /**
      *
      */
      void setV(int v);
      /**
      *
      */
      int getV();
      /**
      *
      */
      void setJ(int j);
      /**
      *
      */
      int getJ();
      /**
      *
      */
      void setT(int t);
      /**
      *
      */
      int getT();
      /**
      *
      */
      void setC(bool c);
      /**
      *
      */
      bool getC();

    //protected:

      /// Solution Value
      int V;
      /// Supernode index
      int J;
      /// 
      int T;
      /// Has been checked
      bool C;
      /// Decision variables
      std::vector<int> X;

  };

  /**
  * @class Solution List
  *
  * A Solution List
  */
  class SolutionList{
    public:
      /**
      * Create an empty Solution list
      */
      SolutionList();
      /**
      * Create an empty Solution list reserving space for further elements
      * @param Estimated number of elements that will be inserted in the list
      */
      SolutionList(int estimatedElements);
      /**
      */
      ~SolutionList();

      /**
      * Sort current SolutionList in non increasing order
      */
      void sortNonIncreasing();
      /**
      * Sort in non increasing order a slice
      * @param begin index
      * @param end index
      */
      void sortNonIncreasing(int begin, int end);
      /**
      * Retrieve a Solution
      * @param index of the solution to retrieve
      */
      Solution & get(int i);
      /**
      * Retrieve last Solution
      */
      Solution & getLast();      
      /**
      * Retrieve a Solution
      * @param index of the solution to retrieve
      */
      Solution & get1Based(int i);
      /**
      * @param Solution to check
      */
      bool isIn(Solution & sol);
      /**
      * @param Solution to add
      */
      void addSolution(Solution & sol);
      /**
      *
      * @param Solution to insert
      */
      void insertSorted(Solution & sol);
      /**
      * Insert a Solution at specified index
      * @param Insertion index
      * @param Solution to insert
      */
      void insertAt(int i, Solution & sol);
      /**
      * Insert a Solution at specified index
      * @param Insertion index
      * @param Solution to insert
      */
      void insertAt1Based(int i, Solution &sol);
      /**
      * Find the insertion index considering the current SolutionList sorted. 
      * @param Solution to insert
      * @param Start search insertion index from specified position
      */
      int getInsertionIndex(Solution & sol, int startFrom);
      /**
      * Find the insertion index considering the current SolutionList sorted. 
      * @param Solution to insert
      * @param Start search insertion index from specified position
      */
      int getInsertionIndex1Based(Solution & sol, int startFrom);
      /**
      * Merge two sorted SolutionLists, keep first values of the lists 
      * @param Solution List to merge
      * @param Maximum number of element to merge
      */
      void merge(SolutionList & solList, int maxElements);
      /**
      * Return the number of Solutions in the list
      */
      int size();

    private:
      /// Solution Vector
      std::vector<Solution> * slist;

  };

  /**
  * Less operator. This make Solution sortable.
  */
  bool operator < (const Solution & sol0, const Solution & sol);
  /**
  * Equal operator.
  */
  bool operator == (const Solution & sol0, const Solution sol);
  /**
  * Pretty printing for Solution
  */
  ostream & operator <<(ostream & os, Solution & sol);
  /**
  * Pretty printing for SolutionList
  */
  ostream & operator << (ostream & os, SolutionList & sl);
  /**
  * Definition of "+" operator for vector if integers
  */
  std::vector<int> operator + ( std::vector<int> & v1, std::vector<int> & v2 );

}

#endif /* !SOLUTION_H */


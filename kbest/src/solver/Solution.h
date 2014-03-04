/**
 * Solution.h
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#ifndef SOLUTION_H
#define SOLUTION_H

#include <solver/Problem.h>
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
      * @param problem Problem
      */
      Solution(kbest::Problem & problem);
      /**
      */
      ~Solution();

      /**
      * Set the decision variables
      * @param dvars Vector containing new decision vars values
      */
      void setDecisionVars(std::vector<int> dvars);
      /**
      * Retrieve the decision variables
      */
      std::vector<int> & getDecisionVars();
      /**
      * Add values to current decision variables
      * @param dvars Vector containing the decision vars to add
      */
      void addDecisionVars(std::vector<int> dvars);
      /**
      * Increment a decision variable
      * @param j Index of the decision variable to increment
      */
      void incDecisionVar(int j);
      /**
      * Increment a decision variable
      * @param j Index of the decision variable to increment
      */
      void incDecisionVar1Based(int j);

      /**
      * Set Solution value
      * @param v Value
      */
      void setV(int v);
      /**
      * Retrieve Solution value
      */
      int getV();
      /**
      * Set Supernode J index
      * @param j Index
      */
      void setJ(int j);
      /**
      * Retrieve Supernode J index
      */
      int getJ();
      /**
      * Set Supernode T index
      * @param t Index
      */
      void setT(int t);
      /**
      * Retrieve Supernode T index
      */
      int getT();
      /**
      * Set Check
      * @param c is Checked
      */
      void setC(bool c);
      /**
      * Retrieve Check
      */
      bool getC();

    //protected:

      /// Solution Value
      int V;
      /// Supernode J index
      int J;
      /// Supernode T index
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
      * @param estimatedElements Estimated number of elements that will be inserted in the list
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
      * @param i index of the solution to retrieve
      */
      Solution & get(int i);
      /**
      * Retrieve last Solution
      */
      Solution & getLast();      
      /**
      * Retrieve a Solution
      * @param i index of the solution to retrieve
      */
      Solution & get1Based(int i);
      /**
      * @param sol Solution to check
      */
      bool isIn(Solution & sol);
      /**
      * @param sol Solution to add
      */
      void addSolution(Solution & sol);
      /**
      * Insert a solution assuming current SolutionList sorted
      * @param sol Solution to insert
      */
      void insertSorted(Solution & sol);
      /**
      * Insert a Solution at specified index
      * @param i Insertion index
      * @param sol Solution to insert
      */
      void insertAt(int i, Solution & sol);
      /**
      * Insert a Solution at specified index
      * @param i Insertion index
      * @param sol Solution to insert
      */
      void insertAt1Based(int i, Solution &sol);
      /**
      * Find the insertion index considering the current SolutionList sorted. 
      * @param sol Solution to insert
      * @param startFrom Start search insertion index from specified position
      */
      int getInsertionIndex(Solution & sol, int startFrom);
      /**
      * Find the insertion index considering the current SolutionList sorted. 
      * @param sol Solution to insert
      * @param startFrom Start search insertion index from specified position
      */
      int getInsertionIndex1Based(Solution & sol, int startFrom);
      /**
      * Merge two sorted SolutionLists, keep first values of the lists 
      * @param solList Solution List to merge
      * @param maxElements Maximum number of element to merge
      */
      void merge(SolutionList & solList, int maxElements);
      /**
      * Return the number of Solutions in the list
      */
      int size();

    private:
      /// Solution Vector
      std::vector<Solution *> * slist;

  };

  bool comparePtrToSolution(Solution* a, Solution* b);
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


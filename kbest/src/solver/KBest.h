/**
 * KBest.h
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#ifndef KBEST_H
#define KBEST_H

#include "Matrix.h"
#include "Solution.h"
#include "Problem.h"

namespace kbest{

  /**
  * @class Knapsack K-Best Solver 
  * 
  * Implementation of the algorithm for solving 1-dimensional K-best Knapsack problem described in:
  * 'Yanasse, Soma, Maculan - An Algorithm For Determining The K-Best Solutions Of The One-Dimensional
  * Knapsack Problem (http://dx.doi.org/10.1590/S0101-74382000000100011)'
  */
  class KBestSolver{
  
    public:

      /**
      * Create a K-Best Solver
      */
      KBestSolver();
      /**
      */
      ~KBestSolver();
      /**
      *
      * @param Problem to solve
      * @param number of best solution to find 
      */
      SolutionList & kbest(Problem & prob, int k);

    protected:

      /**
      * Forward enumeration, it populate the matrix 
      * representation of the problem and enumerate solution 
      * from root to leaves.
      */
      void forward();
      /**
      * Perform the backward enumeration of best solutions
      */
      void backward();
      /**
      * Recover initial solution from matrix representation of the problem,
      * It will populate the SolutionList associated with this instance of the solver. 
      */
      void builtInitKBest();
      /**
      * Iterate SolutionList and perform backtracking in the Solution found in order to
      * reconstruct decision variables associated to the Solutions.  
      */
      void recoverSol();
      /**
      * Backtrack a solution for recovering associated decision variables and eventually
      * search for alternative solution.
      * @param Solution to consider
      * @param Index of the considered Solution in SolutionList
      * @param Check for alternative solution if needed
      */
      void backtracking(Solution & current_sol, int sol_index, bool alternative);
      /**
      * Search for alternative solution starting from a particular node in the 
      * matrix representation of the problem.
      * @param Supernode index
      * @param Node index
      * @param Solution value accumulator
      * @param Previous node index
      * @param Minimum Solution index in SolutionList where to insert new Solutions 
      */
      void searchAltSol(int t, int j, int zcum, int j1, int sol_index);

    private:

      /// Backtracked Solution List
      kbest::SolutionList * L;
      /// Problem matricial representation
      kbest::Matrix * M ;
      /// Problem
      kbest::Problem * prob;
      /// K-best
      int k;
      /// index of found solutions
      int p;

  };


}

#endif /* !KBEST_H */


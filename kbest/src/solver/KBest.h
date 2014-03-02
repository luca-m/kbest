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
      *
      */
      KBestSolver();
      /**
      *
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
      *
      */
      void forward();
      /**
      *
      */
      void backward();
      /**
      *
      */
      void builtInitKBest();
      /**
      *
      */
      void recoverSol();
      /**
      *
      */
      void backtracking(Solution & current_sol, int sol_index, bool alternative);
      /**
      *
      */
      void searchAltSol(int t, int j, int zcum, int j1, int sol_index);

    private:

      /// Backtracked Solution List
      kbest::SolutionList L;
      /// Problem matricial representation
      kbest::Matrix M;
      /// Problem
      kbest::Problem prob;
      /// K-best
      int k;
      /// index of found solutions
      int p;

  };


}

#endif /* !KBEST_H */


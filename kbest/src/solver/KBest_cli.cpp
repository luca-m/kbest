/**
 * KBest_cli.cpp
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#include "KBest.h"
#include "Solution.h"
#include "Problem.h"

#include <algorithm>
#include <iostream>

using namespace std;
using namespace kbest;

void print_help(){

}

void print_solutions(SolutionList & slist, int k){


}

void print_performance_csv_row(Problem & prob, int k, double forward_time, double backward_time ){
  //  nvar, b, k, forward_time, backward_time, total_time
  cout<<prob.getNVar()<<","<<prob.getCapacity()<<","<<k<<","<<forward_time<<","<<backward_time<<","<<forward_time+backward_time<<endl;
}


char * getCmdOption(char ** begin, char ** end, const std::string & option){
    char ** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end){
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option){
    return std::find(begin, end, option) != end;
}

int main(int argc, char *argv[]){
  bool perf_only=false;
  int k=15;
  int i=-1;
  int it=-1;

  if(cmdOptionExists(argv, argv+argc, "-h")){
    print_help();
    it=std::find(argv, argv+argc, "-h")+1;
    if(it>i){
      i=it;
    }
    exit(-1);
  }
  if(cmdOptionExists(argv, argv+argc, "-p")){
    perf_only=true;
    it=std::find(argv, argv+argc, "-p")+1;
    if(it>i){
      i=it;
    }
  }
  if (cmdOptionExists(argv, argv+argc, "-k")){
    k=atoi(getCmdOption(argv, argv+argc, "-k"));
    it=std::find(argv, argv+argc, "-k")+2;
    if(it>i){
      i=it;
    }
  }

  KBestSolver kbs();
  for (; i < argc; i++){
    string filename(argv[i]);
    Problem prob=Problem(filename);
    SolutionList slist = kbs.kbest(prob, k);
    // TODO: retrieve performance
    if (perf_only){
      print_performance_csv_row(prob, k, -1.0, -1.0);
    } else {
      print_solutions(slist, k);
    }
  }

  return 0;
}



/**
 * KBest_cli.cpp
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#include <solver/KBest.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace kbest;

void print_help(){

  string help="\n"
  " #### Knapsack K-Best Solver ####\n\n"
  "---------------------- luca.mella@studio.unibo.it\n"
  "    \n"
  "    Implementation of the algorithm for solving 1-dimensional\n"
  "    K-best Knapsack problem described in:\n"
  "    'Yanasse, Soma, Maculan - An Algorithm For Determining \n"
  "    The K-Best Solutions Of The One-Dimensional Knapsack Problem'\n"
  "    \n"
  "    See http://dx.doi.org/10.1590/S0101-74382000000100011\n"
  " \n"
  " Usage:\n"
  " \n"
  "    ./kbest [-h] [-p] [--sample] [-k NUM] PROBLEMFILE + \n"
  " \n"
  " Options:"
  "\n"
  "      -h         This help\n"
  "      -p         Print out performance data only (no solution)\n"
  "      -k         Number of best solution to find\n"
  "      --sample   Run with sample problem from the paper\n"
  " \n"
  " Params:\n"
  " \n"
  "       PROBLEMFILE+  File where to load a 1-D knapsack problem\n";
  cout<<help;
}

void print_solutions(SolutionList & slist, int k){
  cout<<k<<"-best solutions found:"<<endl;
  for(int i=0; i<k && i < slist.size(); i++){
    cout<<slist.get(i)<<endl;
  }
}

void print_performance_csv_row(Problem & prob, int k, double forward_time, double backward_time ){
  //  nvar, b, k, forward_time, backward_time, total_time
  cout<<prob.getNVar()<<","<<prob.getCapacity()<<","<<k<<","<<forward_time<<","<<backward_time<<","<<forward_time+backward_time<<endl;
}

string getCmdOption(vector<string>::iterator begin, vector<string>::iterator end, const std::string & option){
    vector<string>::iterator itr = std::find(begin, end, option);
    if (itr != end && ++itr != end){
        return *itr;
    }
    return 0;
}

bool cmdOptionExists(vector<string>::iterator begin, vector<string>::iterator end, const std::string & option){
    return std::find(begin, end, option) != end;
}

int main(int argc, char *argv[]){
  bool perf_only=false;
  bool sample_only=false;
  int k=15;
  vector<string>::iterator i;
  vector<string>::iterator it;

  vector<string> args= vector<string>(argc);

  for (int k=0;k<argc;k++){
    args.push_back(string(argv[k]));
  }

  if(cmdOptionExists(args.begin(), args.end(), "-h")){
    print_help();
    it = std::find(args.begin(), args.end(), "-h")+1;
    if(it>i){
      i=it;
    }
    exit(-1);
  }
  if(cmdOptionExists(args.begin(), args.end(), "-p")){
    perf_only=true;
    it = std::find(args.begin(), args.end(), "-p")+1;
    if(it>i){
      i=it;
    }
  }
  if(cmdOptionExists(args.begin(), args.end(), "--sample")){
    sample_only=true;
    it = std::find(args.begin(), args.end(), "--sample")+1;
    if(it>i){
      i=it;
    }
  }
  if (cmdOptionExists(args.begin(), args.end(), "-k")){
    k=atoi(getCmdOption(args.begin(), args.end(), "-k").c_str());
    it = std::find(args.begin(), args.end(), "-k")+2;
    if(it>i){
      i=it;
    }
  }

  KBestSolver kbs=KBestSolver();
  if (!sample_only){
    for (; i < args.end(); ++i){
      try{
        string filename=*it;
        Problem prob=Problem(filename);
        SolutionList slist = kbs.kbest(prob, k);
        // TODO: retrieve performance
        if (perf_only){
          print_performance_csv_row(prob, k, -1.0, -1.0);
        } else {
          print_solutions(slist, k);
        }
      } catch (int e) {
        cerr<<"FATAL: exception "<<e<<""<<endl;
        exit(-2);
      }
    }
  }
  else{
    Problem prob=Problem();
    try{
      SolutionList slist = kbs.kbest(prob, k);
      // TODO: retrieve performance
      if (perf_only){
        print_performance_csv_row(prob, k, -1.0, -1.0);
      } else {
        print_solutions(slist, k);
      }
    } catch (int e) {
      cerr<<"FATAL: exception "<<e<<""<<endl;
      exit(-2);
    }
  }

  return 0;
}



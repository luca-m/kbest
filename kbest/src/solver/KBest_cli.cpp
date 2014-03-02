/**
 * KBest_cli.cpp
 * Copyright (C) Luca Mella <luca.mella@studio.unibo.it>
 *
 * Distributed under terms of the CC-BY-NC license.
 */

#include "KBest.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

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
  if (cmdOptionExists(args.begin(), args.end(), "-k")){
    k=atoi(getCmdOption(args.begin(), args.end(), "-k").c_str());
    it = std::find(args.begin(), args.end(), "-k")+2;
    if(it>i){
      i=it;
    }
  }

  KBestSolver kbs=KBestSolver();
  for (; i < args.end(); ++i){
    string filename=*it;
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



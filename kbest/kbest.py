#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
'''
  @module KBest

  Implementation of the algorithm for solving 1-dimensional K-best Knapsack problem described in:
  'Yanasse, Soma, Maculan - An Algorithm For Determining The K-Best Solutions Of The One-Dimensional
  Knapsack Problem (http://dx.doi.org/10.1590/S0101-74382000000100011)'
'''
import string
import numpy as np
import argparse
import logging

LOGGER=logging.getLogger()
LOGGER.setLevel(logging.DEBUG)

#
# Utilities
#

def reverse_bisect(a, x, lo=0, hi=None):
    """Insert item x in list a, and keep it reverse-sorted assuming a
    is reverse-sorted.

    If x is already in a, insert it to the right of the rightmost x.

    Optional args lo (default 0) and hi (default len(a)) bound the
    slice of a to be searched.
    """
    if lo < 0:
        raise ValueError('lo must be non-negative')
    if hi is None:
        hi = len(a)
    while lo < hi:
        mid = (lo+hi)//2
        if x > a[mid]: hi = mid
        else: lo = mid+1
    return lo
#
# Data Structures
#

class list1(list):
  ''' 
  @class One-based version of list 
  '''

  def _zerobased(self, i):
    if type(i) is slice:
      return slice(self._zerobased(i.start), self._zerobased(i.stop), i.step)
    else:
      if i is None or i<0:
        return i
      elif not i:
        raise IndexError('element 0 does not exist in 1-based list')
      return i-1

  def __getitem__(self, i):
    return list.__getitem__(self, self._zerobased(i))

  def __setitem__(self, i, value):
    list.__setitem__(self, self._zerobased(i), value)

  def __delitem__(self, i):
    list.__delitem__(self, self._zerobased(i))

  def __getslice__(self, i, j):
    return list.__getslice__(self, self._zerobased(i or 1), self._zerobased(j))

  def __setslice__(self, i, j, value):
    list.__setslice__(self, self._zerobased(i or 1), self._zerobased(j), value)

  def index(self, value, start=1, stop=-1):
    return list.index(self, value, self._zerobased(start), self._zerobased(stop)) + 1

  def insert(self, index, value):
    return list.insert(self, self._zerobased(index), value)

  def pop(self, i):
    return list.pop(self, self._zerobased(i))

  def clear(self):
    for x in self:
      list.remove(self, x)

class Problem(object):
  '''
  @class Problem data representation
  '''
  
  @staticmethod
  def newMatrixRepresentation(problem):
    assert issubclass(type(problem), Problem)
    M=np.empty((problem.b+1, problem.n+1), dtype=int)
    M[:]=-1
    M[0,:]=-2
    M[:,0]=-2
    return M
  
  @staticmethod
  def problemFromFile(filePath):
    with open(filePath,'r') as f:
      fields=[string.strip(x).replace('\t',' ') for x in f.read().split('\n')]
      prob=Problem(nvar=0,b=0)
      prob.n=int(fields[0])
      for i in xrange(1,prob.n+1):
        _,c,a=fields[i].split(' ')
        c=int(c)
        a=int(a)
        prob.addVariable(self, c, a)
      prob.b=int(fields[prob.n+1])
      return prob

  def __init__(self, nvar=0, b=0):
    self.n=nvar
    self.c=np.zeros((self.n+1),dtype=int)
    self.a=np.zeros((self.n+1),dtype=int)
    self.b=b
  
  def addVariable(self, cost, weight):
    self.a=np.append(self.a,[weight],0)
    self.c=np.append(self.c,[cost],0)
    self.n+=1


  def __str__(self):
    return str(self.__dict__)

class SolInfo(object):
  '''
  @class Solution information representation
  '''
  
  @staticmethod
  def findInsertionIndex(slist, sol, startfrom=1):
    ''' 
    Find the proper insertion index on the sorted list of solutions provided (binary search)
    '''
    assert issubclass(type(slist), list1)
    up=len(slist)
    low=startfrom
    return reverse_bisect(slist, sol, lo=startfrom, hi=len(slist)+1)

  @staticmethod
  def sortNonIncreasing(slist, slce=(0,-1)):
    ''' 
    Sort solutions in list in non increasing order 
    '''
    slist.sort(reverse=True)
    return slist
    #l=sorted(slist[slce[0]:slce[1]], lambda x,y: int(x.V-y.V), reverse=True)
    #l.extend(slist[slce[1]:])
    #l1=list1()
    #l1.extend([el for el in l])
    #return l1
  
  def __init__(self, problem=None):
    if problem is None:
      self.X=np.zeros((1))
    else:
      self.X=np.zeros((problem.n+1))
    self.V=-1
    self.J=-1
    self.T=-1
    self.C=False
 
  def getDecisionVars(self):
    return self.X[1:]

  def setDecisionVars(self, dvars):
    self.X=np.append(np.zeros((1)),dvars,0)

  def __str__(self):
    return str(self.__dict__)

  def __eq__(self, other):
    if issubclass(type(other), SolInfo):
      return self.J==other.J and self.T==other.T and self.V==other.V  
    else:
      return id(self)==id(other)

  def __lt__(self, other):
    assert issubclass(type(other), SolInfo)
    return  self.V<other.V

  def __ne__(self, other):
    return not self==other

  def __gt__(self, other):
    return not self==other and not self<other

  def __le__(self, other):
    return self==other or self<other

  def __ge__(self, other):
    return self==other or self>other

  def __hash__(self):
    h=1493
    h=h*17+np.sum(self.X);
    h=h*31+hash(self.T)
    h=h*13+hash(self.J)
    h=h*43+hash(self.T)
    h=h*23+hash(self.V)
    return h


class KBestSolver(object):
  '''
  @class KKP Problem Solver

  See: 
    Yanasse, Soma, Maculan
    An Algorithm For Determining The K-Best Solutions Of The One-Dimensional Knapsack Problem 
    http://dx.doi.org/10.1590/S0101-74382000000100011
  '''

  def __init__(self):
    pass

  #
  # K-Best: Backtracking
  #

  def _searchAltSol(self, t, j, zcum, j1, sol_index):
    '''
    Search alternative solutions in the specified supernode. 
    '''
    #logging.debug('searchAltSol: t={}, j={}, j1={}, zcum={}'.format(t,j,j1,zcum))
    for s in xrange(1, j1+1):
      if s!=j:
        if self.M[t,s]>=0:
          if self.M[t,s]+zcum >= self.L[self.p].V:
            # Determine position g: g>i to insert this alternative 
            # solution in ordered list L
            alt_sol=SolInfo(problem=self.prob)
            alt_sol.V=self.M[t,s]+zcum
            alt_sol.T=t
            alt_sol.J=s

            g=SolInfo.findInsertionIndex(self.L, alt_sol, startfrom=sol_index)
            self.L.insert(g,alt_sol)    # alternative solution is in pos g, now compute its value.

            if self.p<self.k:
              self.p+=1
            #f=self.p
            #while f>g:
            #  self.L[f]=self.L[f-1]
            #  f-=1
            
            #self.L[g].V=self.M[t,s]+zcum
            #self.L[g].J=self.L[i].J
            #self.L[g].T=self.L[i].T
            
            current_sol=SolInfo(problem=self.prob)
            current_sol.V=self.M[t,s]
            current_sol.J=s
            current_sol.T=t
            #if auxl1 in self.L:
            #  self._backtracking(auxl1, g, alternative=True)
            #else:
            self._backtracking(current_sol, g, alternative=False)
            self.L[g].C=True
            self.L[g].setDecisionVars(self.L[sol_index].getDecisionVars()+current_sol.getDecisionVars())
            logging.debug(''.join(('searchAltSol: alternative sol!\n  current_sol.V={}, current_sol.X={}\n  ',
                           'L[{}].V={}, L[i].X={}\n  ',
                           'L[{}].V={}, L[g].X={}')).format(current_sol.V, current_sol.getDecisionVars(), 
                                          sol_index, self.L[sol_index].V, self.L[sol_index].getDecisionVars(), 
                                          g, self.L[g].V, self.L[g].getDecisionVars()))
            
            if self.M[t,s]>=self.L[self.p].V:
              # Determine position kk : kk>g to insert this alternative 
              # solution in ordered list L
              kk=SolInfo.findInsertionIndex(self.L, auxl1, startfrom=g)
              if self.p<self.k:
                self.p+=1
              self.L.insert(kk,auxl1)
              #f=self.p
              #while(f>kk):
              #  self.L[f]=self.L[f-1]
              #  f-=1
              #self.L[kk]=auxl1


  def _backtracking(self, current_sol, sol_index, alternative=True):
    ''' 
    Backtrack the matrix representation of the problem and recover solutions 
    '''
    t=current_sol.T
    j=current_sol.J
    j1=j
    z=current_sol.V
    zcum=0
    logging.debug('backtracking current_sol: j={}, t={}, z={}, M[t,j]={}'.format(j,t,z,self.M[t,j]))
    while t>0:
      t-=self.prob.a[j]
      z-=self.prob.c[j]
      zcum+=self.prob.c[j]
      current_sol.X[j]+=1
      logging.debug('backtracking current_sol: j={}, auxj.X={}'.format(j,current_sol.getDecisionVars()))
      if z==0:
        #logging.debug('backtracking root: t={}, z={}, j={}, M[t,:]={}'.format(t,z,j,self.M[t,:]))
        break
      
      # j = {s: M[t,s] = z , 1 <= s <= j}
      for jj in xrange(1,j+1):
        if self.M[t,jj]==z:
          j=jj
          logging.debug('backtracking sol: z={} found at {} in M[{},{}]={}'.format(z,j,t,j,self.M[t,1:]))
      if t>0 and alternative:
        self._searchAltSol(t, j, zcum, j1, sol_index)
      j1=j
    current_sol.C=True

  def _recoverSol(self):
    ''' 
    Recover solution and search for alternatives
    '''
    i=0
    while i<self.p:
      i+=1
      if not self.L[i].C:
        current_sol=self.L[i]
        self._backtracking(current_sol, i)
        self.L[i]=current_sol

  def _builtInitKBest(self):
    ''' 
    Build the initial K-Best solution 
    '''
    self.L=list1()
    counter=0
    i=self.prob.b+1
    fim=False
    moreleft=False
    
    while i>self.prob.a[1]:
      i-=1
      j=self.prob.n+1
      while j>1:
        j-=1
        if self.M[i,j]>=0:
          
          sol=SolInfo(problem=self.prob)
          sol.V=self.M[i,j]
          sol.J=j
          sol.T=i
          if sol in self.L:
            logging.debug('buildInitKBest: (L) duplicate found {}'.format(sol))
          else:
            logging.debug('buildInitKBest: found solution with value {} .i={}, j={} (L)'.format(self.M[i,j],i,j))
            counter+=1
            self.L.append(sol)  
          if counter==self.k:
            i1=i; j1=j
            moreleft=True
            i=0; j=0
    
    self.p=counter
    SolInfo.sortNonIncreasing(self.L)
    
    if self.p==self.k and (i1>self.prob.a[1] or j1>1):
      fim=True
    
    while fim:
      L1=list1()
      counter=0
      i=i1+1
      fim=False
      while i>self.prob.a[1]:
        i-=1
        j=self.prob.n+1
        if moreleft:
          j=j1
          moreleft=False
        while j>1:
          j-=1
          if self.M[i,j]>self.L[self.p].V:
            
            sol=SolInfo(problem=self.prob)
            sol.V=self.M[i,j]
            sol.J=j
            sol.T=i
            if sol in self.L or sol in L1:
              logging.debug('buildInitKBest: (L1) duplicate found {}'.format(sol))
            else:
              logging.debug('buildInitKBest: found solution with value {} .i={}, j={} (L1)'.format(self.M[i,j],i,j))
              L1.append(sol)
              counter+=1
            if counter==self.k:
              i1=i; j1=j
              moreleft=True
              i=0; j=0
      p1=counter
      L1=SolInfo.sortNonIncreasing(L1)
      if len(L1)>0 and L1[1].V>self.L[self.k].V:

        # Merging:
        # Using L and L1 build sorted list of k objects having largest value V 
        # and put those values in L[0:k]
        #logging.debug('buildInitKBest: merging\nL=\n{}\nL1=\n{}'.format('\n'.join([str(x) for x in L]),'\n'.join([str(x) for x in L1])))
        x=1; y=1; z=1
        mergedL=list1()
        while x<len(self.L)+1 and y<len(L1)+1 and z<self.k+1:
          if self.L[x].V>L1[y].V:
            sol=self.L[x]; x+=1
          else:
            sol=L1[y]; y+=1
          #if sol:# not in mergedL:
          mergedL.append(sol)
          z+=1
        while x<len(self.L)+1 and z<self.k+1:
          #if self.L[x]:# not in mergedL:
          mergedL.append(self.L[x]); z+=1
          x+=1
        while y<len(L1)+1 and z<self.k+1:
          #if L1[y]:# not in mergedL:
          mergedL.append(L1[y]); z+=1
          y+=1
        
        for xx in xrange(1,self.k+1):
          self.L[xx]=mergedL[xx]
        
        #logging.debug('buildInitKBest: i1={}, a1={}, j1={}'.format(i1,prob.a[1],j1))
        if i1>self.prob.a[1] or j1>1:
          fim=True
    
    for i in xrange(1, self.p+1):
      self.L[i].X=np.zeros((self.prob.n+1))
      self.L[i].C=False

    self.L

  def _backtrack(self):
    ''' 
    Backward enumeration. It recover solution from matrix representation of the problem.
    '''
    self._builtInitKBest()
    LOGGER.debug('backward: init kbest:\n{}'.format('\n'.join('supernode V={}, J={}, T={} '.format(x.V,x.J,x.T) for x in self.L) ))
    self._recoverSol()

  #
  # K-Best: Forward enumeration
  #

  def _forward(self):
    ''' 
    Forward enumeration. It fills the matrix representation of the problem
    '''
    #logging.debug('forward: '.format())
    self.M[0,1:]=0

    for j in xrange(1,prob.n+1):
      self.M[self.prob.a[j],j]=self.prob.c[j]
    #logging.debug('Matrix before forward enumeration:\n{}'.format(M[0:,1:])) 
    
    for t in xrange(self.prob.a[1], self.prob.b-self.prob.a[1]+1):
      
      valid_values=self.M[t,:]>=0
      if len(valid_values)>0:
        tmp=np.where(valid_values)
        m=min(tmp[0])
        logging.debug('forward: M[{},:]={} min found at {}'.format(t,self.M[t,1:],m,tmp))
      else:
        logging.debug('forward: x>=0 NOT found in M[{},:{}]={}'.format(t,j,self.M[t,1:]))
        continue
      
      z=self.M[t,m]
      for i in xrange(m, self.prob.n+1):
        if t+self.prob.a[i]<=self.prob.b:
          if self.M[t,i]>z:
            z=self.M[t,i]
          self.M[t+self.prob.a[i],i]=z+self.prob.c[i]

  #
  # K-Best: Algorithm
  #

  def kbest(self, prob, k):
    ''' 
    Determine the k-best solutions of a 1-D Knapsack problem 
    '''
    assert issubclass(type(prob),Problem)
    assert k>0
    self.k=k
    self.prob=prob
    self.M=Problem.newMatrixRepresentation(self.prob)
    self._forward()
    logging.debug('Matrix after forward enumeration:\n{}'.format(self.M[1:,1:]))
    self._backtrack()
    return list(self.L[:self.k+1])
    #return list(self.L)

#
# CLI interface
#

def get_sample_problem():
  prob=Problem()
  prob.addVariable(4,3)
  prob.addVariable(3,4)
  prob.addVariable(5,5)
  prob.addVariable(7,6)
  prob.addVariable(8,7)
  prob.b=15
  return prob

if __name__=='__main__':
  parser = argparse.ArgumentParser(description=__doc__)
  parser.add_argument('--sample',help='run with sample problem', action='store_true')
  parser.add_argument('-k',help='Number of best solution to retrieve', type=int, default=5)
  parser.add_argument('prob',help='Text file containing problem data', default=None, action='store')
  args = parser.parse_args()

  if args.sample is True:
    prob=get_sample_problem()
  else: 
    prob=Problem.problemFromFile(args.prob)

  kbs=KBestSolver()
  L=kbs.kbest(prob, args.k)
  print('Best {} Solutions:'.format(len(L)))
  for sol in L:
    print('X: {} V: {} '.format(sol.getDecisionVars(),sol.V))


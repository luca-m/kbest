#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
'''
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
        prob.c=np.append(prob.c,[c])
        prob.a=np.append(prob.a,[a])
      prob.b=int(fields[prob.n+1])
      return prob

  def __init__(self, nvar=0, b=0):
    self.n=nvar
    self.c=np.zeros((self.n+1),dtype=int)
    self.a=np.zeros((self.n+1),dtype=int)
    self.b=b
  
  def addVariable(self, weight, cost):
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
  def findInsertionIndex(slist, solV, startfrom=1):
    up=len(slist)
    low=startfrom
    done=False
    while not done:
      done=up==low
      mid=(low+up)/2
      if slist[mid].V>=solV:
        if len(slist)==mid+1:   # last element
          return len(slist)
        elif slist[mid+1].V<solV:
          return mid+1
        else:
          low=mid+1
      elif slist[mid].V<solV:
        if mid==0 or slist[mid-1].V>=solV:
          return mid
        else:
          up=mid-1
    return None

  @staticmethod
  def sortNonIncreasing(slist, slce=(0,-1)):
    slist.sort(lambda x,y: int(x.V-y.V), reverse=True)
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

  def __str__(self):
    return str(self.__dict__)

  def __eq__(self, other):
    if issubclass(type(other), SolInfo):
      return self.J==other.J and self.T==other.T and self.V==other.V  
    else:
      return id(self)==id(other)

  def __hash__(self):
    h=1493
    h=h*17+np.sum(self.X);
    h=h*31+hash(self.T)
    h=h*13+hash(self.J)
    h=h*43+hash(self.T)
    h=h*23+hash(self.V)
    return h

#
# K-Best: Backtracking
#

def searchAltSol(M, prob, t, j, zcum, j1, i, L, p, k):
  '''
  Search alternative solutions in the specified supernode. 
  '''
  logging.debug('searchAltSol: t={}, j={}, j1={}, zcum={}'.format(t,j,j1,zcum))
  for s in xrange(1, j1+1):
    if s!=j:
      if M[t,s]>=0:
        if M[t,s]+zcum >= L[p].V:
          # Determine position g: g>i to insert this alternative 
          # solution in ordered list L
          g=SolInfo.findInsertionIndex(L, M[t,s]+zcum, startfrom=i)
          if p<k:
            p+=1
          f=p
          while f>g:
            L[f]=L[f-1]
            f-=1
          
          L[g].V=M[t,s]+zcum
          L[g].J=L[i].J
          L[g].T=L[i].T
          
          auxl1=SolInfo(problem=prob)
          auxl1.V=M[t,s]
          auxl1.J=s
          auxl1.T=t
          
          backtracking(M, prob, L, auxl1, g, p, k)
          
          L[g].C=True
          L[g].X[1:]=L[i].X[1:]+auxl1.X[1:]
          
          if M[t,s]>=L[p].V:
            # Determine position kk : kk>g to insert this alternative 
            # solution in ordered list L
            kk=SolInfo.findInsertionIndex(L, M[t,s], startfrom=g)
            if p<k:
              p+=1
            f=p
            while(f>kk):
              L[f]=L[f-1]
              f-=1
            L[kk]=auxl1


def backtracking(M, prob, L, auxl, i, p, k):
  ''' 
  Backtrack the matrix representation of the problem and recover solutions 
  '''
  logging.debug('backtracking: auxl={}'.format(auxl))
  t=auxl.T
  j=auxl.J
  j1=j
  z=auxl.V
  zcum=0
  while t>0:
    t-=prob.a[j]
    z-=prob.c[j]
    zcum+=prob.c[j]
    auxl.X[j]=auxl.X[j]+1
    # j = {s: M[t,s] = z , 1 <= s <= j}
    tmp=np.where(M[t,:j]==z)
    if tmp[0].shape==(0,):
      logging.debug('backtracking: z={} NOT found in M[{},1:{}]={}'.format(z,t,j,M[t,1:j]))
      continue
    else:
      logging.debug('backtracking: z={} found at {} in M[{},1:{}]={}'.format(z,tmp[0][0],t,j,M[t,1:j]))
      j=tmp[0][0]
    
    #j1=auxl.J
    if t>0:
      searchAltSol(M,prob,t,j,zcum,j1,j,L,p,k)
    j1=j
  auxl.C=True

def recoverSol(M, prob, L, p, k):
  ''' 
  Recover solution and search for alternatives
  '''
  i=0
  while i<p:
    i+=1
    if not L[i].C:
      auxl=L[i]
      backtracking(M,prob,L,auxl,i,p,k)
      L[i]=auxl
  return L

def builtInitKBest(M, prob, k):
  ''' 
  Build the initial K-Best solution 
  '''
  L=list1()
  counter=0
  i=prob.b+1
  fim=False
  moreleft=False
  
  while i>prob.a[1]:
    i-=1
    j=prob.n+1
    while j>1:
      j-=1
      if M[i,j]>=0:
        
        sol=SolInfo(problem=prob)
        sol.V=M[i,j]
        sol.J=j
        sol.T=i
        if sol in L:
          logging.debug('buildInitKBest: (L) duplicate found {}'.format(sol))
        else:
          logging.debug('buildInitKBest: found solution with value {} .i={}, j={} (L)'.format(M[i,j],i,j))
          counter+=1
          L.append(sol)  
        if counter==k:
          i1=i; j1=j
          moreleft=True
          i=0; j=0
  
  p=counter
  L=SolInfo.sortNonIncreasing(L)
  
  if p==k and (i1>prob.a[1] or j1>1):
    fim=True
  
  while fim:
    L1=list1()
    counter=0
    i=i1+1
    fim=False
    while i>prob.a[1]:
      i-=1
      j=prob.n+1
      if moreleft:
        j=j1
        moreleft=False
      while j>1:
        j-=1
        if M[i,j]>L[p].V:
          
          sol=SolInfo(problem=prob)
          sol.V=M[i,j]
          sol.J=j
          sol.T=i
          if sol in L or sol in L1:
            logging.debug('buildInitKBest: (L1) duplicate found {}'.format(sol))
          else:
            logging.debug('buildInitKBest: found solution with value {} .i={}, j={} (L1)'.format(M[i,j],i,j))
            L1.append(sol)
            counter+=1
          if counter==k:
            i1=i; j1=j
            moreleft=True
            i=0; j=0
    p1=counter
    L1=SolInfo.sortNonIncreasing(L1)
    if len(L1)>0 and L1[1].V>L[k].V:

      # Merging:
      # Using L and L1 build sorted list of k objects having largest value V 
      # and put those values in L[0:k]
      #logging.debug('buildInitKBest: merging\nL=\n{}\nL1=\n{}'.format('\n'.join([str(x) for x in L]),'\n'.join([str(x) for x in L1])))
      x=1; y=1; z=1
      mergedL=list1()
      while x<len(L)+1 and y<len(L1)+1 and z<k+1:
        if L[x].V>L1[y].V:
          sol=L[x]; x+=1
        else:
          sol=L1[y]; y+=1
        #if sol:# not in mergedL:
        mergedL.append(sol)
        z+=1
      while x<len(L)+1 and z<k+1:
        #if L[x]:# not in mergedL:
        mergedL.append(L[x]); z+=1
        x+=1
      while y<len(L1)+1 and z<k+1:
        #if L1[y]:# not in mergedL:
        mergedL.append(L1[y]); z+=1
        y+=1
      
      for xx in xrange(1,k+1):
        L[xx]=mergedL[xx]
      
      #logging.debug('buildInitKBest: i1={}, a1={}, j1={}'.format(i1,prob.a[1],j1))
      if i1>prob.a[1] or j1>1:
        fim=True
  
  for i in xrange(1, p+1):
    L[i].X=np.zeros((prob.n+1))
    L[i].C=False
  return (L,p)

def backtrack(M, prob, k):
  ''' 
  Backward enumeration. It recover solution from matrix representation of the problem.
  '''
  L,p=builtInitKBest(M,prob,k)
  LOGGER.debug('backward: init kbest:\n{}'.format('\n'.join(str(x) for x in L) ))
  recoverSol(M,prob,L,p,k)
  return L

#
# K-Best: Forward enumeration
#

def forward(M, prob):
  ''' 
  Forward enumeration. It fills the matrix representation of the problem
  '''
  logging.debug('forward: '.format())
  M[0,:]=0

  for j in xrange(1,prob.n+1):
    M[prob.a[j],j]=prob.c[j]
  logging.debug('Matrix before forward enumeration:\n{}'.format(M[0:,1:])) 
  
  for t in xrange(prob.a[1], prob.b-prob.a[1]+1):
    
    valid_values=M[t,:]>=0
    if len(valid_values)>0:
      tmp=np.where(valid_values)
      m=min(tmp[0])
      logging.debug('forward: M[{},:]={} min found at {}'.format(t,M[t,1:],m,tmp))
    else:
      logging.debug('forward: x>=0 NOT found in M[{},:{}]={}'.format(t,j,M[t,1:]))
      continue
    
    z=M[t,m]
    for i in xrange(m,prob.n+1):
      if t+prob.a[i]<=prob.b:
        if M[t,i]>z:
          z=M[t,i]
        M[t+prob.a[i],i]=z+prob.c[i]

#
# K-Best: Algorithm
#

def kbest(prob, k):
  assert issubclass(type(prob),Problem)
  assert k>0
  M=Problem.newMatrixRepresentation(prob)
  forward(M, prob)
  logging.debug('Matrix after forward enumeration:\n{}'.format(M[1:,1:]))
  L=backtrack(M, prob, k)
  return list(L[:k+1])

#
# CLI interface
#

if __name__=='__main__':
  parser = argparse.ArgumentParser(description=__doc__)
  parser.add_argument('-k',help='Number of best solution to retrieve', type=int, default=5)
  parser.add_argument('prob',help='Text file containing problem data',action='store')
  args = parser.parse_args()
  prob=Problem.problemFromFile(args.prob)
  L=kbest(prob, args.k)
  print('Best {} Solutions:'.format(len(L)))
  for sol in L:
    print('X: {} V: {} '.format(sol.getDecisionVars(),sol.V))


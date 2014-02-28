'''
@module Problem List Creator
'''
import argparse
from os import listdir
from os.path import join
from os.path import isdir
from os.path import abspath
from re import match 
from collections import defaultdict

class Instance(object):
  def __init__(self, n=0,type='unknown', b=0, path='unknown'):
    self.varn=n
    self.type=type
    self.b=b
    self.path=abspath(path)

  def __eq__(self, other):
    if issubclass(type(other), Instance):
      return self.path==other.path  
    else:
      return id(self)==id(other)

  def __hash__(self):
    h=1493
    h=h*17+hash(self.path)
    return h

def find_instances(directory):
  print('Searching instances in folder {}'.format(directory))
  instances=list()
  types=defaultdict(lambda:list())
  varnum=defaultdict(lambda:list())
  type_varnum=defaultdict(lambda:list())
  for f in listdir(directory):
    typedir=join(directory,f)
    if isdir(typedir):
      typee=f
      print('Searching instances in of type {}'.format(typee))
      for nf in listdir(typedir):
        m=match('^n([0-9]*)$',nf)
        if m:
          instancedir=join(typedir,nf)
          n=int(m.groups()[0])
          print('Searching instances in of nvar {}'.format(n))
          for inf in listdir(instancedir):
            if match('^i([0-9]*)$',inf):
              ifolder=join(instancedir,inf)
              for inst in listdir(ifolder):
                if match('^test\.in$',inst):
                  ipath=abspath(join(ifolder,inst))
                  b=-1
                  with open(ipath,'r') as fh:
                    line=''
                    for line in fh: pass
                    b=int(line)
                  print('Instances found at {}'.format(join(ifolder,inst)))
                  instance=Instance(n=n,type=typee,b=b,path=ipath)
                  instances.append(instance)
                  types[typee].append(instance)
                  varnum[n].append(instance)
                  type_varnum[(typee,n)].append(instance)
  return instances, types, varnum, type_varnum


def write_problem_lists(outdir, instances, types, varnum, type_varnum, byType=False, byNumber=False):
  print('Writing instances paths in folder {}'.format(outdir))
  if byType is False and byNumber is False:
    with open(join(outdir,'instances.lst'),'w') as f:
      for inst in instances:
        f.write('{}\n'.format(inst.path))
  elif byType is True and byNumber is True:
    for (typee,n), instlist in type_varnum.items():
      with open(join(outdir,'instances_{}_{}.lst'.format(n,typee)),'w') as f:
        for inst in instlist:
          f.write('{}\n'.format(inst.path))
  elif byType is True:
    for typee, instlist in types.items():
      with open(join(outdir,'instances_{}.lst'.format(typee)),'w') as f:
        for inst in instlist:
          f.write('{}\n'.format(inst.path))
  elif byNumber is True:
    for n, instlist in varnum.items():
      with open(join(outdir,'instances_{}.lst'.format(n)),'w') as f:
        for inst in instlist:
          f.write('{}\n'.format(inst.path))

if __name__=='__main__':
  parser = argparse.ArgumentParser(description=__doc__)
  parser.add_argument('-d','--dir',help='Directory that contain a the problem library', default='.')
  parser.add_argument('-o','--outdir',help='Directory where to save problem lists', default='.')
  parser.add_argument('-n',help='Group problems by number of vars', action='store_true')
  parser.add_argument('-t',help='Group problems by instance type', action='store_true')
  args = parser.parse_args()

  instances,types,varnum,type_varnum=find_instances(args.dir)
  write_problem_lists(args.outdir, instances, types, varnum, type_varnum, byType=args.t, byNumber=args.n)
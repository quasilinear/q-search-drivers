

import random
import sys

def qio_init():
  print "qio_init"
  # no special initialization needed for this example

def qio_exec(funct, xs1, xs2):
  if funct == 0:
    return oracle_acquire()
  elif funct == 1:
    return oracle_uniform_int(xs1, xs2)
  elif funct == 2:
    oracle_ensure(xs1)
    return 0
  elif funct == 3:
    oracle_release()
    return 0
  else:
    return 0

def oracle_acquire():
  print "oracle_acquire"
  return 1 # optimistically assume we will generate a feasible solution later

def oracle_uniform_int(lo, hi):
  res = random.randint(lo, hi)
  print "oracle_uniform_int(%d,%d) -> %d" % (lo, hi, res)
  return res

def oracle_ensure(cond):
  if cond:
    print "oracle_ensure succeeded"
  else:
    print "oracle_ensure failed"
    sys.exit(-2)


def oracle_release():
  print "oracle_release"

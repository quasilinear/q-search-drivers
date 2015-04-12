import sys
import os
import multiprocessing
import random

manager = multiprocessing.Manager()
context = manager.Namespace()

VERBOSE = False
SHUFFLE = False

def qio_init():

  global VERBOSE, SHUFFLE

  if 'Q_VERBOSE' in os.environ:
    VERBOSE = True

  if 'Q_SHUFFLE' in os.environ:
    try:
      random.seed(int(os.environ['Q_SHUFFLE']))
    except ValueError:
      pass
    SHUFFLE = True

def qio_exec(funct, xs1, xs2):
  if funct == 0:
    return oracle_acquire()
  elif funct == 1:
    return oracle_uniform_int(xs1, xs2)
  elif funct == 2:
    oracle_ensure(xs1)
  elif funct == 3:
    oracle_release()
  return 0

def oracle_acquire():
  if VERBOSE:
    print "oracle_acquire"

  context.partial = []
  context.witness = None
  kidpid = os.fork()
  if kidpid:
    os.waitpid(kidpid, 0)
    if context.witness is None:
      return 0
    else:
      return 1
  else:
    # in the child, optimistically return 1 to allow search to continue
    return 1

def oracle_uniform_int(lo, hi):

  if context.witness is not None:

    # we have a valid witness, so let's return it piece by piece
    res = context.witness[0]
    context.witness = context.witness[1:]
    return res

  else:

    # we still need to search for a witness, and now we're at a choice point
    if SHUFFLE:
      values = list(range(lo,hi))
      random.shuffle(values)
    else:
      values = xrange(lo,hi)

    for i in values:

      kidpid = os.fork()
      if kidpid:

        os.waitpid(kidpid, 0)
        if context.witness is not None:
          # the recursive search found a witness
          os._exit(-1)
        else:
          context.partial = context.partial[:-1]

      else:
        # try exploring this value
        context.partial = context.partial + [i]

        if VERBOSE:
          print "oracle_uniform_int", lo, hi, "-->", context.partial
        return i

    # if we finish the loop, all values have been exhausted
    os._exit(-1)

def oracle_ensure(cond):
  if context.witness is None:
    if VERBOSE:
      print "oracle_ensure", cond
    if not cond:
      os._exit(-1)
  else:
    assert(cond)


def oracle_release():
  if context.witness is None:
    if VERBOSE:
      print "oracle_release", context.partial
    context.witness = context.partial
    os._exit(-1)



#include "qio.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

static FILE* g_tracefile;

void qio_init(qio_callbacks_t callbacks) {

  char* filename = getenv("Q_TRACE");
  assert(filename != NULL);

  g_tracefile = fopen(filename, "r");
  assert(g_tracefile);

}

uint64_t qio_exec(uint8_t funct, uint64_t xs1, uint64_t xs2) {

  uint64_t res = 0;
  fscanf(g_tracefile, "%" SCNx64 "\n", &res);
  return res;

}

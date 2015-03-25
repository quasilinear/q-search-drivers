
#include <stdio.h>
#include <qio.h>

static qio_callbacks_t g_callbacks;
static void* g_instance;

// upon initialization, save instance and callbacks struct for later
void qio_init(void* instance, qio_callbacks_t callbacks) {
  fprintf(stderr, "qio_init(%p,...)\n", instance);
  g_instance = instance;
  g_callbacks = callbacks;
}

// upon execution of extended instruction, just return 42
uint64_t qio_exec(uint8_t funct, uint64_t xs1, uint64_t xs2) {

  uint64_t res = 42;
  fprintf(stderr, "qio_exec(0x%x,0x%llx,0x%llx) -> 0x%llx\n", (unsigned int)funct, xs1, xs2, res);

  // use callbacks to get program counter and the current instruction
  uint64_t pc = g_callbacks.get_reg(g_instance, 0);
  fprintf(stderr, "\tpc=%llx\n", pc);

  return res;
}

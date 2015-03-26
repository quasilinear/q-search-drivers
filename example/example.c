
#include <stdio.h>
#include <qio.h>
#include <inttypes.h>

static qio_callbacks_t* callbacks;

// upon initialization, callbacks for later
void qio_init(qio_callbacks_t* _callbacks) {
  fprintf(stderr, "qio_init(%p,...)\n", _callbacks);
  callbacks = _callbacks;
}

// upon execution of extended instruction, just return 42
uint64_t qio_exec(uint8_t funct, uint64_t xs1, uint64_t xs2) {

  uint64_t res = 42;
  fprintf(stderr, "qio_exec(0x%"PRIx8",0x%016"PRIx64",0x%016"PRIx64") -> 0x%016"PRIx64"\n", funct, xs1, xs2, res);

  // use callbacks to get program counter and the current instruction
  uint64_t pc = callbacks->get_pc();
  fprintf(stderr, "\tpc=0x%016"PRIx64"\n", pc);
  uint32_t insn = callbacks->get_insn(pc);
  fprintf(stderr, "\tinsn=0x%08"PRIx32"\n", insn);

  return res;
}

QIO=../../riscv-isa-sim/q_ext/
INCLUDES = -I$(QIO)

all: tracefile.so

%.so: %.c $(QIO)/qio.h
	$(CC) $< $(INCLUDES) -shared -o $@

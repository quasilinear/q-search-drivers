QIO=../../riscv-isa-sim/q_ext/
INCLUDES = -I$(QIO)

all: example.so

%.so: %.c $(QIO)/qio.h
	$(CC) $< $(INCLUDES) -shared -o $@

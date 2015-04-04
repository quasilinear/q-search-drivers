QIO=../../riscv-isa-sim/q_ext/
INCLUDES = -I$(QIO)
PYTHON_STUFF=$(shell python-config --cflags) $(shell python-config --libs)

all: pydriver.so

%.so: %.c $(QIO)/qio.h
	$(CC) $< $(INCLUDES) $(PYTHON_STUFF) -shared -o $@

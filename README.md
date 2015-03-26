# q-search-drivers

Some programs that implement the probabilistic semantics of the Q instruction
set extension, more or less.

To build these driver programs, you need to know the path to `qio.h` (which
defines the driver interface and available callbacks). You will also need a
compiled extension to `spike` (called `libq_ext.so` last time I checked). You
should define the `Q_DRIVER` environment variable so your driver can be found
and loaded by `spike`.

## Example Usage

    cd example
    make
    Q_DRIVER=example.so spike --extlib=/path/to/libq_ext.so --extension=q_rocc pk some.rv

If `some.rv` is a RISC-V binary making some use of Q instructions, this will
execute that program with the policy of returning 42 in response to every Q
instruction.

## Drivers So far

- example: always returns 42
- tracefile: returns hex-coded values from the file named by Q\_TRACE

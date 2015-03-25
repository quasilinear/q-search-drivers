# q-search-drivers

Some programs that implement the probabilistic semantics of the Q instruction
set extension, sorta.

## Usage

    cd example
    make example.so
    Q_DRIVER=example.so spike --extlib=/path/to/libq_rocc.so --extension=q_rocc pk some.rv


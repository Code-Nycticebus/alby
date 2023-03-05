# alby

alby is a low level virtual machine that tries to simulate cpu instructions. 
These instructions are very similar to those of x86_64 assembly.
I use my own buildsystem to build this.


## Quickstart

### pybuildc
´´´bash
pybuildc build --release
.build/bin/alby examples/fib.ergot -c
.build/bin/alby fib.lsd -r
´´´



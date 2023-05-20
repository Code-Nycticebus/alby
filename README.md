# alby

alby is a low level virtual machine that interpretes bytecode and simulates cpu instructions. These instructions are very similar to those of x86_64 assembly.
I use [pybuildc], my own buildsystem to build this.


### Usage
First compile the example file into bytecode.
```terminal
alby examples/fib.lsd -c ./fib
```

Then run the file with the interpreter.
```terminal
alby ./fib
```

[pybuildc]: https://github.com/Code-Nycticebus/pybuildc

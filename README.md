# Fred

Docs can be found on [esolangs.org](https://esolangs.org/wiki/Fred)

NEWS:
Fred is able to simulate a 3-cell BF, and so is now proved to be turing-complete.

## Useage
To start, run `python client.py`. In the interpreter, simply type the path of the program you want to execute, and Fred will execute it. For example:
```
fred> myprogram.fred
```

## Modules
This implementation of Fred adds support for 'modules' - imported programs that add extra functionality to your program. To include a module, simply put `@module-file-path` in your program file.

## Comments
The definition loader will ignore all **lines** beginning with `#`.

## Debugging
This implementation has two words (that do not affect the stack at all) for debugging. `debug.dump` prints the entire stack, while `debug.log` prints the top element.

## Compilation
To (very buggily) compile your Fred programs to C, compile fredc with cabal, and run `fredc <input-file> <output-file>` on your compile-friendly `.fred` file.

A Fred project is 'compile-friendly' (this will probably change to become less restrictive in the future) if it:
- has a flat file structure
- references the `std.fmod` module (see [esolangs docs](https://esolangs.org/wiki/Fred) or `example.fred`)
- finishes `main` with a call to `end` (this will output the stack in reverse order)
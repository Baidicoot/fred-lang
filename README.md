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
# Fred

Docs can be found on [esolangs.org](https://esolangs.org/wiki/Fred)

## Useage
To start, run `python client.py`. In the interpreter, simply type the path of the program you want to execute, and Fred will execute it. For example:
```
fred> myprogram.fred
```

## Modules
This implementation of Fred adds support for 'modules' - imported programs that add extra functionality to your program. To include a module, simply put `@module-file-path` in your program file.
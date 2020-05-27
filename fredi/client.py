import eval
import std
from os import chdir, getcwd
from os.path import dirname, basename

def loadPrg(name, loaded):
    old = getcwd()
    scope = {}
    if name in loaded:
        return scope
    loaded.append(basename(name))
    with open(name, "r") as file:
        thisdir = dirname(name)
        if thisdir != '':
            chdir(thisdir)
        for l in file.readlines():
            line = l.lstrip().rstrip()
            if line == "" or line[0] == "#":
                continue
            elif line[0] == "@":
                module = loadPrg(line[1:], loaded)
                for k in module:
                    if k in scope:
                        raise Exception("merge error", k)
                    scope[k] = module[k]
            else:
                s, d = line.split(":")
                symbol = s.rstrip()
                defn = d.lstrip().split()
                scope[symbol] = defn
    chdir(old)
    return scope

def runFile(name):
    loaded = []
    scope = loadPrg(name, loaded)
    print("loaded", len(loaded), "modules:", end=" ")
    for m in loaded:
        print(m, end=" ")
    print("\n")
    state = eval.State(scope["main"], scope, eval.ops)
    while True:
        try:
            state.step()
        except Exception as err:
            print(err.args[0])
            return
        except eval.ProgramEnd:
            for e in state.stack:
                if hasattr(e, "log"):
                    print(e.log, end=" ")
                else:
                    print(e.type, end=" ")
            print("")
            return

while True:
    try:
        print("fred> ", end="")
        data = input().rstrip()
        if (data == "exit"):
            break
    except KeyboardInterrupt:
        print("\ntype `exit` to exit")
        continue
    try:
        runFile(data)
    except KeyboardInterrupt:
        print("\nhalted")
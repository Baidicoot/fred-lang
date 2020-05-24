import eval
import std

def loadPrg(name, loaded):
    scope = {}
    if name in loaded:
        return scope
    loaded.append(name)
    with open(name, "r") as file:
        for l in file.readlines():
            line = l.lstrip().rstrip()
            if line == "":
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
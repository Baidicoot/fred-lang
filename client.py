import eval
import std

def loadPrg(name):
    scope = {}
    with open(name, "r") as file:
        for l in file.readlines():
            line = l.lstrip().rstrip()
            if line == "":
                continue
            s, d = line.split(":")
            symbol = s.rstrip()
            defn = d.lstrip().split()
            scope[symbol] = defn
    return scope

def runFile(name):
    scope = loadPrg(name)
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
import eval

@eval.builtin
def call(state):
    a = state.stack.pop()
    eval.assertType(a, "symbol")
    if a.log in state.ops:
        state.ops[a.log](state)
    elif a.log in state.scope:
        state.ins.call(state.scope[a.log])
    else:
        raise Exception("reference error")

@eval.builtin
def drop(state):
    state.stack.pop()

@eval.builtin
def dup(state):
    state.stack.append(state.stack[-1])

@eval.builtin
def pair(state):
    b = state.stack.pop()
    a = state.stack.pop()
    state.stack.append(eval.ProdType(a, b))

@eval.builtin
def uncons(state):
    pair = state.stack.pop()
    eval.assertType(pair, "pair")
    state.stack.append(pair.snd)
    state.stack.append(pair.fst)

@eval.builtin
def reorder(state):
    pair = state.stack.pop()
    eval.assertType(pair, "pair")
    a = pair.fst
    bc = pair.snd
    eval.assertType(bc, "pair")
    b = bc.fst
    c = bc.snd
    state.stack.append(eval.ProdType(b, eval.ProdType(a, c)))

@eval.builtin("debug.dump")
def dump(state):
    for i in state.stack:
        if hasattr(i, "log"):
            print(i.log, end=" ")
        else:
            print(i.type, end=" ")
    print("")

@eval.builtin("debug.log")
def log(state):
    if hasattr(state.stack[-1], "log"):
        print(state.stack[-1].log)
    else:
        print(state.stack[-1].type)

@eval.builtin("debug.newline")
def nl(state):
    print("")
from copy import deepcopy

ops = {}

class Type:
    def __init__(self):
        self.type = "bottom"

class SymbolType(Type):
    def __init__(self, string):
        self.log = string
        self.type = "symbol"

class ProdType(Type):
    def __init__(self, a, b):
        self.type = "pair"
        self.fst = a
        self.snd = b
        if (hasattr(a, "log") and hasattr(b, "log")):
            self.log = "(" + a.log + ", " + b.log + ")"

def assertType(obj, string):
    if (obj.type != string):
        raise Exception("typeerror")

def builtin(fn):
    global ops
    if (callable(fn)):
        ops[fn.__name__] = fn
        return fn
    elif (type(fn) is str):
        def internal(func):
            ops[fn] = func
            return func
        return internal

class ProgramEnd(BaseException):
    def __init__(self):
        pass

class Ins:
    def __init__(self, ins):
        self.ins = [i for i in ins]
        self.parent = None
    
    def clone(self):
        if self.parent is None:
            return Ins(self.ins)
        else:
            o = Ins(self.ins)
            o.parent = self.parent.clone()
            return o
    
    def nextIns(self):
        if (self.ins != []):
            return self.ins.pop(0)
        else:
            if self.parent is None:
                raise ProgramEnd()
            else:
                self.ins = self.parent.ins
                self.parent = self.parent.parent
                return self.nextIns()
    
    def call(self, ins):
        self.parent = self.clone()
        self.ins = [i for i in ins]

class State:
    def __init__(self, ins, scope, ops):
        self.stack = []
        self.ins = Ins(ins)
        self.scope = scope
        self.ops = ops
    
    def step(self):
        ins = self.ins.nextIns()
        if ins in self.ops:
            self.ops[ins](self)
        elif ins in self.scope:
            self.ins.call(self.scope[ins])
        elif ins[0] == '\'':
            self.stack.append(SymbolType(ins[1:]))
        else:
            raise Exception("reference error")
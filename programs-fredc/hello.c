#include <stdio.h>
#include <string.h>
#include <stdint.h>
int64_t* stack[65536];
uint64_t stackptr;
void _0();void _1();void _2();void _3();void _4();void _5();void _6();void _7();void _8();void _9();void _10();void _11();void _12();void _13();void _14();void _15();void _16();void _17();void _18();void _19();void _20();void _21();void _22();void _23();void _24();void _25();void _26();void _27();void _28();void _29();void _30();void _31();void _32();void _33();void _34();void _35();void _36();void _37();void _38();void _39();void _40();void _41();void _42();void main();void true();void start();void end();void call();void drop();void dup();void pair();void uncons();void reorder();void println();void ascii();
struct LabelPtr {
   void (*fn)(void);
   char* label;
};
struct LabelPtr labels[] = {{_0,"*"},{_1,"+"},{_2,"10"},{_3,"100"},{_4,"108"},{_5,"111"},{_6,"114"},{_7,"2"},{_8,"20"},{_9,"3"},{_10,"32"},{_11,"33"},{_12,"44"},{_13,"5"},{_14,"72"},{_15,"87"},{_16,"A"},{_17,"B"},{_18,"C"},{_19,"D"},{_20,"E"},{_21,"F"},{_22,"G"},{_23,"H"},{_24,"I"},{_25,"J"},{_26,"K"},{_27,"L"},{_28,"M"},{_29,"O"},{_30,"Q"},{_31,"R"},{_32,"S"},{_33,"T"},{_34,"U"},{_35,"V"},{_36,"W"},{_37,"X"},{_38,"Y"},{_39,"Z"},{_40,"a"},{_41,"b"},{_42,"c"},{main,"main"},{true,"true"},{start,"start"},{end,"end"},{call,"call"},{drop,"drop"},{dup,"dup"},{pair,"pair"},{uncons,"uncons"},{reorder,"reorder"},{println,"println"},{ascii,"ascii"}};

#define PUSH_LABEL(label) { \
stack[stackptr] = (uint64_t)label; \
stackptr++; \
stack[stackptr] = 2; \
stackptr++; \
}

#define SWAP(size0, size1) { \
memmove(stack + stackptr, stack + stackptr - size0, size0 * sizeof(uint64_t)); \
memmove(stack + stackptr - size1, stack + stackptr - size0 - size1, size1 * sizeof(uint64_t)); \
memmove(stack + stackptr - size0 - size1, stack + stackptr, size0 * sizeof(uint64_t)); \
}

void call() {
    stackptr -= 2;
    void (*funptr)(void) = stack[stackptr];
    funptr();
}

void drop() {
    uint64_t n = stack[stackptr-1];
    stackptr -= n;
}

void dup() {
    size_t n = stack[stackptr-1];
    memcpy(stack + stackptr, stack + stackptr - n, n * sizeof(uint64_t));
    stackptr += n;
}

void pair() {
    size_t size0 = stack[stackptr-1];
    size_t size1 = stack[stackptr-size0-1];
    stack[stackptr] = size0 + size1 + 1;
    stackptr++;
}

#define UNPAIR { \
stackptr--; \
}

void uncons() {
    UNPAIR;
    size_t size0 = stack[stackptr-1];
    size_t size1 = stack[stackptr-size0-1];
    SWAP(size0, size1);
}

void reorder() {
    stackptr -= 2; /* fast unpair twice */
    size_t size0 = stack[stackptr-1];
    size_t size1 = stack[stackptr-size0-1];
    size_t size2 = stack[stackptr-size0-size1-1];
    memmove(stack + stackptr - size0 + size1, stack + stackptr - size0, size0 * sizeof(uint64_t)); /* move top element out of harm's way */
    stackptr -= size0;
    SWAP(size1, size2);
    stackptr += size0;
    memmove(stack + stackptr - size0, stack + stackptr - size0 + size1, size0 * sizeof(uint64_t)); /* move top element back into position */
    pair();
    pair();
}

void (*getLabel(char* name))(void) {
    for (int i = 0; i < sizeof(labels)/sizeof(labels[0]); i++) {
        if (strcmp(name, labels[i].label) == 0) {
            printf("found: %s, at: %x\n", labels[i].label, labels[i].fn);
            return labels[i].fn;
        }
    }
}

//void (*trueFn)(void);
void start() {
    //trueFn = getLabel("true");
}

int countNat() {
    int c = 0;
    UNPAIR;
    while (stack[stackptr-2] != true) {
        drop();
        UNPAIR;
        c++;
    }
    drop();
    drop();
    return c;
}

void ascii() {
    int c = countNat();
    putchar(c);
}

void print_dropStackitem() {
    uint64_t s = stack[stackptr-1];
    if (s == 2) {
        for (int i = 0; i < sizeof(labels)/sizeof(labels[0]); i++) {
            if (stack[stackptr-2] == labels[i].fn) {
                printf("%s", labels[i].label);
                break;
            }
        }
        drop();
    } else {
        UNPAIR;
        printf("{");
        print_dropStackitem();
        printf(" ");
        print_dropStackitem();
        printf("}");
    }
}

void println() {
    dup();
    print_dropStackitem();
    printf("\n");
}

void end() {
    while (stackptr > 0) {
        print_dropStackitem();
        printf(" ");
    }
    printf("\n");
}
void _0() {PUSH_LABEL(_41);PUSH_LABEL(_42);_39();}
void _1() {_21();_36();_31();PUSH_LABEL(_40);_23();_28();_20();}
void _2() {_13();_7();_0();}
void _3() {_13();_13();_0();_7();_0();_7();_0();}
void _4() {_3();_7();_7();_0();_7();_0();_1();}
void _5() {_4();_9();_1();}
void _6() {_5();_9();_1();}
void _7() {_32();_33();_33();}
void _8() {_2();_7();_0();}
void _9() {_32();_33();_33();_33();}
void _10() {_8();_2();_1();_7();_1();}
void _11() {_10();_33();}
void _12() {_8();_7();_0();_7();_7();_0();_1();}
void _13() {_32();_33();_33();_33();_33();_33();}
void _14() {_8();_9();_0();_2();_1();_7();_1();}
void _15() {_14();_2();_1();_13();_1();}
void _16() {pair();}
void _17() {uncons();}
void _18() {reorder();}
void _19() {call();}
void _20() {drop();}
void _21() {dup();}
void _22() {ascii();}
void _23() {_16();_17();}
void _24() {_17();_23();}
void _25() {_16();_16();_18();_24();_17();}
void _26() {_19();_19();}
void _27() {_25();_26();}
void _28() {PUSH_LABEL(_29);_23();_26();}
void _29() {}
void _30() {_23();_20();}
void _31() {PUSH_LABEL(_30);PUSH_LABEL(true);_25();_19();}
void _32() {PUSH_LABEL(_29);PUSH_LABEL(true);_16();}
void _33() {PUSH_LABEL(_30);_16();}
void _34() {_20();_32();}
void _35() {_24();PUSH_LABEL(_34);_23();PUSH_LABEL(_29);_23();_26();}
void _36() {_17();_20();}
void _37() {_20();_24();_20();_19();}
void _38() {_35();_23();_24();_23();_20();_19();}
void _39() {_16();_23();_21();_36();PUSH_LABEL(_37);PUSH_LABEL(_38);_27();}
void _40() {_23();_33();_23();_35();_21();_36();_31();PUSH_LABEL(_40);_23();_28();}
void _41() {_34();}
void _42() {_23();_21();_25();_0();_1();}
void main() {start();_14();_22();_3();_33();_22();_4();_22();_4();_22();_5();_22();_12();_22();_10();_22();_15();_22();_5();_22();_6();_22();_4();_22();_3();_22();_11();_22();end();}
void true() {_20();}
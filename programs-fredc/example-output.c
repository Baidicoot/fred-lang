#include <stdio.h>
#include <string.h>
#include <stdint.h>
int64_t* stack[65536];
uint64_t stackptr;
void main();void _0();void _1();void _2();void _3();void _4();void _5();void _6();void _7();void _8();void _9();void _10();void _11();void _12();void _13();void _14();void _15();void _16();void _17();void _18();void _19();void _20();void _21();void _22();void _23();void _24();void _25();void _26();void _27();void _28();void _29();void _30();void _31();void _32();void _33();void _34();void _35();void _36();void _37();void _38();void _39();void _40();void _41();void _42();void _43();void _44();void _45();void _46();void _47();void _48();void _49();void _50();void _51();void _52();void _53();void _54();void _55();void start();void end();void call();void drop();void dup();void pair();void uncons();void reorder();void println();void ascii();
struct LabelPtr {
   void (*fn)(void);
   char* label;
};
struct LabelPtr labels[] = {{main,"main"},{_0,"*"},{_1,"*_case_succ"},{_2,"*_case_zero"},{_3,"+"},{_4,"_constzero"},{_5,"add_internal"},{_6,"case_succ"},{_7,"case_zero"},{_8,"iszero"},{_9,"pred"},{_10,"rec_nat"},{_11,"succ"},{_12,"zero"},{_13,"0"},{_14,"1"},{_15,"10"},{_16,"11"},{_17,"12"},{_18,"13"},{_19,"14"},{_20,"15"},{_21,"16"},{_22,"17"},{_23,"18"},{_24,"19"},{_25,"2"},{_26,"20"},{_27,"21"},{_28,"22"},{_29,"23"},{_30,"24"},{_31,"25"},{_32,"3"},{_33,"4"},{_34,"5"},{_35,"6"},{_36,"7"},{_37,"8"},{_38,"9"},{_39,"()"},{_40,"branch"},{_41,"cons"},{_42,"false"},{_43,"if"},{_44,"ifr"},{_45,"not"},{_46,"rot3"},{_47,"rot4"},{_48,"rot5"},{_49,"swap"},{_50,"true"},{_51,"unit"},{_52,"unpair"},{_53,"unrot3"},{_54,"unrot4"},{_55,"unrot5"},{start,"start"},{end,"end"},{call,"call"},{drop,"drop"},{dup,"dup"},{pair,"pair"},{uncons,"uncons"},{reorder,"reorder"},{println,"println"},{ascii,"ascii"}};

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

void (*trueFn)(void);
void start() {
    trueFn = getLabel("true");
}

int countNat() {
    int c = 0;
    UNPAIR;
    while (stack[stackptr-2] != trueFn) {
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
void main() {start();_23();_33();_0();ascii();_27();_34();_0();ascii();_16();_32();_0();ascii();end();}
void _0() {PUSH_LABEL(_2);PUSH_LABEL(_1);_10();}
void _1() {_49();dup();_46();_0();_3();}
void _2() {_4();}
void _3() {dup();_8();_45();PUSH_LABEL(_5);_49();_40();drop();}
void _4() {drop();_12();}
void _5() {_49();_11();_49();_9();dup();_8();_45();PUSH_LABEL(_5);_49();_40();}
void _6() {_9();_49();_52();_49();drop();call();}
void _7() {drop();_52();drop();call();}
void _8() {uncons();drop();}
void _9() {_52();PUSH_LABEL(_4);_49();PUSH_LABEL(_51);_49();_43();}
void _10() {pair();_49();dup();_8();PUSH_LABEL(_7);PUSH_LABEL(_6);_44();}
void _11() {PUSH_LABEL(_42);pair();}
void _12() {_39();PUSH_LABEL(_50);pair();}
void _13() {_12();}
void _14() {_13();_11();}
void _15() {_38();_11();}
void _16() {_15();_11();}
void _17() {_16();_11();}
void _18() {_17();_11();}
void _19() {_18();_11();}
void _20() {_19();_11();}
void _21() {_20();_11();}
void _22() {_21();_11();}
void _23() {_22();_11();}
void _24() {_23();_11();}
void _25() {_14();_11();}
void _26() {_24();_11();}
void _27() {_26();_11();}
void _28() {_27();_11();}
void _29() {_28();_11();}
void _30() {_29();_11();}
void _31() {_30();_11();}
void _32() {_25();_11();}
void _33() {_32();_11();}
void _34() {_33();_11();}
void _35() {_34();_11();}
void _36() {_35();_11();}
void _37() {_36();_11();}
void _38() {_37();_11();}
void _39() {PUSH_LABEL(_39);}
void _40() {PUSH_LABEL(_51);_49();_43();}
void _41() {_49();pair();}
void _42() {_49();drop();}
void _43() {call();call();}
void _44() {_46();_43();}
void _45() {PUSH_LABEL(_42);PUSH_LABEL(_50);_46();call();}
void _46() {pair();pair();reorder();_52();uncons();}
void _47() {pair();pair();pair();reorder();_52();reorder();_52();uncons();}
void _48() {pair();pair();pair();pair();reorder();_52();reorder();_52();reorder();_52();uncons();}
void _49() {pair();uncons();}
void _50() {drop();}
void _51() {}
void _52() {uncons();_49();}
void _53() {_46();_46();}
void _54() {_47();_47();_47();}
void _55() {_48();_48();_48();_48();}
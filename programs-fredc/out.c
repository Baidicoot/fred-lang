#include <stdio.h>
#include <string.h>
#include <stdint.h>
uint64_t stack[65536];
uint64_t stackptr;
void main();
void _0();
void _1();
void _2();
void _3();
void _4();
void _5();
void _6();
void _7();
void _8();
void _9();
void _10();
void _11();
void _12();
void _13();
void _14();
void _15();
void _16();
void _17();
void _18();
void _19();
void _20();
void _21();
void end();
void call();
void drop();
void dup();
void pair();
void uncons();
void reorder();
void print_dropStackitem();
uint64_t labels[] = {main,"main",_0,"_constzero",_1,"add",_2,"add_internal",_3,"branch",_4,"cons",_5,"false",_6,"if",_7,"iszero",_8,"not",_9,"pred",_10,"rot3",_11,"rot4",_12,"rot5",_13,"succ",_14,"swap",_15,"true",_16,"unit",_17,"unpair",_18,"unrot3",_19,"unrot4",_20,"unrot5",_21,"zero",end,"end",call,"call",drop,"drop",dup,"dup",pair,"pair",uncons,"uncons",reorder,"reorder",print_dropStackitem,"print.dropStackitem",};

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

void print_dropStackitem() {
    uint64_t s = stack[stackptr-1];
    if (s == 2) {
        for (int i = 0; i < sizeof(labels)/sizeof(labels[0]); i+=2) {
            if (stack[stackptr-2] == labels[i]) {
                printf("%s", labels[i+1]);
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
        printf("} ");
    }
}

void end() {
    while (stackptr > 0) {
        print_dropStackitem();
        printf(" ");
    }
    printf("\n");
}void main() {
_21();
_13();
_13();
_13();
_21();
_13();
_13();
_1();
end();

}
void _0() {
drop();
_21();

}
void _1() {
dup();
_7();
_8();
PUSH_LABEL(_2);
_14();
_3();
drop();

}
void _2() {
_14();
_13();
_14();
_9();
dup();
_7();
_8();
PUSH_LABEL(_2);
_14();
_3();

}
void _3() {
PUSH_LABEL(_16);
_14();
_6();

}
void _4() {
_14();
pair();

}
void _5() {
_14();
drop();

}
void _6() {
call();
call();

}
void _7() {
uncons();
drop();

}
void _8() {
PUSH_LABEL(_5);
PUSH_LABEL(_15);
_10();
call();

}
void _9() {
_17();
PUSH_LABEL(_0);
_14();
PUSH_LABEL(_16);
_14();
_6();

}
void _10() {
pair();
pair();
reorder();
_17();
uncons();

}
void _11() {
pair();
pair();
pair();
reorder();
_17();
reorder();
_17();
uncons();

}
void _12() {
pair();
pair();
pair();
pair();
reorder();
_17();
reorder();
_17();
reorder();
_17();
uncons();

}
void _13() {
PUSH_LABEL(_5);
pair();

}
void _14() {
pair();
uncons();

}
void _15() {
drop();

}
void _16() {

}
void _17() {
uncons();
_14();

}
void _18() {
_10();
_10();

}
void _19() {
_11();
_11();
_11();

}
void _20() {
_12();
_12();
_12();
_12();

}
void _21() {
PUSH_LABEL(_16);
PUSH_LABEL(_15);
pair();

}

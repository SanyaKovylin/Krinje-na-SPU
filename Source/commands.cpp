#define NDEBUG 100
#include <TXLib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/stat.h>
#include <windows.h>
#include <conio.h>

#include "commands.h"
#include "runheader.h"

template <typename T>
inline T val (T value) {return StackCheck(value);}

// template <typename S>
// inline S paste(S value) {}

#define val(value) ( value )
#define verifiedby(what) || val(what)
#define IS_CMD(cmd) (stricmp(command, SkipCmd(#cmd)) == 0)

int to_int(char *val, int *elem);
int to_d(char *val, double *elem);
size_t Read (const char *src, char **Buffer);
int IsZero(double el);
void get_arg(cond_t *cons, vtype **cell);
void pr_bin(void* el, int size);

#define INIT_EL1_EL2_ANS vtype el1 = (vtype) DefaultTypeValue;\
    vtype el2 = (vtype) DefaultTypeValue;\
    vtype ans = (vtype) DefaultTypeValue;\
    GetTwoValues(maincons->Buffer, &el1, &el2);

#define INIT_EL1_EL2 vtype el1 = (vtype) DefaultTypeValue;\
    vtype el2 = (vtype) DefaultTypeValue;\
    GetTwoValues(maincons->Buffer, &el1, &el2);

#define PUSH_ANS(ans)  StackPush(maincons->Buffer, &ans) verifiedby(maincons->Buffer);

#define GET_ELEM vtype elem = DefaultTypeValue;\
    StackPop (maincons->Buffer, &elem) verifiedby(maincons->Buffer);

#define GET_VAL int val = 0;\
    memcpy(&val, maincons->Ips + maincons->ip, sizeof(int));\
    maincons->ip += sizeof(int);

FILE* fllog = fopen("Regs.log", "w");

void Run(const char* src){
    Stack stack = {};
    StackCtor(&stack, DefaultSize, sizeof(vtype), 'p', print_d);

    Stack calls = {};
    StackCtor(&calls, DefaultSize, sizeof(int), 'e', print_int);

    cond_t RunConditions {
            .Buffer = &stack,
            .Calls = &calls,
            .RAM =  (vtype*) calloc (LenRam, sizeof(vtype)),
            .Regs = (vtype*) calloc (nreg  , sizeof(vtype)),
            .Ips = NULL,
            .ip = 0,
            .decodefunc = castfromstr,
            .tocontinue = true,
            .cmd = {},
            .ipm = 0,
        };

    size_t nip = Read(src, &RunConditions.Ips);

    // Parser(src, &RunConditions.Ips, &nip);

    RunConditions.ipm = nip;
    while (RunConditions.tocontinue){

        memcpy(&RunConditions.cmd, &RunConditions.Ips[RunConditions.ip], 1);
        RunConditions.ip += cmdsize;
        // printf("ip - %d, cmd - %d, ico %d\n", RunConditions.ip, RunConditions.cmd.cmd,RunConditions.cmd.ico);

        for (int i = 0; i < ncmds; i++){
            if (RunConditions.cmd.cmd == GetFunc[i].command){
                GetFunc[i].cmdfunc(&RunConditions);
            }
        }
        // DUMP(&stack);
    }
}

void Parser (const char *src, char **Ips, size_t *ordn){

    char *Buffer = NULL;
    size_t len = Read(src, &Buffer);

    *(ordn) = len;
    *(Ips) = Buffer;
}

size_t Read (const char *src, char **Buffer) {

    assert (src    != NULL);
    assert (Buffer != NULL);

    int fo = open (src, O_RDONLY | O_BINARY);

    struct stat st = {};

    stat (src, &st);
    _off_t readlen = st.st_size;

    *Buffer = (char*) calloc (readlen + 1, sizeof (char));

    size_t lenbuf = read (fo, *Buffer, readlen);
    return lenbuf;
}

int to_int(char *val, int *elem){
    char* end;
    *elem = (int) strtod(val, &end);
    return 0;
}

int to_d(char *val, double *elem){
    char* end;
    *elem = strtod(val, &end);
    return 0;
}

void SetCastFunc(int (*castfunc) (char *str, vtype *val)){
    castfromstr = castfunc;
}

const double eps = 10e-6;
int IsZero(vtype el){
    return abs(el) < eps;
}

static err_t GetTwoValues(Stack *src, vtype *elem1, vtype *elem2){

    StackPop(src, elem1) verifiedby(src);
    StackPop(src, elem2) verifiedby(src);
    return CAT_CONDITION;
}

//=================================================================================
//TODO: Checker!!!

#define COMMAND(name, num, gets,...)\
    static err_t func_##name(cond_t *maincons){\
        __VA_ARGS__\
        return CAT_CONDITION;\
    }

#include "gena.h"

#undef COMMAND

void get_arg(cond_t *Conditions, vtype **cell){

    static vtype value = 0;
    value = 0;
    vtype cnst = 0;
    int jump_ip = 0;
    int registr = 0;
    com_t cmd = Conditions->cmd;

    if (cmd.ico) {

        if (CMD_JMP <= cmd.cmd && cmd.cmd <= CMD_JNE){//TODO

            memcpy(&jump_ip, Conditions->Ips + Conditions->ip, sizeof(int));
            Conditions->ip += sizeof(int);
            value += cnst;
        }
        else{

            memcpy(&cnst, Conditions->Ips + Conditions->ip, sizeof(vtype));
            Conditions->ip += sizeof(vtype);
            value += cnst;
        }
    }

    if (cmd.reg) {

        memcpy(&registr, Conditions->Ips + Conditions->ip, sizeof(char));
        Conditions->ip += sizeof(char);
        value += Conditions->Regs[registr - 1];
    }

    if (cmd.mem) {*(cell) = &Conditions->RAM[(int) value];
    }
    else if      (cmd.ico) *(cell) = &value;
    else        *(cell) = Conditions->Regs + registr - 1;
}

void pr_bin(void* el, int size){
    for (int i = 0; i < size; i++){
        int x = 7;
        uint8_t y = *((char*) el + i);
        while (x >= 0){
            printf("%d", (y >> x) & 1);
            x--;
        }
    }
}

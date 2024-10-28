#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

#include "cmd.h"

vtype DefaultTypeValue = 0;

const size_t RegLen = sizeof("ax");
const int nreg = 4;

const int withcolor = 0;
const int LenRam = 72*160*(withcolor + 1);
// const int LenRam = 200*(withcolor + 1);
const int RamRow = 160*(withcolor + 1);
// const int RamRow = 20*(withcolor + 1);

typedef struct Conditions {
    Stack *Buffer;
    Stack *Calls;
    vtype *RAM;
    vtype *Regs;
    char *Ips;
    size_t ip;
    int (*decodefunc) (char *LineWithValue, vtype *elem);
    bool tocontinue;
    com_t cmd;
    size_t ipm;
} cond_t;

struct CmdNum {
    cmd_t command;
    err_t (*cmdfunc)(cond_t *MainConditions);
    int gets_int;
};

#define COMMAND(name, num, gets, ...)\
    static err_t func_##name (cond_t *maincons);

    #include "gena.h"

#undef COMMAND

#define COMMAND(name, num, gets_vtype, ...)\
    CMD_##name , func_##name, (1 - gets_vtype),

    struct CmdNum GetFunc[] {
        #include "gena.h"
    };

#undef COMMAND

void print_int(FILE *fw, void* elem);

int (*castfromstr) (char *str, vtype* val) = NULL;

#ifndef NPRINT
    #define FUNC_PRINT(value) printf("%s value = %d\n", __func__, value);
#else
    #define FUNC_PRINT(value) ;
#endif
#endif //COMMANDS_H_INCLUDED

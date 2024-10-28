#ifndef CMD_H_INCLUDED
#define CMD_H_INCLUDED

#include <stdio.h>
#include "..\HellStack/Include/stack.h"

const int vsize   = 8;
typedef double vtype;
const int cmdsize = 1;
const int scale = 2;

#define COMMAND(name, num, ...)\
    CMD_##name = num,

typedef enum Commands {
     #include "gena.h"
     CMD_COUNT,
} cmd_t;

#undef COMMAND

const int ncmds = CMD_COUNT-1;

typedef enum CompilerErrors {
    CAT_CONDITION         = 0, //MEOW !!! MURRRRRRRRR !!!
    IN_YOUR_FUCKING_STACK = 1, //Debil pisal etot stack
    I_CHTO_MNE_S_THIS_DO  = 2, //check function arguments !!!
    YOUR_READER_ERROR     = 3,
    NE_PODELISH_NA_NICHTO = 4,
    COMANDA_KRINGE        = 5,
    SAM_PRIDUMAL_REG_TAKOY= 6,
    THIS_PAR_IS_INVALID   = 7,
} err_t;

const int MaxCmdSize     =  8;
const int LenOfCmdPhrase =  4;
const int MaxCommands    = 30;

typedef struct Command {
    cmd_t cmd        : 5;
    unsigned int mem : 1;
    unsigned int reg : 1;
    unsigned int ico : 1;
} com_t;

#endif //CMD_H_INCLUDED

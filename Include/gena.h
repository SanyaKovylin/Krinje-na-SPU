// LABEL: COMMAND(name, number, gets_vtype, action)

COMMAND(PUSH, 1, 1,
    {
    vtype *cell = NULL;
    get_arg(maincons, &cell);

    StackPush(maincons->Buffer, cell) verifiedby(maincons->Buffer);
    }
)

COMMAND(OUT, 2, 0,
    {
    vtype topelem = (vtype) DefaultTypeValue;
    StackPop(maincons->Buffer, &topelem) verifiedby(maincons->Buffer);

    printf("YOUR RES :: %g\n", topelem);
    }
)

COMMAND(IN, 3, 0,
    {
    static char *cmd = (char*) calloc (MaxCmdSize, sizeof(char));
    scanf("%s", cmd);

    vtype elem = (vtype) DefaultTypeValue;
    maincons->decodefunc(cmd, &elem) verifiedby(maincons->Buffer);
    StackPush(maincons->Buffer, &elem) verifiedby(maincons->Buffer);
    }
)

COMMAND(DUMP, 4, 0,
    {

    StackDump(maincons->Buffer, "DefaultDump", 0, "DefaultDump");
    printf("ax - %g, bx - %g, cx-%g, dx-%g\n", maincons->Regs[0], maincons->Regs[1],
                                               maincons->Regs[2], maincons->Regs[3]);
    }
)

COMMAND(HLT, 5, 0,
    {
    maincons->tocontinue = false;
    }
)

COMMAND(ADD, 6, 0,
    {
    INIT_EL1_EL2_ANS;
    ans = el1 + el2;
    PUSH_ANS(ans);
    }
)

COMMAND(SUB, 7, 0,
    {
    INIT_EL1_EL2_ANS;
    ans = el2 - el1;
    PUSH_ANS(ans);
    }
)

COMMAND(MUL, 8, 0,
    {
    INIT_EL1_EL2_ANS;
    ans = el1 * el2;
    PUSH_ANS(ans);
    }
)

COMMAND(DIV, 9, 0,
    {
    INIT_EL1_EL2_ANS;
    if (IsZero(el1)){
        printf("division by zero");
        return NE_PODELISH_NA_NICHTO;
    }
    ans = el2 / el1;
    PUSH_ANS(ans);
    }
)

COMMAND(SQRT, 10, 0,
    {
    GET_ELEM;
    elem = sqrt(elem);
    PUSH_ANS(elem);
    }
)

COMMAND(SIN, 11, 0,
    {
    GET_ELEM;
    elem = sin(elem);
    PUSH_ANS(elem);
    }
)

COMMAND(COS, 12, 0,
    {
    GET_ELEM;
    elem = cos(elem);
    PUSH_ANS(elem);
    }
)

COMMAND(JMP, 13, 0,
    {
    GET_VAL;
    maincons->ip = (int) val;
    }
)


COMMAND(JA, 14, 0,
    {
    INIT_EL1_EL2;
    GET_VAL;
    if (el1 > el2){
        maincons->ip = (int) val;
    }
    }
)

COMMAND(JAE, 15, 0,
    {
    INIT_EL1_EL2;
    GET_VAL;
    if (el1 >= el2){
        maincons->ip = (int) val;
    }
    }
)

COMMAND(JB, 16, 0,
    {
    INIT_EL1_EL2;
    GET_VAL;
    if (el1 < el2){
        maincons->ip = (int) val;
    }
    }
)

COMMAND(JBE, 17, 0,
    {
    INIT_EL1_EL2;
    GET_VAL;
    if (el1 <= el2){
        maincons->ip = (int) val;
    }
    }
)

COMMAND(JE, 18, 0,
    {
    INIT_EL1_EL2;
    GET_VAL;
    if (IsZero(el1 - el2)){
        maincons->ip = (int) val;
    }
    }
)

COMMAND(JNE, 19, 0,
    {
    INIT_EL1_EL2;
    GET_VAL;
    if (!IsZero(el1 - el2)){
        maincons->ip = (int) val;
    }
    }
)

COMMAND(CALL, 20, 0,
    {
    GET_VAL;
    StackPush(maincons->Calls, &maincons->ip) verifiedby(maincons->Calls);
    maincons->ip = (int) val;
    }
)

COMMAND(RET, 21, 0,
    {
    int ip = maincons->ip;
    StackPop(maincons->Calls, &ip) verifiedby(maincons->Calls);
    maincons->ip = ip;
    return CAT_CONDITION;
    }
)

COMMAND(POP, 22, 1,
    {
    vtype *cell = NULL;
    get_arg(maincons, &cell);

    StackPop(maincons->Buffer, cell) verifiedby(maincons->Buffer);
    }
)

COMMAND(DRAW, 23, 0,
    {
    char *out = (char *) calloc (LenRam + LenRam/RamRow, sizeof(char));

    for (int i = 0; i*RamRow < LenRam; i++){
        for (int j = 0; j < RamRow; j+=(withcolor+1)){

            out[(1+RamRow)*i + j] = maincons->RAM[RamRow*i + j];
        }
        out[(1+RamRow)*i + RamRow] = '\n';
        // putc('\n', stdout);
    }
    setvbuf(stdout,NULL,_IONBF,NULL);
    puts(out);
    // putc('e', stdout);
    putc('\n', stdout);

    // Sleep(10);
    // txClearConsole();
    return CAT_CONDITION;
    }
)

COMMAND(MEOW, 24, 0,{
        vtype val = 0;
        StackPop(maincons->Buffer, &val) verifiedby(maincons->Buffer);
        int count = (int) val;
        for (int i = 0; i<count; i++){
            printf("MEOW!!!\n");
        }
    }
)

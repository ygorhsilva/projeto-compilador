/*
Tipos de vari�veis
O c�digo abaixo foi escrito por Felipo Soranz e � uma adapta��o
do c�digo original em Pascal escrito por Jack E. Crenshaw em sua
s�rie "Let's Build a Compiler".
Este c�digo � de livre distribui��o e uso.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SYMTBL_SZ 26

char symtbl[SYMTBL_SZ]; /* tabela de s�mbolos */
char look; /* O caracter lido "antecipadamente" (lookahead) */


/* PROGRAMA PRINCIPAL */
int main()
{
    init();
    topdecls();
    match('B');
    newline();
    block();
    dumptable();
    return 0;
}

/* inicializa��o do compilador */
void init()
{
    int i;
    for (i = 0; i < SYMTBL_SZ; i++)
        symtbl[i] = '?';
    nextchar();
    skipwhite();
}

/* l� pr�ximo caracter da entrada em lookahead */
void nextchar()
{
    look = getchar();
}

/* imprime mensagem de erro sem sair */
void error(char *s)
{
    fprintf(stderr, "Error: %s\n", s);
}

/* imprime mensagem de erro e sai */
void fatal(char *s)
{
    error(s);
    exit(1);
}

/* mensagem de erro para string esperada */
void expected(char *s)
{
    fprintf(stderr, "Error: %s expected\n", s);
    exit(1);
}

/* exibe a tabela de s�mbolos */
void dumptable()
{
    int i;
    printf("Symbol table dump:\n");
    for (i = 0; i < SYMTBL_SZ; i++)
        if (symtbl[i] != '?')
            printf("%c = %c\n", i + 'A', symtbl[i]);
}

/* testa operadores de adi��o */
int isaddop(char c)
{
    return (c == '+' || c == '-');
}

/* testa operadores de multiplica��o */
int ismulop(char c)
{
    return (c == '*' || c == '/');
}

/* testa operadores OU */
int isorop(char c)
{
    return (c == '|' || c == '~');
}

/* testa operadores relacionais */
int isrelop(char c)
{
    return (strchr("=#<>", c) != NULL);
}

/* pula caracteres em branco */
void skipwhite()
{
    while (look == ' ' || look == '\t')
        nextchar();
}

/* reconhece uma quebra de linha */
void newline()
{
    if (look == '\n')
        nextchar();
}

/* verifica se look combina com caracter esperado */
void match(char c)
{
    char s[2];
    if (look == c)
        nextchar();
    else
    {
        s[0] = c; /* uma conversao r�pida (e feia) */
        s[1] = '\0'; /* de um caracter em string */
        expected(s);
    }
    skipwhite();
}

/* analisa e traduz um nome (identificador ou palavra-chave) */
char getname()
{
    char name;
    if (!isalpha(look))
        expected("Name");
    name = toupper(look);
    nextchar();
    skipwhite();
    return name;
}

/* analisa e traduz um n�mero inteiro */
char getnum()
{
    char num;
    if (!isdigit(look))
        expected("Integer");
    num = look;
    nextchar();
    skipwhite();
    return num;
}

char unop()
{
    asm_clear();
    return 'w';
}

/* retorna o tipo de um identificador */
char symtype(char name)
{
    return symtbl[name - 'A'];
}

/* verifica se "name" consta na tabela de s�mbolos */
char intable(char name)
{
    return (symtbl[name - 'A'] != '?');
}

/* avisa a respeito de um identificador desconhecido */
void duplicated(char name)
{
    fprintf(stderr, "Error: Duplicated identifier %c\n", name);
    exit(1);
}

/* verifica se um identificador j� foi declarado */
void checkdup(char name)
{
    if (intable(name))
        duplicated(name);
}

/* adiciona novo identificador � tabela de s�mbolos */
void addsymbol(char name, char type)
{
    checkdup(name);
    symtbl[name - 'A'] = type;
}

/* avisa a respeito de uma palavra-chave desconhecida */
void unrecognized(char name)
{
    fprintf(stderr, "Error: Unrecognized keyword %c\n", name);
    exit(1);
}

/* aloca espa�o de armazenamento para vari�vel */
void alloc(char name, char type)
{
    addsymbol(name, type);
    asm_allocvar(name, type);
}

/* analiza e traduz a declara��o de uma vari�vel */
void decl()
{
    char type = look;
    nextchar();
    alloc(getname(), type);
}

/* analiza e traduz as declara��es globais */
void topdecls()
{
    while (look != '.')
    {
        switch (look)
        {
        case 'b':
        case 'w':
        case 'l':
            decl();
            break;
        default:
            unrecognized(look);
            break;
        }
        newline();
    }
}

/* reconhece um tipo de vari�vel v�lido */
int isvartype(char c)
{
    return (c == 'b' || c == 'w' || c == 'l');
}

/* avisa a respeito de um identificador que n�o � vari�vel */
void notvar(char name)
{
    fprintf(stderr, "Error: %c is not a variable\n", name);
    exit(1);
}

/* pega o tipo da vari�vel da tabela de s�mbolos */
char vartype(char name)
{
    char type;
    type = symtype(name);
    if (!isvartype(type))
        notvar(name);
    return type;
}

/* carrega vari�vel */
char loadvar(char name)
{
    char type = vartype(name);
    asm_loadvar(name, type);
    return type;
}

/* armazena vari�vel */
void storevar(char name, char srctype)
{
    char type = vartype(name);
    asm_convert(srctype, vartype);
    asm_storevar(name, vartype);
}

/* aloca espa�o de armazenamento para vari�vel */
void asm_allocvar(char name)
{
    addsymbol(name, 'v');
    printf("%c: dw 0\n", name);
}

/* gera c�digo para armazenar vari�vel de acordo com o tipo */
void asm_storevar(char name, char type)
{
    switch (type)
    {
    case 'b':
        printf("\tmov %c, al\n", name);
        break;
    case 'w':
        printf("\tmov %c, ax\n", name);
        break;
    case 'l':
        printf("\tmov word ptr [%c+2], dx\n", name);
        printf("\tmov word ptr [%c], ax\n", name);
    }
}

/* analisa e traduz uma express�o */
char expression()
{
    char type;
    if (isaddop(look))
        type = unop();
    else
        type = term();
    while (isaddop(look))
    {
        asm_push(type);
        switch (look)
        {
        case '+':
            type = add(type);
            break;
        case '-':
            type = subtract(type);
            break;
        }
    }
    return type;
}

/* analisa e traduz uma atribui��o */
void assignment()
{
    char name, type;
    name = getname();
    match('=');
    type = expression();
    storevar(name, type);
}

/* analisa traduz um bloco de comandos */
void block()
{
    while (look != '.')
    {
        assignment();
        newline();
    }
}

/* gera c�digo para carregar vari�vel de acordo com o tipo */
void asm_loadvar(char name, char type)
{
    switch (type)
    {
    case 'b':
        printf("\tmov al, %c\n", name);
        break;
    case 'w':
        printf("\tmov ax, %c\n", name);
        break;
    case 'l':
        printf("\tmov dx, word ptr [%c+2]\n", name);
        printf("\tmov ax, word ptr [%c]\n", name);
    }
}

/* converte tipo origem para destino */
void asm_convert(char src, char dst)
{
    if (src == dst)
        return;
    if (src == 'b')
        printf("\tcbw\n");
    if (dst == 'l')
        printf("\tcwd\n");
}

/* carrega uma constante no registrador prim�ria */
char loadnum(long val)
{
    char type;
    if (val >= -128 && val <= 127)
        type = 'b';
    else if (val >= -32768 && val <= 32767)
        type = 'w';
    else
        type = 'l';
    asm_loadconst(val, type);
    return type;
}

void asm_loadconst(long val, char type)
{
    switch (type)
    {
    case 'b':
        printf("\tmov al, %d\n", (int) val);
        break;
    case 'w':
        printf("\tmov ax, %d\n", (int) val);
        break;
    case 'l':
        printf("\tmov dx, %u\n", val >> 16);
        printf("\tmov ax, %u\n", val & 0xFFFF);
        break;
    }
}

/* reconhece e traduz uma soma */
char add(char type)
{
    match('+');
    return asm_popadd(type, term());
}

/* reconhece e traduz uma subtra��o */
char subtract(char type)
{
    match('-');
    return asm_popsub(type, term());
}


/* gera c�digo para trocar registradores prim�rio e secund�rio */
void asm_swap(char type)
{
    switch (type)
    {
    case 'b':
        printf("\txchg al, bl\n");
        break;
    case 'w':
        printf("\txchg ax, bx\n");
        break;
    case 'l':
        printf("\txchg ax, bx\n");
        printf("\txchg dx, cx\n");
        break;
    }
}

/* soma valor na pilha com valor no reg. prim�rio */
char asm_pop (char t1, char t2)
{
    char type;
    asm_pop(t1,t2);
    type = asm_sametype(t1, t2, 0);
    switch (type)
    {
    case 'b':
        printf("\tadd al, bl\n");
        break;
    case 'w':
        printf("\tadd ax, bx\n");
        break;
    case 'l':
        printf("\tadd ax, bx\n");
        printf("\tadc dx, cx\n");
        break;
    }
    return type;
}

/* subtrai do valor da pilha o valor no reg. prim�rio */
char asm_popsub(char t1, char t2)
{
    char type;
    asm_pop(t1,t2);
    type = asm_sametype(t1, t2, 0);
    switch (type)
    {
    case 'b':
        printf("\tsub al, bl\n");
        break;
    case 'w':
        printf("\tsub ax, bx\n");
        break;
    case 'l':
        printf("\tsub ax, bx\n");
        printf("\tsbb dx, cx\n");
        break;
    }
    return type;
}

/* faz a promo��o dos tipos dos operandos e inverte a ordem dos mesmos se necess�rio */
char asm_sametype(char t1, char t2, int ord_matters)
{
    int swaped = 0;
    int type = t1;
    if (t1 != t2)
    {
        if ((t1 == 'b') || (t1 == 'w' && t2 == 'l'))
        {
            type = t2;
            asm_swap(type);
            asm_convert(t1, t2);
            swaped = 1;
        }
        else
        {
            type = t1;
            asm_convert(t2, t1);
        }
    }
    if (!swaped && ord_matters)
        asm_swap(type);
    return type;
}

/* analisa e traduz um fator matem�tico */
char factor()
{
    char type;
    if (look == '(')
    {
        match('(');
        type = expression();
        match(')');
    }
    else if (isalpha(look))
        type = loadvar(getname());
    else
        type = loadnum(getnum());
    return type;
}

/* reconhece e traduz uma multiplica��o */
char multiply(char type)
{
    match('*');
    return asm_popmul(type, factor());
}

/* reconhece e traduz uma multiplica��o */
char divide(char type)
{
    match('/');
    return asm_popdiv(type, factor());
}

/* analisa e traduz um termo matem�tico */
char term()
{
    char type;
    type = factor();
    while (ismulop(look))
    {
        asm_push(type);
        switch (look)
        {
        case '*':
            type = multiply(type);
            break;
        case '/':
            type = divide(type);
            break;
        }
    }
    return type;
}

/* multiplica valor na pilha com valor no reg. prim�rio */
char asm_popmul(char t1, char t2)
{
    char type, multype;
    asm_pop(t1,t2);
    type = asm_sametype(t1, t2, 0);
    multype = 'l';
    switch (type)
    {
    case 'b':
        printf("\tmul bl\n");
        multype = 'w';
        break;
    case 'w':
        printf("\tmul bx\n");
        break;
    case 'l':
        printf("\tcall MUL32\n");
        break;
    }
    return multype;
}

/* divide valor na pilha por valor do reg. prim�rio */
char asm_popdiv(char t1, char t2)
{
    asm_pop(t1,t2);
    /* se dividendo for 32-bits divisor deve ser tamb�m */
    if (t1 == 'l')
        asm_convert(t2, 'l');
    /* coloca operandos na ordem certa conforme o tipo */
    if (t1 == 'l' || t2 == 'l')
        asm_swap('l');
    else if (t1 == 'w' || t2 == 'w')
        asm_swap('w');
    else
        asm_swap('b');
    /* dividendo _REAL_ sempre ser� LONG...
       mas WORD se divisor for BYTE */
    if (t2 == 'b')
        asm_convert(t1, 'w');
    else
        asm_convert(t1, 'l');
    /* se um dos operandos for LONG, divis�o de 32-bits */
    if (t1 == 'l' || t2 == 'l')
        printf("\tcall DIV32\n");
    else if (t2 == 'w') /* 32 / 16 */
        printf("\tidiv bx\n");
    else if (t2 == 'b') /* 16 / 8 */
        printf("\tidiv bl\n");
    /* tipo do quociente � sempre igual ao do dividendo */
    return t1;
}

void asm_clear()
{
    printf("\txor ax, ax\n");
}

/* coloca reg. prim. na pilha */
void asm_push()
{
                printf("\tpush ax\n");
}


/* adiciona o topo da pilha ao reg. prim. */
void asm_popadd()
{
                printf("\tpop bx\n");
                printf("\tadd ax, bx\n");
}

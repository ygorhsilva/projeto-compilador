/*
Mais expressões: Variáveis, funções, tokens multi-caracter, espaços...
O código abaixo foi escrito por Felipo Soranz e é uma adaptação
do código original em Pascal escrito por Jack W. Crenshaw em sua
série "Let's Build a Compiler".
Este código é de livre distribuição e uso.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "header.h"
#define MAXNAME 30
#define MAXNUM 5

char look; /* O caracter lido "antecipadamente" (lookahead) */

#define MAXVAR 26
int var[MAXVAR];


/* PROGRAMA PRINCIPAL */
int main()
{
        init();
        do {
                assignment();
                newLine();
        } while (look != '.');
        printf("%d\n", expression());
        return 0;
}

/* captura um caracter de nova linha */
void newLine()
{
        if (look == '\n')
                nextChar();
}

/* inicialização do compilador */
void init()
{
    initVar();
    nextChar();
    skipWhite();
}

/* lê próximo caracter da entrada */
void nextChar()
{
    look = getchar();
}

/* exibe uma mensagem de erro formatada */
void error(char *fmt, ...)
{
    va_list args;
    fputs("Error: ", stderr);
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fputc('\n', stderr);
}

/* exibe uma mensagem de erro formatada e sai */
void fatal(char *fmt, ...)
{
    va_list args;
    fputs("Error: ", stderr);
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fputc('\n', stderr);
    exit(1);
}

/* alerta sobre alguma entrada esperada */
void expected(char *fmt, ...)
{
    va_list args;
    fputs("Error: ", stderr);
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fputs(" expected!\n", stderr);
    exit(1);
}

/* pula caracteres de espaço */
void skipWhite()
{
    while (look == ' ' || look == '\t')
        nextChar();
}

/* verifica se entrada combina com o esperado */
void match(char c)
{
    if (look != c)
        expected("'%c'", c);
    nextChar();
    skipWhite();
}

/* recebe o nome de um identificador */
char getName()
{
    int i;
    char *name;
    if (!isalpha(look))
        expected("Name");
    for (i = 0; isalnum(look); i++)
    {
        if (i >= MAXNAME)
            fatal("Identifier too long!");
        name[i] = toupper(look);
        nextChar();
    }
    name[i] = '\0';
    skipWhite();
    return name;
}

/* recebe um número inteiro */
int getNum()
{
        char num;

        if (!isdigit(look))
                expected("Integer");
        num = look;
        nextChar();

        return num - '0';
}

/* emite uma instrução seguida por uma nova linha */
void emit(char *fmt, ...)
{
    va_list args;
    putchar('\t');
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    putchar('\n');
}

/* reconhece operador aditivo */
int isAddOp(char c)
{
    return (c == '+' || c == '-');
}

/* analisa e traduz um identificador */
void ident()
{
    char name[MAXNAME + 1];
    getName(name);
    if (look == '(')
    {
        match('(');
        match(')');
        emit("CALL %s", name);
    }
    else
        emit("MOV AX, [%s]", name);
}

/* avalia um fator */
int factor()
{
        int val;

        if (look == '(') {
                match('(');
                val = expression();
                match(')');
        } else if (isalpha(look))
                val = var[getName() - 'A'];
        else
                val = getNum();

        return val;
}

/* reconhece e traduz uma multiplicação */
void multiply()

{
    match('*');
    factor();
    emit("POP BX");
    emit("IMUL BX");
}

/* reconhece e traduz uma divisão */
void divide()
{
    match('/');
    factor();
    emit("POP BX");
    emit("XCHG AX, BX");
    emit("CWD");
    emit("IDIV BX");
}

/* inicializa variáveis */
void initVar()
{
        int i;

        for (i = 0; i < MAXVAR; i++)
                var[i] = 0;
}


/* avalia um termo */
int term()
{
        int val;

        val = getNum();
        while (isAddOp(look)) {
                switch (look) {
                  case '*':
                        match('*');
                        val *= getNum();
                        break;
                  case '/':
                        match('/');
                        val /= getNum();
                        break;
                }
        }

        return val;
}

/* reconhece e traduz uma adição */
void add()
{
    match('+');
    term();
    emit("POP BX");
    emit("ADD AX, BX");
}

/* reconhece e traduz uma subtração */
void subtract()
{
    match('-');
    term();
    emit("POP BX");
    emit("SUB AX, BX");
    emit("NEG AX");
}



/* avalia um comando de atribuição */
void assignment()
{
        char name;
        name = getName();
        match('=');
        var[name - 'A'] = expression();
}

/* interpreta um comando de entrada */
void input()
{
        char name;
        char buffer[20];

        match('?');
        name = getName();
        printf("%c? ", name);
        fgets(buffer, 20, stdin);
        var[name - 'A'] = atoi(buffer);
}

/* interpreta um comando de saída */
void output()
{
        char name;

        match("!");
        name = getName();
        printf("%c -> %d", name, var[name - 'A']);
}
/* avalia o resultado de uma expressão */
int expression()
{
        return getNum();
}

#ifndef _OUTPUT_H

#define _OUTPUT_H

#include <stdarg.h>

void emit(char *s, ...); /* emite uma instrução */

#endif

/* output.c */

#include <stdio.h>

#include <stdarg.h>

void emit(char *fmt, ...)

{

    va_list args;

    putchar('\t');

    va_start(args, fmt);

    vprintf(fmt, args);

    va_end(args);

    putchar('\n');
}
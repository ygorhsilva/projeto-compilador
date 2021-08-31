#include <stdio.h>

#include "input.h"

#include "input.c"

#include "output.h"

#include "errors.h"

#include "scanner.h"

#include "parser.h"

#include "codegen.h"

void initInput();

int main()

{

    initInput();

    assignment();

    return 0;
}

void initInput()

{
    nextChar();
}
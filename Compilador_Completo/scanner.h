#ifndef _SCANNER_H

#define _SCANNER_H

#define MAXNAME 100

#define MAXNUM 100

int isAddOp(char c);

int isMulOp(char c);

void match(char c);

char getName();

char getNum();

#endif
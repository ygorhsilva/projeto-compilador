#ifndef _CODEGEN_H

#define _CODEGEN_H

void asmLoadConstant(char *s);

void asmLoadVariable(char *s);

void asmPush();

void asmPopAdd();

void asmPopSub();

void asmPopMul();

void asmPopDiv();

void asmStoreVariable(char *name);

void asmPopOr();

void asmPopXor();

void asmPopAnd();

void asmNot();

#endif
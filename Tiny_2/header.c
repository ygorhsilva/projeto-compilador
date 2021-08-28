/* rotinas utilitárias */
void init();
void nextchar();
void error(char *s);
void fatal(char *s);
void expected(char *s);
void undefined(char name);
void duplicated(char name);
void unrecognized(char name);
void notvar(char name);
/* tratamento da tabela de símbolos */
char symtype(char name);
char intable(char name);
void addsymbol(char name, char type);
void checkvar(char name);
/* analisador léxico rudimentar */
int isaddop(char c);
int ismulop(char c);
void skipwhite();
void newline();
void match(char c);
char getname();
char getnum();
/* geração de código */
void asm_loadvar(char name);
void asm_storevar(char name);
void asm_allocvar(char name);
/* analisador sintático */
void expression();
void assignment(char name);
void doblock();
void beginblock();
void decl();
void topdecls();
//
void doproc();
void asm_call(char name);
void formallist();
void formalparam();
void param();
int paramlist();
void docallproc(char name);
void clearparams();
int paramnum(char name);
int isparam(char name);
void addparam(char name);
int asm_offsetpar(int par);
void asm_loadparam(int par);
void asm_storeparam(int par);
void asm_push();
void formalparam();
void asm_cleanstack(int bytes);
void asm_procprolog(char name, int nlocals);
void asm_procepilog();
void header();
void prolog();
void epilog();
void locdecl();
int locdecls();
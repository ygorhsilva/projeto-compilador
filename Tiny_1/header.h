/* rotinas utilitárias */
void init();
void nextchar();
void error(char *s);
void fatal(char *s);
void expected(char *s);
void undefined(char *name);
/* reconhecedores */
int isaddop(char c);
int ismulop(char c);
int isorop(char c);
int isrelop(char c);
/* tratamento de símbolos */
int lookup(char *s, char *list[], int size);
int intable(char *name);
void addsymbol(char *name);
/* analisador léxico */
void skipwhite();
void newline();
void match(char c);
void getname();
int getnum();
void scan();
void matchstring(char *s);
/* rótulos */
int newlabel();
/* rotinas de geração de código */
void asm_clear();
void asm_negative();
void asm_loadconst(int i);
void asm_loadvar(char *name);
void asm_push();
void asm_popadd();
void asm_popsub();
void asm_popmul();
void asm_popdiv();
void asm_store(char *name);
void asm_not();
void asm_popand();
void asm_popor();
void asm_popxor();
void asm_popcompare();
void asm_relop(char op);
void asm_jmp(int label);
void asm_jmpfalse(int label);
void asm_read();
void asm_write();
void header();
void prolog();
void epilog();
/* rotinas do analisador sintático */
/* expressões aritméticas */
void factor();
void negfactor();
void firstfactor();
void multiply();
void divide();
void term1();
void term();
void firstterm();
void add();
void subtract();
void expression();
/* expressões booleanas e relações */
void relation();
void notfactor();
void boolterm();
void boolor();
void boolxor();
void boolexpression();
/* bloco, estruturas de controle e comandos */
void assignment();
void doif();
void dowhile();
void doread();
void dowrite();
void block();
/* declarações */
void allocvar(char *name);
void decl();
void topdecls();
/* programa principal */
void mainblock();
void prog();
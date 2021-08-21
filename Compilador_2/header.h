/* protótipos */
void init();
void nextChar();
void error(char *fmt, ...);
void fatal(char *fmt, ...);
void expected(char *fmt, ...);
void match(char c);
char getName();
char getNum();
void emit(char *fmt, ...);

int newLabel();
int postLabel(int lbl);

void condition();
void expression();

void other();
void doIf(int exitLabel);
void doWhile();
void doLoop();
void doRepeat();
void doFor();
void doDo();
void block(int exitLabel);
void program();

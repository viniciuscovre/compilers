enum {BEGIN = 0x4097 ,IF, THEN, ELSE, WHILE, FOR, DO, TO, DOWNTO, REPEAT, UNTIL,VAR, PROCEDURE, FUNCTION, INTEGER, REAL, BOOLEAN, DIV, MOD, AND, OR, NOT, END};

extern char *keywords[];
extern int iskeyword(char const *identifier);

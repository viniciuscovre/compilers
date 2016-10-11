enum {BEGIN = 0x4097 ,IF , THEN, ELSE, WHILE, DO, UNTIL, REPEAT, PROCEDURE, FUNCTION, VAR, END};

char *keywords[] = {"if","then","else","while","do","until","repeat","procedure","function","var","end"};

extern char *keywords[];
extern int iskeyword();


extern int is_keyword(char const *identifier);

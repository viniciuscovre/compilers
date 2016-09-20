enum {BEGIN = 0x4097 ,IF , THEN, ELSE, WHILE, DO, UNTIL, REPEAT, PROCEDURE, FUNCTION, VAR, END};

char *keywords[] = {"if","then","else","while","do","until","repeat","end","procedure","function","var"};

extern char *keywords[];
extern int iskeyword();


extern int is_keyword(char const *identifier);

enum {
  BEGIN = 0x4097,
  IF, //0x4098
  THEN, //0x4099
  ELSE, //0x409A
  WHILE, //0x409B
  DO, //0x409C
  REPEAT, //0x409D
  UNTIL, //0x409E
  VAR, //0x409F
  BOOLEAN, //0x40A0
  INTEGER, //0x40A1
  REAL, //0x40A2
  DOUBLE, //0x40A3
  DIV, //0x40A4
  MOD, //0x40A5
  AND, //0x40A6
  OR, //0x40A7
  NOT, //0x40A8
  TRUE, //0x40A9
  FALSE, //0x40AA
  END //0x40AB
};

extern char *keywords[];
extern int iskeyword(char const *identifier);

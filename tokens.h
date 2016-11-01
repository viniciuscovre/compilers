/**@<tokens.h>::**/

enum {
	ID = 1025,
	DEC,
	OCTAL,
	HEX,
	FLOAT,
	ASGN
};

enum {
	SYNTAX_ERR = -64,
	ALOCATION_ERR = -65,
	STACK_OVERFLOW_ERR = -66,
	PUSH_ERR = -67,
};

enum {
	EOL = 10, //END OF LINE
};

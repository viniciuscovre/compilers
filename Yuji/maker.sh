rm *.o
rm ~*
gcc -o mybc mybc.c lexer.c parser.c -I. -g

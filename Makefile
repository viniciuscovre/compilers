CFLAGS=-I. -g

project = mypas

relocatables = $(project).o lexer.o parser.o vmachine.o

executable = $(project)

$(executable): $(relocatables)
	cc -o $(executable) $(relocatables)
clean:
	$(RM)  $(relocatables)
mostlyclean: clean
	$(RM) $(executable) *~
indent: 
indent -nfca -orig - nuts - ts4 *.[ch]

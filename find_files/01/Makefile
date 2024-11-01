# Makefile: A standard Makefile for main.c

all  : main

clean:
	/bin/rm -rf main main.o
	test -d a && rm -rf a

test: create_dir run

create_dir:
	mkdir -p a/bb/ccc/ddddd/eeeee/uuuu/zzzz
	eval \>a/bb/ccc/ddddd/eeeee/uuuu/zzzz/a{1..3}.txt\;
	eval \>a/bb/ccc/ddddd/eeeee/b{1..6}.txt\;

run:
	./main  ./a

# END OF FILE

# Makefile: A standard Makefile for main.c
dir := my_dir

all  : main

clean:
	/bin/rm -rf main main.o

set_test:
	bash ./set_test.sh $(dir)

test: set_test run

run:
	./main ./$(dir)

# END OF FILE

# Makefile: A standard Makefile for main.c
# vim:set foldtext=getline(v\:foldstart) foldmethod=expr foldexpr=getline(v\:lnum)=~'^##'?'>1'\:'=':

dir := my_dir

## all
all : main

## clean
clean:
	/bin/rm -rf main main.o

## set tets
set_test:
	bash ./set_test.sh $(dir)

## tets
test: set_test run

## run
run:
	./main ./$(dir)

# END OF FILE

all: a.out

cat.o: cat.asm
	nasm -f elf64 cat.asm

a.out: cat.o
	ld cat.o

clean:
	rm a.out cat.o

# Makefile: A standard Makefile for main.c



all  : run

.c.o:
	$(CC) -o $@ -c $<

libsum.so: sum.o
	$(LD) -shared -o libsum.so sum.o

program.exe: program.cs
	mcs program.cs

run: program.exe libsum.so
	mono program.exe

clean:
	/bin/rm -rf libsum.so sum.o program.exe
# END OF FILE

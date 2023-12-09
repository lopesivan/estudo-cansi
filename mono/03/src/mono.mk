all: run

program.exe: program.cs
	$(MCS) program.cs

run: program.exe ./.libs/libsum.so
	$(MONO) program.exe

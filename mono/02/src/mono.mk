
program.exe: program.cs
	mcs program.cs

run: program.exe ./.libs/libsum.so
	mono program.exe

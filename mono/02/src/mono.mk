
program.exe: program.cs
	mcs program.cs

run: program.exe libsum.so
	mono program.exe

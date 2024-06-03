# Makefile: A standard Makefile for main.c

LDLIBS = -lssl -lcrypto
all  : main

curl:
	curl --insecure https://localhost:8080/index.html --output index.html
wget:
	wget --no-check-certificate https://localhost:8080/index.html
clean:
	/bin/rm -rf main main.o
# END OF FILE

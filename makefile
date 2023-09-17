CC=gcc #compiler

# Target OS detection
ifeq ($(OS),Windows_NT) # OS is a preexisting environment variable on Windows
	OS = windows
else
	UNAME := $(shell uname -s)
	ifeq ($(UNAME),Linux)
		OS = linux
	else
    		$(error OS not supported by this Makefile)
	endif
endif


main: Main.c
	$(CC) Main.c -o main

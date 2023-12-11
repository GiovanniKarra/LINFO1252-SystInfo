MAKEFLAGS += --silent

CFLAGS=-g -Wall -Werror
CC = gcc
RM = lib_tar.o tests soumission.tar

all: tests lib_tar.o

lib_tar.o: lib_tar.c lib_tar.h

tests: tests.c lib_tar.o

clean:
	rm -f $(RM)

submit: all
	tar --posix --pax-option delete=".*" --pax-option delete="*time*" --no-xattrs --no-acl --no-selinux -c *.h *.c Makefile > soumission.tar
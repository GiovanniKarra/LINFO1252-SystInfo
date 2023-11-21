GCC	= gcc
CFLAGS = -Wall -std=c99 -g -lpthread
RM = philosophes prodcons

all:

philo: philosophes.c
	@$(GCC) -o philosophes philosophes.c $(CFLAGS)

prodcons: prodcons.c
	@$(GCC) -o prodcons prodcons.c $(CFLAGS)

clean:
	@rm -f $(RM)
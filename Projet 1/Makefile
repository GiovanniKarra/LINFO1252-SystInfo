GCC	= gcc
CFLAGS = -Wall -std=c99 -g -lpthread
RM = philosophes prodcons

all:

philo: philosophes.c
	@$(GCC) -o philosophes philosophes.c $(CFLAGS)

prodcons: prodcons.c
	@$(GCC) -o prodcons prodcons.c $(CFLAGS)

reader_writer: reader_writer.c
	$(GCC) -o reader_writer reader_writer.c

clean:
	@rm -f $(RM)
GCC	= gcc
CFLAGS = -Wall -std=c99 -g -lpthread
RM = philosophes prodcons reader_writer test_and_set test_and_test_and_set *.o

all: philosophes prodcons reader_writer test_and_set

philo: philosophes.c
	@$(GCC) -o philosophes philosophes.c $(CFLAGS)

prodcons: prodcons.c
	@$(GCC) -o prodcons prodcons.c $(CFLAGS)

reader_writer: reader_writer.c
	@$(GCC) -o reader_writer reader_writer.c $(CFLAGS)

test_and_set: testandset.c spinlock.c
	@$(GCC) -o test_and_set testandset.c spinlock.c $(CFLAGS)

test_and_test_and_set: testandset.c spinlock.c
	@$(GCC) -o test_and_test_and_set testandset.c spinlock.c $(CFLAGS) -D METH2

clean:
	@rm -f $(RM)
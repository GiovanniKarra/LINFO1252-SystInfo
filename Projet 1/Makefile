GCC	= gcc
CFLAGS = -Wall -std=c99 -g -lpthread
RM = philo philo_active prodcons prodcons_active reader_writer reader_writer_active test_and_set test_and_test_and_set *.o

all: philosophes prodcons reader_writer test_and_set

philo: philosophes.c
	@$(GCC) -o philo philosophes.c $(CFLAGS)

prodcons: prodcons.c
	@$(GCC) -o prodcons prodcons.c $(CFLAGS)

reader_writer: reader_writer.c
	@$(GCC) -o reader_writer reader_writer.c $(CFLAGS)

test_and_set: testandset.c spinlock.c
	@$(GCC) -o test_and_set testandset.c spinlock.c $(CFLAGS)

test_and_test_and_set: testandset.c spinlock.c
	@$(GCC) -o test_and_test_and_set testandset.c spinlock.c $(CFLAGS) -D METH2

philo_active: philosophes.c spinlock.c
	@$(GCC) -o philo_active philosophes.c spinlock.c $(CFLAGS) -D METH2 -D MY_MUTEX

prodcons_active: prodcons.c spinlock.c
	@$(GCC) -o prodcons_active prodcons.c spinlock.c $(CFLAGS) -D METH2 -D MY_MUTEX

reader_writer_active: reader_writer.c spinlock.c
	@$(GCC) -o reader_writer_active reader_writer.c spinlock.c $(CFLAGS) -D METH2 -D MY_MUTEX

clean:
	@rm -f $(RM)
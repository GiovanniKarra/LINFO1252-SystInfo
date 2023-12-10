GCC	= gcc
CFLAGS = -Wall -std=c99 -g -lpthread
RM = archive.* philo philo_active prodcons prodcons_active reader_writer reader_writer_active test_and_set test_and_test_and_set backoff *.o

all: philo prodcons reader_writer test_and_set test_and_test_and_set backoff philo_active prodcons_active reader_writer_active

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

archive:
	@tar -czf archive.tar.gz intercept.h Makefile philosophes.c reader_writer.c spinlock.c spinlock.h testandset.c prodcons.c plots.py mesures/* plots/* README.md RapportProjet1.pdf mesurer_perf.sh

backoff: testandset.c spinlock.c
	@$(GCC) -o backoff testandset.c spinlock.c $(CFLAGS) -D BACKOFF

test: mesurer_perf.sh
	@sh mesurer_perf.sh

plot: plots.py
	@python3 plots.py

clean:
	@rm -f $(RM)
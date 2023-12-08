#!/bin/bash

NB_MESURES=5

mesure_perf() {
    TITRE="NombreDeThreads"
    for i in $(seq 1 $NB_MESURES); do
        TITRE="$TITRE,Essai$i"
    done
    echo $TITRE

    make $1

    for i in 2 4 8 16 32 64
    do
        LIGNE=$i
        for j in 1 2 3 4 5
        do
            if [ $2 = 1 ]; then
                INFO=`/usr/bin/time -f %e ./$1 $i 2>&1`
                LIGNE="$LIGNE,$INFO"
            else
                a=`expr $i / 2`
                INFO=`/usr/bin/time -f %e ./$1 $a $a 2>&1`
                LIGNE="$LIGNE,$INFO"
            fi
        done
        echo $LIGNE
    done
}


# PHILO
# mesure_perf philo 1
# mesure_perf philo_active 1

# CONSPROD
# mesure_perf prodcons 2
# mesure_perf prodcons_active 2

# READWRITE
# mesure_perf reader_writer 2
# mesure_perf reader_writer_active 2

# TEST AND SET
# mesure_perf test_and_set 1

# TEST AND TEST AND SET
# mesure_perf test_and_test_and_set 1

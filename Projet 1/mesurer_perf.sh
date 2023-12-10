#!/bin/bash

NB_MESURES=5
FICHIER_MESURES_PHILO="mesures_philo.csv"
FICHIER_MESURES_PHILO_ACTIVE="mesures_philo_active.csv"
FICHIER_MESURES_PRODCONS="mesures_prodcons.csv"
FICHIER_MESURES_PRODCONS_ACTIVE="mesures_prodcons_active.csv"
FICHIER_MESURES_READWRITE="mesures_readwrite.csv"
FICHIER_MESURES_READWRITE_ACTIVE="mesures_readwrite_active.csv"
FICHIER_MESURES_SPINLOCK="mesures_spinlock.csv"
FICHIER_MESURES_SPINLOCK2="mesures_spinlock2.csv"
FICHIER_MESURES_BACKOFF="mesures_backoff.csv"

# Fonction qui mesure la performance d'un des problèmes, et qui stock me résultat dans
# un fichier csv
mesure_perf() {
    TITRE="NombreDeThreads"
    for i in $(seq 1 $NB_MESURES); do
        TITRE="$TITRE,Essai$i"
    done
    echo $TITRE > mesures/$1

    #make clean
    #make $2

    for i in 2 4 8 16 32 64
    do
        LIGNE=$i
        for j in 1 2 3 4 5
        do
            if [ $3 = 1 ]; then
                INFO=`/usr/bin/time -f %e ./$2 $i 2>&1`
                LIGNE="$LIGNE,$INFO"
            else
                a=`expr $i / 2`
                INFO=`/usr/bin/time -f %e ./$2 $a $a 2>&1`
                LIGNE="$LIGNE,$INFO"
            fi
        done
        echo $LIGNE >> mesures/$1
    done
}


# PHILO
mesure_perf $FICHIER_MESURES_PHILO philo 1
mesure_perf $FICHIER_MESURES_PHILO_ACTIVE philo_active 1

# CONSPROD
mesure_perf $FICHIER_MESURES_PRODCONS prodcons 2
mesure_perf $FICHIER_MESURES_PRODCONS_ACTIVE prodcons_active 2

# READWRITE
mesure_perf $FICHIER_MESURES_READWRITE reader_writer 2
mesure_perf $FICHIER_MESURES_READWRITE_ACTIVE reader_writer_active 2

# TEST AND SET
mesure_perf $FICHIER_MESURES_SPINLOCK test_and_set 1

# TEST AND TEST AND SET
mesure_perf $FICHIER_MESURES_SPINLOCK2 test_and_test_and_set 1

# BACKOFF
mesure_perf $FICHIER_MESURES_BACKOFF backoff 1

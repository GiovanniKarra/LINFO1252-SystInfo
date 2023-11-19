#!/bin/bash

FICHIER_MESURES_PHILO="mesures_philo.csv"
NB_MESURES=5

TITRE="NombreDeThreads"
for i in $(seq 1 $NB_MESURES); do
    TITRE="$TITRE,Essai$i"
done
echo $TITRE > $FICHIER_MESURES_PHILO

#echo "NombreDeThreads,Essai 1,Essai 2,Essai 3,Essai 4,Essai 5" > $FICHIER_MESURES_PHILO
make clean
make philo

for i in 2 4 8 16 32 64
do
    LIGNE=$i
    for j in 1 2 3 4 5
    do
        INFO=`/usr/bin/time -f %e ./philosophes $i 2>&1`
        LIGNE="$LIGNE,$INFO"
    done
    echo $LIGNE >> $FICHIER_MESURES_PHILO
done
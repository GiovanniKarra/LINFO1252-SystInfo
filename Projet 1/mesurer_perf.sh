#!/bin/bash

NB_MESURES=5
FICHIER_MESURES_PHILO="mesures_philo.csv"
FICHIER_MESURES_PRODCONS="mesures_prodcons.csv"
FICHIER_MESURES_READWRITE="mesures_readwrite.csv"

# PHILO
# TITRE="NombreDeThreads"
# for i in $(seq 1 $NB_MESURES); do
#     TITRE="$TITRE,Essai$i"
# done
# echo $TITRE > $FICHIER_MESURES_PHILO

# make clean
# make philo

# for i in 2 4 8 16 32 64
# do
#     LIGNE=$i
#     for j in 1 2 3 4 5
#     do
#         INFO=`/usr/bin/time -f %e ./philosophes $i 2>&1`
#         LIGNE="$LIGNE,$INFO"
#     done
#     echo $LIGNE >> $FICHIER_MESURES_PHILO
# done


# CONSPROD
# TITRE="NombreDeThreads"
# for i in $(seq 1 $NB_MESURES); do
#     TITRE="$TITRE,Essai$i"
# done
# echo $TITRE > $FICHIER_MESURES_PRODCONS

# make clean
# make prodcons

# for i in 2 4 8 16 32 64
# do
#     LIGNE=$i
#     for j in 1 2 3 4 5
#     do
#         a=`expr $i / 2`
#         INFO=`/usr/bin/time -f %e ./prodcons $a $a 2>&1`
#         LIGNE="$LIGNE,$INFO"
#     done
#     echo "Thread $i fait"
#     echo $LIGNE >> $FICHIER_MESURES_PRODCONS
# done


# READWRITE
TITRE="NombreDeThreads"
for i in $(seq 1 $NB_MESURES); do
    TITRE="$TITRE,Essai$i"
done
echo $TITRE > $FICHIER_MESURES_READWRITE

make clean
make reader_writer

for i in 2 4 8 16 32 64
do
    LIGNE=$i
    for j in 1 2 3 4 5
    do
        a=`expr $i / 2`
        INFO=`/usr/bin/time -f %e ./reader_writer $a $a 2>&1`
        LIGNE="$LIGNE,$INFO"
    done
    #echo "Thread $i fait"
    echo $LIGNE >> $FICHIER_MESURES_READWRITE
done
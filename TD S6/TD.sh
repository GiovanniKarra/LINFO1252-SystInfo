#!/bin/bash

cd Modest

echo "Essai 1,Essai 2,Essai 3,Essai 4,Essai 5\n" > mesures.csv


for i in 2 4 6 8
do
    for j in 1 2 3 4 5
    do
        /usr/bin/time -f %e make -j $i -s 2>&1 | grep "\." >> mesures.csv
        make clean
    done
    echo "\n" >> mesures.csv
done
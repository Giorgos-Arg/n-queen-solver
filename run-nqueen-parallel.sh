#!/bin/bash
queens=10
threshold=7
threads=8
counter=1
while [ $counter -le $1 ]
do
	printf "\n"
	time ./nqueen-parallel.exe $counter $threads $threshold
	((counter++))
	((threshold++))
done
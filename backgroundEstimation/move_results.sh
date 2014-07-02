#!/bin/bash

if [[ "$#" -eq "0" ]]; then
 	echo "Need the name of a directory as argument"
fi


dirlist="
reports
prediction
scaleFactors
systematics
"

if [[ ! -d results/$1 ]]; then
	mkdir results/$1
fi

for i in $dirlist; do
	cp -r $i results/$1
done

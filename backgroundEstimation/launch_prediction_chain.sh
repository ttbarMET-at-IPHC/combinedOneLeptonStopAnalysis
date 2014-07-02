#!/bin/bash

if [[ "$#" -eq "0" ]]; then
 	echo "Need the name of a directory as argument"
fi

make tables
make predictions
make report
./move_results.sh $1

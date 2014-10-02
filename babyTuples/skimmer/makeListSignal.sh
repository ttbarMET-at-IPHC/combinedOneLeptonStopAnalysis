#!/bin/bash

for STOP in `seq 125 50 800`
do
    for LSP in `seq 25 50 400`
    do
        if [ $((STOP - LSP)) -lt 300  ] 
        then
            continue
        fi
        if [ $((STOP)) -lt 400  ] 
        then
            continue
        fi
        if [ $((STOP)) -gt 575  ] 
        then
            continue
        fi
        echo T2bw-075 $STOP $LSP
    done
done


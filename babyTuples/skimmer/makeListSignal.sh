#!/bin/bash

for STOP in `seq 125 50 800`
do
    for LSP in `seq 25 50 400`
    do
        if [ $((STOP - LSP)) -lt 100  ] 
        then
            continue
        fi
        if [ $((STOP)) -gt 550  ] 
        then
            continue
        fi
        if [ $((LSP)) -gt 250 ] 
        then
            continue
        fi
        echo T2tt $STOP $LSP
    done
done


#!/bin/bash

BENCHMARK_LIST=`ls ../signalContamination/`

while read line
do
    SIGNAL_REGION=$line
    rm -f $SIGNAL_REGION.dump

    for BENCHMARK in $BENCHMARK_LIST
    do

        TYPE=`echo $BENCHMARK | tr '_' ' ' | awk '{print $1}'`
        STOP=`echo $BENCHMARK | tr '_' ' ' | awk '{print $2}'`
        LSP=`echo $BENCHMARK | tr '_' ' ' | awk '{print $3}'`

        echo $TYPE $STOP $LSP

        SIGNAL_IN_MTPEAK=`cat ../signalContamination/$BENCHMARK/rawYieldTables/$SIGNAL_REGION.tab  | grep signal  | awk '{print $12}' | grep -v "|"`
        SIGNAL_IN_0BTAG=` cat ../signalContamination/$BENCHMARK/rawYieldTables/$SIGNAL_REGION.tab  | grep signal  | awk '{print $24}' | grep -v "|"`
        SIGNAL_IN_SR=`    cat ../signalContamination/$BENCHMARK/rawYieldTables/$SIGNAL_REGION.tab  | grep signal  | awk '{print $16}' | grep -v "|"`
        BACKGRPLUSSIG_IN_MTPEAK=`cat ../signalContamination/$BENCHMARK/rawYieldTables/$SIGNAL_REGION.tab  | grep totalSM  | awk '{print $12}'`
        BACKGRPLUSSIG_IN_0BTAG=` cat ../signalContamination/$BENCHMARK/rawYieldTables/$SIGNAL_REGION.tab  | grep totalSM  | awk '{print $24}'`
        BACKGRPLUSSIG_IN_SR=`    cat ../signalContamination/$BENCHMARK/rawYieldTables/$SIGNAL_REGION.tab  | grep totalSM  | awk '{print $16}'`
    
        C_MTPEAK=0`echo "scale=3; $SIGNAL_IN_MTPEAK / ($BACKGRPLUSSIG_IN_MTPEAK - $SIGNAL_IN_MTPEAK)" | bc`
        C_0BTAG=0` echo "scale=3; $SIGNAL_IN_0BTAG  / ($BACKGRPLUSSIG_IN_0BTAG  - $SIGNAL_IN_0BTAG )" | bc`
        C_SR=0`    echo "scale=3; $SIGNAL_IN_SR     / ($BACKGRPLUSSIG_IN_SR     - $SIGNAL_IN_SR    )" | bc`

        R_MTPEAK=0`echo "scale=3; $C_MTPEAK / $C_SR" | bc`
        R_0BTAG=0` echo "scale=3; $C_0BTAG  / $C_SR" | bc`

        echo $TYPE $STOP $LSP $C_MTPEAK $C_0BTAG $C_SR $R_MTPEAK $R_0BTAG >> $SIGNAL_REGION.dump
    done

    TYPE_FROM_SIGNAL_REGION=`echo $SIGNAL_REGION | tr '_' ' ' | awk '{print $2}' | sed 's/T2bw0/T2bw-0/g'`
    if [[ $TYPE_FROM_SIGNAL_REGION == "" ]]
    then
        continue
    fi

    for STOP in `seq 100 25 800`
    do
    for LSP in `seq 0 25 400`
    do
        if [ $((STOP - LSP)) -lt 100  ] 
        then
            continue
        fi

        BENCHMARK=${TYPE_FROM_SIGNAL_REGION}_${STOP}_${LSP}
        TEST=`ls ../signalContamination/ | grep $BENCHMARK`
        if [[ $TEST != "" ]]
        then
            continue
        fi

        echo $TYPE_FROM_SIGNAL_REGION $STOP $LSP -1 -1 -1 -1 -1 >> $SIGNAL_REGION.dump
    done
    done

done < "listSignalRegions"




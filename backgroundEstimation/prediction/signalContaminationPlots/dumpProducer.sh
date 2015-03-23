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
        CORRECTED_SF_PRE=`cat ../signalContamination/$BENCHMARK/scaleFactors/$SIGNAL_REGION.tab  | grep SF_pre  | awk '{print $4}'`
        CORRECTED_SF_POST=`cat ../signalContamination/$BENCHMARK/scaleFactors/$SIGNAL_REGION.tab | grep SF_post | awk '{print $4}'`
        CORRECTED_SFR_1ltop=`cat ../signalContamination/$BENCHMARK/scaleFactors/$SIGNAL_REGION.tab | grep SF_MTtail_1ltop | awk '{print $4}'`
        CORRECTED_SFR_Wjets=`cat ../signalContamination/$BENCHMARK/scaleFactors/$SIGNAL_REGION.tab | grep SF_MTtail_Wjets | awk '{print $4}'`
        
        echo $TYPE $STOP $LSP $CORRECTED_SF_PRE $CORRECTED_SF_POST $CORRECTED_SFR_1ltop $CORRECTED_SFR_Wjets >> $SIGNAL_REGION.dump
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

        echo $TYPE_FROM_SIGNAL_REGION $STOP $LSP -1 -1 -1 -1 >> $SIGNAL_REGION.dump
    done
    done

done < "listSignalRegions"




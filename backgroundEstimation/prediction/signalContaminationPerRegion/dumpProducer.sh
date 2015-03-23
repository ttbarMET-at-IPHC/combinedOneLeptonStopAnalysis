#!/bin/bash

POLARIZATION_SCENARIO="T2bw_rl"
BENCHMARK_LIST=`ls ../signalContamination/$POLARIZATION_SCENARIO`

while read line
do
    SIGNAL_REGION=$line
    rm -f $SIGNAL_REGION.dump

    for BENCHMARK in $BENCHMARK_LIST
    do
        TYPE=`echo $BENCHMARK | tr '_' ' ' | awk '{print $1}'`
        STOP=`echo $BENCHMARK | tr '_' ' ' | awk '{print $2}'`
        LSP=`echo $BENCHMARK | tr '_' ' ' | awk '{print $3}'`
        CORRECTED_BACKGROUND_YIELD=`cat ../signalContamination/$POLARIZATION_SCENARIO/$BENCHMARK/prediction/$SIGNAL_REGION.tab | grep totalSM | awk '{print $8}'`
        CORRECTED_BACKGROUND_UNCERT=`cat ../signalContamination/$POLARIZATION_SCENARIO/$BENCHMARK/prediction/$SIGNAL_REGION.tab | grep totalSM | awk '{print $10}'`
        CORRECTED_REL_UNCERT=`echo "scale=3; $CORRECTED_BACKGROUND_YIELD / $CORRECTED_BACKGROUND_UNCERT" | bc`
        
        NONCORRECTED_BACKGROUND_YIELD=`cat ../../common/results/latest/prediction/$SIGNAL_REGION.tab | grep totalSM | awk '{print $8}'`
        NONCORRECTED_BACKGROUND_UNCERT=`cat ../../common/results/latest/prediction/$SIGNAL_REGION.tab | grep totalSM | awk '{print $10}'`
        NONCORRECTED_REL_UNCERT=`echo "scale=3; $NONCORRECTED_BACKGROUND_YIELD / $NONCORRECTED_BACKGROUND_UNCERT" | bc`
        
        GLOBAL_SF_YIELD=0`echo "scale=3; $CORRECTED_BACKGROUND_YIELD / $NONCORRECTED_BACKGROUND_YIELD" | bc`
        GLOBAL_SF_UNCERT=0`echo "scale=3; $CORRECTED_BACKGROUND_UNCERT / $NONCORRECTED_BACKGROUND_UNCERT" | bc`
        echo $TYPE $STOP $LSP $NONCORRECTED_BACKGROUND_YIELD $NONCORRECTED_BACKGROUND_UNCERT $CORRECTED_BACKGROUND_YIELD $CORRECTED_BACKGROUND_UNCERT $GLOBAL_SF_YIELD $GLOBAL_SF_UNCERT >> $SIGNAL_REGION.dump
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

        NONCORRECTED_BACKGROUND_YIELD=`cat ../../common/results/latest/prediction/$SIGNAL_REGION.tab | grep totalSM | awk '{print $8}'`
        NONCORRECTED_BACKGROUND_UNCERT=`cat ../../common/results/latest/prediction/$SIGNAL_REGION.tab | grep totalSM | awk '{print $10}'`
        NONCORRECTED_REL_UNCERT=`echo "scale=3; $NONCORRECTED_BACKGROUND_YIELD / $NONCORRECTED_BACKGROUND_UNCERT" | bc`

        echo $TYPE_FROM_SIGNAL_REGION $STOP $LSP $NONCORRECTED_BACKGROUND_YIELD $NONCORRECTED_BACKGROUND_UNCERT -1 -1 -1 -1 >> $SIGNAL_REGION.dump
    done
    done

done < "listSignalRegions"




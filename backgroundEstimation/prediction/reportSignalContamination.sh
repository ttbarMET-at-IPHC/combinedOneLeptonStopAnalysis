#!/bin/bash

echo "SignalRegion oldSF_pre newSF_pre oldSFR_1ltop newSFR_1ltop globalSF"
while read line
do
    SIGNAL_REGION=`echo $line | awk '{print $1}'`
    SIGNAL_TYPE=`echo $line | awk '{print $2}'`
    SIGNAL_STOP_MASS=`echo $line | awk '{print $3}'`
    SIGNAL_NEUTRALINO_MASS=`echo $line | awk '{print $4}'`
    SIGNAL_NAME=${SIGNAL_TYPE}_${SIGNAL_STOP_MASS}_${SIGNAL_NEUTRALINO_MASS}

    B_CORRECTED=`cat signalContamination/$SIGNAL_NAME/prediction/$SIGNAL_REGION.tab | grep totalSM | awk '{print $8}'`
    B_NOTCORRECTED=`cat ../common/results/latest/prediction/$SIGNAL_REGION.tab | grep totalSM | awk '{print $8}'`

    newSF_pre=`cat signalContamination/$SIGNAL_NAME/scaleFactors/$SIGNAL_REGION.tab | grep SF_pre | awk '{print $4}'`
    oldSF_pre=`cat ../common/results/latest/prediction/scaleFactors/$SIGNAL_REGION.tab | grep SF_pre | awk '{print $4}'`

    newSFR_1ltop=`cat signalContamination/$SIGNAL_NAME/scaleFactors/$SIGNAL_REGION.tab | grep SF_MTtail_1ltop | awk '{print $4}'`
    oldSFR_1ltop=`cat ../common/results/latest/prediction/scaleFactors/$SIGNAL_REGION.tab | grep SF_MTtail_1ltop | awk '{print $4}'`

    GLOBAL_SF=0`echo "scale=3; $B_CORRECTED / $B_NOTCORRECTED" | bc`
    
    echo $SIGNAL_REGION $oldSF_pre $newSF_pre $oldSFR_1ltop $newSFR_1ltop $GLOBAL_SF
    
done < "listSignalContamination"




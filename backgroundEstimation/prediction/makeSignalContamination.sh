#!/bin/bash

mkdir -p signalContamination
while read line
do
    echo Running on $line
    SIGNAL_TYPE=`echo $line | awk '{print $1}'`
    SIGNAL_STOP_MASS=`echo $line | awk '{print $2}'`
    SIGNAL_NEUTRALINO_MASS=`echo $line | awk '{print $3}'`
    SIGNAL_NAME=${SIGNAL_TYPE}_${SIGNAL_STOP_MASS}_${SIGNAL_NEUTRALINO_MASS}

    sed -i "s/#define SIGNAL_CONTAMINATION_INPUT.*/#define SIGNAL_CONTAMINATION_INPUT \"${SIGNAL_NAME}\"/g" backgroundEstimation.C
    sed -i "s/#define SIGNAL_CONTAMINATION_INPUT.*/#define SIGNAL_CONTAMINATION_INPUT \"${SIGNAL_NAME}\"/g" yieldTableProducer.C

    make clean tables predictions
   
    mkdir -p signalContamination/${SIGNAL_NAME}
    cp -r prediction rawYieldTables scaleFactors secondLeptonInAcceptance systematics signalContamination/${SIGNAL_NAME}
done < "listSignalContamination"




#!/bin/bash

while read line
do
    echo Running on $line
    SIGNAL_TYPE=`echo $line | awk '{print $1}'`
    SIGNAL_STOP_MASS=`echo $line | awk '{print $2}'`
    SIGNAL_NEUTRALINO_MASS=`echo $line | awk '{print $3}'`
    echo $SIGNAL_TYPE > list
    INPUT_STOP_MASS=${SIGNAL_STOP_MASS} INPUT_NEUTRALINO_MASS=${SIGNAL_NEUTRALINO_MASS} make clean skim
    mv ${SIGNAL_TYPE}.root ${SIGNAL_TYPE}_${SIGNAL_STOP_MASS}_${SIGNAL_NEUTRALINO_MASS}.root
done < "listSplitSignal"

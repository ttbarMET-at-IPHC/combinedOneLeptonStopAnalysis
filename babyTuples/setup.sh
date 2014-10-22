#!/bin/bash

export NTUPLE_DATAFORMAT=/opt/sbg/data/data4/cms/aaubin/analysisStudy_July13/CMSSW_5_3_2_patch4/src/IPHCDataFormat/
export NTUPLE_ANALYSISFRAMEWORK=/opt/sbg/data/data4/cms/aaubin/analysisStudy_July13/CMSSW_5_3_2_patch4/src/NTuple/NTupleAnalysis/
export ANALYSIS_SELECTION=$PWD/objectAndEventSelection/

echo " #   Adding the following paths to LD_LIBRARY_PATH :"
echo " #    -" $NTUPLE_DATAFORMAT/NTFormat/src
echo " #    -" $NTUPLE_ANALYSISFRAMEWORK
echo " #    -" $ANALYSIS_SELECTION

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NTUPLE_DATAFORMAT/NTFormat/src/:$NTUPLE_ANALYSISFRAMEWORK:$ANALYSIS_SELECTION


#!/bin/bash

export NTUPLE_ANALYSIS=/opt/sbg/data/data4/cms/aaubin/analysisStudy_July13/CMSSW_5_3_2_patch4/src/NTuple/NTupleAnalysis
export NTUPLE_DATAFORMAT=/opt/sbg/data/data4/cms/aaubin/analysisStudy_July13/CMSSW_5_3_2_patch4/src/IPHCDataFormat/NTFormat/

echo " #   Adding the following paths to LD_LIBRARY_PATH :"
echo " #    -" $NTUPLE_DATAFORMAT/src/
echo " #    -" $NTUPLE_ANALYSIS/../.lib/

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NTUPLE_DATAFORMAT/src/:$NTUPLE_ANALYSIS/../.lib/


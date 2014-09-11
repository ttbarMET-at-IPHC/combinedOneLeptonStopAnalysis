
1-lepton stop analysis
======================

Install
-------

    git clone https://github.com/ttbarMET-at-IPHC/combinedOneLeptonStopAnalysis.git
    cd combinedOneLeptonStopAnalysis
    git submodule init
    git submodule update
    cd common && git checkout master && git pull && cd ..
    <here, make sure to have a ROOT sourced somewhere>
    cd sonicScrewdriver && git checkout master && git pull && make && cd ..

Setup environment (each time)
-----------------

    <make sure you have ROOT sourced from somewhere>
    source setup.sh

Directories
-----------

### Common definitions

 * AN-14-067 - Common analysis definitions (from Michaels repo)
 * inputs - Object and correction definitions/inputs 
 * common.h - Misc tools for code operation
    
### BabyTuples operations

 * babyTupling - To read the Strasbourg nTuples and create the babyTuples
 * babyFilter - To apply the MET filters after babyTuple production
 * babyReader - Example on how to read the babyTuple
 * babySkimmer - Template to skim the babyTuples
 * babyExtender - Template to add information to the babyTuples
 * babyValidation - Code attempting to validate the babyTuples

### Background estimation 

 * backgroundEstimation

### Cut and count optimization and studies

 * cutAndCount_T2bw
 * cutAndCount_T2tt
 * cutAndCount_testCorrelations
 * cutAndCount_variableRanking

### W-tagging studies

 * WTagPerformances
 * WTagTMVAPlotting
 * WTagTMVATraining

### Plotting tool 

 * sonicScrewdriver - from Alexs repo

### Misc checks

 * checkPileUpFastSim
 * dataMCChecks
 * synchronization
 * testSelection
    

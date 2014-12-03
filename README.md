
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
    cd sonicScrewdriver && git checkout master && git pull && make clean lib && cd ..

Setup environment (each time)
-----------------

    # If this is the first time, you might want to change
    # the CMSSW area where setup.sh goes to source root
    source setup.sh


#!/usr/bin/env python

import subprocess
'''
signalRegions = [ "cutAndCount_T2tt_offShellLoose" ]
'''
signalRegions = [ "cutAndCount_T2tt_offShellLoose",
                  "cutAndCount_T2tt_offShellTight",
                  "cutAndCount_T2tt_lowDeltaM",    
                  "cutAndCount_T2tt_mediumDeltaM", 
                  "cutAndCount_T2tt_highDeltaM" ]

for region in signalRegions :
    print "[Running on "+region+"]"
    p = subprocess.Popen(["make", "--quiet", "clean"])
    p.wait()
    p = subprocess.Popen(["make", "--quiet", "compile", "run", "TARGET=yieldTableProducer", "SIGNAL_REGION_CUTS="+region, "SIGNAL_REGION_TAG=\\\""+region+"\\\""])
    p.wait()

#!/bin/bash

#First change the CR boolean in BDTDefinition.h

CRs="
CR4_1j
CR4_2j
CR4_3j
CR4_4j
"

for i in $CRs ; do
	echo "### Compute SF for $i" 
	sed -e "s/#define $i false/#define $i true/g" BDTDefinition.h.org > BDTDefinition.h
	make clean;
	time make controlPlots 
	cp controlPlotsProducer_preselection.log BDTcuts_$i.res
done


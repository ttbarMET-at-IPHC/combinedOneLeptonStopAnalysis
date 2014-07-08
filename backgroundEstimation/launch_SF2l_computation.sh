#!/bin/bash

#First change the CR boolean in BDTDefinition.h


CRs="
CR4_1j
CR4_2j
CR4_3j
CR4_4j
CR4_4j_50evts
CR4_4j_100evts
CR4_4j_150evts
"

for i in $CRs ; do
	echo "### Compute SF for $i" 
	sed -e "s/#define $i false/#define $i true/g" BDTDefinition_link.h > BDTDefinition.h
	make clean;
	time make tables && time make predictions
	cd scaleFactors;
	mv BDT_*_2leptons.tab $i/
	cd -;
done


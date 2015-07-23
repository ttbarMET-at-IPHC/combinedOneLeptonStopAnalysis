#/bin/bash

./topnessProducer3.run config_def.txt &
#for i in `seq 0 44`; do

#new update: start @ 45
#for i in `seq 45 59`; do
#for i in `seq 70 73`; do
#	./topnessProducer3.run  config_${i}.txt &
#done
for i in `seq 17 19`; do
	./topnessProducer3.run  config2_${i}.txt &
done

#./topnessProducer3.run config_12.txt &

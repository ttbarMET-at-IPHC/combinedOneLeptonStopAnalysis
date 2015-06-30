default: topnessProducer
#yieldTableProducer

include ../common/common.make

#--------------------------------------------------------------------------
#CUSTOM_BDT_CUTS_LIST  = CR4_1j CR4_2j CR4_3j CR4_4j
#CUSTOM_BDT_CUTS_LIST += CR4_4j_50evts CR4_4j_100evts CR4_4j_150evts

#CUSTOM_CNC_CUTS_LIST = $(shell cat ../checkCR4CR5modeling/cutAndCountDefinitions_cutsLoosenedForCR4CR5.list)

#--------------------------------------------------------------------------

yieldTableProducer: yieldTableProducer.$(ExeSuf)
	./yieldTableProducer.$(ExeSuf)

plotsProducer: plotsProducer.$(ExeSuf)
	./plotsProducer.$(ExeSuf)

topnessProducer: topnessProducer.$(ExeSuf)
	./topnessProducer.$(ExeSuf)


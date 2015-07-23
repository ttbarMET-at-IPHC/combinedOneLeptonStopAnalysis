default: topnessProducer4
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

topnessProducer2: topnessProducer2.$(ExeSuf)
	./topnessProducer2.$(ExeSuf)

topnessProducer3: topnessProducer3.$(ExeSuf)
	./topnessProducer3.$(ExeSuf)

topnessProducer4: topnessProducer4.$(ExeSuf)
	./topnessProducer4.$(ExeSuf)

topnessProducer5: topnessProducer5.$(ExeSuf)
	./topnessProducer5.$(ExeSuf)


metResolutionStudy: metResolutionStudy.$(ExeSuf)
	./metResolutionStudy.$(ExeSuf)

skimming: skimming.$(ExeSuf)
	./skimming.$(ExeSuf)

resoTermStudy: resoTermStudy.$(ExeSuf)
	resoTermStudy.$(ExeSuf)

bjetPosition: bjetPosition.$(ExeSuf)
	bjetPosition.$(ExeSuf)

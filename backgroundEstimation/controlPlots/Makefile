default: controlPlots

include ../common/common.make

#--------------------------------------------------------------------------
CUSTOM_BDT_CUTS_LIST  = CR4_1j CR4_2j CR4_3j CR4_4j
CUSTOM_BDT_CUTS_LIST += CR4_4j_50evts CR4_4j_100evts CR4_4j_150evts

CUSTOM_CNC_CUTS_LIST = $(shell cat ../checkCR4CR5modeling/cutAndCountDefinitions_cutsLoosenedForCR4CR5.list)

#--------------------------------------------------------------------------

checkContamination: checkContamination.$(ExeSuf)
	./checkContamination.$(ExeSuf)

preselectionControlPlots: preselectionControlPlots.$(ExeSuf)
	./preselectionControlPlots.$(ExeSuf)

MlbControlPlots: MlbControlPlots.$(ExeSuf)
	./MlbControlPlots.$(ExeSuf)

MlbShapeComparison: MlbShapeComparison.$(ExeSuf)
	./MlbShapeComparison.$(ExeSuf)

variablePlots: variablePlots.$(ExeSuf)
	./variablePlots.$(ExeSuf)

fullMTwithSFRinTail: fullMTwithSFRinTail.$(ExeSuf)
	./fullMTwithSFRinTail.$(ExeSuf)


#BDT_REGIONS = $(BDT_T2bw075)

BDToutputUnblinded:
	@make --no-print-directory clean;
	@for REGION in $(BDT_REGIONS);        \
	 do                                      \
		   echo "[Launching signal region $$REGION]";     \
	 	   make --quiet --no-print-directory compile TARGET=BDToutputUnblinded SIGNAL_REGION_CUTS=$$REGION SIGNAL_REGION_TAG=\\\"$$REGION\\\"; \
		   mv BDToutputUnblinded.$(ExeSuf) BDToutputUnblinded_$$REGION.$(ExeSuf);			\
		   ./BDToutputUnblinded_$$REGION.$(ExeSuf) > BDToutputUnblinded_$$REGION.$(LogSuf) & \
	done;
	@for REGION in $(BDT_REGIONS); 	\
	do									\
		echo "Waiting for $$REGION";	\
		CHECK="nonEmpty";				\
		while [ "$$CHECK" != "" ];		\
		do								\
			sleep 1;					\
			CHECK=`ps kstart_time | grep BDToutputUnblinded_$$REGION.$(ExeSuf) | grep -v grep`;	\
		done;							\
	done;								\
	echo "Done";







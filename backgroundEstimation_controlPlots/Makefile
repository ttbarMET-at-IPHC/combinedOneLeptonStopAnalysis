default: controlPlots

include ../backgroundEstimation_common/common.make

SIGNAL_REGIONS = $(CnC_REGIONS)

#--------------------------------------------------------------------------
CUSTOM_BDT_CUTS_LIST  = CR4_1j CR4_2j CR4_3j CR4_4j
CUSTOM_BDT_CUTS_LIST += CR4_4j_50evts CR4_4j_100evts CR4_4j_150evts

CUSTOM_CNC_CUTS_LIST = $(shell cat ../backgroundEstimation_checkCR4CR5modeling/cutAndCountDefinitions_cutsLoosenedForCR4CR5.list)

#--------------------------------------------------------------------------


controlPlots:
	@make --no-print-directory clean;
	@for REGION in $(SIGNAL_REGIONS);        \
	 do                                      \
		   echo "[Launching signal region $$REGION]";     \
	 	   make --quiet --no-print-directory compile TARGET=controlPlotsProducer SIGNAL_REGION_CUTS=$$REGION SIGNAL_REGION_TAG=\\\"$$REGION\\\"; \
		   mv controlPlotsProducer.$(ExeSuf) controlPlotsProducer_$$REGION.$(ExeSuf);			\
		   ./controlPlotsProducer_$$REGION.$(ExeSuf) > controlPlotsProducer_$$REGION.$(LogSuf) & \
	done;
	@for REGION in $(SIGNAL_REGIONS); 	\
	do									\
		echo "Waiting for $$REGION";	\
		CHECK="nonEmpty";				\
		while [ "$$CHECK" != "" ];		\
		do								\
			sleep 1;					\
			CHECK=`ps kstart_time | grep controlPlotsProducer_$$REGION.$(ExeSuf) | grep -v grep`;	\
		done;							\
	done;								\
	echo "Done";

checkCutAndCount:
	@make --no-print-directory clean;
	@for REGION in $(SIGNAL_REGIONS);        \
	 do                                      \
		   echo "[Launching signal region $$REGION]";     \
	 	   make --quiet --no-print-directory compile TARGET=checkCutAndCount SIGNAL_REGION_CUTS=$$REGION SIGNAL_REGION_TAG=\\\"$$REGION\\\"; \
		   mv checkCutAndCount.$(ExeSuf) checkCutAndCount_$$REGION.$(ExeSuf);			\
		   ./checkCutAndCount_$$REGION.$(ExeSuf) > checkCutAndCount_$$REGION.$(LogSuf) & \
	done;
	@for REGION in $(SIGNAL_REGIONS); 	\
	do									\
		echo "Waiting for $$REGION";	\
		CHECK="nonEmpty";				\
		while [ "$$CHECK" != "" ];		\
		do								\
			sleep 1;					\
			CHECK=`ps kstart_time | grep checkCutAndCount_$$REGION.$(ExeSuf) | grep -v grep`;	\
		done;							\
	done;								\
	echo "Done";


controlPlots_relaxedSignalRegions:
	@make --no-print-directory clean 
	@echo "[Making plots for cut and count relaxed regions]"
	@for REGION in $(CUSTOM_CNC_CUTS_LIST); 	  \
	 do									  \
	    echo "[Lauching signal region $$REGION]";     										\
	    make --quiet --no-print-directory compile TARGET=controlPlotsProducer_relaxedSignalRegions SIGNAL_REGION_CUTS=$$REGION SIGNAL_REGION_TAG=\\\"$$REGION\\\"; 	\
		mv controlPlotsProducer_relaxedSignalRegions.$(ExeSuf) controlPlotsProducer_relaxedSignalRegions_$$REGION.$(ExeSuf); 													\
		./controlPlotsProducer_relaxedSignalRegions_$$REGION.$(ExeSuf) > controlPlotsProducer_relaxedSignalRegions_$$REGION.$(LogSuf) & 										\
	 done;
	@for REGION in $(CUSTOM_CNC_CUTS_LIST)  \
	 do									    \
		echo "Waiting for $$REGION";		\
		CHECK="nonEmpty";					\
		while [ "$$CHECK" != "" ];			\
		do									\
			sleep 1;						\
			CHECK=`ps kstart_time | grep controlPlotsProducer_relaxedSignalRegions_$$REGION.$(ExeSuf) | grep -v grep`;	\
		done;								\
	done;									\
	echo "Done";
	@make --no-print-directory clean
	@echo "[Making tables for BDT regions]"
	@for CUSTOM_BDT_CUTS in $(CUSTOM_BDT_CUTS_LIST);	        				\
	 do																			\
		echo "[Running with custom BDT cuts $$CUSTOM_BDT_CUTS]";    			\
		for REGION in $(BDT_REGIONS); 		  									\
		do									      								\
	    	    echo "[Lauching signal region $$REGION]";     					\
	        	make --quiet --no-print-directory compile TARGET=controlPlotsProducer_relaxedSignalRegions SIGNAL_REGION_CUTS=$$REGION SIGNAL_REGION_TAG=\\\"$$REGION\\\"; 	\
				mv controlPlotsProducer_relaxedSignalRegions.$(ExeSuf) controlPlotsProducer_relaxedSignalRegions_$$REGION.$(ExeSuf); 																	\
				./controlPlotsProducer_relaxedSignalRegions_$$REGION.$(ExeSuf) $$CUSTOM_BDT_CUTS > controlPlotsProducer_relaxedSignalRegions_$$REGION.$(LogSuf) & 									\
		done;								\
		for REGION in $(BDT_REGIONS); 		\
		do									\
			echo "Waiting for $$REGION";	\
			CHECK="nonEmpty";				\
			while [ "$$CHECK" != "" ];		\
			do								\
				sleep 1;					\
				CHECK=`ps kstart_time | grep controlPlotsProducer_relaxedSignalRegions_$$REGION.$(ExeSuf) | grep -v grep`;	\
			done;							\
		done;								\
		echo "Done";						\
	 done;







signalContamination:
	@make --no-print-directory clean;
	@for REGION in $(SIGNAL_REGIONS);        \
	 do                                      \
		   echo "[Launching signal region $$REGION]";     \
	 	   make --quiet --no-print-directory compile TARGET=signalContamination SIGNAL_REGION_CUTS=$$REGION SIGNAL_REGION_TAG=\\\"$$REGION\\\"; \
		   mv signalContamination.$(ExeSuf) signalContamination_$$REGION.$(ExeSuf);			\
		   ./signalContamination_$$REGION.$(ExeSuf) > signalContamination_$$REGION.$(LogSuf) & \
	done;
	@for REGION in $(SIGNAL_REGIONS); 	\
	do									\
		echo "Waiting for $$REGION";	\
		CHECK="nonEmpty";				\
		while [ "$$CHECK" != "" ];		\
		do								\
			sleep 1;					\
			CHECK=`ps kstart_time | grep signalContamination_$$REGION.$(ExeSuf) | grep -v grep`;	\
		done;							\
	done;								\
	echo "Done";


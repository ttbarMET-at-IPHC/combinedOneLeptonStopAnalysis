
#------------------------------------------------------------------------------

default: tables scaleFactors reports

#------------------------------------------------------------------------------

CUSTOM_BDT_CUTS_LIST  =  30evts_in_CR4_2jets_MTTail
CUSTOM_BDT_CUTS_LIST +=  30evts_in_CR4_3jets_MTTail
CUSTOM_BDT_CUTS_LIST +=  30evts_in_CR4_4jets_MTTail
CUSTOM_BDT_CUTS_LIST +=  50evts_in_CR4_4jets_MTTail
CUSTOM_BDT_CUTS_LIST += 100evts_in_CR4_4jets_MTTail
CUSTOM_BDT_CUTS_LIST += 150evts_in_CR4_4jets_MTTail

#------------------------------------------------------------------------------

tables: tablesBDT tablesCnC

scaleFactors: scaleFactorsBDT scaleFactorsCnC

reports: reportBDT reportCnC

#------------------------------------------------------------------------------

include ../backgroundEstimation_common/common.make

tablesBDT:
	@make --no-print-directory clean
	@echo "[Making tables for BDT regions]"
	@for CUSTOM_BDT_CUTS in $(CUSTOM_BDT_CUTS_LIST);	        				\
	 do																			\
		echo "[Running with custom BDT cuts $$CUSTOM_BDT_CUTS]";    			\
		for REGION in $(BDT_REGIONS); 		  									\
		do									      								\
	    	    echo "[Lauching signal region $$REGION]";     					\
	        	make --quiet --no-print-directory compile TARGET=yieldTableProducer SIGNAL_REGION_CUTS=$$REGION SIGNAL_REGION_TAG=\\\"$$REGION\\\"; 	\
				mv yieldTableProducer.$(ExeSuf) yieldTableProducer_$$REGION.$(ExeSuf); 																	\
				./yieldTableProducer_$$REGION.$(ExeSuf) $$CUSTOM_BDT_CUTS > yieldTableProducer_$$REGION.$(LogSuf) & 									\
		done;								\
		for REGION in $(BDT_REGIONS); 		\
		do									\
			echo "Waiting for $$REGION";	\
			CHECK="nonEmpty";				\
			while [ "$$CHECK" != "" ];		\
			do								\
				sleep 1;					\
				CHECK=`ps kstart_time | grep yieldTableProducer_$$REGION.$(ExeSuf) | grep -v grep`;	\
			done;							\
		done;								\
		echo "Done";						\
	 done;

tablesCnC:
	@make --no-print-directory clean 
	@echo "[Making tables for cut and count regions]"
	@for CUTANDCOUNT in $(CnC_REGIONS); 	  \
	 do									  \
	 	for REGION in `cat cutAndCountDefinitions_cutsLoosenedForCR4CR5.list | grep $$CUTANDCOUNT`;	\
		do																						\
	        echo "[Lauching signal region $$REGION]";     										\
	        make --quiet --no-print-directory compile TARGET=yieldTableProducer SIGNAL_REGION_CUTS=$$REGION SIGNAL_REGION_TAG=\\\"$$REGION\\\"; 	\
			mv yieldTableProducer.$(ExeSuf) yieldTableProducer_$$REGION.$(ExeSuf); 																	\
			./yieldTableProducer_$$REGION.$(ExeSuf) > yieldTableProducer_$$REGION.$(LogSuf) & 									\
		done;																																		\
	 done;
	@for CUTANDCOUNT in  $(CnC_REGIONS); 	  \
	 do									      \
	 	for REGION in `cat cutAndCountDefinitions_cutsLoosenedForCR4CR5.list | grep $$CUTANDCOUNT`;	\
		do																						\
			echo "Waiting for $$REGION";	\
			CHECK="nonEmpty";				\
			while [ "$$CHECK" != "" ];		\
			do								\
				sleep 1;					\
				CHECK=`ps kstart_time | grep yieldTableProducer_$$REGION.$(ExeSuf) | grep -v grep`;	\
			done;							\
		done;								\
	done;									\
	echo "Done";


scaleFactorsBDT: scaleFactorsProducer.$(ExeSuf)
	@echo "[Making scale factors for BDT regions]"
	@for CUSTOM_BDT_CUTS in $(CUSTOM_BDT_CUTS_LIST);	        				 \
	do									      		  	        			 \
		echo "[Running with custom BDT cuts $$CUSTOM_BDT_CUTS]";    		 \
		for REGION in $(BDT_REGIONS);										 \
		do																	 \
			echo "[Running on $$REGION]";			  	  	  	  	  	  	 \
			./scaleFactorsProducer.$(ExeSuf) $$REGION $$CUSTOM_BDT_CUTS;	 \
		done;																 \
	done;

scaleFactorsCnC: scaleFactorsProducer.$(ExeSuf)
	@echo "[Making scale factors for cut and count regions]"
	@for CUTANDCOUNT in $(CnC_REGIONS); 	  \
	 do									  \
	 	for REGION in `cat cutAndCountDefinitions_cutsLoosenedForCR4CR5.list | grep $$CUTANDCOUNT`;	\
		do																							\
			echo "[Running on $$REGION]"; 									 						\
			./scaleFactorsProducer.$(ExeSuf) $$REGION;	 											\
		done;																 						\
	done;

reportBDT: reportCR4CR5modeling_BDT.$(ExeSuf)
	./reportCR4CR5modeling_BDT.$(ExeSuf) $(BDT_REGIONS) | tee reportCR4CR5modeling_BDT.$(LogSuf)

reportCnC: reportCR4CR5modeling_CnC.$(ExeSuf)
	./reportCR4CR5modeling_CnC.$(ExeSuf) $(CnC_REGIONS) | tee reportCR4CR5modeling_CnC.$(LogSuf)


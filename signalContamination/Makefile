default: tables

include ../backgroundEstimation_common/common.make

#--------------------------------------------------------------------------

.PHONY: tables summary

MTpeakTables:
	@make --no-print-directory clean;
	@for REGION in $(SIGNAL_REGIONS);        \
	 do                                      \
		   echo "[Launching signal region $$REGION]";     \
	 	   make --quiet --no-print-directory compile TARGET=tableProducer_MTpeak SIGNAL_REGION_CUTS=$$REGION SIGNAL_REGION_TAG=\\\"$$REGION\\\"; \
		   mv tableProducer_MTpeak.$(ExeSuf) tableProducer_MTpeak_$$REGION.$(ExeSuf);			\
		   ./tableProducer_MTpeak_$$REGION.$(ExeSuf) > tableProducer_MTpeak_$$REGION.$(LogSuf) & \
	done;
	@for REGION in $(SIGNAL_REGIONS); 	\
	do									\
		echo "Waiting for $$REGION";	\
		CHECK="nonEmpty";				\
		while [ "$$CHECK" != "" ];		\
		do								\
			sleep 1;					\
			CHECK=`ps kstart_time | grep tableProducer_MTpeak_$$REGION.$(ExeSuf) | grep -v grep`;	\
		done;							\
	done;								\
	echo "Done";

summary:
	@make --no-print-directory produceSummary SIGNAL_REGIONS="$(CnC_T2tt)"    TAG="CnC_T2tt"    LABEL="Cut & count, T2tt"
	@make --no-print-directory produceSummary SIGNAL_REGIONS="$(CnC_T2bw025)" TAG="CnC_T2bw025" LABEL="Cut & count, T2bw, x=0.25"
	@make --no-print-directory produceSummary SIGNAL_REGIONS="$(CnC_T2bw050)" TAG="CnC_T2bw050" LABEL="Cut & count, T2bw, x=0.50"
	@make --no-print-directory produceSummary SIGNAL_REGIONS="$(CnC_T2bw075)" TAG="CnC_T2bw075" LABEL="Cut & count, T2bw, x=0.75"
	@make --no-print-directory produceSummary SIGNAL_REGIONS="$(BDT_T2tt)"    TAG="BDT_T2tt"    LABEL="BDT, T2tt"
	@make --no-print-directory produceSummary SIGNAL_REGIONS="$(BDT_T2bw025)" TAG="BDT_T2bw025" LABEL="BDT, T2bw, x=0.25"
	@make --no-print-directory produceSummary SIGNAL_REGIONS="$(BDT_T2bw050)" TAG="BDT_T2bw050" LABEL="BDT, T2bw, x=0.50"
	@make --no-print-directory produceSummary SIGNAL_REGIONS="$(BDT_T2bw075)" TAG="BDT_T2bw075" LABEL="BDT, T2bw, x=0.75"

produceSummary: summaryProducer.$(ExeSuf)
	./summaryProducer.$(ExeSuf) 		"$(TAG)" "$(LABEL)" $(SIGNAL_REGIONS)



#SIGNAL_REGIONS=BDT_T2bw025_1 BDT_T2bw025_3 BDT_T2bw025_4_highLSP BDT_T2bw025_4_lowLSP BDT_T2bw025_6 BDT_T2bw050_5 BDT_T2bw050_6       
SIGNAL_REGIONS=BDT_T2bw050_5_highDM

syncCheck:
	@make --no-print-directory clean;
	@for REGION in $(SIGNAL_REGIONS);        \
	 do                                      \
		   echo "[Launching signal region $$REGION]";     \
	 	   make --quiet --no-print-directory compile TARGET=syncCheck SIGNAL_REGION_CUTS=$$REGION SIGNAL_REGION_TAG=\\\"$$REGION\\\"; \
		   mv syncCheck.$(ExeSuf) syncCheck_$$REGION.$(ExeSuf);			\
		   ./syncCheck_$$REGION.$(ExeSuf) > syncCheck_$$REGION.$(LogSuf) & \
	done;
	@for REGION in $(SIGNAL_REGIONS); 	\
	do									\
		echo "Waiting for $$REGION";	\
		CHECK="nonEmpty";				\
		while [ "$$CHECK" != "" ];		\
		do								\
			sleep 1;					\
			CHECK=`ps kstart_time | grep syncCheck_$$REGION.$(ExeSuf) | grep -v grep`;	\
		done;							\
	done;								\
	echo "Done";



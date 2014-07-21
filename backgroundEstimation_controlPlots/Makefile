default: controlPlots

include ../backgroundEstimation_common/common.make

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


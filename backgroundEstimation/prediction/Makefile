
#------------------------------------------------------------------------------

default: tables predictions

#------------------------------------------------------------------------------

include ../common/common.make

tables:
	@make --no-print-directory clean 
	@for REGION in $(SIGNAL_REGIONS); 		  \
	 do									      \
	        echo "[Lauching signal region $$REGION]";     \
	        make --quiet --no-print-directory compile TARGET=yieldTableProducer SIGNAL_REGION_CUTS=$$REGION SIGNAL_REGION_TAG=\\\"$$REGION\\\"; 	\
			mv yieldTableProducer.$(ExeSuf) yieldTableProducer_$$REGION.$(ExeSuf); \
			./yieldTableProducer_$$REGION.$(ExeSuf) ${ARG} > yieldTableProducer_$$REGION.$(LogSuf) & \
	done;
	@for REGION in $(SIGNAL_REGIONS); 	\
	do									\
		echo "Waiting for $$REGION";	\
		CHECK="nonEmpty";				\
		while [ "$$CHECK" != "" ];		\
		do								\
			sleep 1;					\
			CHECK=`ps kstart_time | grep yieldTableProducer_$$REGION.$(ExeSuf) | grep -v grep`;	\
		done;							\
	done;								\
	echo "Done";

predictions: backgroundEstimation.$(ExeSuf)
	@for REGION in $(SIGNAL_REGIONS);							\
	 do															\
	 	   echo "[Calculating prediction for $$REGION]";		\
		   ./backgroundEstimation.$(ExeSuf) $$REGION $(ARG);	\
	 done;


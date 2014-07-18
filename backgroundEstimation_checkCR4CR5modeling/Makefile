ObjSuf        = o
SrcSuf        = C
ExeSuf        = run
LogSuf        = log

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs) 

DEFINES       = -DSIGNAL_REGION_CUTS=$(SIGNAL_REGION_CUTS)
DEFINES      += -DSIGNAL_REGION_TAG=$(SIGNAL_REGION_TAG)

CXX           = g++
CXXFLAGS      = -std=c++0x
CXXFLAGS     += -O -Wall -fPIC $(DEFINES) -Wno-unused-result -Wshadow
CXXFLAGS     += $(ROOTCFLAGS) -I./
CXXFLAGS     += -I../sonicScrewdriver/

LD            = g++ 
LDFLAGS       = -g -O -Wall -fPIC -Wl,--no-undefined 
LDFLAGS      += $(ROOTLIBS)  -lEG -lTMVA 
ROOFITFLAGS	     += -lRooFit -lRooFitCore
LDFLAGS      += -l:`pwd`/../.vectorDictionnary_C.so 
LDFLAGS      += -l:`pwd`/../sonicScrewdriver/libSonicScrewdriver.so

SOFLAGS       = -shared
LIBS          =  

# Uncomment this line if you want to use a script to parse & colorize gcc output
# (You can also export this variable from your bashrc)
#GCCPARSER     = 2>&1 | python ~/littleScripts/colorGcc.py

#------------------------------------------------------------------------------
SOURCES       = $(wildcard *.$(SrcSuf))
OBJECTS       = $(SOURCES:.$(SrcSuf)=.$(ObjSuf))
EXECUTABLES   = $(SOURCES:.$(SrcSuf)=.$(ExeSuf))
LOGS          = $(SOURCES:.$(SrcSuf)=.$(LogSuf))
#------------------------------------------------------------------------------

# Running the background estimation on every signal regions

SIGNAL_REGIONS = 
SIGNAL_REGIONS += preselection

#SIGNAL_REGIONS += cutAndCount_T2tt_offShellLoose         cutAndCount_T2tt_offShellTight     cutAndCount_T2tt_lowDeltaM             cutAndCount_T2tt_mediumDeltaM   cutAndCount_T2tt_highDeltaM
#SIGNAL_REGIONS += cutAndCount_T2bw025_offShell  cutAndCount_T2bw025_lowMasses    cutAndCount_T2bw025_highMasses
#SIGNAL_REGIONS += cutAndCount_T2bw050_offShell  cutAndCount_T2bw050_lowMasses    cutAndCount_T2bw050_mediumDeltaM cutAndCount_T2bw050_highDeltaM
#SIGNAL_REGIONS += cutAndCount_T2bw075_lowDeltaM cutAndCount_T2bw075_mediumDeltaM cutAndCount_T2bw075_highDeltaM

SIGNAL_REGIONS += BDT_T2tt_1			BDT_T2tt_2_lowDM		BDT_T2tt_2_highDM	BDT_T2tt_5_lowDM	BDT_T2tt_5_mediumDM		BDT_T2tt_5_highDM
SIGNAL_REGIONS += BDT_T2bw075_1			BDT_T2bw075_2			BDT_T2bw075_3		BDT_T2bw075_5_lowDM	BDT_T2bw075_5_highDM
SIGNAL_REGIONS += BDT_T2bw050_1_lowDM	BDT_T2bw050_1_highDM	BDT_T2bw050_3		BDT_T2bw050_4		BDT_T2bw050_5			BDT_T2bw050_6
SIGNAL_REGIONS += BDT_T2bw025_1			BDT_T2bw025_3			BDT_T2bw025_4		BDT_T2bw025_6

#------------------------------------------------------------------------------

JET_REQUIREMENTS  =  CR4_1j CR4_2j CR4_3j CR4_4j
JET_REQUIREMENTS += CR4_4j_50evts CR4_4j_100evts CR4_4j_150evts

#------------------------------------------------------------------------------

default: tablesCR4CR5modeling scaleFactorsCR4CR5modeling reportCR4CR5modeling

tablesCR4CR5modeling: yieldTableProducer.$(ExeSuf)
	@for JET_REQUIREMENT in $(JET_REQUIREMENTS); 		      \
	 do									      		  	      \
		echo "[Checking jet requirement $$JET_REQUIREMENT]";  \
	   	make --no-print-directory tables JET_REQUIREMENT=$$JET_REQUIREMENT;    \
	done;

scaleFactorsCR4CR5modeling: scaleFactorsProducer.$(ExeSuf)
	@for JET_REQUIREMENT in $(JET_REQUIREMENTS); 		      \
	 do									      		  	      \
		echo "[Checking jet requirement $$JET_REQUIREMENT]";  \
		make --no-print-directory scaleFactors JET_REQUIREMENT=$$JET_REQUIREMENT;  \
	done;

reportCR4CR5modeling: reportCR4CR5modeling.$(ExeSuf)
	./reportCR4CR5modeling.$(ExeSuf) $(SIGNAL_REGIONS) | tee reportCR4CR5modeling.$(LogSuf)

tables:
	@make --no-print-directory clean 
	@for REGION in $(SIGNAL_REGIONS); 		  \
	 do									      \
	        echo "[Lauching signal region $$REGION]";     \
	        make --quiet --no-print-directory compile TARGET=yieldTableProducer SIGNAL_REGION_CUTS=$$REGION SIGNAL_REGION_TAG=\\\"$$REGION\\\"; 	\
			mv yieldTableProducer.$(ExeSuf) yieldTableProducer_$$REGION.$(ExeSuf); \
			./yieldTableProducer_$$REGION.$(ExeSuf) $(JET_REQUIREMENT) > yieldTableProducer_$$REGION.$(LogSuf) & \
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

scaleFactors: scaleFactorsProducer.$(ExeSuf)
	@for REGION in $(SIGNAL_REGIONS);					\
	 do													\
	 	   echo "[Running on $$REGION]";			  	\
		   ./scaleFactorsProducer.$(ExeSuf) $$REGION $(JET_REQUIREMENT);	\
	 done;

#------------------------------------------------------------------------------

compile: ../.vectorDictionnary_C.so $(TARGET).$(ExeSuf)

run:
	./$(TARGET).$(ExeSuf) | tee $(TARGET).$(LogSuf)

clean:
	@echo "Cleaning..."
	@rm -f *.$(ObjSuf) *.$(ExeSuf) *.$(LogSuf)

../.vectorDictionnary_C.so: ../.vectorDictionnary.C
	cd ..; root -q -l -L .vectorDictionnary.C+

%.$(ExeSuf): %.$(SrcSuf) 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) ${ARGS} -o $@ $< $(LIBS) $(GCCPARSER)

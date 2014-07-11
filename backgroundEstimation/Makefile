SHELL := /bin/bash

ObjSuf        = o
SrcSuf        = C
SrcSuf2       = cpp
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
TARGET = yieldTableProducer
#TARGET = backgroundEstimation
#TARGET = controlPlotsProducer
#TARGET = reportProducer
#------------------------------------------------------------------------------

default: compile run
	
compile: ../.vectorDictionnary_C.so $(TARGET).$(ExeSuf)

run:
	./$(TARGET).$(ExeSuf) | tee $(TARGET).$(LogSuf)

browse:
	export PLOTS_TO_BROWSE=`ls ../plots/$(shell basename `pwd`)/*.root`; root -l .browse

clean:
	@echo "Cleaning..."
	@rm -f *.$(ObjSuf) *.$(ExeSuf) *.$(LogSuf)

#------------------------------------------------------------------------------

# Running the background estimation on every signal regions

SIGNAL_REGIONS = 
SIGNAL_REGIONS += preselection
#SIGNAL_REGIONS += cutAndCount_T2tt_offShellLoose         cutAndCount_T2tt_offShellTight     cutAndCount_T2tt_lowDeltaM             cutAndCount_T2tt_mediumDeltaM   cutAndCount_T2tt_highDeltaM
#SIGNAL_REGIONS += cutAndCount_T2bw025_veryOffShell_loose cutAndCount_T2bw025_offShell_loose cutAndCount_T2bw025_lowDeltaM_tight    cutAndCount_T2bw025_highDeltaM
#SIGNAL_REGIONS += cutAndCount_T2bw050_offShell_loose     cutAndCount_T2bw050_lowMass        cutAndCount_T2bw050_mediumDeltaM_loose cutAndCount_T2bw050_highDeltaM
#SIGNAL_REGIONS += cutAndCount_T2bw075_lowDeltaM_tight    cutAndCount_T2bw075_mediumDeltaM   cutAndCount_T2bw075_highDeltaM
SIGNAL_REGIONS += BDT_T2tt_1  BDT_T2tt_2  BDT_T2tt_5_loose  BDT_T2tt_5_tight 
SIGNAL_REGIONS += BDT_T2bw025_1 BDT_T2bw025_3 BDT_T2bw025_4  BDT_T2bw025_6       
SIGNAL_REGIONS += BDT_T2bw050_1_loose BDT_T2bw050_1_tight BDT_T2bw050_3 BDT_T2bw050_4 BDT_T2bw050_5  BDT_T2bw050_6  
SIGNAL_REGIONS += BDT_T2bw075_1 BDT_T2bw075_2 BDT_T2bw075_3 BDT_T2bw075_5

CONTROL_REGIONS_2l =  CR4_1j CR4_2j CR4_3j CR4_4j
CONTROL_REGIONS_2l += CR4_4j_50evts CR4_4j_100evts CR4_4j_150evts

SF_2l:
	@for REGION in $(CONTROL_REGIONS_2l); 		  \
	 do									      \
		echo "[Lauching signal region $$REGION]";     \
	    	make tables ARG=$$REGION;     \
		make predictions ARG=--CR4 CR=$$REGION;		\
	done;
	make SFreport

#mv scaleFactors/*.tab scaleFactors/$$REGION/;	 

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


plots4CR1: plots4CR1Estimation.$(ExeSuf)
	./plots4CR1Estimation.$(ExeSuf) 

templateFitCR1: templateFitCR1.$(ExeSuf) 
	./templateFitCR1.$(ExeSuf)

computeBDTCuts: computeBDTCuts.$(ExeSuf)
	./computeBDTCuts.$(ExeSuf) 



predictions: backgroundEstimation.$(ExeSuf)
	@for REGION in $(SIGNAL_REGIONS);					\
	 do													\
	 	   echo "[Running on $$REGION]";			    \
		   ./backgroundEstimation.$(ExeSuf) $$REGION  ${ARG} ${CR};	\
	 done;

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

SFreport: SF_ttbar2l.$(ExeSuf)
	./SF_ttbar2l.$(ExeSuf) $(SIGNAL_REGIONS) | tee SF_ttbar2l.$(LogSuf) > SF_ttbar2l.log

report: reportProducer.$(ExeSuf)
	./reportProducer.$(ExeSuf) $(SIGNAL_REGIONS) | tee reportProducer.$(LogSuf) > reports/report.tex && cd reports && pdflatex report.tex 

syst: reportGenSystProducer.$(ExeSuf)
	./reportGenSystProducer.$(ExeSuf) $(SIGNAL_REGIONS) | tee reportGenSystProducer.$(LogSuf) > reports/reportWithSyst.tex && cd reports && pdflatex report.tex

#------------------------------------------------------------------------------

../.vectorDictionnary_C.so: ../.vectorDictionnary.C
	cd ..; root -q -l -L .vectorDictionnary.C+

%.$(ExeSuf): %.$(SrcSuf) 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) ${ARGS} -o $@ $< $(LIBS) $(GCCPARSER)

%.$(ExeSuf): %.$(SrcSuf2) 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) ${ROOFITFLAGS} -o $@ $< $(LIBS) $(GCCPARSER)


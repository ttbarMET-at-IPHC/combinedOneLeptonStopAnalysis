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
ROOFITFLAGS  += -lRooFit -lRooFitCore
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
#TARGET = yieldTableProducer
#TARGET = backgroundEstimation
#TARGET = controlPlotsProducer
#TARGET = reportProducer
#------------------------------------------------------------------------------

everything: MTtailCorrection checkCR4CR5modeling backgroundPrediction report

#------------------------------------------------------------------------------

MTtailCorrection:
	@make -C ../backgroundEstimation_MTtailCorrection 	 default

checkCR4CR5modeling:
	@make -C ../backgroundEstimation_checkCR4CR5modeling default

backgroundPrediction:
	@make -C ../backgroundEstimation_prediction 		 default

report:
	@DATE=$(shell date +%Y_%m_%d_%H:%M:%S);			\
	 mkdir -p ./results/$$DATE/;					\
	 cp -r ../backgroundEstimation_MTtailCorrection/results    			./results/$$DATE/MTtailCorrection/;						\
	 cp -r ../backgroundEstimation_MTtailCorrection/inputPlots 			./results/$$DATE/MTtailCorrection/inputPlots/;			\
	 cp -r ../backgroundEstimation_checkCR4CR5modeling/results			./results/$$DATE/checkCR4CR5modeling/;					\
	 cp -r ../backgroundEstimation_checkCR4CR5modeling/rawYieldTables	./results/$$DATE/checkCR4CR5modeling/rawYieldTables/;	\
	 cp -r ../backgroundEstimation_checkCR4CR5modeling/scaleFactors		./results/$$DATE/checkCR4CR5modeling/scaleFactors/;		\
	 cp -r ../backgroundEstimation_prediction/prediction				./results/$$DATE/prediction/;							\
	 cp -r ../backgroundEstimation_prediction/rawYieldTables			./results/$$DATE/prediction/rawYieldTables/;			\
	 cp -r ../backgroundEstimation_prediction/scaleFactors				./results/$$DATE/prediction/scaleFactors/;				\
	 cp -r ../backgroundEstimation_prediction/systematics				./results/$$DATE/prediction/systematics/;				\
	 rm -f ./results/latest; cd results; ln -s ./$$DATE/ ./latest

#./reportProducer.$(ExeSuf) $(SIGNAL_REGIONS) | tee reportProducer.$(LogSuf) > reports/report.tex && cd reports && pdflatex report.tex 



#computeBDTCuts: computeBDTCuts.$(ExeSuf)
#	./computeBDTCuts.$(ExeSuf) 

#syst: reportGenSystProducer.$(ExeSuf)
#	./reportGenSystProducer.$(ExeSuf) $(SIGNAL_REGIONS) | tee reportGenSystProducer.$(LogSuf) > reports/reportWithSyst.tex && cd reports && pdflatex report.tex

#------------------------------------------------------------------------------

compile: ../.vectorDictionnary_C.so $(TARGET).$(ExeSuf)

run:
	./$(TARGET).$(ExeSuf) | tee $(TARGET).$(LogSuf)

browse:
	export PLOTS_TO_BROWSE=`ls ../plots/$(shell basename `pwd`)/*.root`; root -l .browse

clean:
	@echo "Cleaning..."
	@rm -f *.$(ObjSuf) *.$(ExeSuf) *.$(LogSuf)

cleanAll:
	@make -C ../backgroundEstimation_MTtailCorrection 	 clean
	@make -C ../backgroundEstimation_checkCR4CR5modeling clean
	@make -C ../backgroundEstimation_prediction 		 clean
	@make -C ./ 										 clean

../.vectorDictionnary_C.so: ../.vectorDictionnary.C
	cd ..; root -q -l -L .vectorDictionnary.C+

%.$(ExeSuf): %.$(SrcSuf) 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LIBS) $(GCCPARSER)


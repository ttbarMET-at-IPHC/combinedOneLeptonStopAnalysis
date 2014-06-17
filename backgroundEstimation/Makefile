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

SIGNAL_REGIONS = cutAndCount_T2tt_offShellLoose cutAndCount_T2tt_offShellTight cutAndCount_T2tt_lowDeltaM   cutAndCount_T2tt_mediumDeltaM  cutAndCount_T2tt_highDeltaM

tables:
	@make --no-print-directory clean 
	@for REGION in $(SIGNAL_REGIONS); 		  \
	 do									      \
	        echo "[Lauching signal region $$REGION]";     \
	        make --quiet --no-print-directory compile TARGET=yieldTableProducer SIGNAL_REGION_CUTS=$$REGION SIGNAL_REGION_TAG=\\\"$$REGION\\\"; 	\
			mv yieldTableProducer.$(ExeSuf) yieldTableProducer_$$REGION.$(ExeSuf); \
			./yieldTableProducer_$$REGION.$(ExeSuf) > yieldTableProducer_$$REGION.$(LogSuf) & \
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
	@for REGION in $(SIGNAL_REGIONS);					\
	 do													\
	 	   echo "[Running on $$REGION]";			    \
		   ./backgroundEstimation.$(ExeSuf) $$REGION;	\
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


report: reportProducer.$(ExeSuf)
	./reportProducer.$(ExeSuf) $(SIGNAL_REGIONS) | tee reportProducer.$(LogSuf) > reports/report.tex && cd reports && pdflatex report.tex

#------------------------------------------------------------------------------

../.vectorDictionnary_C.so: ../.vectorDictionnary.C
	cd ..; root -q -l -L .vectorDictionnary.C+

%.$(ExeSuf): %.$(SrcSuf) 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LIBS) $(GCCPARSER)


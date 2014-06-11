ObjSuf        = o
SrcSuf        = C
ExeSuf        = run
LogSuf        = log

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs) 

#SIGNAL_REGION_CUTS = cutAndCount_T2tt_offShellLoose
#SIGNAL_REGION_TAG = \"T2tt_offShellLoose\"

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
TARGET = backgroundEstimation
#TARGET = yieldTableProducer
#TARGET = controlPlots
#------------------------------------------------------------------------------

all: compile run
	
compile: ../.vectorDictionnary_C.so $(TARGET).$(ExeSuf)

run:
	./$(TARGET).$(ExeSuf) | tee $(TARGET).$(LogSuf)

browse:
	export PLOTS_TO_BROWSE=`ls ../plots/$(shell basename `pwd`)/*.root`; root -l .browse

clean:
	@echo "Cleaning..."
	@rm -f $(OBJECTS) $(EXECUTABLES) $(LOGS)

#------------------------------------------------------------------------------

../.vectorDictionnary_C.so: ../.vectorDictionnary.C
	cd ..; root -q -l -L .vectorDictionnary.C+

%.$(ExeSuf): %.$(SrcSuf) 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LIBS) $(GCCPARSER)


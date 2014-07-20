SHELL := /bin/bash

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
LDFLAGS      += $(ROOFITLDFLAGS)
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

default:../.vectorDictionnary_C.so plots MTTailCorrection

plots: plotsProducer.$(ExeSuf)
	./plotsProducer.$(ExeSuf)

MTTailCorrectionProducer:
	make --no-print-directory MTTailCorrectionProducer.run 											\
	ROOTCFLAGS="$(shell /opt/sbg/data/safe1/cms/echabert/root_v5.34.05/bin/root-config --cflags)" 	\
	ROOTLIBS="$(shell /opt/sbg/data/safe1/cms/echabert/root_v5.34.05/bin/root-config --libs)"     	\
	ROOFITLDFLAGS="-lRooFit -lRooFitCore"

MTTailCorrection: MTTailCorrectionProducer
	@echo ""
	@echo " [Producing scale factors to correct MT tail...]"
	@echo ""
	@LD_LIBRARY_PATH="$(shell /opt/sbg/data/safe1/cms/echabert/root_v5.34.05/bin/root-config --libdir):$$LD_LIBRARY_PATH" ./MTTailCorrectionProducer.$(ExeSuf) > MTTailCorrectionProducer.$(LogSuf)
	@cat results/SF_MTtail.tab

clean:
	@echo "Cleaning..."
	@rm -f *.$(ObjSuf) *.$(ExeSuf) *.$(LogSuf)

#------------------------------------------------------------------------------

../.vectorDictionnary_C.so: ../.vectorDictionnary.C
	cd ..; root -q -l -L .vectorDictionnary.C+

%.$(ExeSuf): %.$(SrcSuf) 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LIBS) $(GCCPARSER)



ObjSuf        = o
SrcSuf        = C

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs) 

DEFINES       = 

IPHC_FRAMEWORK = /opt/sbg/data/data4/cms/aaubin/analysisStudy_July13/CMSSW_5_3_2_patch4/src/

CXX           = g++
CXXFLAGS      = -O -Wall -fPIC $(DEFINES) -Wno-unused-result
CXXFLAGS      += $(ROOTCFLAGS) 
CXXFLAGS      += -I./
CXXFLAGS      += -I $(IPHC_FRAMEWORK)/IPHCDataFormat/
CXXFLAGS      += -I $(IPHC_FRAMEWORK)/NTuple/NTupleAnalysis/


LD            = g++ 
LDFLAGS       = -g -O -Wall -fPIC -Wl,--no-undefined 
LDFLAGS       += $(ROOTLIBS)  -lEG -lTMVA -lMinuit -lMinuit2
LDFLAGS       += -l:$(IPHC_FRAMEWORK)/IPHCDataFormat/NTFormat/src/libNTuple.so
LDFLAGS       += -l:$(IPHC_FRAMEWORK)/NTuple/NTupleAnalysis/libNTupleAna.so

SOFLAGS       = -shared
LIBS          = 

# Uncomment this line if you want to use a script to parse & colorize gcc output
# (You can also export this variable from your bashrc)
#GCCPARSER     = 2>&1 | python ~/littleScripts/colorGcc.py

#------------------------------------------------------------------------------
SOURCES       = $(wildcard *.$(SrcSuf))
OBJECTS       = $(SOURCES:.$(SrcSuf)=.$(ObjSuf))
EXECUTABLES   = $(SOURCES:.$(SrcSuf)=)
#------------------------------------------------------------------------------
DEFAULT_TARGET = testSelection
#------------------------------------------------------------------------------

all: compile run
	
compile: $(DEFAULT_TARGET)

run:
	./$(DEFAULT_TARGET)

clean:
	@echo "Cleaning..."
	@rm -f $(OBJECTS) $(EXECUTABLES)

#------------------------------------------------------------------------------

$(DEFAULT_TARGET): $(DEFAULT_TARGET).$(SrcSuf)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LIBS) $(GCCPARSER)


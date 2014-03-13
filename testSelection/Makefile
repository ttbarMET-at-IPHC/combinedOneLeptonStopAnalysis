ObjSuf        = o
SrcSuf        = C

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs) 

DEFINES       = 

CXX           = g++
CXXFLAGS      = -O -Wall -fPIC $(DEFINES) -Wno-unused-result
CXXFLAGS      += $(ROOTCFLAGS) 
CXXFLAGS      += -I./
CXXFLAGS      += -I `pwd`/../../../../../IPHCDataFormat/
CXXFLAGS      += -I `pwd`/../../../

LD            = g++ 
LDFLAGS       = -g -O -Wall -fPIC -Wl,--no-undefined 
LDFLAGS       += $(ROOTLIBS)  -lEG -lTMVA
LDFLAGS       += -l:`pwd`/../../../../../IPHCDataFormat/NTFormat/src/libNTuple.so
LDFLAGS       += -l:`pwd`/../../../libNTupleAna.so

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


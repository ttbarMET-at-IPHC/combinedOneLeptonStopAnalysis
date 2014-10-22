# Extensions configuration
SrcSuf        = cc
HeadSuf       = h
ObjSuf        = o

# ROOT flags
ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs) -lMinuit -lMathMore

# Compiler
CXX           = g++

# Linker
LD			  = g++

# Include paths
INCLUDES       = $(ROOTCFLAGS) 
INCLUDES      += -I ./
INCLUDES      += -I $(NTUPLE_ANALYSISFRAMEWORK)
INCLUDES      += -I $(NTUPLE_DATAFORMAT)

# Compiler options
CXXFLAGS      = -std=c++0x -g -O -Wall -fPIC -Wno-unused-result -Wshadow -Wl,--no-undefined $(INCLUDES)

# Linker options
LDFLAGS        = -g -O -Wall -fPIC
LDFLAGS       += $(ROOTLIBS) -lEG -lTMVA -lProof -lProofPlayer -lMinuit -lMinuit2
LDFLAGS       += -l:$(NTUPLE_DATAFORMAT)/NTFormat/src/libNTuple.so
LDFLAGS       += -l:$(NTUPLE_ANALYSISFRAMEWORK)/libNTupleAna.so

SOFLAGS        = -shared

#------------------------------------------------------------------------------
SOURCES       = $(wildcard *.$(SrcSuf))
OBJECTS       = $(SOURCES:.$(SrcSuf)=.$(ObjSuf))
HEADERS       = $(SOURCES:.$(SrcSuf)=.$(HeadSuf))
#------------------------------------------------------------------------------

libObjectAndEventSelection.so: $(OBJECTS)
	@echo "Building libObjectAndEventSelection..."
	$(LD) -L$(ROOTSYS)/lib $(LIBS) $(SOFLAGS) $(LDFLAGS) $+ -o $@

clean:
	@echo "Cleaning..."
	@rm -rf $(OBJECTS) *.so

#------------------------------------------------------------------------------


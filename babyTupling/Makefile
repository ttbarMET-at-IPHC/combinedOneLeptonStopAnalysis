# Extensions configuration
ObjSuf        = o
SrcSuf        = C
ExeSuf        = run
LogSuf        = log

# Root flags

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs) 

# Compiler
CXX           = g++

# Compiler options
CXXFLAGS      = -std=c++0x
CXXFLAGS      += -O -Wall -fPIC -Wno-unused-result -Wshadow

# Include paths
INCLUDES      += $(ROOTCFLAGS) 
INCLUDES      += -I./
INCLUDES      += -I `pwd`/../../../../../IPHCDataFormat/

# Linker options
LDFLAGS       = -g -O -Wall -fPIC -Wl,--no-undefined 
LDFLAGS       += $(ROOTLIBS) -lEG -lTMVA
LDFLAGS       += -l:`pwd`/libNTuple.so
LDFLAGS       += -l:`pwd`/../../../libNTupleAna.so

# Uncomment this line if you want to use a script to parse & colorize gcc output
# (You can also export this variable from your bashrc)
#GCCPARSER     = 2>&1 | python ~/littleScripts/colorGcc.py

LIBNTUPLE = `pwd`/../../../libNTupleAna.so

#------------------------------------------------------------------------------
SOURCES       = $(wildcard *.$(SrcSuf))
OBJECTS       = $(SOURCES:.$(SrcSuf)=.$(ObjSuf))
EXECUTABLES   = $(SOURCES:.$(SrcSuf)=.$(ExeSuf))
LOGS          = $(SOURCES:.$(SrcSuf)=.$(LogSuf))
#------------------------------------------------------------------------------
DEFAULT_TARGET = ProofLauncher
#------------------------------------------------------------------------------

all: compile run
	
compile: copyLibAna $(DEFAULT_TARGET).$(ExeSuf)

copyLibAna:
	cp $(LIB_NTUPLEANA) ./libNTuple.so

run:
	./$(DEFAULT_TARGET).$(ExeSuf) | tee $(DEFAULT_TARGET).$(LogSuf)

clean:
	@echo "Cleaning..."
	@rm -f $(OBJECTS) $(EXECUTABLES) $(LOGS)

#------------------------------------------------------------------------------

$(DEFAULT_TARGET).$(ExeSuf): $(DEFAULT_TARGET).$(SrcSuf)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $< $(GCCPARSER)


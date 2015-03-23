ObjSuf        = o
SrcSuf        = C
ExeSuf        = run
LogSuf        = log

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs) 

DEFINES       = 

CXX           = g++
CXXFLAGS      = -std=c++0x
CXXFLAGS      += -O -Wall -Wextra -fPIC $(DEFINES) -Wno-unused-result -Wshadow -Wno-unused-parameter
CXXFLAGS      += $(ROOTCFLAGS) -I./
CXXFLAGS     += -I../../../sonicScrewdriver/

LD            = g++ 
LDFLAGS       = -g -O -Wall -fPIC -Wl,--no-undefined 
LDFLAGS       += $(ROOTLIBS)  -lEG -lTMVA
LDFLAGS      += -l:`pwd`/../../../sonicScrewdriver/libSonicScrewdriver.so

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
DEFAULT_TARGET = mapProducer_BDT
#------------------------------------------------------------------------------

all: compile run
	
compile: $(DEFAULT_TARGET).$(ExeSuf)

run:
	./$(DEFAULT_TARGET).$(ExeSuf) | tee $(DEFAULT_TARGET).$(LogSuf)

clean:
	@echo "Cleaning..."
	@rm -f $(OBJECTS) $(EXECUTABLES) $(LOGS)

#------------------------------------------------------------------------------

$(DEFAULT_TARGET).$(ExeSuf): $(DEFAULT_TARGET).$(SrcSuf)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LIBS) $(GCCPARSER)


ObjSuf        = o
SrcSuf        = C

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs) 

DEFINES       = 

CXX           = g++
CXXFLAGS      = -std=c++0x
CXXFLAGS      += -O -Wall -fPIC $(DEFINES) -Wno-unused-result
CXXFLAGS      += $(ROOTCFLAGS) -I./ -I../../sonicScrewdriver/

LD            = g++ 
LDFLAGS       = -g -O -Wall -fPIC -Wl,--no-undefined 
LDFLAGS       += $(ROOTLIBS)  -lEG -lTMVA
LDFLAGS       += -l:`pwd`/../../.vectorDictionnary_C.so 
LDFLAGS       += -l:`pwd`/../../sonicScrewdriver/libSonicScrewdriver.so

SOFLAGS       = -shared
LIBS          = 

#------------------------------------------------------------------------------
SOURCES       = $(wildcard *.$(SrcSuf))
OBJECTS       = $(SOURCES:.$(SrcSuf)=.$(ObjSuf))
EXECUTABLES   = $(SOURCES:.$(SrcSuf)=)
#------------------------------------------------------------------------------

DEFAULT_FILE  = validation

all: compile run browse 
	
compile: ../../.vectorDictionnary_C.so $(DEFAULT_FILE)

run:
	./$(DEFAULT_FILE)

browse:
	root -q -l -X .plots

../../.vectorDictionnary_C.so: ../../.vectorDictionnary.C
	cd ..; root -q -l -L .vectorDictionnary.C+

%.o: %.C
	$(CXX) $(CXXFLAGS) -c $<

%: %.o
	$(LD) $(LDFLAGS) -o $@ $< $(LIBS) 

clean:
	@echo "Cleaning..."
	@rm -f $(OBJECTS) $(EXECUTABLES)

#.SUFFIXES: .$(SrcSuf) .C .o .so




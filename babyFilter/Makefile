ObjSuf        = o
SrcSuf        = C

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs) 

DEFINES       = 

CXX           = g++
CXXFLAGS      = -O -Wall -fPIC $(DEFINES) -Wno-unused-result
CXXFLAGS      += $(ROOTCFLAGS) -I./

LD            = g++ 
LDFLAGS       = -g -O -Wall -fPIC -Wl,--no-undefined 
LDFLAGS       += $(ROOTLIBS)  -lEG -lTMVA
LDFLAGS       += -l:`pwd`/../.vectorDictionnary_C.so 

SOFLAGS       = -shared
LIBS          = 

#------------------------------------------------------------------------------
SOURCES       = $(wildcard *.$(SrcSuf))
OBJECTS       = $(SOURCES:.$(SrcSuf)=.$(ObjSuf))
EXECUTABLES   = $(SOURCES:.$(SrcSuf)=)
#------------------------------------------------------------------------------

DEFAULT_FILE  = babyFilter

all: compile run 
	
compile: ../.vectorDictionnary_C.so $(DEFAULT_FILE)

run:
	./$(DEFAULT_FILE)

../.vectorDictionnary_C.so: ../.vectorDictionnary.C
	cd ..; root -l -L .vectorDictionnary.C+

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $<

%: %.o
	$(LD) $(LDFLAGS) -o $@ $< $(LIBS) 

clean:
	@echo "Cleaning..."
	@rm -f $(OBJECTS) $(EXECUTABLES)

#.SUFFIXES: .$(SrcSuf) .C .o .so




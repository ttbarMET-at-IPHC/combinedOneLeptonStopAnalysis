ObjSuf        = o
SrcSuf        = C

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs) 

# Linux with egcs
DEFINES       = 

CXX           = g++
CXXFLAGS      = -O -Wall -fPIC $(DEFINES) -Wno-unused-result
CXXFLAGS      += $(ROOTCFLAGS) -I./

LD            = g++ 
LDFLAGS       = -g -O -Wall -fPIC -Wl,--no-undefined 
LDFLAGS       += $(ROOTLIBS)  -lEG -lTMVA
LDFLAGS       += -l:`pwd`/.vectorDictionnary_C.so 
LDFLAGS       += -I ../sonicScrewdriver -l:`pwd`/../sonicScrewdriver/libSonicScrewdriver.so


SOFLAGS       = -shared
LIBS          = 

#------------------------------------------------------------------------------
SOURCES       = $(wildcard *.$(SrcSuf))
OBJECTS       = $(SOURCES:.$(SrcSuf)=.$(ObjSuf))
EXECUTABLES   = $(SOURCES:.$(SrcSuf)=)
#------------------------------------------------------------------------------

all: .vectorDictionnary_C.so Plotting

.vectorDictionnary_C.so: .vectorDictionnary.C
	root -l -L .vectorDictionnary.C+

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $<

%: %.o
	$(LD) $(LDFLAGS) -o $@ $< $(LIBS) 

clean:
	@echo "Cleaning..."
	@rm -f $(OBJECTS) $(EXECUTABLES)

#.SUFFIXES: .$(SrcSuf) .C .o .so




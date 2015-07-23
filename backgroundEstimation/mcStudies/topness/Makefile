
ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs) 

CXX           = g++
CXXFLAGS      = -std=c++0x
CXXFLAGS     += -O -Wall -fPIC $(DEFINES) -Wno-unused-result -Wshadow
CXXFLAGS     += $(ROOTCFLAGS) -I./

LD            = g++ 
LDFLAGS       = -g -O3 -Wall -fPIC -Wl,-undefined,error
LDFLAGS      += $(ROOTLIBS)  -lEG -lTMVA 

SOFLAGS       = -shared
LIBS          =  

# Uncomment this line if you want to use a script to parse & colorize gcc output
# (You can also export this variable from your bashrc)
#GCCPARSER     = 2>&1 | python ~/littleScripts/colorGcc.py

#------------------------------------------------------------------------------

lib: libTopness.so

clean:
	@echo "Cleaning..."
	@rm -f *.so

#------------------------------------------------------------------------------

libTopness.so: topness.cc topness2.cc
	$(LD) $(CXXFLAGS) $(LDFLAGS) $(SOFLAGS) -o $@ $< $ $(word 2,$^)$ $(LIBS) $(GCCPARSER)

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
LDFLAGS      += -l:`pwd`/../.vectorDictionnary_C.so 
LDFLAGS      += -l:`pwd`/../sonicScrewdriver/libSonicScrewdriver.so

SOFLAGS       = -shared
LIBS          =  

# Uncomment this line if you want to use a script to parse & colorize gcc output
# (You can also export this variable from your bashrc)
#GCCPARSER     = 2>&1 | python ~/littleScripts/colorGcc.py

#------------------------------------------------------------------------------

CnC_T2tt 	= cutAndCount_T2tt_offShellLoose    cutAndCount_T2tt_offShellTight      cutAndCount_T2tt_lowDeltaM         cutAndCount_T2tt_mediumDeltaM   cutAndCount_T2tt_highDeltaM
CnC_T2bw025 = cutAndCount_T2bw025_offShell  	cutAndCount_T2bw025_lowMasses       cutAndCount_T2bw025_highMasses
CnC_T2bw050 = cutAndCount_T2bw050_offShell  	cutAndCount_T2bw050_lowMasses       cutAndCount_T2bw050_mediumDeltaM 	cutAndCount_T2bw050_highDeltaM
CnC_T2bw075 = cutAndCount_T2bw075_lowDeltaM 	cutAndCount_T2bw075_mediumDeltaM    cutAndCount_T2bw075_highDeltaM

#BDT_T2tt 	= BDT_T2tt_1_lowLSP       BDT_T2tt_1_highLSP
BDT_T2tt 	= BDT_T2tt_1_lowLSP       BDT_T2tt_1_highLSP      BDT_T2tt_2_lowDM      BDT_T2tt_2_highDM       BDT_T2tt_5_lowDM        BDT_T2tt_5_mediumDM     BDT_T2tt_5_highDM
BDT_T2bw025 = BDT_T2bw025_1           BDT_T2bw025_3_lowDM     BDT_T2bw025_3_highDM  BDT_T2bw025_4           BDT_T2bw025_6
BDT_T2bw050 = BDT_T2bw050_1_lowLSP    BDT_T2bw050_1_highLSP   BDT_T2bw050_3         BDT_T2bw050_4           BDT_T2bw050_5_lowDM     BDT_T2bw050_5_highDM    BDT_T2bw050_6
BDT_T2bw075 = BDT_T2bw075_1           BDT_T2bw075_2           BDT_T2bw075_3         BDT_T2bw075_5_lowDM     BDT_T2bw075_5_highDM
                  
CnC_REGIONS  = preselection 
CnC_REGIONS += $(CnC_T2tt) $(CnC_T2bw025) $(CnC_T2bw050) $(CnC_T2bw075)
BDT_REGIONS  = $(BDT_T2tt) $(BDT_T2bw025) $(BDT_T2bw050) $(BDT_T2bw075)
SIGNAL_REGIONS = $(CnC_REGIONS) $(BDT_REGIONS)

#------------------------------------------------------------------------------

SOURCES       = $(wildcard *.$(SrcSuf))
OBJECTS       = $(SOURCES:.$(SrcSuf)=.$(ObjSuf))
EXECUTABLES   = $(SOURCES:.$(SrcSuf)=.$(ExeSuf))
LOGS          = $(SOURCES:.$(SrcSuf)=.$(LogSuf))

#------------------------------------------------------------------------------
compile: ../.vectorDictionnary_C.so $(TARGET).$(ExeSuf)

run:
	./$(TARGET).$(ExeSuf) | tee $(TARGET).$(LogSuf)

browse:
	export PLOTS_TO_BROWSE=`ls ../plots/$(shell basename `pwd`)/*.root`; root -l .browse

clean:
	@echo "Cleaning..."
	@rm -f *.$(ObjSuf) *.$(ExeSuf) *.$(LogSuf)

#------------------------------------------------------------------------------

../.vectorDictionnary_C.so: ../.vectorDictionnary.C
	cd ..; root -q -l -L .vectorDictionnary.C+

%.$(ExeSuf): %.$(SrcSuf) ../.vectorDictionnary_C.so
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $< $(LIBS) $(GCCPARSER)

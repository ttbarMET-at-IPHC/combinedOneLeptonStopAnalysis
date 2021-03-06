# Extensions configuration
ObjSuf        = o
SrcSuf        = C
ExeSuf        = run
LogSuf        = log

# Root flags
ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs) -lEG -lTMVA -lProof -lProofPlayer -lMinuit -lMinuit2

# Compiler
CXX           = g++

# Compiler options
CXXFLAGS      = -std=c++0x -g -O -Wall -fPIC -Wno-unused-result -Wshadow -Wl,--no-undefined

# Include paths
INCLUDES       = $(ROOTCFLAGS) 
INCLUDES      += -I ./
INCLUDES      += -I $(NTUPLE_ANALYSISFRAMEWORK)
INCLUDES      += -I $(NTUPLE_DATAFORMAT)

# Linker options
LDFLAGS       += $(ROOTLIBS)
LDFLAGS       += -l:$(NTUPLE_DATAFORMAT)/NTFormat/src/libNTuple.so
LDFLAGS       += -l:$(NTUPLE_ANALYSISFRAMEWORK)/libNTupleAna.so
LDFLAGS       += -l:$(ANALYSIS_SELECTION)/libObjectAndEventSelection.so

# Uncomment this line if you want to use a script to parse & colorize gcc output
# (You can also export this variable from your bashrc)
#GCCPARSER     = 2>&1 | python ~/littleScripts/colorGcc.py

# Filter the useless PROOF output
PROOFFILTER = 2>&1 | tr '\r' '\n' | grep -v --line-buffered "Opening connections\|Setting up worker servers\|Info in <\|no dictionary for\|TUnixSystem::DynamicPathName\|TDataSetManager::ScanDataSet\|exact location of files\|Validating files\|Output file:\|set to parallel mode\|Starting PROOF-Lite\|merging output objects\|objects have been merged\|still sending\|0.00 %" | perl -e '$$| = 1 ;' -pe 's|MB/s]\n|MB/s]\x0D|g;' 

#------------------------------------------------------------------------------
SOURCES       = $(wildcard *.$(SrcSuf))
OBJECTS       = $(SOURCES:.$(SrcSuf)=.$(ObjSuf))
EXECUTABLES   = $(SOURCES:.$(SrcSuf)=.$(ExeSuf))
LOGS          = $(SOURCES:.$(SrcSuf)=.$(LogSuf))
ROOTMESS1     = $(wildcard *.d)
ROOTMESS2     = $(wildcard *.so)
#------------------------------------------------------------------------------
PROOF_LAUNCHER = ProofLauncher
PROOF_JOB      = ProofJob
PROOF_CONFIG   = ProofConfig.h
#------------------------------------------------------------------------------


prod: job $(PROOF_LAUNCHER).$(ExeSuf)
	@./$(PROOF_LAUNCHER).$(ExeSuf) $(PROOFFILTER) | tee $(PROOF_LAUNCHER).$(LogSuf)

job: $(PROOF_JOB)_C.so

clean:
	@echo "Cleaning..."
	@rm -rf $(OBJECTS) $(EXECUTABLES) $(LOGS) $(ROOTMESS1) $(ROOTMESS2) backup_outputProof* FrameworkAndSelectionPackage

#------------------------------------------------------------------------------

$(PROOF_JOB)_C.so: $(PROOF_CONFIG)
	@echo "Compiling job description..."
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o .makeJob.run .makeJob.C
	@./.makeJob.run $(GCCPARSER)

%.$(ExeSuf): %.$(SrcSuf) $(PROOF_CONFIG)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $< $(GCCPARSER)



#ifndef ProofJob_h
#define ProofJob_h

#include <TSelector.h>
#include <TTree.h>
#include <TBranch.h>
#include <TFile.h>
#include <TProofOutputFile.h>
#include <TRandom.h>

#include "Plots/interface/HistoManager.h"
#include "NTFormat/interface/NTEvent.h"
#include "NTFormat/interface/NTGenParticle.h"

#include "Tools/interface/Dataset.h"
#include "Tools/interface/AnalysisEnvironmentLoader.h"
//#include "Selection/interface/DiLeptonSelection.h"
//#include "Plots/interface/DiLepAnaHistoManager.h"
#include "Tools/interface/PUWeighting.h"
#include "Tools/interface/LumiReweightingStandAlone.h"
#include "Tools/interface/JetCorrector.h"

//Stop specific
#include "Selection/interface/combined1LeptonStopSelection.h"
#include "Selection/interface/StopMCinfo.h"
#include "EffEstimation/interface/WJetAlgoEff.h"
#include "Plots/interface/StopMCCharacterization.h"

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h> 
#include <TH3.h>
#include <TCanvas.h>
#include <TRandom.h>
#include <TLorentzVector.h>

#include <iostream>
#include <algorithm>

#include "EventReco/interface/StopAnaReco.h"
#include "EventReco/interface/Resolution.h"
//#include "CandUtils/interface/EventShapeVariables.h"
//#include "CandUtils/interface/Thrust.h"

#include "ProofFormat.h"


//Definition des structures
class AnalysisEnvironmentLoader;
class combined1LeptonStopSelection;

class ProofJob : public TSelector {
 public :
  
  // Specific members
  //Access to the tree and outputs
  TTree* fChain;
  TBranch* branch;
  IPHCTree::NTEvent* event;
  TFile            *fFile;
  TProofOutputFile *fProofFile; // For optimized merging of the ntuple
  //Pointer on results from xml file  reading
  AnalysisEnvironmentLoader* anaEL; 
  //Minimimal info
  vector<Dataset> datasets;
  Dataset* dataset;
  vector<string> CutName;
  vector<string> ChannelName;
  vector<string> VecChannelName;
  float Luminosity;
  int verbosity;
  // 0: MC - 1: Data - 2 Data & MC
  int DataType;
  //Info analysis macro specific 
  
  reweight::LumiReWeighting *LumiWeights;
  float LumiError ;
  string PUWeightFileName;
  
  string datasetName ;
  
  //------------------------------------
  // Additionnal info for Stop
  //------------------------------------
  combined1LeptonStopSelection sel;
  combined1LeptonStopSelection Void;
  StopMCinfo* stopMCinfo;

  TTree* theTree;

  babyEvent  myEvent;

  //------------------------------------
  //definition of member functions
  //------------------------------------
  ProofJob();
  virtual ~ProofJob();
  virtual Int_t   Version() const { return 2; }
  virtual void    Begin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual Bool_t  Process(Long64_t entry);
  virtual void    SetOption(const char *option) { fOption = option; }
  virtual void    SetObject(TObject *obj) { fObject = obj; }
  virtual void    SetInputList(TList *input) { fInput = input; }
  virtual TList  *GetOutputList() const { return fOutput; }
  virtual void    SlaveTerminate();
  virtual void    Terminate();

  virtual void InitializeBranches(TTree* theTree, babyEvent* pointerToEvent);

  
  ClassDef(ProofJob,0);
};



#endif


#include <cmath>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TBranch.h>
#include <TLorentzVector.h>

#include <iostream>
using namespace std;

#include "interface/SonicScrewdriver.h" 
using namespace theDoctor;


// Define format and input files
#include "Reader_beta1002.h"
#include "../microTupling/MicroTuple_Format_Synchro0902.h"

#define INPUT_IPHC "synchronizationBaby1002.root"
#define INPUT_FNAL "../store/microTuples_Synchro0902/fnal.root"

bool inclusiveChannelSelector() { return true; }
bool Selection_presel() { return true; }

// #########################################################################
//                              Main function
// #########################################################################

int main (int argc, char *argv[])
{

  cout << endl;
  cout << "   ┌───────────────────┐  " << endl;
  cout << "   │   Starting dump   │  " << endl;
  cout << "   └───────────────────┘  " << endl; 
  cout << endl;

  // ####################
  // ##   Init tools   ##
  // ####################
  
	 // Create a sonic Screwdriver
 	 SonicScrewdriver mySonic;

  // ##########################
  // ##   Create Variables   ##
  // ##########################

     float triggerE;   mySonic.AddVariable("triggerE",   "#Delta triggerE",        "", 3,-1.5,1.5,  &(triggerE));
     float triggerM;   mySonic.AddVariable("triggerM",   "#Delta triggerM",        "", 3,-1.5,1.5,  &(triggerM));
     
     float leptonPt;   mySonic.AddVariable("leptonPt",   "#Delta lepton p_{T}",    "", 21,-0.5,0.5, &(leptonPt));
     float leptonEta;  mySonic.AddVariable("leptonEta",  "#Delta lepton #eta",     "", 21,-0.5,0.5, &(leptonEta));
     float leptonPhi;  mySonic.AddVariable("leptonPhi",  "#Delta lepton #phi",     "", 21,-0.5,0.5, &(leptonPhi));
     float leptonE;    mySonic.AddVariable("leptonE",    "#Delta lepton E",        "", 21,-0.5,0.5, &(leptonE));
     float nJets;      mySonic.AddVariable("nJets",      "#Delta nJets",           "", 7,-3.5,3.5,  &(nJets));
     float nBtag;      mySonic.AddVariable("nBTag",      "#Delta nBTag",           "", 7,-3.5,3.5,  &(nBtag));
     
     float MET;        mySonic.AddVariable("MET",        "#Delta MET",             "", 21,-0.5,0.5, &(MET));
     float MT;         mySonic.AddVariable("MT",         "#Delta MT",              "", 21,-0.5,0.5, &(MT));
     float MT2W;       mySonic.AddVariable("MT2W",       "#Delta MT2W",            "", 21,-0.5,0.5, &(MT2W));
     float dPhiMETjet; mySonic.AddVariable("dPhiMETjet", "#Delta dPhiMetJet",      "", 21,-0.5,0.5, &(dPhiMETjet));
     float Chi2;       mySonic.AddVariable("Chi2",       "#Delta Chi2",            "", 21,-0.5,0.5, &(Chi2));
     float HTRatio;    mySonic.AddVariable("HTRatio",    "#Delta HTRatio",         "", 21,-0.5,0.5, &(HTRatio));
     
     float trackVeto;  mySonic.AddVariable("trackVeto",  "#Delta trackVeto",       "", 3,-1.5,1.5,  &(trackVeto));
     float tauVeto;    mySonic.AddVariable("tauVeto",    "#Delta tauVeto",         "", 3,-1.5,1.5,  &(tauVeto));
     
  // #########################################################
  // ##   Create ProcessClasses (and associated datasets)   ##
  // #########################################################

     mySonic.AddProcessClass("diff","diff","background",COLORPLOT_AZURE);
     mySonic.AddRegion("presel","Preselection",&Selection_presel);  
	 mySonic.AddChannel("inclusiveChannel","",&inclusiveChannelSelector);
  	 
  	 mySonic.Create1DHistos();
     mySonic.SchedulePlots("1DSuperpRenorm");

  // #################################
  // ##        Fill the map         ##
  // #################################
  
     babyEvent  myEventIPHC;
     microEvent myEventFNAL;

    TFile fInputFNAL(INPUT_FNAL,"UPDATE");
    TTree* theTreeFNAL = (TTree*) fInputFNAL.Get("microTuple"); 
    theTreeFNAL->SetBranchAddress("microEvents",&myEventFNAL);

    TFile fInputIPHC(INPUT_IPHC,"UPDATE");
    TTree* theTreeIPHC = (TTree*) fInputIPHC.Get("babyTuple"); 
    intermediatePointers pointers;
    InitializeBranches(theTreeIPHC,&myEventIPHC,&pointers);

  vector<int> eventsIPHC;
  for (int i = 0 ; i < theTreeIPHC->GetEntries() ; i++) { theTreeIPHC->GetEntry(i); eventsIPHC.push_back(myEventIPHC.event); }

  vector<int> eventsFNAL;
  for (int i = 0 ; i < theTreeFNAL->GetEntries() ; i++) 
  { 

      theTreeFNAL->GetEntry(i); 
      eventsFNAL.push_back(myEventFNAL.event);

      int eventIPHC = -1;
      for (int j = 0 ; j < eventsIPHC.size() ; j++)
      {
            if (eventsIPHC[j] == myEventFNAL.event) { eventIPHC = j; break; }
      }

      if (eventIPHC == -1) continue;
      ReadEvent(theTreeIPHC,eventIPHC,&pointers,&myEventIPHC);
      
      triggerE   = (myEventFNAL.triggerEl    - myEventIPHC.triggerElec   );
      triggerM   = (myEventFNAL.triggerMu    - myEventIPHC.triggerMuon   );
                                                                                        
      leptonPt   = (myEventFNAL.leptonPt     - myEventIPHC.leadingLepton.Pt()  ) / myEventFNAL.leptonPt    ;
      leptonEta  = (myEventFNAL.leptonEta    - myEventIPHC.leadingLepton.Eta() ) / myEventFNAL.leptonEta   ;
      leptonPhi  = (myEventFNAL.leptonPhi    - myEventIPHC.leadingLepton.Phi() ) / myEventFNAL.leptonPhi   ;
      leptonE    = (myEventFNAL.leptonE      - myEventIPHC.leadingLepton.E()   ) / myEventFNAL.leptonE     ;
      nJets      = (myEventFNAL.nJets        - myEventIPHC.nJets       );
      nBtag      = (myEventFNAL.nB           - myEventIPHC.nBTag       );
                                                                                        
      MET        = (myEventFNAL.MET          - myEventIPHC.MET             ) / myEventFNAL.MET         ;
      MT         = (myEventFNAL.MT           - myEventIPHC.MT              ) / myEventFNAL.MT          ;
      MT2W       = (myEventFNAL.MT2W         - myEventIPHC.MT2W            ) / myEventFNAL.MT2W        ;
      dPhiMETjet = (myEventFNAL.dPhiMETjet   - myEventIPHC.deltaPhiMETJets ) / myEventFNAL.dPhiMETjet  ;
      Chi2       = (myEventFNAL.hadronicChi2 - myEventIPHC.hadronicChi2    ) / myEventFNAL.hadronicChi2;
      HTRatio    = (myEventFNAL.HTratio      - myEventIPHC.HTRatio         ) / myEventFNAL.HTratio     ;
      
      trackVeto  = (myEventFNAL.isoTrackVeto - myEventIPHC.isolatedTrackVeto); 
      tauVeto    = (myEventFNAL.tauVeto      - myEventIPHC.tauVeto          );      

      mySonic.AutoFillProcessClass("diff");
  }

  cout << "   > Making plots..." << endl;
  mySonic.MakePlots();
  cout << "   > Saving plots..." << endl;
  mySonic.WritePlots("../plots/checkBabySynchro/","");

  
  return (0);

}



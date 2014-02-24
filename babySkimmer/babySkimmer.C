
// C / C++ headers
#include <cmath>
#include <iostream>
using namespace std;

// ROOT headers
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TBranch.h>
#include <TLorentzVector.h>

// Define format and input file
#include "Reader.h"
#include "SkimmedFormat_0110.h"

#define FOLDER_BABYTUPLES "../store/babyTuples_1102/"

//#define INPUT "../store/babyTuples_1102/babyTuple_ttbar.root"
//#define INPUT "../store/babyTuples_1102/babyTuple_Wjets.root"
//#define INPUT "../store/babyTuples_1102/babyTuple_others.root"
//#define INPUT "../store/babyTuples_1102/babyTuple_DoubleElec.root"
//#define INPUT "../store/babyTuples_1102/babyTuple_DoubleMuon.root"
//#define INPUT "../store/babyTuples_1102/babyTuple_MuEl.root"
//#define INPUT "../store/babyTuples_1102/babyTuple_SingleElec.root"
//#define INPUT "../store/babyTuples_1102/babyTuple_SingleMuon.root"
//#define INPUT "../store/babyTuples_1102/babyTuple_T2tt.root"
//#define INPUT "../store/babyTuples_1102/babyTuple_T2bw-025.root"
//#define INPUT "../store/babyTuples_1102/babyTuple_T2bw-050.root"
#define INPUT "../store/babyTuples_1102/babyTuple_T2bw-075.root"
//#define INPUT "synchronizationBaby1102.root"

#define OUTPUT "T2bw-075.root"

void printProgressBar(int current, int max);


// ###################
// #  Main function  #
// ###################

int main (int argc, char *argv[])
{

  // ################################
  // ##       Open the tree        ##
  // ################################

  // Input tree   
  TFile fInput(INPUT,"UPDATE");
  TTree* theInputTree = (TTree*) fInput.Get("babyTuple"); 

  babyEvent myEvent;
  intermediatePointers pointers;
  InitializeBranches(theInputTree,&myEvent,&pointers);
 
  // Output tree
  TFile fOutput(OUTPUT,"RECREATE");
  TTree* theOutputTree = new TTree("babyTuple","");
  
  babyEventSkimmed myEventSkimmed;
  InitializeBranches(theOutputTree,&myEventSkimmed);

  // ########################################
  // ##        Run over the events         ##
  // ########################################

  for (int i = 0 ; i < theInputTree->GetEntries() ; i++)
  {
      
      if (i % (theInputTree->GetEntries() / 50) == 0) 
              printProgressBar(i,theInputTree->GetEntries());

      // Read event
      
      ReadEvent(theInputTree,i,&pointers,&myEvent);

      // Apply selection
    
        //if ((!myEvent.triggerMuon) && (!myEvent.triggerElec))   continue;
        if (myEvent.numberOfLepton != 1)                        continue;
        if ((myEvent.nJets <= 3) || (myEvent.nBTag == 0))       continue;
        if ((!myEvent.isolatedTrackVeto) || (!myEvent.tauVeto)) continue;
        if ((myEvent.MET < 80) || (myEvent.MT < 100))           continue;

      // Store event
    
        myEventSkimmed.run                     = myEvent.run;                       
        myEventSkimmed.lumi                    = myEvent.lumi;  
        myEventSkimmed.event                   = myEvent.event; 
        myEventSkimmed.triggerMuon             = myEvent.triggerMuon;       
        myEventSkimmed.xtriggerMuon            = myEvent.xtriggerMuon;      
        myEventSkimmed.triggerElec             = myEvent.triggerElec;       
        myEventSkimmed.triggerDoubleElec       = myEvent.triggerDoubleElec; 
        myEventSkimmed.triggerDoubleMuon       = myEvent.triggerDoubleMuon; 
        myEventSkimmed.triggerMuonElec         = myEvent.triggerMuonElec;   
        myEventSkimmed.numberOfLepton          = myEvent.numberOfLepton;       
        myEventSkimmed.leadingLepton           = myEvent.leadingLepton;        
        myEventSkimmed.leadingLeptonPDGId      = myEvent.leadingLeptonPDGId;   
        myEventSkimmed.secondLepton            = myEvent.secondLepton;         
        myEventSkimmed.secondLeptonPDGId       = myEvent.secondLeptonPDGId;    
        myEventSkimmed.isolatedTrackVeto       = myEvent.isolatedTrackVeto;    
        myEventSkimmed.tauVeto                 = myEvent.tauVeto;              
        myEventSkimmed.nJets                   = myEvent.nJets;             
        myEventSkimmed.nBTag                   = myEvent.nBTag;             
        myEventSkimmed.jets                    = myEvent.jets;              
        myEventSkimmed.jets_CSV_raw            = myEvent.jets_CSV_raw;      
        myEventSkimmed.jets_CSV_reshaped       = myEvent.jets_CSV_reshaped;
        myEventSkimmed.jets_partonFlav         = myEvent.jets_partonFlav;   
        myEventSkimmed.MET                     = myEvent.MET;                              
        myEventSkimmed.MT                      = myEvent.MT;                               
        myEventSkimmed.deltaPhiMETJets         = myEvent.deltaPhiMETJets;                  
        myEventSkimmed.hadronicChi2            = myEvent.hadronicChi2;                     
        myEventSkimmed.MT2W                    = myEvent.MT2W;                             
        myEventSkimmed.HT                      = myEvent.HT;                               
        myEventSkimmed.HTRatio                 = myEvent.HTRatio;                          
        myEventSkimmed.leadingBPt              = myEvent.leadingBPt;                       
        myEventSkimmed.leadingLeptonPt         = myEvent.leadingLeptonPt;                  
        myEventSkimmed.leadingJetPt            = myEvent.leadingJetPt;                     
        myEventSkimmed.Mlb                     = myEvent.Mlb;                              
        myEventSkimmed.Mlb_hemi                = myEvent.Mlb_hemi;                         
        myEventSkimmed.M3b                     = myEvent.M3b;                              
        myEventSkimmed.deltaRLeptonLeadingB    = myEvent.deltaRLeptonLeadingB;             
        myEventSkimmed.METoverSqrtHT           = myEvent.METoverSqrtHT;                    
        myEventSkimmed.HTPlusLeptonPtPlusMET   = myEvent.HTPlusLeptonPtPlusMET;            
        myEventSkimmed.nWTag                   = myEvent.nWTag;                           
        myEventSkimmed.leadingWjetPt           = myEvent.leadingWjetPt;                    
        myEventSkimmed.mStop                   = myEvent.mStop;                            
        myEventSkimmed.mNeutralino             = myEvent.mNeutralino;                      
        myEventSkimmed.mCharginoParameter      = myEvent.mCharginoParameter;               
        myEventSkimmed.numberOfGenLepton       = myEvent.numberOfGenLepton;               
        myEventSkimmed.genParticles            = myEvent.genParticles;      
        myEventSkimmed.genParticlesPDGId       = myEvent.genParticlesPDGId; 
        myEventSkimmed.genParticlesMother      = myEvent.genParticlesMother;
        myEventSkimmed.numberOfInitialEvents   = myEvent.numberOfInitialEvents;            
        myEventSkimmed.crossSection            = myEvent.crossSection;                     
        myEventSkimmed.numberOfTruePU          = myEvent.numberOfTruePU;                  
        myEventSkimmed.numberOfPrimaryVertices = myEvent.numberOfPrimaryVertices;               
        myEventSkimmed.weightCrossSection      = myEvent.weightCrossSection;  
        myEventSkimmed.weightPileUp            = myEvent.weightPileUp;        
        myEventSkimmed.weightISRmodeling       = myEvent.weightISRmodeling;   
        myEventSkimmed.weightTopPt             = myEvent.weightTopPt;         
             
      // Fill tree
    
        theOutputTree->Fill();
  }           
              
  theOutputTree->Write(0, TObject::kOverwrite); 

  return (0); 
}             

void printProgressBar(int current, int max)
{
    std::string bar;
    int percent = 100 * (float) current / (float) max;

    for(int i = 0; i < 50; i++)
    {
        if( i < (percent/2))       bar.replace(i,1,"=");
        else if( i == (percent/2)) bar.replace(i,1,">");
        else                       bar.replace(i,1," ");
    }

    std::cout << "  [Progress]  ";
    std::cout << "[" << bar << "] ";
    std::cout.width( 3 );
    std::cout << percent << "%     ";
    std::cout << "(" << current << " / " << max << ")" << "\r" << std::flush;
}




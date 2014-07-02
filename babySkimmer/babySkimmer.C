
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

#include "../common.h"


// Define format and input file
//#include "Reader.h"
#include "Reader_Caro.h"


// Event container --- global variable instantiated after the inclusion of Reader.h
babyEvent myEvent;
intermediatePointers pointers;
string sampleType;


//Files needed to compute on-fly variables (needs myEvent)
#include "../AN-14-067/SecondLeptonInAcceptance.h"


//#define FOLDER_INPUT  "../store/babyTuples_0603/"
//#define FOLDER_OUTPUT "../store/babyTuples_0603_1lepton4jetsMET80/"
//#define FOLDER_INPUT  "babyTuple_BDT_0618/"
//#define FOLDER_OUTPUT "babyTuple_BDT_0618_skimmed/"
#define FOLDER_INPUT  "babyTuple_fix2leptonAtLeast1jet_2/"
#define FOLDER_OUTPUT "babyTuple_BDT_0620_2l_skimmed_Caro_v2/"
//#define FOLDER_OUTPUT "babyTuple_BDT_0618_skimmed_Caro/"


// ISR jet definition (extracted from ../AN-14-067/cutAndCountDefinitions.h)
bool findISRJet()
{
    if (myEvent.nJets < 5) return false;

    bool foundISRJet = false;
    for (unsigned int i = 0 ; i < myEvent.jets.size() ; i++)
    {
        // Check jet is high-pt
        if ((myEvent.jets)[i].Pt() < 200) continue;

        // Check jet isn't b-tagged
        if (sampleType == "data") { if ((myEvent.jets_CSV_raw)[i]      > 0.679) continue; }
        else                      { if ((myEvent.jets_CSV_reshaped)[i] > 0.679) continue; }

        foundISRJet = true;
    }

    return foundISRJet;
}
          
Float_t leadingNonBPt(){	  
	  Float_t leadingNonBPt_=-1;
          for (unsigned int j = 0 ; j < myEvent.jets.size()  ; j++)
          {

             float btag_info = (myEvent.jets_CSV_reshaped)[j];
             if (sampleType == "data") btag_info = (myEvent.jets_CSV_raw)[j];

            if ((myEvent.jets)[j].Pt() > leadingNonBPt_
                && btag_info < 0.679)
                leadingNonBPt_= (myEvent.jets)[j].Pt();
          }
	  return leadingNonBPt_;
}

// ###################
// #  Main function  #
// ###################

int main (int argc, char *argv[])
{
  if (argc <= 1)
  {
      cout << "No dataset specified in argument" << endl;
      return -1;
  }
  
  string dataset = argv[1]; 
     
     

  // ################################
  // ##       Open the tree        ##
  // ################################


  // Input tree   
  TFile fInput((FOLDER_INPUT+dataset+".root").c_str(),"READ");
  TTree* theInputTree = (TTree*) fInput.Get("babyTuple"); 
  InitializeBranchesForReading(theInputTree,&myEvent,&pointers);
 
  // Output tree
  TFile fOutput((FOLDER_OUTPUT+dataset+".root").c_str(),"RECREATE");
  TTree* theOutputTree = new TTree("babyTuple","");
  InitializeBranchesForWriting(theOutputTree,&myEvent);

  // ########################################
  // ##        Run over the events         ##
  // ########################################

  for (int i = 0 ; i < theInputTree->GetEntries() ; i++)
  {
      
      if (i % (theInputTree->GetEntries() / 100) == 0) 
          printProgressBar(i,theInputTree->GetEntries(),dataset);

      // Read event

      ReadEvent(theInputTree,i,&pointers,&myEvent);

      // Apply selection
      /*
      if (myEvent.numberOfLepton != 1)                        continue;
      if (myEvent.nJets < 4)                                  continue;
      if (myEvent.MET < 80)                                   continue;
      */
      ///*
      if (myEvent.numberOfLepton != 2)                        continue;
      if (myEvent.nJets < 1)                                  continue;
      if (myEvent.MET < 50)                                   continue;
	//*/

      if(dataset=="DoubleMuon" || dataset=="DoubleElec" || dataset=="MuEl" || dataset=="SingleMuon" || dataset=="SingleElec")
      	sampleType = "data";

      // Compute some variables before storing them
      myEvent.secondLeptonInAcceptance = IsMultilepEvtWithTrueTrkFromTau();
      myEvent.ISRJet = findISRJet();
      myEvent.leadingNonBPt = leadingNonBPt();
   

      // Fill tree

      theOutputTree->Fill();
  }           
           
  cout << endl;
  theOutputTree->Write(0, TObject::kOverwrite); 

  return (0); 
}             



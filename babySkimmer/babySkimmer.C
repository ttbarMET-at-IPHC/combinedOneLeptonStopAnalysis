
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
#include "Reader.h"

#define FOLDER_INPUT  "../store/babyTuples_0328/"
//#define FOLDER_OUTPUT "../store/babyTuples_0219_preSelectionSkimmed/"
#define FOLDER_OUTPUT "./"

// ###################
// #  Main function  #
// ###################

int main (int argc, char *argv[])
{
  string dataset = argv[1]; 

  // ################################
  // ##       Open the tree        ##
  // ################################

  // Event container
  babyEvent myEvent;
  intermediatePointers pointers;

  // Input tree   
  TFile fInput((FOLDER_INPUT+dataset).c_str(),"READ");
  TTree* theInputTree = (TTree*) fInput.Get("babyTuple"); 
  InitializeBranchesForReading(theInputTree,&myEvent,&pointers);
 
  // Output tree
  TFile fOutput((FOLDER_OUTPUT+dataset).c_str(),"RECREATE");
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

      if (myEvent.numberOfLepton != 1)                        continue;
      if ((myEvent.nJets <= 3) || (myEvent.nBTag == 0))       continue;
      if ((!myEvent.isolatedTrackVeto) || (!myEvent.tauVeto)) continue;
      if ((myEvent.MET < 80) || (myEvent.MT < 100))           continue;

      // Fill tree

      theOutputTree->Fill();
  }           
           
  cout << endl;
  theOutputTree->Write(0, TObject::kOverwrite); 

  return (0); 
}             



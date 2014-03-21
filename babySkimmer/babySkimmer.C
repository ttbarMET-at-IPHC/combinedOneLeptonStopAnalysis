
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

#define FOLDER_INPUT  "../store/babyTuples_0219/"
//#define FOLDER_OUTPUT "../store/babyTuples_0219_preSelectionSkimmed/"
#define FOLDER_OUTPUT "./"
#define DATASET       "T2bw-050.root"

void printProgressBar(int current, int max);

// ###################
// #  Main function  #
// ###################

int main (int argc, char *argv[])
{

  // ################################
  // ##       Open the tree        ##
  // ################################

  // Event container
  babyEvent myEvent;
  intermediatePointers pointers;

  // Input tree   
  TFile fInput((string(FOLDER_INPUT)+DATASET).c_str(),"READ");
  TTree* theInputTree = (TTree*) fInput.Get("babyTuple"); 
  InitializeBranchesForReading(theInputTree,&myEvent,&pointers);
 
  // Output tree
  TFile fOutput((string(FOLDER_OUTPUT)+DATASET).c_str(),"RECREATE");
  TTree* theOutputTree = new TTree("babyTuple","");
  InitializeBranchesForWriting(theOutputTree,&myEvent);

  // ########################################
  // ##        Run over the events         ##
  // ########################################

  for (int i = 0 ; i < theInputTree->GetEntries() ; i++)
  {
      
      if (i % (theInputTree->GetEntries() / 100) == 0) 
          printProgressBar(i,theInputTree->GetEntries());

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




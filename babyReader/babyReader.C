
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
#include "Reader_prefinal1024.h"
#define INPUT "synchronizationBaby1102.root"

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
  

  // ########################################
  // ##        Run over the events         ##
  // ########################################

  //for (int i = 0 ; i < theInputTree->GetEntries() ; i++)
  for (int i = 0 ; i < 10 ; i++)
  {
      ReadEvent(theInputTree,i,&pointers,&myEvent);

      
      cout << endl;
      cout << " index = " << i << endl;
      cout << " event n°" << myEvent.event << endl;
      
      cout << " +---------+ " << endl;
      cout << " | Leptons | " << endl;
      cout << " +---------+ " << endl;

      cout << " number of selected leptons : " << myEvent.numberOfLepton << endl;
      cout << " leading lepton Pt : " << myEvent.leadingLepton.Pt() << endl;

      cout << " +------+ " << endl;
      cout << " | Jets | " << endl;
      cout << " +------+ " << endl;

      cout << " number of selected jets : " << myEvent.nJets << endl; 
      cout << " number of selected b-tagged jets : " << myEvent.nBTag << endl;
      for (unsigned int j = 0 ; j < myEvent.jets.size() ; j++)
      {
          cout << " jet n°"   << j 
               << " ; pT = "  << myEvent.jets[j].Pt() 
               << " ; eta = " << myEvent.jets[j].Eta() 
               << endl;
      }

      cout << " +------------------------+ " << endl;
      cout << " | 'High-level' variables | " << endl;
      cout << " +------------------------+ " << endl;

      cout << " MET = " << myEvent.MET << endl;
      cout << " MT = " << myEvent.MT << endl;
   
      cout << " +-----------------------------+ " << endl;
      cout << " | Generator-level information | " << endl;
      cout << " +-----------------------------+ " << endl;

      cout << " number of generated leptons (tau and e/µ not coming from a tau) : " << myEvent.numberOfGenLepton << endl;
      
      for (unsigned int j = 0 ; j < myEvent.genParticles.size() ; j++)
      {
          cout << " genParticles n° " << j 
              << " ; PDGId : " << myEvent.genParticlesPDGId[j]
              << " ; Pt : "    << myEvent.genParticles[j].Pt() 
              << " ; Eta : "   << myEvent.genParticles[j].Eta() 
              << " ; Mass : "   << myEvent.genParticles[j].M() 
              << endl;
      }

      cout << endl;
      
      cout << " scalePDF = " << myEvent.scalePDF << endl;
      
  } 

  
  return (0);
}



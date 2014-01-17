
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

// Define format of input file
#include "Reader.h"
#define INPUT "synchronizationBaby1102.root"

#include "Writer.h"
#define OUTPUT "output.root"

void printProgressBar(int current, int max);

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
  InitializeBranchesForReading(theInputTree,&myEvent,&pointers);
 
  // Output tree
  TFile fOutput(OUTPUT,"RECREATE");
  TTree* theOutputTree = new TTree("babyTuple","");
  InitializeBranchesForWriting(theOutputTree,&myEvent);
    
  // Add a vector<float> branch that will contain the additional
  // info, like BDT outputs.

  vector<Float_t> BDToutputs;
  theOutputTree->Branch("BDToutputs",     "std::vector<Float_t>",          &(BDToutputs));

  //
  //
  // Here, you might want to init TMVA readers you will need later
  //
  //

  // ########################################
  // ##        Run over the events         ##
  // ########################################

  for (int i = 0 ; i < theInputTree->GetEntries() ; i++)
  {
      
      if (i % (theInputTree->GetEntries() / 50) == 0) 
              printProgressBar(i,theInputTree->GetEntries());

      // Read entry from input tree 
      
      ReadEvent(theInputTree,i,&pointers,&myEvent);

      // 
      //
      // Here, you might want to compute the things you want to add to the tree
      // (for instance, BDT outputs value)
      // As an example, I'm using dummy values
      //
      //

      BDToutputs.clear();           // Don't forget to clear the vector from the previous info
      BDToutputs.push_back(3.14);
      BDToutputs.push_back(2.71828);


      // Fill output tree
    
      theOutputTree->Fill();
  }           

  printProgressBar(theInputTree->GetEntries(),theInputTree->GetEntries());
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




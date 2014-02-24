
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

#define INPUT "../store/babyTuples_0219/toBeFiltered/babyTuple_ttbar.root"
#define OUTPUT "test.root"
#define FILTER "filter.root"

typedef struct
{

    float runId;
    float lumiId;
    float eventId;
    float rawPFMET;

} EventId;

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
  TFile fInput(INPUT,"READ");
  TTree* theInputTree = (TTree*) fInput.Get("babyTuple"); 

  babyEvent myEvent;
  intermediatePointers pointers;
  InitializeBranchesForReading(theInputTree,&myEvent,&pointers);

  // Filter tree
  EventId eventToFilter;
  TFile fFilter(FILTER,"READ");
  TTree* theFilterTree = (TTree*) fFilter.Get("eventsToFilter");
  theFilterTree->SetBranchAddress("eventsToFilter",&eventToFilter);

  // Output tree
  TFile fOutput(OUTPUT,"RECREATE");
  TTree* theOutputTree = new TTree("babyTuple","");
  
  babyEvent myEventFiltered;
  InitializeBranchesForWriting(theOutputTree,&myEvent);

  // #######################################################
  // ##        Read all the events to be filtered         ##
  // #######################################################

  vector<EventId> eventsToFilter;

  for (int i = 0 ; i < theFilterTree->GetEntries() ; i++)
  {
      theFilterTree->GetEntry(i);
      eventsToFilter.push_back(eventToFilter);
  }

  // ########################################
  // ##        Run over the events         ##
  // ########################################

  int nFiltered = 0;
  for (int i = 0 ; i < theInputTree->GetEntries() ; i++)
  {

      //if (i % (theInputTree->GetEntries() / 50) == 0) 
      if (i % 1000  == 0) 
              printProgressBar(i,theInputTree->GetEntries());

      // Read event
      
      theInputTree->GetEntry(i);
      //ReadEvent(theInputTree,i,&pointers,&myEvent);

      // Check if current is event is supposed to be filter
   
      /*
      bool foundInFilter = false;
      for (unsigned int j = 0 ; j < eventsToFilter.size() ; j++)
      {
          EventId check = eventsToFilter[j];
          if ((((int) check.eventId) == myEvent.event)
           && (check.rawPFMET - myEvent.rawPFMET == 0)
           && (((int) check.lumiId)  == myEvent.lumi ) 
           && (((int) check.runId)   == myEvent.run  )) 
          {
              nFiltered++;
              foundInFilter = true;
              break;
          }
      }
        
      if (foundInFilter) continue;
      */

      // Keep event
    
      //  theOutputTree->Fill();
  }           
             
  cout << "nTot = " << theInputTree->GetEntries() << endl;
  cout << "nFiltered = " << nFiltered << endl;

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




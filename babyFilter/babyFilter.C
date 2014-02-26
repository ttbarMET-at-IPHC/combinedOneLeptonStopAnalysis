
// C / C++ headers
#include <cmath>
#include <iostream>
#include <algorithm>
using namespace std;

// ROOT headers
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TBranch.h>
#include <TLorentzVector.h>

// Define format and input file
#include "Reader.h"

#define INPUT_FOLDER "../store/babyTuples_0219/toBeFiltered/babyTuple_"
#define OUTPUT_FOLDER "/opt/sbg/data/data1/cms/aaubin/METFilters/babyTuple_filtered/"
#define FILTER_FOLDER "/opt/sbg/data/data1/cms/aaubin/METFilters/filters/"

typedef struct
{

    float runId;
    float lumiId;
    float eventId;
    float rawPFMET;

} EventId;

void printProgressBar(int current, int max);

bool sortingFunction(EventId a, EventId b) {  return (a.eventId < b.eventId); }
void sortVector( vector<EventId>* v)
{
    sort(v->begin(),v->end(),sortingFunction);
}

// ###################
// #  Main function  #
// ###################

int main (int argc, char *argv[])
{

  string dataset = argv[1]; 
  string input = string(INPUT_FOLDER)+dataset+".root";
  string output = string(OUTPUT_FOLDER)+dataset+".root";
  string filter = string(FILTER_FOLDER)+dataset+".root";

  // ################################
  // ##       Open the tree        ##
  // ################################

  // Input tree   
  TFile fInput(input.c_str(),"READ");
  TTree* theInputTree = (TTree*) fInput.Get("babyTuple"); 

  babyEvent myEvent;
  intermediatePointers pointers;
  InitializeBranchesForReading(theInputTree,&myEvent,&pointers);

  // Filter tree
  EventId eventToFilter;
  TFile fFilter(filter.c_str(),"READ");
  TTree* theFilterTree = (TTree*) fFilter.Get("eventsToFilter");
  theFilterTree->SetBranchAddress("eventsToFilter",&eventToFilter);

  // Output tree
  TFile fOutput(output.c_str(),"RECREATE");
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

  sortVector(&eventsToFilter);

  // ########################################
  // ##        Run over the events         ##
  // ########################################

  cout << "nEvent in filter =" << eventsToFilter.size() << endl;

  int nFiltered = 0;
  for (int i = 0 ; i < theInputTree->GetEntries() ; i++)
  {

      if (i % (theInputTree->GetEntries() / 200) == 0) 
              printProgressBar(i,theInputTree->GetEntries());

      // Read event
      
      ReadEvent(theInputTree,i,&pointers,&myEvent);
      
      // Check if current is event is supposed to be filter

      int min = 0;
      int max = eventsToFilter.size()  - 1;
      int mid = (max - min)/2;
      
      int k = 0;
      while ((eventsToFilter[min].eventId != eventsToFilter[max].eventId) && (abs(max-min) > 1))
      {
               if ((eventsToFilter[mid].eventId) > ((float) myEvent.event))  { max = mid; mid -= (max - min) / 2; }
          else if ((eventsToFilter[mid].eventId) < ((float) myEvent.event))  { min = mid; mid += (max - min) / 2; }
          else break;
          k++;
      }

      bool foundInFilter = false;
      for (unsigned int j = min ; j < max ; j++)
      {
          EventId check = eventsToFilter[j];
          if ((check.eventId == ((float) myEvent.event))
           && (check.rawPFMET - myEvent.rawPFMET == 0)
           && (check.lumiId  == ((float) myEvent.lumi) ) 
           && (check.runId   == ((float) myEvent.run)  )) 
          {
              nFiltered++;
              foundInFilter = true;
              break;
          }
      }
       
      if (foundInFilter) continue;

      // Keep event
    
      theOutputTree->Fill();
  }           
   
  cout << endl;
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




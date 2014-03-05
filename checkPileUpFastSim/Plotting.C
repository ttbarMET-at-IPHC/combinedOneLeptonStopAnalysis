
// C/C++ headers

#include <cmath>
#include <iomanip>
#include <iostream>
#include <time.h>
#include <set>
using namespace std;

// ROOT headers

#include <TFile.h>
#include <TRandom.h>
#include <TMarker.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLorentzVector.h>
#include <TMVA/Reader.h>

// Sonic Screwdriver headers

#include "interface/Table.h" 
#include "interface/SonicScrewdriver.h" 
using namespace theDoctor;

// BabyTuple format and location

//#define FOLDER_BABYTUPLES "../store/babyTuples_1102/"
#define FOLDER_BABYTUPLES "../store/babyTuples_1102/"
#include "Reader.h"
babyEvent* myEventPointer;

void printBoxedMessage(string message);
void printProgressBar(int current, int max);

float weightS10[60] = {0.246323,0.195985,0.217915,0.331417,0.3034,0.475434,0.429483,0.421169,0.582882,0.903066,1.31179,1.69658,1.75778,1.56258,1.32624,1.15559,1.07227,1.05049,1.07055,1.11465,1.15467,1.17724,1.1861,1.18228,1.15941,1.11262,1.04233,0.951951,0.844631,0.725668,0.603026,0.484859,0.376801,0.282502,0.203678,0.140667,0.093096,0.0592184,0.0363736,0.0217555,0.0128045,0.00751369,0.00446529,0.00273412,0.00175596,0.00119529,0.000864613,0.000661264,0.000529328,0.0004387,0.000373219,0.000323506,0.00028436,0.000252393,0.000225177,0.000201384,0.000180034,0.000160547,0.000142487,2.41963};

float weightS7[60] = {0.0269021,0.043804,0.132013,0.707654,0.0647969,0.274386,1.2209,2.72771,4.9408,7.79127,8.97063,10.7276,9.00422,7.67177,6.42841,5.13058,3.91257,2.95823,2.30159,1.86724,1.57693,1.38108,1.24625,1.14121,1.03899,0.924164,0.796685,0.664279,0.535259,0.416568,0.313718,0.229296,0.162909,0.112386,0.0751246,0.0485403,0.0303353,0.0184099,0.0109312,0.00642164,0.00378445,0.00227666,0.00142716,0.000951419,0.000688742,0.000548894,0.000484397,0.000471327,0.000501664,0.000578531,0.000717492,0.000950629,0.00133986,0.00200221,0.00315807,0.00524998,0.0091659,0.0167848,0.0322017,591.61};

// #########################################################################
//                          Region selectors
// #########################################################################

bool inclusiveChannelSelector() { return true; }
bool elecChannelSelector() { return (abs(myEventPointer->leadingLeptonPDGId) == 11); }
bool muonChannelSelector() { return (abs(myEventPointer->leadingLeptonPDGId) == 13); }

bool inclusiveRegionSelector() { if (myEventPointer->numberOfLepton != 1) return false; else return true; }

// #########################################################################
//                              Main function
// #########################################################################

int main (int argc, char *argv[])
{

  printBoxedMessage("Starting plot generation");

  // ####################
  // ##   Init tools   ##
  // ####################
  
     // Create a sonic Screwdriver
      SonicScrewdriver screwdriver;

       // Create a container for the event
     babyEvent myEvent;
     myEventPointer = &myEvent;

  // ##########################
  // ##   Create Variables   ##
  // ##########################

     float numberOfPrimaryVertices;
     float numberOfTruePU;
     screwdriver.AddVariable("nPV",       "# of primary vertices",        "",    51,-0.5,50.5,       &(numberOfPrimaryVertices), "");
     screwdriver.AddVariable("nTPU",      "# of true pile-up",            "",    60,0,60,            &(numberOfTruePU), "");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     screwdriver.AddProcessClass("signal",   "T2tt",     "background",kAzure+2);
             screwdriver.AddDataset("T2tt",   "signal", 0, 0);
     
     screwdriver.AddProcessClass("data",   "data",                     "data",COLORPLOT_BLACK);
             //screwdriver.AddDataset("SingleElec",   "data", 0, 0);
             screwdriver.AddDataset("SingleMuon",   "data", 0, 0);

  // ##########################
  // ##    Create Regions    ##
  // ##########################

     screwdriver.AddRegion("skimming", "Skimming : 1 lep, >= 3jets", &inclusiveRegionSelector);

  // ##########################
  // ##   Create Channels    ##
  // ##########################
   
     screwdriver.AddChannel("inclusiveChannel","",&inclusiveChannelSelector);
     //screwdriver.AddChannel("elecChannel","e-channel",&elecChannelSelector);
     screwdriver.AddChannel("muonChannel","#mu-channel",&muonChannelSelector);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     // Create histograms
     screwdriver.Create1DHistos();

     screwdriver.SetGlobalBoolOption  ("1DSuperimposed",   "includeSignal",                   true   );
     screwdriver.SetGlobalBoolOption  ("1DSuperimposed",   "includeData",                     true   );

     screwdriver.SetGlobalStringOption("1DStack",          "includeSignal",                   "stack");
     screwdriver.SetGlobalFloatOption ("1DStack",          "factorSignal",                    1.0    );

     screwdriver.SetGlobalStringOption("DataMCComparison", "includeSignal",                   "stack");
     screwdriver.SetGlobalFloatOption ("DataMCComparison", "factorSignal",                    1.0    );

     // Schedule plots
     screwdriver.SchedulePlots("1DSuperimposed");
     screwdriver.SchedulePlots("1DStack");
     screwdriver.SchedulePlots("1DDataMCComparison");

     // Config plots

     screwdriver.SetGlobalStringOption("Plot", "infoTopRight", "CMS Internal");
     screwdriver.SetGlobalStringOption("Plot", "infoTopLeft",  "#sqrt{s} = 8 TeV");

     screwdriver.SetGlobalBoolOption("Plot", "exportPdf", true);
     screwdriver.SetGlobalBoolOption("Plot", "exportEps", false);
     screwdriver.SetGlobalBoolOption("Plot", "exportPng", false);

  // ########################################
  // ##       Run over the datasets        ##
  // ########################################

  vector<string> datasetsList;
  screwdriver.GetDatasetList(&datasetsList);

  cout << "   > Running on dataset : " << endl;

  vector< vector<float> > listBackground;
  vector< vector<float> > listSignal;

  for (unsigned int d = 0 ; d < datasetsList.size() ; d++)
  {
     string currentDataset = datasetsList[d];
     string currentProcessClass = screwdriver.GetProcessClass(currentDataset); 
     // Open the tree
     TFile f((string(FOLDER_BABYTUPLES)+"babyTuple_"+currentDataset+".root").c_str());
     TTree* theTree = (TTree*) f.Get("babyTuple"); 

     intermediatePointers pointers;
     InitializeBranches(theTree,&myEvent,&pointers);

     cout << "                    " << currentDataset << endl; 

  // ########################################
  // ##        Run over the events         ##
  // ########################################

      //for (int i = 0 ; i < theTree->GetEntries() ; i++)
      for (int i = 0 ; i < 1000000 ; i++)
      {
          if (i % (theTree->GetEntries() / 50) == 0) 
              printProgressBar(i,theTree->GetEntries());

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          float weight = 1.0;

          if (currentProcessClass != "data")
          {
              // Apply PU reweighting
              weight *= weightS10[myEvent.numberOfTruePU];
          }

          numberOfPrimaryVertices = myEvent.numberOfPrimaryVertices;
          numberOfTruePU          = myEvent.numberOfTruePU;
          screwdriver.AutoFillProcessClass(currentProcessClass,weight);

      } 
      
      cout << endl;
      f.Close();

  }

  // ###################################
  // ##   Make plots and write them   ##
  // ###################################
 
  cout << endl;
  cout << "   > Making plots..." << endl;
  screwdriver.MakePlots();
  cout << "   > Saving plots..." << endl;
  screwdriver.WritePlots("../plots/checkPileUpFastSim/");

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################
  
  printBoxedMessage("Now computing misc tests ... ");

  printBoxedMessage("Program done.");
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

void printBoxedMessage(string message)
{
    cout << endl;

    cout << "   ┌──";
    for(unsigned int i = 0 ; i <= message.size() ; i++) cout << "─";
    cout << "─┐  " << endl;

    cout << "   │  " << message << "  │  " << endl;
    
    cout << "   └──";
    for(unsigned int i = 0 ; i <= message.size() ; i++) cout << "─";
    cout << "─┘  " << endl; 
 
    cout << endl;

}

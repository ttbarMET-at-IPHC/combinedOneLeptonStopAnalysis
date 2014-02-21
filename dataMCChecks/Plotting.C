
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
#define FOLDER_BABYTUPLES "../store/babyTuples_1102_skimmed/"
#include "Reader.h"
babyEventSkimmed* myEventPointer;

void printBoxedMessage(string message);
void printProgressBar(int current, int max);

// #########################################################################
//                          Region selectors
// #########################################################################

bool inclusiveChannelSelector() { return true; }
bool elecChannelSelector() { return (abs(myEventPointer->leadingLeptonPDGId) == 11); }
bool muonChannelSelector() { return (abs(myEventPointer->leadingLeptonPDGId) == 13); }

bool Selector_presel() 
{
    babyEventSkimmed myEvent = *myEventPointer;

    // Reject event that don't pass the trigger
    if ((!myEvent.triggerMuon) && (!myEvent.xtriggerMuon) && (!myEvent.triggerElec)) return false;
    
    // Require nLepton == 1
    if (myEvent.numberOfLepton != 1)                      return false;

    // Require nJets >= 4, nBTag >= 1
    if ((myEvent.nJets <= 3) || (myEvent.nBTag == 0))     return false; 

    // Apply vetos
    if ((!myEvent.isolatedTrackVeto) || (!myEvent.tauVeto))return false;

    // Apply MET and MT cuts
    if ((myEvent.MET < 80) || (myEvent.MT < 100))         return false;

    return true; 
}

bool Selector_cutAndCount(float cutMEToverSqrtHT, float cutMT, float cutMT2W, float cutMET, bool enableDeltaPhiAndChi2Cuts, bool enableISRJetRequirement)
{
    if (myEventPointer->METoverSqrtHT < cutMEToverSqrtHT) return false;
    if (myEventPointer->MT            < cutMT)            return false;
    if (myEventPointer->MT2W          < cutMT2W)          return false;
    if (myEventPointer->MET           < cutMET)           return false;
    
    if (enableDeltaPhiAndChi2Cuts)
    {
        if (myEventPointer->deltaPhiMETJets < 0.8) return false;
        if (myEventPointer->hadronicChi2    > 5)   return false;
    }

    if (enableISRJetRequirement)
    {
       if (myEventPointer->nJets < 5) return false;

       bool foundISRJet = false;
       for (unsigned int i = 0 ; i < myEventPointer->jets.size() ; i++)
       {
          // Check jet is high-pt
         if ((myEventPointer->jets)[i].Pt() < 200) continue;
          // Check jet isn't b-tagged
         if ((myEventPointer->jets_CSV_reshaped)[i] > 0.679) continue;

         foundISRJet = true;
       }
       if (foundISRJet == false) return false;
    }

    return Selector_presel();
}

bool Selector_cutAndCount_highDeltaM()    { return Selector_cutAndCount(15,190,240,-1,false,false); }
bool Selector_cutAndCount_mediumDeltaM()  { return Selector_cutAndCount(10,140,180,-1,true,false); }
bool Selector_cutAndCount_lowDeltaM()     { return Selector_cutAndCount(7,140,120,-1,true,true);  }
bool Selector_cutAndCount_offShellLoose() { return Selector_cutAndCount(-1,110,-1,100,true,false);  }
bool Selector_cutAndCount_offShellTight() { return Selector_cutAndCount(-1,140,-1,100,true,false);  }

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
     babyEventSkimmed myEvent;
     myEventPointer = &myEvent;

  // ##########################
  // ##   Create Variables   ##
  // ##########################

/*
     screwdriver.AddVariable("MET",            "MET",                     "GeV",    15,50,500,      &(myEvent.MET),             "logY=true");
     screwdriver.AddVariable("MT",             "MT",                      "GeV",    17,0,510,       &(myEvent.MT),              "logY=true");
     screwdriver.AddVariable("deltaPhiMETJets","#Delta#Phi(MET,j_{1,2})", "rad",    16,0,3.2,       &(myEvent.deltaPhiMETJets), "");
     screwdriver.AddVariable("MT2W",           "M_{T2}^{W}",              "GeV",    20,0,500,       &(myEvent.MT2W),            "");
     screwdriver.AddVariable("HT",             "H_{T}",                   "",       45,150,1500,    &(myEvent.HT),              "");
     screwdriver.AddVariable("HTratio",        "H_{T}^{ratio}",           "",       20,0,1.2,       &(myEvent.HTRatio),         "");
     screwdriver.AddVariable("HadronicChi2",   "Hadronic #chi^{2}",       "",       40,0,20,        &(myEvent.hadronicChi2),    "");
     screwdriver.AddVariable("METoverSqrtHT",  "MET / #sqrt{H_{T}}",      "",       32,0,32,        &(myEvent.METoverSqrtHT),   "");
*/
     screwdriver.AddVariable("leptonPt",       "p_{T}(lepton)",           "GeV",    25,0,500,       &(myEvent.leadingLeptonPt), "logY=true");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     screwdriver.AddProcessClass("ttbar_1l", "t#bar{t} #rightarrow l+jets","background",kRed-7);
            screwdriver.AddDataset("ttbar",    "ttbar_1l",  0, 0);
     screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}",    "background",kCyan-3);

     screwdriver.AddProcessClass("W+jets",   "W+jets",                     "background",kOrange-2);
             screwdriver.AddDataset("Wjets",    "W+jets", 0, 0);

     screwdriver.AddProcessClass("others",   "others",                     "background",kMagenta-5);
             screwdriver.AddDataset("others",   "others", 0, 0);
     
     screwdriver.AddProcessClass("data",   "data",                     "data",COLORPLOT_BLACK);
             screwdriver.AddDataset("SingleElec",   "data", 0, 0);
             screwdriver.AddDataset("SingleMuon",   "data", 0, 0);

  // ##########################
  // ##    Create Regions    ##
  // ##########################

     screwdriver.AddRegion("presel",             "Preselection",                 &Selector_presel);
/*
     screwdriver.AddRegion("CC_highDM",          "Cut&Count high#DeltaM",        &Selector_cutAndCount_highDeltaM);
     screwdriver.AddRegion("CC_mediumDM",        "Cut&Count medium#DeltaM",      &Selector_cutAndCount_mediumDeltaM);
     screwdriver.AddRegion("CC_lowDM",           "Cut&Count low#DeltaM",         &Selector_cutAndCount_lowDeltaM);
     screwdriver.AddRegion("CC_offShell_Tight",  "Cut&Count offShell Tight",     &Selector_cutAndCount_offShellTight);
     screwdriver.AddRegion("CC_offShell_Loose",  "Cut&Count offShell Loose",     &Selector_cutAndCount_offShellLoose);
*/

  // ##########################
  // ##   Create Channels    ##
  // ##########################
   
     screwdriver.AddChannel("inclusiveChannel","",&inclusiveChannelSelector);
     screwdriver.AddChannel("elecChannel","e-channel",&elecChannelSelector);
     screwdriver.AddChannel("muonChannel","#mu-channel",&muonChannelSelector);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     screwdriver.SetLumi(20000);

     // Create histograms
     screwdriver.Create1DHistos();

     screwdriver.SetGlobalBoolOption  ("1DSuperimposed",   "includeSignal",                   true   );

     screwdriver.SetGlobalStringOption("1DStack",          "includeSignal",                   "stack");
     screwdriver.SetGlobalFloatOption ("1DStack",          "factorSignal",                    1.0    );

     screwdriver.SetGlobalStringOption("DataMCComparison", "includeSignal",                   "stack");
     screwdriver.SetGlobalFloatOption ("DataMCComparison", "factorSignal",                    1.0    );

     screwdriver.SetGlobalFloatOption ("FigureOfMerit",    "backgroundSystematicUncertainty", 0.15   );

     // Schedule plots
     screwdriver.SchedulePlots("1DSuperimposed");
     screwdriver.SchedulePlots("1DStack");
     screwdriver.SchedulePlots("1DDataMCComparison");

     // Config plots

     screwdriver.SetGlobalStringOption("Plot", "infoTopRight", "CMS Internal");
     screwdriver.SetGlobalStringOption("Plot", "infoTopLeft",  "#sqrt{s} = 8 TeV, L = 20 fb^{-1}");

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
     TFile f((string(FOLDER_BABYTUPLES)+currentDataset+".root").c_str());
     TTree* theTree = (TTree*) f.Get("babyTuple"); 

     intermediatePointersSkimmed pointers;
     InitializeBranches(theTree,&myEvent,&pointers);

     cout << "                    " << currentDataset << endl; 

  // ########################################
  // ##        Run over the events         ##
  // ########################################

      for (int i = 0 ; i < theTree->GetEntries() ; i++)
      {
          if (i % (theTree->GetEntries() / 50) == 0) 
              printProgressBar(i,theTree->GetEntries());

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          // Keep only events that pass preselection
          if (!Selector_presel()) continue;
 
          float weight = 1.0;
          if (currentProcessClass != "data")
          {
              if (abs(myEvent.leadingLeptonPDGId) == 11)       weight *= myEvent.weightCrossSection * 17100;
              else if (abs(myEvent.leadingLeptonPDGId) == 13)  weight *= myEvent.weightCrossSection * 19200;
              else                                             weight *= 0.0;

              // Apply pile-up weight
              weight *= myEvent.weightPileUp;
              // For ttbar, apply topPt reweighting
              if (currentDataset == "ttbar") 
                  weight *= myEvent.weightTopPt;
          }
          
          // Split 1-lepton ttbar and 2-lepton ttbar
          string currentProcessClass_ = currentProcessClass;
          if ((currentDataset == "ttbar") && (myEvent.numberOfGenLepton == 2)) 
              currentProcessClass_ = "ttbar_2l";

          screwdriver.AutoFillProcessClass(currentProcessClass_,weight);

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
  screwdriver.WritePlots("../plots/dataMCChecks/");

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

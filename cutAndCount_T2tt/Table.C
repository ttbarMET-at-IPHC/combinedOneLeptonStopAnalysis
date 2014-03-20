
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

// Misc

#include "../common.h"

// BabyTuple format and location

#define FOLDER_BABYTUPLES "../store/babyTuples_0219_preSelectionSkimmed/"
#include "Reader.h"
babyEvent* myEventPointer;

void fillMCSignalTable(SonicScrewdriver* screwdriver, vector<string> region, vector<string> process, Table* table);

// #########################################################################
//                          Region selectors
// #########################################################################

bool inclusiveChannelSelector() { return true; }

bool Selector_presel() 
{
    babyEvent myEvent = *myEventPointer;

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
bool Selector_cutAndCount_lowDeltaM()     { return Selector_cutAndCount(-1,130,-1,130,true,false);  }
bool Selector_cutAndCount_offShellLoose() { return Selector_cutAndCount(-1,120,-1,200,false,true);  }
bool Selector_cutAndCount_offShellTight() { return Selector_cutAndCount(-1,140,-1,250,false,true);  }

bool Selector_MTAnalysis(float METcut, bool useMT2Wcut)
{
    // Don't consider muon with pT < 25 for MT analysis
    if ((abs(myEventPointer->leadingLeptonPDGId) == 13) 
         && (myEventPointer->leadingLepton.Pt()   < 25)) return false;

    if (myEventPointer->deltaPhiMETJets < 0.8) return false;
    if (myEventPointer->hadronicChi2    > 5) return false;
    if (myEventPointer->MT              < 120) return false;
    if (myEventPointer->MET             < METcut) return false;
    if (useMT2Wcut && (myEventPointer->MT2W < 200)) return false;

    return Selector_presel();
}

bool Selector_MTAnalysis_LM150() { return Selector_MTAnalysis(150,false); }
bool Selector_MTAnalysis_LM200() { return Selector_MTAnalysis(200,false); }
bool Selector_MTAnalysis_LM250() { return Selector_MTAnalysis(250,false); }
bool Selector_MTAnalysis_LM300() { return Selector_MTAnalysis(300,false); }
bool Selector_MTAnalysis_HM150() { return Selector_MTAnalysis(150,true);  }
bool Selector_MTAnalysis_HM200() { return Selector_MTAnalysis(200,true);  }
bool Selector_MTAnalysis_HM250() { return Selector_MTAnalysis(250,true);  }
bool Selector_MTAnalysis_HM300() { return Selector_MTAnalysis(300,true);  }

// #########################################################################
//                          Others tools/stuff
// #########################################################################

float getYield(vector< vector<float> > listEvent, vector<float> cuts);
vector<float> optimizeCuts(vector< vector<float> > listBackground,  vector< vector<float> > listSignal, bool* use, float* bestFOM, float* bestYieldSig, float* bestYieldBkg);
void formatAndWriteMapPlot(SonicScrewdriver* screwdriver, TH2F* theHisto, string name, string comment, bool enableText);

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

     screwdriver.AddVariable("MET",            "MET",                     "GeV",    15,50,500,      &(myEvent.MET),                  "logY=true");

     screwdriver.AddVariable("mStop",          "m_{#tilde{t}}",           "GeV",    28,112.5,812.5,  &(myEvent.mStop),               "");
     screwdriver.AddVariable("mNeutralino",    "m_{#chi^{0}}",            "GeV",    16,-12.5,387.5,  &(myEvent.mNeutralino),         "");
     
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

     //screwdriver.AddProcessClass("T2tt",     "T2tt",                       "signal",kViolet-1);
             screwdriver.AddDataset("T2tt",     "T2tt",   0, 0);

     screwdriver.AddProcessClass("signal_250_100",  "T2tt (250/100)",             "signal",COLORPLOT_AZURE);
     screwdriver.AddProcessClass("signal_450_100",  "T2tt (450/100)",             "signal",kCyan-3);
     screwdriver.AddProcessClass("signal_650_100",  "T2tt (650/100)",             "signal",COLORPLOT_GREEN);

  // ##########################
  // ##    Create Regions    ##
  // ##########################

     screwdriver.AddRegion("presel",             "Preselection",                 &Selector_presel);

     screwdriver.AddRegion("CC_highDM",          "Cut&Count high#DeltaM",        &Selector_cutAndCount_highDeltaM);
     screwdriver.AddRegion("CC_mediumDM",        "Cut&Count medium#DeltaM",      &Selector_cutAndCount_mediumDeltaM);
     screwdriver.AddRegion("CC_lowDM",           "Cut&Count low#DeltaM",         &Selector_cutAndCount_lowDeltaM);
     screwdriver.AddRegion("CC_offShell_Tight",  "Cut&Count offShell Tight",     &Selector_cutAndCount_offShellTight);
     screwdriver.AddRegion("CC_offShell_Loose",  "Cut&Count offShell Loose",     &Selector_cutAndCount_offShellLoose);

     screwdriver.AddRegion("MT_LM150",           "MT analysis LM150",            &Selector_MTAnalysis_LM150);
     screwdriver.AddRegion("MT_LM200",           "MT analysis LM200",            &Selector_MTAnalysis_LM200);
     screwdriver.AddRegion("MT_LM250",           "MT analysis LM250",            &Selector_MTAnalysis_LM250);
     screwdriver.AddRegion("MT_LM300",           "MT analysis LM300",            &Selector_MTAnalysis_LM300);
     screwdriver.AddRegion("MT_HM150",           "MT analysis HM150",            &Selector_MTAnalysis_HM150);
     screwdriver.AddRegion("MT_HM200",           "MT analysis HM200",            &Selector_MTAnalysis_HM200);
     screwdriver.AddRegion("MT_HM250",           "MT analysis HM250",            &Selector_MTAnalysis_HM250);
     screwdriver.AddRegion("MT_HM300",           "MT analysis HM300",            &Selector_MTAnalysis_HM300);


  // ##########################
  // ##   Create Channels    ##
  // ##########################
   
     screwdriver.AddChannel("inclusiveChannel","",&inclusiveChannelSelector);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     screwdriver.SetLumi(20000);

     // Create histograms
     screwdriver.Create1DHistos();
     screwdriver.Add2DHisto("mStop","mNeutralino");

     screwdriver.SetGlobalBoolOption  ("1DSuperimposed",   "includeSignal",                   true   );

     screwdriver.SetGlobalStringOption("1DStack",          "includeSignal",                   "stack");
     screwdriver.SetGlobalFloatOption ("1DStack",          "factorSignal",                    1.0    );

     screwdriver.SetGlobalStringOption("DataMCComparison", "includeSignal",                   "stack");
     screwdriver.SetGlobalFloatOption ("DataMCComparison", "factorSignal",                    1.0    );

     screwdriver.SetGlobalFloatOption ("FigureOfMerit",    "backgroundSystematicUncertainty", 0.15   );

     // Schedule plots
     screwdriver.SchedulePlots("1DSuperimposed");
     screwdriver.SchedulePlots("1DStack");
     screwdriver.SchedulePlots("2D");
     screwdriver.SchedulePlots("2DSuperimposed");
     screwdriver.SchedulePlots("1DFigureOfMerit","var=METoverSqrtHT,cutType=keepHighValues");

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

     intermediatePointers pointers;
     InitializeBranches(theTree,&myEvent,&pointers);

     cout << "                    " << currentDataset << endl; 

  // ########################################
  // ##        Run over the events         ##
  // ########################################

      for (int i = 0 ; i < theTree->GetEntries() ; i++)
      //for (int i = 0 ; i < min(200000, (int) theTree->GetEntries()); i++)
      {
          if (i % (theTree->GetEntries() / 50) == 0) 
              printProgressBar(i,theTree->GetEntries());

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          // Keep only events that pass preselection
          if (!Selector_presel()) continue;

          // Weight to lumi and apply trigger efficiency
          float weight = myEvent.weightCrossSection * screwdriver.GetLumi() * myEvent.weightTriggerEfficiency;
          
          // Apply PU weight except for signal
          //if (currentDataset != "T2tt")  
              weight *= myEvent.weightPileUp;
          
          // For ttbar, apply topPt reweighting
          if (currentDataset == "ttbar") weight *= myEvent.weightTopPt;

          // For signal, apply ISR reweighting
          if (currentDataset == "T2tt")  weight *= myEvent.weightISRmodeling;

          // Split 1-lepton ttbar and 2-lepton ttbar
          string currentProcessClass_ = currentProcessClass;
          if ((currentDataset == "ttbar") && (myEvent.numberOfGenLepton == 2)) 
              currentProcessClass_ = "ttbar_2l";

          // Fill all the variables with autoFill-mode activated
          if (currentDataset != "T2tt")
            screwdriver.AutoFillProcessClass(currentProcessClass_,weight);
          if ((myEvent.mStop == 250) && (myEvent.mNeutralino == 100))
              screwdriver.AutoFillProcessClass("signal_250_100",weight);
          if ((myEvent.mStop == 450) && (myEvent.mNeutralino == 100))
              screwdriver.AutoFillProcessClass("signal_450_100",weight);
          if ((myEvent.mStop == 650) && (myEvent.mNeutralino == 100))
              screwdriver.AutoFillProcessClass("signal_650_100",weight);
      } 
      
      cout << endl;
      f.Close();

  }

  // ###################################
  // ##   Make plots and write them   ##
  // ###################################
 
  /*
  cout << endl;
  cout << "   > Making plots..." << endl;
  screwdriver.MakePlots();
  cout << "   > Saving plots..." << endl;
  screwdriver.WritePlots("../plots/cutAndCount_T2tt/");
  */

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################
  
  printBoxedMessage("Now computing misc tests ... ");

  // ##########################
  // ##   Compute FOM maps   ##
  // ##########################

  vector<string> regionsOldCC_LM;
  regionsOldCC_LM.push_back("MT_LM150");
  regionsOldCC_LM.push_back("MT_LM200");
  regionsOldCC_LM.push_back("MT_LM250");
  regionsOldCC_LM.push_back("MT_LM300");

  vector<string> regionsOldCC_HM;
  regionsOldCC_HM.push_back("MT_HM150");
  regionsOldCC_HM.push_back("MT_HM200");
  regionsOldCC_HM.push_back("MT_HM250");
  regionsOldCC_HM.push_back("MT_HM300");

  vector<string> regionsNewCC;
  regionsNewCC.push_back("CC_offShell_Loose");
  regionsNewCC.push_back("CC_offShell_Tight");
  regionsNewCC.push_back("CC_lowDM");
  regionsNewCC.push_back("CC_mediumDM");
  regionsNewCC.push_back("CC_highDM");

  vector<string> processes;
  processes.push_back("ttbar_1l");
  processes.push_back("ttbar_2l");
  processes.push_back("W+jets");
  processes.push_back("others");
  processes.push_back("total");
  processes.push_back("signal_250_100");
  processes.push_back("signal_450_100");
  processes.push_back("signal_650_100"); 

  Table yieldTableOldCC_LM(regionsOldCC_LM,processes);
  Table yieldTableOldCC_HM(regionsOldCC_HM,processes);
  Table yieldTableNewCC(regionsNewCC,processes);

  fillMCSignalTable(&screwdriver,regionsOldCC_LM,processes,&yieldTableOldCC_LM);
  fillMCSignalTable(&screwdriver,regionsOldCC_HM,processes,&yieldTableOldCC_HM);
  fillMCSignalTable(&screwdriver,regionsNewCC,processes,&yieldTableNewCC);

  yieldTableOldCC_LM.PrintTableLatex();
  yieldTableOldCC_HM.PrintTableLatex();
  yieldTableNewCC.PrintTableLatex();

  printBoxedMessage("Program done.");
  return (0);
}

void fillMCSignalTable(SonicScrewdriver* screwdriver, vector<string> region, vector<string> process, Table* table)
{
    string varUsedToGetYields = "MET";
    string channelUsedToGetYields = "inclusiveChannel";

    for (unsigned int r = 0 ; r < region.size()          ; r++)
    {
        Figure tmpTotal(0.0,0.0);
        for (unsigned int p = 0 ; p < process.size() ; p++)
        {
            if (process[p] == "total") continue;
            table->Set(region[r],
                      process[p],
                      screwdriver->GetYieldAndError(varUsedToGetYields,
                                               process[p],
                                               region[r],
                                               channelUsedToGetYields));
            
            if ((process[p] != "signal_250_100") 
             && (process[p] != "signal_450_100") 
             && (process[p] != "signal_650_100"))
                tmpTotal += screwdriver->GetYieldAndError(varUsedToGetYields,
                                                     process[p],
                                                     region[r],
                                                     channelUsedToGetYields);
        }
        table->Set(region[r],"total",tmpTotal);
    }

}




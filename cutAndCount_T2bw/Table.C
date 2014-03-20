
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

bool Selector_offShellLoose() 
{ 
    if ((myEventPointer->MET > 200)         
     && (myEventPointer->MT  > 100) 
     && (myEventPointer->deltaPhiMETJets > 0.8)) 
        return Selector_presel(); 
    else 
        return false; 
}
bool Selector_offShellTight()  
{ 
    if ((myEventPointer->MET > 300)         
     && (myEventPointer->MT  > 120) 
     && (myEventPointer->deltaPhiMETJets > 0.8)) 
        return Selector_presel(); 
    else 
        return false; 
}

bool Selector_lowDeltaM_1()  
{ 
    if ((myEventPointer->METoverSqrtHT > 10) 
     && (myEventPointer->MT > 140) 
     && (myEventPointer->MT2W > 180) 
     && (myEventPointer->deltaPhiMETJets > 0.8) 
     && (myEventPointer->hadronicChi2 < 5)) 
        return Selector_presel(); 
    else return false; 
}
bool Selector_lowDeltaM_1_noChi2()  
{ 
    if ((myEventPointer->METoverSqrtHT > 10) 
     && (myEventPointer->MT > 140) 
     && (myEventPointer->MT2W > 180) 
     && (myEventPointer->deltaPhiMETJets > 0.8)) 
        return Selector_presel(); 
    else 
        return false; 
}
bool Selector_lowDeltaM_2() 
{ 
    if ((myEventPointer->METoverSqrtHT > 6)  
     && (myEventPointer->MT > 120) 
     && (myEventPointer->MT2W > 200)  
     && (myEventPointer->deltaPhiMETJets > 0.8) 
     && (myEventPointer->leadingBPt > 100)) 
        return Selector_presel(); 
    else 
        return false; 
}
bool Selector_lowDeltaM_3() 
{ 
    if ((myEventPointer->MET > 150)          
     && (myEventPointer->MT > 120) 
     && (myEventPointer->MT2W > 200)  
     && (myEventPointer->deltaPhiMETJets > 0.8) 
     && (myEventPointer->leadingBPt > 150)) 
        return Selector_presel(); 
    else 
        return false; 
}
bool Selector_highDeltaM_1()
{ 
    if ((myEventPointer->MET > 270)
     && (myEventPointer->MT > 270)
     && (myEventPointer->MT2W > 220) 
     && (myEventPointer->deltaPhiMETJets > 0.8)) 
        return Selector_presel(); 
    else 
        return false; 
}
bool Selector_highDeltaM_2()
{ 
    if ((myEventPointer->MET > 270)
     && (myEventPointer->MT > 190) 
     && (myEventPointer->MT2W > 250)  
     && (myEventPointer->deltaPhiMETJets > 0.8)) 
        return Selector_presel();
    else 
        return false; 
}
bool Selector_highDeltaM_3() 
{ 
    if ((myEventPointer->MET > 260)  
        && (myEventPointer->leadingBPt > 140) 
        && (myEventPointer->MT2W > 200) 
        &&  (myEventPointer->deltaPhiMETJets > 0.8)) 
        return Selector_presel(); 
    else 
        return false; 
}
bool Selector_highDeltaM_4() 
{ 
    if ((myEventPointer->MET > 300)  
     && (myEventPointer->leadingBPt > 200) 
     && (myEventPointer->MT2W > 290) 
     && (myEventPointer->deltaPhiMETJets > 0.8)) 
        return Selector_presel(); 
    else
        return false; 
}


bool Selector_MTAnalysis(float METcut, bool useHighDeltaMCuts)
{
    // Don't consider muon with pT < 25 for MT analysis
    if ((abs(myEventPointer->leadingLeptonPDGId) == 13) 
         && (myEventPointer->leadingLepton.Pt()   < 25)) return false;

    if (myEventPointer->deltaPhiMETJets < 0.8) return false;
    if (myEventPointer->MT              < 120) return false;
    if (myEventPointer->MET             < METcut) return false;
    if (useHighDeltaMCuts && ((myEventPointer->MT2W < 200) || (myEventPointer->leadingBPt < 100))) return false;

    return Selector_presel();
}

bool Selector_MTAnalysis_LM100() { return Selector_MTAnalysis(100,false); }
bool Selector_MTAnalysis_LM150() { return Selector_MTAnalysis(150,false); }
bool Selector_MTAnalysis_LM200() { return Selector_MTAnalysis(200,false); }
bool Selector_MTAnalysis_LM250() { return Selector_MTAnalysis(250,false); }
bool Selector_MTAnalysis_HM100() { return Selector_MTAnalysis(100,true);  }
bool Selector_MTAnalysis_HM150() { return Selector_MTAnalysis(150,true);  }
bool Selector_MTAnalysis_HM200() { return Selector_MTAnalysis(200,true);  }
bool Selector_MTAnalysis_HM250() { return Selector_MTAnalysis(250,true);  }

// #########################################################################
//                          Others tools/stuff
// #########################################################################

float getYield(vector< vector<float> > listEvent, vector<float> cuts);
vector<float> optimizeCuts(vector< vector<float> > listBackground,  vector< vector<float> > listSignal, bool* use, float* bestFOM, float* bestYieldSig, float* bestYieldBkg);
void formatAndWriteMapPlot(SonicScrewdriver* screwdriver, TH2F* theHisto, string name, string comment, bool enableText, float lineOffset);

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

//     screwdriver.AddProcessClass("T2bw-025",     "T2bw (x=0.25)",          "signal",COLORPLOT_AZURE);
//          screwdriver.AddDataset("T2bw-025",     "T2bw-025",   0, 0);

//     screwdriver.AddProcessClass("T2bw-050",     "T2bw (x=0.50)",          "signal",kCyan-3);
//          screwdriver.AddDataset("T2bw-050",     "T2bw-050",   0, 0);

     screwdriver.AddProcessClass("T2bw-075",     "T2bw (x=0.75)",          "signal",COLORPLOT_GREEN);
          screwdriver.AddDataset("T2bw-075",     "T2bw-075",   0, 0);
    
     screwdriver.AddProcessClass("signal_300_50", "T2bw (300/50)",         "signal",COLORPLOT_GREEN);
     screwdriver.AddProcessClass("signal_450_50", "T2bw (450/50)",         "signal",COLORPLOT_GREEN);
     screwdriver.AddProcessClass("signal_600_50", "T2bw (600/50)",         "signal",COLORPLOT_GREEN);

  // ##########################
  // ##    Create Regions    ##
  // ##########################

     screwdriver.AddRegion("presel",             "Preselection",                 &Selector_presel);
     
     screwdriver.AddRegion("offShellLoose",      "Off-shell loose",              &Selector_offShellLoose     );
     screwdriver.AddRegion("offShellTight",      "Off-shell tight",              &Selector_offShellTight     );
     screwdriver.AddRegion("lowDeltaM_1",        "low #DeltaM - 1",              &Selector_lowDeltaM_1       ); 
     screwdriver.AddRegion("lowDeltaM_1_noChi2", "low #DeltaM - 1noChi2",        &Selector_lowDeltaM_1_noChi2);
     screwdriver.AddRegion("lowDeltaM_2",        "low #DeltaM - 2",              &Selector_lowDeltaM_2       );
     screwdriver.AddRegion("lowDeltaM_3",        "low #DeltaM - 3",              &Selector_lowDeltaM_3       );
     screwdriver.AddRegion("highDeltaM_1",       "high #DeltaM - 1",             &Selector_highDeltaM_1      );
     screwdriver.AddRegion("highDeltaM_2",       "high #DeltaM - 2",             &Selector_highDeltaM_2      );
     screwdriver.AddRegion("highDeltaM_3",       "high #DeltaM - 3",             &Selector_highDeltaM_3      );
     screwdriver.AddRegion("highDeltaM_4",       "high #DeltaM - 4",             &Selector_highDeltaM_4      );

     screwdriver.AddRegion("MT_LM100",           "MT analysis;(LM 100)",         &Selector_MTAnalysis_LM150);
     screwdriver.AddRegion("MT_LM150",           "MT analysis;(LM 150)",         &Selector_MTAnalysis_LM150);
     screwdriver.AddRegion("MT_LM200",           "MT analysis;(LM 200)",         &Selector_MTAnalysis_LM200);
     screwdriver.AddRegion("MT_LM250",           "MT analysis;(LM 250)",         &Selector_MTAnalysis_LM250);
     screwdriver.AddRegion("MT_HM100",           "MT analysis;(HM 100)",         &Selector_MTAnalysis_HM150);
     screwdriver.AddRegion("MT_HM150",           "MT analysis;(HM 150)",         &Selector_MTAnalysis_HM150);
     screwdriver.AddRegion("MT_HM200",           "MT analysis;(HM 200)",         &Selector_MTAnalysis_HM200);
     screwdriver.AddRegion("MT_HM250",           "MT analysis;(HM 250)",         &Selector_MTAnalysis_HM250);

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
     //screwdriver.SchedulePlots("1DFigureOfMerit","var=METoverSqrtHT,cutType=keepHighValues");

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
      //for (int i = 0 ; i < min(100000, (int) theTree->GetEntries()); i++)
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
          //if  ((currentDataset != "T2bw-025")
          //&& (currentDataset != "T2bw-050")
          //&& (currentDataset != "T2bw-075")) 
              weight *= myEvent.weightPileUp;
          
          // For ttbar, apply topPt reweighting
          if (currentDataset == "ttbar") weight *= myEvent.weightTopPt;

          // For signal, apply ISR reweighting
          if  ((currentDataset == "T2bw-025")
            || (currentDataset == "T2bw-050")
            || (currentDataset == "T2bw-075")) weight *= myEvent.weightISRmodeling;

          // Split 1-lepton ttbar and 2-lepton ttbar
          string currentProcessClass_ = currentProcessClass;
          if ((currentDataset == "ttbar") && (myEvent.numberOfGenLepton == 2)) 
              currentProcessClass_ = "ttbar_2l";

          if  ((currentDataset != "T2bw-025")
            && (currentDataset != "T2bw-050")
            && (currentDataset != "T2bw-075")) 
            screwdriver.AutoFillProcessClass(currentProcessClass_,weight);
          if ((myEvent.mStop == 300) && (myEvent.mNeutralino == 50))
              screwdriver.AutoFillProcessClass("signal_300_50",weight);
          if ((myEvent.mStop == 450) && (myEvent.mNeutralino == 50))
              screwdriver.AutoFillProcessClass("signal_450_50",weight);
          if ((myEvent.mStop == 600) && (myEvent.mNeutralino == 50))
              screwdriver.AutoFillProcessClass("signal_600_50",weight);

      } 
      
      cout << endl;
      f.Close();

  }

  // ###################################
  // ##   Make plots and write them   ##
  // ###################################
 
  cout << endl;
  cout << "   > Making plots..." << endl;
  //screwdriver.MakePlots();
  cout << "   > Saving plots..." << endl;
  //screwdriver.WritePlots("../plots/cutAndCount_T2bw/");

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################
  
  printBoxedMessage("Now computing misc tests ... ");

  vector<string> regionsNewCC;
  //regionsNewCC.push_back("offShellLoose");
  regionsNewCC.push_back("offShellTight");
  regionsNewCC.push_back("lowDeltaM_1");
  //regionsNewCC.push_back("lowDeltaM_1_noChi2");
  //regionsNewCC.push_back("lowDeltaM_2");
  //regionsNewCC.push_back("lowDeltaM_3");
  regionsNewCC.push_back("highDeltaM_1");
  regionsNewCC.push_back("highDeltaM_2");
  //regionsNewCC.push_back("highDeltaM_3");
  //regionsNewCC.push_back("highDeltaM_4");

  vector<string> processes;
  processes.push_back("ttbar_1l");
  processes.push_back("ttbar_2l");
  processes.push_back("W+jets");
  processes.push_back("others");
  processes.push_back("total");
  processes.push_back("signal_300_50");
  processes.push_back("signal_450_50");
  processes.push_back("signal_600_50");


  Table yieldTableNewCC(regionsNewCC,processes);

  fillMCSignalTable(&screwdriver,regionsNewCC,processes,&yieldTableNewCC);

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
            
            if ((process[p] != "signal_300_50") 
             && (process[p] != "signal_450_50") 
             && (process[p] != "signal_600_50"))
                tmpTotal += screwdriver->GetYieldAndError(varUsedToGetYields,
                                                     process[p],
                                                     region[r],
                                                     channelUsedToGetYields);
        }
        table->Set(region[r],"total",tmpTotal);
    }

}




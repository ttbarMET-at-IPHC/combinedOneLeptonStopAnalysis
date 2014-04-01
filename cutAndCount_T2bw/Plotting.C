
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


bool findISRJet()
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

    return foundISRJet;
}

bool Selector_cutAndCount(float cutMET, float cutMETsig, float cutMT, float cutMT2W, float cutBPt, float cutDeltaPhi, bool enableISRJetRequirement)
{
    if (myEventPointer->MET             < cutMET)           return false;
    if (myEventPointer->METoverSqrtHT   < cutMETsig)        return false;
    if (myEventPointer->MT              < cutMT)            return false;
    if (myEventPointer->MT2W            < cutMT2W)          return false;
    if (myEventPointer->leadingBPt      < cutBPt)           return false;
    if (myEventPointer->deltaPhiMETJets < cutDeltaPhi)      return false;
    if ((enableISRJetRequirement) && (!findISRJet()))       return false;

    return Selector_presel();
}

bool Selector_offShellLoose() { return Selector_cutAndCount(200, -1, 100, -1,  -1, 0.8, false); } // 1
bool Selector_offShellTight() { return Selector_cutAndCount(300, -1, 120, -1,  -1, 0.8, false); } // 2
bool Selector_lowDeltaM_1()   { return Selector_cutAndCount(-1,  10, 140, 180, -1, 0.8, false); } // 3/4
bool Selector_lowDeltaM_2()   { return Selector_cutAndCount(-1,   6, 120, 200, 100,0.8, false); } // 5
bool Selector_lowDeltaM_3()   { return Selector_cutAndCount(150, -1, 120, 200, 150,0.8, false); } // 6
bool Selector_highDeltaM_1()  { return Selector_cutAndCount(270, -1, 270, 220, -1, 0.8, false); } // 7
bool Selector_highDeltaM_2()  { return Selector_cutAndCount(270, -1, 190, 250, -1, 0.8, false); } // 8
bool Selector_highDeltaM_3()  { return Selector_cutAndCount(260, -1, -1,  200, 140,0.8, false); } // 9
bool Selector_highDeltaM_4()  { return Selector_cutAndCount(300, -1, -1,  290, 200,0.8, false); } // 10

                                                              // MET METsig MT MT2W BPt dPhi ISRjet
bool Selector_offShell_new1()      { return Selector_cutAndCount(300, -1,  120, -1,  -1, 0.8, true);  } // 11
bool Selector_offShell_new2()      { return Selector_cutAndCount(350, -1,  120, -1,  -1, 0.8, true);  } // 12 
bool Selector_lowDeltaM_new1()     { return Selector_cutAndCount(-1,   8, 120, 200, 150, 0.8, false); } // 13
bool Selector_lowDeltaM_new2()     { return Selector_cutAndCount(150, -1, 120, 200, 150, 0.8, false); } // 14
bool Selector_highDeltaM_new()     { return Selector_cutAndCount(-1,  11,  -1, 290, 200, 0.8, false); } // 15
bool Selector_offShell_new3()      { return Selector_cutAndCount(300, -1,  120, -1,  -1, 0.2, false); } // 16
bool Selector_offShell_new4()      { return Selector_cutAndCount(350, -1,  120, -1,  -1, 0.2, false); } // 17
bool Selector_lowDeltaM_new3()     { return Selector_cutAndCount(-1,   6, 120, 200, 100, 0.2, false); } // 18
bool Selector_lowDeltaM_new4()     { return Selector_cutAndCount(150, -1, 120, 200, 150, 0.2, false); } // 19
bool Selector_lowDeltaM_new5()     { return Selector_cutAndCount(-1,   7, 120, 200, 125, 0.2, false); } // 20
bool Selector_lowDeltaM_new6()     { return Selector_cutAndCount(-1,   7, 120, 200, 150, 0.8, false); } // 21
bool Selector_lowDeltaM_new7()     { return Selector_cutAndCount(-1,   7, 120, 200, 150, 0.2, false); } // 22
bool Selector_lowDeltaM_new8()     { return Selector_cutAndCount(-1,   6, 120, 200, 150, 0.2, false); } // 23
bool Selector_highDeltaM_new1()    { return Selector_cutAndCount(-1,  10, 140, 200, 180, 0.8, false); } // 24

/*
bool Selector_offShellLoose_mod2() { return Selector_cutAndCount(150, -1, 100, -1,  -1, 0.8, false); } // 12
bool Selector_offShellLoose_mod3() { return Selector_cutAndCount(200, -1, 100, -1,  -1, 0.8, false); } // 
bool Selector_offShellTight_mod1() { return Selector_cutAndCount(300, -1, 150, -1,  -1, 0.8, false); } // 14
bool Selector_offShellTight_mod4() { return Selector_cutAndCount(300, -1, 120, -1,  -1, 0.8, true ); } // 
bool Selector_offShellTight_mod5() { return Selector_cutAndCount(350, -1, 120, -1,  -1, 0.8, true ); } // 16
bool Selector_offShellTight_mod6() { return Selector_cutAndCount(300, -1, 120, -1,  -1, 0.8, false); } //
bool Selector_offShellTight_mod7() { return Selector_cutAndCount(300, -1, 120, -1,  -1, 0.8, true ); } // 18
bool Selector_offShellTight_mod8() { return Selector_cutAndCount(350, -1, 120, -1,  -1, 0.8, true ); } // 
bool Selector_lowDeltaM_3_mod1()   { return Selector_cutAndCount(170, -1, 140, 200, 150,0.8, false); } // 20
bool Selector_lowDeltaM_3_mod2()   { return Selector_cutAndCount(0,    8, 120, 200, 150,0.8, false); } // 
bool Selector_lowDeltaM_3_mod3()   { return Selector_cutAndCount(150, -1, 120, 200, 150,0.8, false); } // 22
bool Selector_lowDeltaM_3_mod4()   { return Selector_cutAndCount(0,    8, 150, 200, 150,0.8, false); } // 
bool Selector_lowDeltaM_3_mod5()   { return Selector_cutAndCount(150, -1, 150, 200, 150,0.8, false); } // 24
bool Selector_highDeltaM_4_mod1()  { return Selector_cutAndCount(-1,  -1, 200, 290, 200,0.8, false); } // 
bool Selector_highDeltaM_4_mod2()  { return Selector_cutAndCount(-1,  11,  -1, 290, 200,0.8, false); } // 26
*/

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

     screwdriver.AddVariable("METoverSqrtHT",  "MET / #sqrt{H_{T}}",      "",       32,0,32,        &(myEvent.METoverSqrtHT),        "");
     screwdriver.AddVariable("MET",            "MET",                     "GeV",    15,50,500,      &(myEvent.MET),                  "logY=true");
     screwdriver.AddVariable("MT",             "MT",                      "GeV",    17,0,510,       &(myEvent.MT),                   "logY=true");
     screwdriver.AddVariable("deltaPhiMETJets","#Delta#Phi(MET,j_{1,2})", "rad",    16,0,3.2,       &(myEvent.deltaPhiMETJets),      "");
     screwdriver.AddVariable("MT2W",           "M_{T2}^{W}",              "GeV",    20,0,500,       &(myEvent.MT2W),                 "");
     screwdriver.AddVariable("HT",             "H_{T}",                   "",       45,150,1500,    &(myEvent.HT),                   "");
     screwdriver.AddVariable("HTratio",        "H_{T}^{ratio}",           "",       20,0,1.2,       &(myEvent.HTRatio),              "");
     screwdriver.AddVariable("HadronicChi2",   "Hadronic #chi^{2}",       "",       40,0,20,        &(myEvent.hadronicChi2),         "");
     screwdriver.AddVariable("leadingBPt",     "p_{T}(leading b-jet)",    "GeV",    25,0,500,       &(myEvent.leadingBPt),           "");
     screwdriver.AddVariable("leadingJetPt",   "p_{T}(leading jet)",      "GeV",    25,0,500,       &(myEvent.leadingJetPt),         "");
     screwdriver.AddVariable("leptonPt",       "p_{T}(lepton)",           "GeV",    25,0,500,       &(myEvent.leadingLeptonPt),      "");

     screwdriver.AddVariable("Mlb",            "M_{lb}",                  "GeV",    25,0,500,       &(myEvent.Mlb),                  "");
     screwdriver.AddVariable("Mlb_hemi",       "M_{lb}_hemi",             "GeV",    25,0,500,       &(myEvent.Mlb_hemi),             "");
     screwdriver.AddVariable("M3b",            "M3b",                     "GeV",    25,0,500,       &(myEvent.M3b),                  "");
     screwdriver.AddVariable("deltaRLeptonB",  "#DeltaR(l,leading b)",    "",       25,0,5,         &(myEvent.deltaRLeptonLeadingB), "");
     screwdriver.AddVariable("HTLeptonPtMET",  "HT + MET + p_{T}(lepton)","GeV",    20,100,2100,    &(myEvent.HTPlusLeptonPtPlusMET),"");

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

     screwdriver.AddProcessClass("T2bw-025",     "T2bw (x=0.25)",          "signal",COLORPLOT_AZURE);
          screwdriver.AddDataset("T2bw-025",     "T2bw-025",   0, 0);

     screwdriver.AddProcessClass("T2bw-050",     "T2bw (x=0.50)",          "signal",kCyan-3);
    //      screwdriver.AddDataset("T2bw-050",     "T2bw-050",   0, 0);

     screwdriver.AddProcessClass("T2bw-075",     "T2bw (x=0.75)",          "signal",COLORPLOT_GREEN);
    //      screwdriver.AddDataset("T2bw-075",     "T2bw-075",   0, 0);
  
  // ##########################
  // ##    Create Regions    ##
  // ##########################

     screwdriver.AddRegion("presel",             "Preselection",                 &Selector_presel);
     
     screwdriver.AddRegion("offShellLoose",      "Off-shell loose",              &Selector_offShellLoose   );
     screwdriver.AddRegion("offShellTight",      "Off-shell tight",              &Selector_offShellTight   );
     screwdriver.AddRegion("lowDeltaM_1",        "low #DeltaM - 1",              &Selector_lowDeltaM_1     ); 
     screwdriver.AddRegion("lowDeltaM_2",        "low #DeltaM - 2",              &Selector_lowDeltaM_2     );
     screwdriver.AddRegion("lowDeltaM_3",        "low #DeltaM - 3",              &Selector_lowDeltaM_3     );
     screwdriver.AddRegion("highDeltaM_1",       "high #DeltaM - 1",             &Selector_highDeltaM_1    );
     screwdriver.AddRegion("highDeltaM_2",       "high #DeltaM - 2",             &Selector_highDeltaM_2    );
     screwdriver.AddRegion("highDeltaM_3",       "high #DeltaM - 3",             &Selector_highDeltaM_3    );
     screwdriver.AddRegion("highDeltaM_4",       "high #DeltaM - 4",             &Selector_highDeltaM_4    );

     screwdriver.AddRegion("MT_LM100",           "MT analysis;(LM 100)",         &Selector_MTAnalysis_LM100);
     screwdriver.AddRegion("MT_LM150",           "MT analysis;(LM 150)",         &Selector_MTAnalysis_LM150);
     screwdriver.AddRegion("MT_LM200",           "MT analysis;(LM 200)",         &Selector_MTAnalysis_LM200);
     screwdriver.AddRegion("MT_LM250",           "MT analysis;(LM 250)",         &Selector_MTAnalysis_LM250);
     screwdriver.AddRegion("MT_HM100",           "MT analysis;(HM 100)",         &Selector_MTAnalysis_HM100);
     screwdriver.AddRegion("MT_HM150",           "MT analysis;(HM 150)",         &Selector_MTAnalysis_HM150);
     screwdriver.AddRegion("MT_HM200",           "MT analysis;(HM 200)",         &Selector_MTAnalysis_HM200);
     screwdriver.AddRegion("MT_HM250",           "MT analysis;(HM 250)",         &Selector_MTAnalysis_HM250);
     
     screwdriver.AddRegion("offShell_new1",      "off-shell;new1",               &Selector_offShell_new1   );
     screwdriver.AddRegion("offShell_new2",      "off-shell;new2",               &Selector_offShell_new2   );
     screwdriver.AddRegion("lowDeltaM_new1",     "low #DeltaM;new1",             &Selector_lowDeltaM_new1  );
     screwdriver.AddRegion("lowDeltaM_new2",     "low #DeltaM;new2",             &Selector_lowDeltaM_new2  );
     screwdriver.AddRegion("highDeltaM_new",     "high #DeltaM;new",             &Selector_highDeltaM_new  );
     screwdriver.AddRegion("offShell_new3",      "off-shell;new3",               &Selector_offShell_new3   );
     screwdriver.AddRegion("offShell_new4",      "off-shell;new4",               &Selector_offShell_new4   );
     screwdriver.AddRegion("lowDeltaM_new3",     "low #DeltaM;new3",             &Selector_lowDeltaM_new3  );
     screwdriver.AddRegion("lowDeltaM_new4",     "low #DeltaM;new4",             &Selector_lowDeltaM_new4  );
     screwdriver.AddRegion("lowDeltaM_new5",     "low #DeltaM;new5",             &Selector_lowDeltaM_new5  );
     screwdriver.AddRegion("lowDeltaM_new6",     "low #DeltaM;new6",             &Selector_lowDeltaM_new6  );
     screwdriver.AddRegion("lowDeltaM_new7",     "low #DeltaM;new7",             &Selector_lowDeltaM_new7  );
     screwdriver.AddRegion("lowDeltaM_new8",     "low #DeltaM;new8",             &Selector_lowDeltaM_new8  );
     screwdriver.AddRegion("highDeltaM_new1",    "high #DeltaM;new1",            &Selector_highDeltaM_new1 );

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

  cout << "   > Reading datasets... " << endl;
  cout << endl;

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

  // ########################################
  // ##        Run over the events         ##
  // ########################################

      int nEntries = theTree->GetEntries();
      for (int i = 0 ; i < nEntries ; i++)
      //for (int i = 0 ; i < min(100000, (int) theTree->GetEntries()); i++)
      {
          if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          // Keep only events that pass preselection
          if (!Selector_presel()) continue;

          // Weight to lumi and apply trigger efficiency
          float weight = myEvent.weightCrossSection * screwdriver.GetLumi() * myEvent.weightTriggerEfficiency;
          
          // Apply PU weight
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

          screwdriver.AutoFillProcessClass(currentProcessClass_,weight);
      } 

      printProgressBar(nEntries,nEntries,currentDataset);
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
  screwdriver.WritePlots("../plots/cutAndCount_T2bw/");

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################
  
  printBoxedMessage("Now computing misc tests ... ");


  vector<string> signals;
  signals.push_back("T2bw-025");
  //signals.push_back("T2bw-050");
  //signals.push_back("T2bw-075");

  for (unsigned int s = 0 ; s < signals.size() ; s++)
  {

      // ##########################
      // ##   Compute FOM maps   ##
      // ##########################

      vector<string> cutAndCountRegions;
      cutAndCountRegions.push_back("offShellLoose"     );
      cutAndCountRegions.push_back("offShellTight"     );
      cutAndCountRegions.push_back("lowDeltaM_1"       );
      cutAndCountRegions.push_back("lowDeltaM_1"       );
      cutAndCountRegions.push_back("lowDeltaM_2"       );
      cutAndCountRegions.push_back("lowDeltaM_3"       );
      cutAndCountRegions.push_back("highDeltaM_1"      );
      cutAndCountRegions.push_back("highDeltaM_2"      );
      cutAndCountRegions.push_back("highDeltaM_3"      );
      cutAndCountRegions.push_back("highDeltaM_4"      );
      cutAndCountRegions.push_back("offShell_new1"     );
      cutAndCountRegions.push_back("offShell_new2"     );
      cutAndCountRegions.push_back("lowDeltaM_new1"    );
      cutAndCountRegions.push_back("lowDeltaM_new2"    );
      cutAndCountRegions.push_back("highDeltaM_new"    );
      cutAndCountRegions.push_back("offShell_new3"     );
      cutAndCountRegions.push_back("offShell_new4"     );
      cutAndCountRegions.push_back("lowDeltaM_new3"    );
      cutAndCountRegions.push_back("lowDeltaM_new4"    );
      cutAndCountRegions.push_back("lowDeltaM_new5"    );
      cutAndCountRegions.push_back("lowDeltaM_new6"    );
      cutAndCountRegions.push_back("lowDeltaM_new7"    );
      cutAndCountRegions.push_back("lowDeltaM_new8"    );
      cutAndCountRegions.push_back("highDeltaM_new1"    );

      vector<TH2F*> signalMaps;
      vector<TH2F*> backgroundMaps;
      vector<TH2F*> FOMMaps;
      vector<TH2F*> efficiencies;

      int nBinsX = -1;
      int nBinsY = -1;

      TH2F* signalMapPresel  = screwdriver.get2DHistoClone("mStop","mNeutralino",signals[s],"presel","inclusiveChannel");
      TH2F* backgroundPresel = screwdriver.get2DCompositeHistoClone("mStop","mNeutralino","2DSumBackground","presel","inclusiveChannel","");

      if (nBinsX == -1) nBinsX = signalMapPresel->GetNbinsX();
      if (nBinsY == -1) nBinsY = signalMapPresel->GetNbinsY();

      // Store background eff in (mStop,mLSP) = (200,300)
      int backgroundBin = signalMapPresel->FindBin(200,300);
      float backgroundYieldPresel = backgroundPresel->Integral(0,nBinsX+1,0,nBinsY+1);

      for (unsigned int i = 0 ; i < cutAndCountRegions.size() ; i++)
      {
          signalMaps.push_back(screwdriver.get2DHistoClone("mStop","mNeutralino",signals[s],cutAndCountRegions[i],"inclusiveChannel"));
          signalMaps[i]->SetName((string("signalMap_")+cutAndCountRegions[i]).c_str());
          backgroundMaps.push_back(screwdriver.get2DCompositeHistoClone("mStop","mNeutralino","2DSumBackground",cutAndCountRegions[i],"inclusiveChannel",""));

          float B = backgroundMaps[i]->Integral(0,nBinsX+1,0,nBinsY+1);
          if (B < 1.0) B = 1.0;
     
          efficiencies.push_back((TH2F*) signalMaps[i]->Clone());
          efficiencies[i]->SetName((string("eff_")+cutAndCountRegions[i]).c_str());
          efficiencies[i]->Divide(signalMapPresel);
          efficiencies[i]->SetBinContent(backgroundBin,B/backgroundYieldPresel);
          
          FOMMaps.push_back((TH2F*) signalMaps[i]->Clone());
          FOMMaps[i]->SetName((string("FOM_")+cutAndCountRegions[i]).c_str());

          for (int x = 1 ; x <= nBinsX ; x++)
          for (int y = 1 ; y <= nBinsY ; y++)
          {
              float S = signalMaps[i]->GetBinContent(x,y);
              float FOM = S / sqrt(B + 0.15*0.15*B*B);
              FOMMaps[i]->SetBinContent(x,y,FOM);
          }

      }

      // ################################
      // ##   Compute "best" FOM map   ##
      // ################################

      TH2F* bestFOMMap = (TH2F*) signalMaps[0]->Clone();  bestFOMMap->SetName("bestFOM");
      TH2F* bestSetMap = (TH2F*) signalMaps[0]->Clone();  bestSetMap->SetName("bestSet");
      TH2F* bestSigEff = (TH2F*) signalMaps[0]->Clone();  bestSigEff->SetName("bestSigEff");
      TH2F* bestBkgEff = (TH2F*) signalMaps[0]->Clone();  bestBkgEff->SetName("bestBkgEff");

      TH2F* nextBestFOMMap = (TH2F*) signalMaps[0]->Clone();  nextBestFOMMap->SetName("nextBestFOM");
      TH2F* nextBestSetMap = (TH2F*) signalMaps[0]->Clone();  nextBestSetMap->SetName("nextBestSet");
      
      TH2F* nextNextBestFOMMap = (TH2F*) signalMaps[0]->Clone();  nextNextBestFOMMap->SetName("nextNextBestFOM");
      TH2F* nextNextBestSetMap = (TH2F*) signalMaps[0]->Clone();  nextNextBestSetMap->SetName("nextNextBestSet");
      
      for (int x = 1 ; x <= nBinsX ; x++)
      for (int y = 1 ; y <= nBinsY ; y++)
      {
          float bestFOM = -1.0;
          float nextBestFOM = -1.0;
          float nextNextBestFOM = -1.0;
          int bestSet = 0;
          int nextBestSet = 0;
          int nextNextBestSet = 0;
          float bestSigEff_ = -1.0;
          float bestBkgEff_ = -1.0;
          for (unsigned int i = 0 ; i < cutAndCountRegions.size() ; i++)
          {
              float FOM = FOMMaps[i]->GetBinContent(x,y);
              if (bestFOM < FOM)
              {
                  nextNextBestFOM = nextBestFOM;
                  nextNextBestSet = nextBestSet;
                  nextBestFOM = bestFOM;
                  nextBestSet = bestSet;

                  bestFOM = FOM;
                  if (bestFOM > 0) bestSet = i+1;
                  bestSigEff_ = efficiencies[i]->GetBinContent(x,y);
                  bestBkgEff_ = efficiencies[i]->GetBinContent(backgroundBin);
              }
              else if (nextBestFOM < FOM)
              {
                  nextNextBestFOM = nextBestFOM;
                  nextNextBestSet = nextBestSet;
                  nextBestFOM = FOM;
                  if (nextBestFOM > 0) nextBestSet = i+1;
              }
              else if (nextNextBestFOM < FOM)
              {
                  nextNextBestFOM = FOM;
                  if (nextNextBestFOM > 0) nextNextBestSet = i+1;
              }

          }
          bestFOMMap->SetBinContent(x,y,bestFOM);
          bestSetMap->SetBinContent(x,y,bestSet);
          nextBestFOMMap->SetBinContent(x,y,nextBestFOM);
          nextBestSetMap->SetBinContent(x,y,nextBestSet);
          nextNextBestFOMMap->SetBinContent(x,y,nextNextBestFOM);
          nextNextBestSetMap->SetBinContent(x,y,nextNextBestSet);
          bestSigEff->SetBinContent(x,y,bestSigEff_);
          bestBkgEff->SetBinContent(x,y,bestBkgEff_);
      }

      // #####################################
      // ##   Ratio between next-to-best,   ##
      // ##   nextnext-to-best and best     ##
      // #####################################
     
      TH2F* ratio_nextBest = (TH2F*) bestFOMMap->Clone();
      TH2F* ratio_nextNextBest = (TH2F*) bestFOMMap->Clone();

      ratio_nextBest->SetName("ratio_nextBest");
      ratio_nextNextBest->SetName("ratio_nextNextBest");

      for (int x = 1 ; x <= nBinsX ; x++)
      for (int y = 1 ; y <= nBinsY ; y++)
      {
          float bestFOM           = bestFOMMap->GetBinContent(x,y);
          float nextBestFOM     = nextBestFOMMap->GetBinContent(x,y);
          float nextNextBestFOM = nextNextBestFOMMap->GetBinContent(x,y);
          if (bestFOM == 0)
          {
             ratio_nextBest->SetBinContent(x,y,0.0);
             ratio_nextNextBest->SetBinContent(x,y,0.0);
          }
          else 
          {
            if (nextBestFOM == 0) ratio_nextBest->SetBinContent(x,y,0.0);
            else                    ratio_nextBest->SetBinContent(x,y,bestFOM/nextBestFOM);

            if (nextNextBestFOM == 0) ratio_nextNextBest->SetBinContent(x,y,0.0);
            else                        ratio_nextNextBest->SetBinContent(x,y,bestFOM/nextNextBestFOM);
          }
      } 

      // #####################################
      // ##   Compute map for MT analysis   ##
      // #####################################

      vector<string> cutAndCountRegions_MTanalysis;
      cutAndCountRegions_MTanalysis.push_back("MT_LM100");
      cutAndCountRegions_MTanalysis.push_back("MT_LM150");
      cutAndCountRegions_MTanalysis.push_back("MT_LM200");
      cutAndCountRegions_MTanalysis.push_back("MT_LM250");
      cutAndCountRegions_MTanalysis.push_back("MT_HM100");
      cutAndCountRegions_MTanalysis.push_back("MT_HM150");
      cutAndCountRegions_MTanalysis.push_back("MT_HM200");
      cutAndCountRegions_MTanalysis.push_back("MT_HM250");

      vector<TH2F*> signalMaps_MTanalysis;
      vector<TH2F*> backgroundMaps_MTanalysis;
      vector<TH2F*> FOMMaps_MTanalysis;

      for (unsigned int i = 0 ; i < cutAndCountRegions_MTanalysis.size() ; i++)
      {
          signalMaps_MTanalysis.push_back(screwdriver.get2DHistoClone("mStop","mNeutralino",signals[s],cutAndCountRegions_MTanalysis[i],"inclusiveChannel"));
          backgroundMaps_MTanalysis.push_back(screwdriver.get2DCompositeHistoClone("mStop","mNeutralino","2DSumBackground",cutAndCountRegions_MTanalysis[i],"inclusiveChannel",""));

          FOMMaps_MTanalysis.push_back((TH2F*) signalMaps_MTanalysis[i]->Clone());
          FOMMaps_MTanalysis[i]->SetName((string("FOM_")+cutAndCountRegions_MTanalysis[i]).c_str());

          float B = backgroundMaps_MTanalysis[i]->Integral(0,nBinsX+1,0,nBinsY+1);
          if (B < 1.0) B = 1.0;

          for (int x = 1 ; x <= nBinsX ; x++)
          for (int y = 1 ; y <= nBinsY ; y++)
          {
              float S = signalMaps_MTanalysis[i]->GetBinContent(x,y);
              float FOM = S / sqrt(B + 0.15*0.15*B*B);
              FOMMaps_MTanalysis[i]->SetBinContent(x,y,FOM);
          }
      }
      
      TH2F* bestFOMMap_MTanalysis = (TH2F*) FOMMaps_MTanalysis[0]->Clone();
      bestFOMMap_MTanalysis->SetName("bestFOMMap_MTanalysis");
      TH2F* ratio_newCC_MTanalysisCC = (TH2F*) FOMMaps_MTanalysis[0]->Clone();
      ratio_newCC_MTanalysisCC->SetName("ratio_newCC_MTanalysisCC");
      
      TFile bestSetMap_MTanalysis_File((string("CC_MTanalysis_mapBestSet_")+signals[s]+".root").c_str(),"READ");
      TH2F* bestSetMap_MTanalysis;
      bestSetMap_MTanalysis_File.GetObject("map",bestSetMap_MTanalysis);
      bestSetMap_MTanalysis->SetName("bestSetMap_MTanalysis");

      for (int x = 1 ; x <= nBinsX ; x++)
      for (int y = 1 ; y <= nBinsY ; y++)
      {
          int bestSet = bestSetMap_MTanalysis->GetBinContent(x,y);
          float bestFOM = 0.0;
          if (bestSet != 0) bestFOM = FOMMaps_MTanalysis[bestSet-1]->GetBinContent(x,y);
          bestFOMMap_MTanalysis->SetBinContent(x,y,bestFOM);
          
          if (bestFOM == 0)
            ratio_newCC_MTanalysisCC->SetBinContent(x,y,0.0);
          else
            ratio_newCC_MTanalysisCC->SetBinContent(x,y,bestFOMMap->GetBinContent(x,y)/bestFOM);
      }
      
      //bestSetMap_MTanalysis_File.Close();

      // #################################
      // ##   Make ratio map with BDT   ##
      // #################################

    /*
      TH2F* ratio_newCC_newBDT = (TH2F*) FOMMaps_MTanalysis[0]->Clone();
      ratio_newCC_newBDT->SetName("ratio_newCC_newBDT");
      for (int x = 1 ; x <= nBinsX ; x++)
      for (int y = 1 ; y <= nBinsY ; y++)
      {
          ratio_newCC_newBDT->SetBinContent(x,y,0.0);
      }


      TFile bestFOMMap_BDT_File((string("BDT_mapFOM_")+signals[s]+".root").c_str(),"READ");
      TH2D* bestFOMMap_BDT;
      bestFOMMap_BDT_File.GetObject("twodplot",bestFOMMap_BDT);

      for (int x = 3 ; x <= nBinsX ; x++)
      for (int y = 2 ; y <= nBinsY ; y++)
      {
          float bestFOM = bestFOMMap_BDT->GetBinContent(x-2,y-1);
          if (bestFOM == 0)
            ratio_newCC_newBDT->SetBinContent(x,y,0.0);
          else 
          {
            float ratio_ = bestFOMMap->GetBinContent(x,y)/bestFOM;
            if (ratio_ >= 2) ratio_ = 2;
            ratio_newCC_newBDT->SetBinContent(x,y,ratio_);
          }
      }
     */

      // #########################
      // ##   Save those maps   ##
      // #########################

      float lineOffset = 0.0;
      string label;
      if (signals[s] == "T2bw-025") { lineOffset = 320; label = "T2bw (x = 0.25);"; }
      if (signals[s] == "T2bw-050") { lineOffset = 160; label = "T2bw (x = 0.50);"; }
      if (signals[s] == "T2bw-075") { lineOffset = 105; label = "T2bw (x = 0.75);"; }

      TFile fOutput((string("../plots/cutAndCount_T2bw/custom")+signals[s]+".root").c_str(),"RECREATE");
      gStyle->SetPaintTextFormat("4.0f");
      formatAndWriteMapPlot(&screwdriver,bestSetMap,bestSetMap->GetName(),label+"Best set of cuts",true,lineOffset);
      formatAndWriteMapPlot(&screwdriver,nextBestSetMap,nextBestSetMap->GetName(),label+"Next-to-best;set of cuts",true,lineOffset);
      formatAndWriteMapPlot(&screwdriver,nextNextBestSetMap,nextNextBestSetMap->GetName(),label+"NextNext-Best;set of cuts",true,lineOffset);
      gStyle->SetPaintTextFormat("4.1f");
      for (unsigned int i = 0 ; i < cutAndCountRegions.size() ; i++)
      {
          FOMMaps[i]->SetMaximum(5.0);
          formatAndWriteMapPlot(&screwdriver,FOMMaps[i],FOMMaps[i]->GetName(),label+string("FOM for ")+cutAndCountRegions[i], true,lineOffset);
          formatAndWriteMapPlot(&screwdriver,efficiencies[i],efficiencies[i]->GetName(),label+string("Efficiencies for")+cutAndCountRegions[i], true,lineOffset);
      }
      bestFOMMap->SetMaximum(5.0);
      bestFOMMap_MTanalysis->SetMaximum(5.0);
      ratio_newCC_MTanalysisCC->SetMaximum(2.0);
      ratio_nextBest->SetMaximum(2.0);
      ratio_nextNextBest->SetMaximum(2.0);
      formatAndWriteMapPlot(&screwdriver,bestFOMMap,bestFOMMap->GetName(),label+"Best FOM",true,lineOffset);
      formatAndWriteMapPlot(&screwdriver,nextBestFOMMap,nextBestFOMMap->GetName(),label+"Next-to-best FOM",true,lineOffset);
      formatAndWriteMapPlot(&screwdriver,nextNextBestFOMMap,nextNextBestFOMMap->GetName(),label+"NextNext-to-best FOM",true,lineOffset);
      formatAndWriteMapPlot(&screwdriver,bestSigEff,bestSigEff->GetName(),label+"Best signal efficiency",true,lineOffset);
      formatAndWriteMapPlot(&screwdriver,bestBkgEff,bestBkgEff->GetName(),label+"Best backgr efficiency",true,lineOffset);
      formatAndWriteMapPlot(&screwdriver,bestFOMMap_MTanalysis,bestFOMMap_MTanalysis->GetName(),label+"Best FOM;from MT analysis",true,lineOffset);
      formatAndWriteMapPlot(&screwdriver,ratio_nextBest,ratio_nextBest->GetName(),label+"Best / nextBest",true,lineOffset);
      formatAndWriteMapPlot(&screwdriver,ratio_nextNextBest,ratio_nextNextBest->GetName(),label+"Best / nextNextBest",true,lineOffset);
      formatAndWriteMapPlot(&screwdriver,ratio_newCC_MTanalysisCC,ratio_newCC_MTanalysisCC->GetName(),label+"FOM gain;wrt MT analysis",true,lineOffset);
      //formatAndWriteMapPlot(&screwdriver,ratio_newCC_newBDT,ratio_newCC_newBDT->GetName(),"FOM gain wrt BDT",true,lineOffset);
      fOutput.Close();
      system((string("rm -r ../plots/cutAndCount_T2bw/pdf/custom_")+signals[s]).c_str());
      system((string("mv ../plots/cutAndCount_T2bw/pdf/custom ../plots/cutAndCount_T2bw/pdf/custom_")+signals[s]).c_str());
  }

  printBoxedMessage("Program done.");
  return (0);
}

void formatAndWriteMapPlot(SonicScrewdriver* screwdriver, TH2F* theHisto, string name, string comment, bool enableText, float lineOffset)
{
    Plot thePlot(name,"custom",screwdriver->GetGlobalOptions());
    thePlot.SetParameter("name",name);
    thePlot.AddToInPlotInfo(comment);

    thePlot.getCanvas()->SetRightMargin(0.1);
	theHisto->GetXaxis()->SetTitle("m_{#tilde{t}} [GeV]");
	theHisto->GetXaxis()->SetTitleFont(PLOTDEFAULTSTYLES_FONT+2);
	theHisto->GetXaxis()->SetTitleSize(0.05);
	theHisto->GetXaxis()->SetTitleOffset(0.9);
	theHisto->GetXaxis()->SetLabelFont(PLOTDEFAULTSTYLES_FONT+3);
	theHisto->GetXaxis()->SetLabelSize(22);
	theHisto->GetYaxis()->SetTitle("m_{#chi^{0}} [GeV]");
	theHisto->GetYaxis()->SetTitleFont(PLOTDEFAULTSTYLES_FONT+2);
	theHisto->GetYaxis()->SetTitleSize(0.05);
	theHisto->GetYaxis()->SetTitleOffset(0.9);
	theHisto->GetYaxis()->SetLabelFont(PLOTDEFAULTSTYLES_FONT+3);
	theHisto->GetYaxis()->SetLabelSize(22);
    theHisto->SetTitle("");
    theHisto->SetStats(0);

    if (!enableText) theHisto->Draw("COLZ");
    else             theHisto->Draw("COLZ TEXT");
    thePlot.Update();
    TPaletteAxis *pal = (TPaletteAxis*) theHisto->GetListOfFunctions()->FindObject("palette");
    if (pal != 0) 
    {
        pal->SetX1NDC(0.901);
        pal->SetY1NDC(0.1);
        pal->SetX2NDC(0.93);
        pal->SetY2NDC(1.0-thePlot.getCanvas()->GetTopMargin());
    }

    TLine* line1 = new TLine(lineOffset-12.5,-12.5,lineOffset+387.5,387.5);
    line1->SetLineWidth(2);
    line1->SetLineStyle(2);
    line1->Draw();
    thePlot.Write("../plots/cutAndCount_T2bw/","custom",screwdriver->GetGlobalOptions());
}



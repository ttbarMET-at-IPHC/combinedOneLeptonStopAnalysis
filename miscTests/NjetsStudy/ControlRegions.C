
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

#define FOLDER_BABYTUPLES "../store/babyTuples_0328/"
#include "Reader.h"
babyEvent* myEventPointer;
string* pCurrentDataset;
string* pCurrentDatasetType;

// #########################################################################
//                          Region selectors
// #########################################################################



bool singleElecChannelSelector() 
{ 
    if (myEventPointer->numberOfLepton != 1) return false;
    if ((*pCurrentDatasetType == "data") && (*pCurrentDataset != "SingleElec")) return false;
    return (abs(myEventPointer->leadingLeptonPDGId) == 11); 
}
bool singleMuonChannelSelector() 
{ 
    if (myEventPointer->numberOfLepton != 1) return false;
    if ((*pCurrentDatasetType == "data") && (*pCurrentDataset != "SingleMuon")) return false;
    return (abs(myEventPointer->leadingLeptonPDGId) == 13); 
}

bool doubleElecChannelSelector() 
{ 
    if (myEventPointer->numberOfLepton != 2) return false;
    if ((*pCurrentDatasetType == "data") && (*pCurrentDataset != "doubleElec")) return false;
    return ((abs(myEventPointer->leadingLeptonPDGId) == 11) 
         && (abs(myEventPointer->secondLeptonPDGId)  == 11)); 
}

bool doubleMuonChannelSelector() 
{ 
    if (myEventPointer->numberOfLepton != 2) return false;
    if ((*pCurrentDatasetType == "data") && (*pCurrentDataset != "doubleMuon")) return false;
    return ((abs(myEventPointer->leadingLeptonPDGId) == 13) 
         && (abs(myEventPointer->secondLeptonPDGId)  == 13)); 
}

bool muonElecChannelSelector() 
{ 
    if (myEventPointer->numberOfLepton != 2) return false;
    if ((*pCurrentDatasetType == "data") && (*pCurrentDataset != "MuEl")) return false;
    return   (((abs(myEventPointer->leadingLeptonPDGId) == 13) 
            && (abs(myEventPointer->secondLeptonPDGId)  == 11))
       ||     ((abs(myEventPointer->leadingLeptonPDGId) == 11) 
            && (abs(myEventPointer->secondLeptonPDGId)  == 13)));
}

bool singleLeptonChannelSelector() { return (singleElecChannelSelector() || singleMuonChannelSelector()); }
bool doubleLeptonChannelSelector() { return (doubleElecChannelSelector() 
                                          || doubleMuonChannelSelector()
                                          || muonElecChannelSelector()); }

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

bool Selector_CR1()
{
    babyEvent myEvent = *myEventPointer;
    // Require nLepton == 1
    if (myEvent.numberOfLepton != 1)                      return false;
    // Require nJets >= 4, nBTag == 0
    if ((myEvent.nJets <= 3) || (myEvent.nBTag != 0))     return false; 
    // Apply vetos
    if ((!myEvent.isolatedTrackVeto) || (!myEvent.tauVeto))return false;
    return true;
}

bool Selector_CR2()
{
    babyEvent myEvent = *myEventPointer;
    // Require nLepton == 2
    if (myEvent.numberOfLepton != 2)                      return false;
    // Z-mass window : missing
    // Require nJets >= 2, nBTag == 0
    if ((myEvent.nJets <= 1) || (myEvent.nBTag != 0))     return false; 
    // Apply vetos
    //if ((!myEvent.isolatedTrackVeto) || (!myEvent.tauVeto))return false;
    return true;
}

bool Selector_CR4()
{
    babyEvent myEvent = *myEventPointer;
    // Require nLepton == 2
    if (myEvent.numberOfLepton != 2)                      return false;
    // Z-mass veto : missing
    // Require nJets >= 2, nBTag >= 1
    if ((myEvent.nJets <= 1) || (myEvent.nBTag == 0))     return false; 
    // Apply vetos
    //if ((!myEvent.isolatedTrackVeto) || (!myEvent.tauVeto))return false;
    return true;
}


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
	screwdriver.AddVariable("Njets",            "N(Jets)",                     "nJets",    6,2,8,      &(myEvent.FnJets),               "");
	screwdriver.AddVariable("MET",            "MET",                     "GeV",    15,50,500,      &(myEvent.MET),                  "logY=true");
	screwdriver.AddVariable("METoverSqrtHT",  "MET / #sqrt{H_{T}}",      "",       32,0,32,        &(myEvent.METoverSqrtHT),        "");
	screwdriver.AddVariable("MT",             "MT",                      "GeV",    17,0,510,       &(myEvent.MT),                   "logY=true");
	screwdriver.AddVariable("MT2W",           "M_{T2}^{W}",              "GeV",    20,0,500,       &(myEvent.MT2W),                 "");
	screwdriver.AddVariable("deltaPhiMETJets","#Delta#Phi(MET,j_{1,2})", "rad",    16,0,3.2,       &(myEvent.deltaPhiMETJets),      "");
	screwdriver.AddVariable("HadronicChi2",   "Hadronic #chi^{2}",       "",       40,0,20,        &(myEvent.hadronicChi2),         "");
	//screwdriver.AddVariable("HT",             "H_{T}",                   "",       45,150,1500,    &(myEvent.HT),                   "");
	screwdriver.AddVariable("leadingBPt",     "p_{T}(leading b-jet)",    "GeV",    25,0,500,       &(myEvent.leadingBPt),           "");
	screwdriver.AddVariable("leadingJetPt",   "p_{T}(leading jet)",      "GeV",    25,0,500,       &(myEvent.leadingJetPt),         "");


     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     screwdriver.AddProcessClass("ttbar_1l", "t#bar{t} #rightarrow l+jets","background",kRed-7);
            screwdriver.AddDataset("ttbar_madgraph_1l",    "ttbar_1l",  0, 0);
     
     screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}",    "background",kCyan-3);
            screwdriver.AddDataset("ttbar_madgraph_2l",    "ttbar_2l",  0, 0);
     
     screwdriver.AddProcessClass("W+jets",   "W+jets",                     "background",kOrange-2);
             screwdriver.AddDataset("Wjets",    "W+jets", 0, 0);

     screwdriver.AddProcessClass("others",   "others",                     "background",kMagenta-5);
             screwdriver.AddDataset("others",   "others", 0, 0);
     
     screwdriver.AddProcessClass("data",   "data",                     "data",COLORPLOT_BLACK);
             screwdriver.AddDataset("SingleElec",   "data", 0, 0);
             screwdriver.AddDataset("SingleMuon",   "data", 0, 0);
             screwdriver.AddDataset("DoubleMuon",   "data", 0, 0);
             screwdriver.AddDataset("DoubleElec",   "data", 0, 0);
             screwdriver.AddDataset("MuEl",         "data", 0, 0);


  // ##########################
  // ##    Create Regions    ##
  // ##########################

     screwdriver.AddRegion("presel",             "Preselection",                 &Selector_presel);
     screwdriver.AddRegion("CR_1",   "CR1",&Selector_CR1);
     screwdriver.AddRegion("CR_2",   "CR2",&Selector_CR2);
     screwdriver.AddRegion("CR_4",   "CR4",&Selector_CR4);

  // ##########################
  // ##   Create Channels    ##
  // ##########################
   
     screwdriver.AddChannel("singleLepton", "e/#mu-channels",         &singleLeptonChannelSelector);
     screwdriver.AddChannel("singleElec",   "e-channel",              &singleElecChannelSelector  );
     screwdriver.AddChannel("singleMuon",   "#mu-channel",            &singleMuonChannelSelector  );
     
     screwdriver.AddChannel("doubleLepton", "ee/#mu#mu/e#mu-channels",&doubleLeptonChannelSelector);
     screwdriver.AddChannel("doubleElec",   "ee-channel",             &doubleElecChannelSelector  );
     screwdriver.AddChannel("doubleMuon",   "#mu#mu-channel",         &doubleMuonChannelSelector  );
     screwdriver.AddChannel("elecMuon",     "e#mu-channel",           &muonElecChannelSelector    );

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

  cout << "   > Reading datasets... " << endl;
  cout << endl;

  for (unsigned int d = 0 ; d < datasetsList.size() ; d++)
  {
     string currentDataset = datasetsList[d];
     string currentProcessClass = screwdriver.GetProcessClass(currentDataset);

     pCurrentDataset = &currentDataset;
     pCurrentDatasetType = &currentProcessClass;

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
      {
          if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

          //if (i > 0.1 * nEntries) break;

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

	  //type conversion for nJets
	  myEvent.FnJets = myEvent.nJets;

          float weight = 1.0;
          if (currentProcessClass != "data")
          {
              // For MC, apply weights
              float lumi;

                   if (singleElecChannelSelector())  lumi = 19250.0;
              else if (singleMuonChannelSelector())  lumi = 19190.0;
              else if (muonElecChannelSelector()  )  lumi = 19447.0;
              else if (doubleMuonChannelSelector())  lumi = 14690.0;
              else if (doubleElecChannelSelector())  lumi = 19316.0;
              else                                   lumi = 0.0;

              // Normalize to cross section times lumi
              weight *= myEvent.weightCrossSection * lumi;

              // Apply trigger efficiency weights for singleLepton channels
              if (myEvent.numberOfLepton != 1) weight *= myEvent.weightTriggerEfficiency;

              // Apply pile-up weight
              weight *= myEvent.weightPileUp;

              // For ttbar, apply topPt reweighting
              if ((currentProcessClass == "ttbar_1l") 
               || (currentProcessClass == "ttbar_2l")) weight *= myEvent.weightTopPt;
          }
          else
          {
              // For data, apply trigger requirements
              if ((currentDataset == "SingleMuon") && (!myEvent.triggerMuon) && (!myEvent.xtriggerMuon))   continue;
              if ((currentDataset == "SingleElec") && (!myEvent.triggerElec))                              continue;
              if ((currentDataset == "MuEl")       && (!myEvent.triggerMuonElec))                          continue;
              if ((currentDataset == "DoubleMuon") && (!myEvent.triggerDoubleMuon))                        continue;
              if ((currentDataset == "DoubleElec") && (!myEvent.triggerDoubleElec))                        continue;
          }

          screwdriver.AutoFillProcessClass(currentProcessClass,weight);

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
  screwdriver.WritePlots("../plots/cutAndCount_Njets/");

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################
  
  printBoxedMessage("Now computing misc tests ... ");

  printBoxedMessage("Program done.");
  return (0);
}


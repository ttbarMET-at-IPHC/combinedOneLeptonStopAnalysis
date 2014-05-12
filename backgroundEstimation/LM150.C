#include "../common.h"

#include <TEnv.h>
#include <TTreePerfStats.h>

// BabyTuple format and location

#define FOLDER_BABYTUPLES "../store/babyTuples_0328/"
//#define FOLDER_BABYTUPLES "../store/babyTuples_0328_preSelectionSkimmed/"
#include "Reader.h"
babyEvent* myEventPointer;
string* pCurrentDataset;
string* pCurrentDatasetType;

// #########################################################################
//                          Region selectors
// #########################################################################



bool singleElecChannelSelector() 
{ 
    // (Commenting this because Alex. G didn't have any explicit requirement on nLepton == 1)
    //if (myEventPointer->numberOfLepton != 1) return false;
    if ((*pCurrentDatasetType == "data") && (*pCurrentDataset != "SingleElec")) return false;
    return (abs(myEventPointer->leadingLeptonPDGId) == 11); 
}
bool singleMuonChannelSelector() 
{ 
    // (Commenting this because Alex. G didn't have any explicit requirement on nLepton == 1)
    //if (myEventPointer->numberOfLepton != 1) return false;
    if ((*pCurrentDatasetType == "data") && (*pCurrentDataset != "SingleMuon")) return false;
    return (abs(myEventPointer->leadingLeptonPDGId) == 13); 
}

bool singleLeptonChannelSelector() { return (singleElecChannelSelector() || singleMuonChannelSelector()); }

bool Selector_LM150() 
{
    babyEvent myEvent = *myEventPointer;

    // Temporary bug fix for broken eta cut on muons
    if (myEvent.leadingLepton.Eta() < -2.1) return false;
    
    // Temporary removing low pt muon for sync with Alex G.
    if ((abs(myEventPointer->leadingLeptonPDGId) == 13) && (myEvent.leadingLepton.Pt() < 25)) return false;
    // He also put a cut on the Eta of TLorentzVector of electron. 
    // The same cut is applied at babyTuple production, but using the eta of supercluster
    if ((abs(myEventPointer->leadingLeptonPDGId) == 11) && (abs(myEvent.leadingLepton.Eta()) > 1.4442)) return false;

    // (Commenting this because Alex. G didn't have any explicit requirement on nLepton == 1)
    // Require nLepton == 1
    //if (myEvent.numberOfLepton != 1)                        return false;

    // Require nJets >= 4, nBTag >= 1
    if ((myEvent.nJets <= 3) || (myEvent.nBTag == 0) )      return false; 

    // Apply vetos
    if ((!myEvent.isolatedTrackVeto) || (!myEvent.tauVeto)) return false;

    // Apply MET and MT cuts
    if ((myEvent.MET < 150)
     || (myEvent.MT < 120))                    return false;

    if (myEventPointer->deltaPhiMETJets < 0.8) return false;
    if (myEventPointer->hadronicChi2    > 5)   return false;

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

     screwdriver.AddVariable("MET",            "MET",                     "GeV",    15,50,500,      &(myEvent.MET),                  "logY");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     screwdriver.AddProcessClass("ttbar_1l", "t#bar{t} #rightarrow l+jets","background",kRed-7);
     screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}",    "background",kCyan-3);
            screwdriver.AddDataset("ttbar",    "ttbar_1l",  0, 0);
     
     screwdriver.AddProcessClass("W+jets",   "W+jets",                     "background",kOrange-2);
             screwdriver.AddDataset("Wjets",    "W+jets", 0, 0);

     screwdriver.AddProcessClass("others",   "others",                     "background",kMagenta-5);
             screwdriver.AddDataset("others",   "others", 0, 0);

     /*
     screwdriver.AddProcessClass("T2tt",     "T2tt",                       "signal",kViolet-1);
             screwdriver.AddDataset("T2tt",     "T2tt",   0, 0);

     screwdriver.AddProcessClass("signal_250_50",  "T2tt (250/50)",      "signal",COLORPLOT_AZURE);
     screwdriver.AddProcessClass("signal_450_50",  "T2tt (450/50)",      "signal",kCyan-3);
     screwdriver.AddProcessClass("signal_650_50",  "T2tt (650/50)",      "signal",COLORPLOT_GREEN);
     */


  // ##########################
  // ##    Create Regions    ##
  // ##########################

     screwdriver.AddRegion("LM150",      "LM 150",            &Selector_LM150);

  // ##########################
  // ##   Create Channels    ##
  // ##########################
   
     screwdriver.AddChannel("singleLepton", "e/#mu-channels",         &singleLeptonChannelSelector);
     screwdriver.AddChannel("singleElec",   "e-channel",              &singleElecChannelSelector  );
     screwdriver.AddChannel("singleMuon",   "#mu-channel",            &singleMuonChannelSelector  );
     
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

     // Config plots

     screwdriver.SetGlobalStringOption("Plot", "infoTopRight", "CMS Internal");
     screwdriver.SetGlobalStringOption("Plot", "infoTopLeft",  "#sqrt{s} = 8 TeV, L = 19.5 fb^{-1}");

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

     pCurrentDataset     = &currentDataset;
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

          //if (i > 0.03 * nEntries) break;

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          float weight = 1.0;
          // For MC, apply weights
          /*
          float lumi;

               if (singleElecChannelSelector())  lumi = 19154.0;
          else if (singleMuonChannelSelector())  lumi = 19096.0;
          else                                   lumi = 0.0;
          */
          float lumi = 19500;

          // Normalize to cross section times lumi
          weight *= myEvent.weightCrossSection * lumi;

          // Apply trigger efficiency weights for singleLepton channels
          if (myEvent.numberOfLepton == 1) weight *= myEvent.weightTriggerEfficiency;

          // Apply pile-up weight except for signal
          if (currentDataset != "T2tt") weight *= myEvent.weightPileUp;

          // For signal, apply ISR reweighting
          if (currentDataset == "T2tt")  weight *= myEvent.weightISRmodeling;

          // For ttbar, apply topPt reweighting
          //if (currentDataset == "ttbar") weight *= myEvent.weightTopPt;

          // Split 1-lepton ttbar and 2-lepton ttbar
          string currentProcessClass_ = currentProcessClass;
          if ((currentDataset == "ttbar") && (myEvent.numberOfGenLepton == 2)) 
              currentProcessClass_ = "ttbar_2l";

          screwdriver.AutoFillProcessClass(currentProcessClass_,weight);

          if ((myEvent.mStop == 250) && (myEvent.mNeutralino == 50))
              screwdriver.AutoFillProcessClass("signal_250_50",weight);
          if ((myEvent.mStop == 450) && (myEvent.mNeutralino == 50))
              screwdriver.AutoFillProcessClass("signal_450_50",weight);
          if ((myEvent.mStop == 650) && (myEvent.mNeutralino == 50))
              screwdriver.AutoFillProcessClass("signal_650_50",weight);

      } 
      printProgressBar(nEntries,nEntries,currentDataset);
      cout << endl;
      cout << "Reading " << f.GetBytesRead() << " bytes in " << f.GetReadCalls() << " transactions" << endl;
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


  // Print yield tables for the signal region LM150
  vector<string> tableRegions = { "LM150" };
  TableBackgroundSignal(&screwdriver,tableRegions,"singleLepton").PrintTable();

  printBoxedMessage("Program done.");
  return (0);
}


#include "../common.h"

// Sonic screwdriver headers

#include "interface/Table.h" 
#include "interface/SonicScrewdriver.h" 
#include "interface/tables/TableBackgroundSignal.h" 
#include "interface/tables/TableDataMC.h" 
using namespace theDoctor;

// BabyTuple format and location

//#define FOLDER_BABYTUPLES "../store/babyTuples_0603_1lepton4jetsMET80/"
//#define FOLDER_BABYTUPLES "../store/babyTuples_0603_withBDT/"
#define FOLDER_BABYTUPLES "../babySkimmer/babyTuple_BDT_0618_skimmed/"
//#include "Reader_newFinal0603_skimmed.h"
#include "Reader_newFinal0603_BDT_skimmed.h"

#include "analysisDefinitions.h"
#include "cutAndCountDefinitions.h"
//required for BDT signal region def
#include "../AN-14-067/signalRegionDefinitions.h"

#ifndef SIGNAL_REGION_CUTS
    #error SIGNAL_REGION_CUTS need to be defined.
#endif
#ifndef SIGNAL_REGION_TAG
    #error SIGNAL_REGION_TAG need to be defined.
#endif

bool LM150(bool applyMTCut)
{
    // Apply MET and MT cuts
    if (myEvent.MET < 150)            return false;

    if (myEvent.deltaPhiMETJets < 0.8) return false;
    if (myEvent.hadronicChi2    > 5)   return false;

    return true; 
}

bool enableMTCut = true;
bool disableMTCut = false;

bool goesInPreVetoSelectionMTpeak_withSRCuts()   { return (goesInPreVetoSelectionMTpeak()   && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInPreVetoSelectionMTtail_withSRCuts()   { return (goesInPreVetoSelectionMTtail()   && SIGNAL_REGION_CUTS(enableMTCut) ); }
                                                                      
bool goesInPreselectionMTpeak_withSRCuts()       { return (goesInPreselectionMTpeak()       && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInPreselectionMTtail_withSRCuts()       { return (goesInPreselectionMTtail()       && SIGNAL_REGION_CUTS(enableMTCut) ); }
bool goesInPreselectionMTtail_withSRCuts_2ndLep()       { return (goesInPreselectionMTtail() && SIGNAL_REGION_CUTS(enableMTCut) && myEvent.secondLeptonInAcceptance ); }
                                                                      
bool goesIn0BtagControlRegionMTpeak_withSRCuts() { return (goesIn0BtagControlRegionMTpeak() && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesIn0BtagControlRegionMTtail_withSRCuts() { return (goesIn0BtagControlRegionMTtail() && SIGNAL_REGION_CUTS(enableMTCut) ); }

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

     // ##########################
     // ##   Create Variables   ##
     // ##########################

     //screwdriver.AddVariable("MT",             "M_{T}",        "GeV",    40,0,400,       &(myEvent.MT),      "logY");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     screwdriver.AddProcessClass("1ltop", "1l top",                             "background",kRed-7);
            //screwdriver.AddDataset("ttbar_powheg",   "1ltop",  0, 0);
            //screwdriver.AddDataset("ttbar_madgraph_scaledown",   "1ltop",  0, 0);
            //screwdriver.AddDataset("ttbar_madgraph_scaleup",   "1ltop",  0, 0);
            //screwdriver.AddDataset("ttbar_madgraph_matchingdown",   "1ltop",  0, 0);
            //screwdriver.AddDataset("ttbar_madgraph_matchingup",   "1ltop",  0, 0);
            screwdriver.AddDataset("ttbar_madgraph_mass166-5",   "1ltop",  0, 0);
            //screwdriver.AddDataset("ttbar_madgraph_mass178-5",   "1ltop",  0, 0);

	    //screwdriver.AddDataset("ttbar_madgraph_1l",   "1ltop",  0, 0);
            screwdriver.AddDataset("singleTop_st",   "1ltop",  0, 0);
     
     screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",kCyan-3);
            //screwdriver.AddDataset("ttbar_madgraph_2l",   "ttbar_2l",  0, 0);
     
     screwdriver.AddProcessClass("W+jets",   "W+jets",                          "background",kOrange-2);
             screwdriver.AddDataset("W+jets",    "W+jets", 0, 0);

     screwdriver.AddProcessClass("rare",   "rare",                              "background",kMagenta-5);
             screwdriver.AddDataset("rare",   "rare", 0, 0);
     
     screwdriver.AddProcessClass("data",   "data",                              "data",COLORPLOT_BLACK);
             screwdriver.AddDataset("SingleElec",   "data", 0, 0);
             screwdriver.AddDataset("SingleMuon",   "data", 0, 0);

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     screwdriver.AddRegion("preveto_MTpeak",          "Preselection (MT peak)",           &goesInPreVetoSelectionMTpeak_withSRCuts);
     screwdriver.AddRegion("preveto_MTtail",          "Preselection (MT tail)",           &goesInPreVetoSelectionMTtail_withSRCuts);

     screwdriver.AddRegion("signalRegion_MTpeak",     "Preselection (MT peak)",           &goesInPreselectionMTpeak_withSRCuts);
     screwdriver.AddRegion("signalRegion_MTtail",     "Preselection (MT tail)",           &goesInPreselectionMTtail_withSRCuts, "blinded");
     screwdriver.AddRegion("signalRegion_MTtail_secondLeptonInAcceptance",     "Preselection (MT tail) + 2nd lept",           &goesInPreselectionMTtail_withSRCuts_2ndLep, "blinded");

     screwdriver.AddRegion("0btag_MTpeak",            "0 b-tag (MT peak)",                &goesIn0BtagControlRegionMTpeak_withSRCuts);
     screwdriver.AddRegion("0btag_MTtail",            "0 b-tag (MT tail)",                &goesIn0BtagControlRegionMTtail_withSRCuts);

     // ##########################
     // ##   Create Channels    ##
     // ##########################
   
     screwdriver.AddChannel("singleLepton", "e/#mu-channels",         &goesInSingleLeptonChannel);
     screwdriver.AddChannel("singleElec",   "e-channel",              &goesInSingleElecChannel  );
     screwdriver.AddChannel("singleMuon",   "#mu-channel",            &goesInSingleMuonChannel  );
     
  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     // Create histograms
     screwdriver.Create1DHistos();

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

     sampleName = currentDataset;
     sampleType = screwdriver.GetProcessClassType(currentProcessClass);
    
     // Open the tree
     TFile f((string(FOLDER_BABYTUPLES)+currentDataset+".root").c_str());
     TTree* theTree = (TTree*) f.Get("babyTuple"); 
     
     intermediatePointers pointers;
     InitializeBranchesForReading(theTree,&myEvent,&pointers);

  // ########################################
  // ##        Run over the events         ##
  // ########################################

     bool ttbarDatasetToBeSplitted = false;
     if (currentDataset.find("ttbar")!=std::string::npos && currentDataset.find("ttbar_madgraph_1l")==std::string::npos && currentDataset.find("ttbar_madgraph_2l")==std::string::npos)
     	ttbarDatasetToBeSplitted = true;

      int nEntries = theTree->GetEntries();
      for (int i = 0 ; i < nEntries ; i++)
      {
          if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          float weight = getWeight();

          // Split 1-lepton ttbar and 2-lepton ttbar
          string currentProcessClass_ = currentProcessClass;
          //if ((currentDataset == "ttbar_powheg") && (myEvent.numberOfGenLepton == 2)) 
          if (ttbarDatasetToBeSplitted && (myEvent.numberOfGenLepton == 2)) 
              currentProcessClass_ = "ttbar_2l";

          //store info about the presence of a second lepton in the event 
	  // already available in new prod ...
	  //myEvent.secondLeptonInAcceptance = IsMultilepEvtWithTrueTrkFromTau();
	  
	  screwdriver.AutoFillProcessClass(currentProcessClass_,weight);
      } 
      printProgressBar(nEntries,nEntries,currentDataset);
      cout << endl;
      f.Close();

  }

  printBoxedMessage("Now computing misc tests ... ");

  vector<string> regions = { "preveto_MTpeak",      "preveto_MTtail",      
                             "signalRegion_MTpeak", "signalRegion_MTtail", 
                             "0btag_MTpeak",        "0btag_MTtail",  "signalRegion_MTtail_secondLeptonInAcceptance"      };
  
  string exportFile = "rawYieldTables/"+string(SIGNAL_REGION_TAG)+".tab";
  TableDataMC(&screwdriver,regions,"singleLepton").Print(exportFile,4);
  
  printBoxedMessage("Table generation completed");
  
  return (0);
}


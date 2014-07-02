#include "../common.h"

// Sonic screwdriver headers

#include "interface/Table.h" 
#include "interface/SonicScrewdriver.h" 
#include "interface/tables/TableBackgroundSignal.h" 
#include "interface/tables/TableDataMC.h" 
using namespace theDoctor;

// BabyTuple format and location

#define FOLDER_BABYTUPLES "/opt/sbg/data/safe1/cms/echabert/StopAlex/combinedOneLeptonStopAnalysis/babySkimmer/babyTuple_BDT_0618_skimmed_Caro/"
#define FOLDER_BABYTUPLES2 "/opt/sbg/data/safe1/cms/echabert/StopAlex/combinedOneLeptonStopAnalysis/babySkimmer/babyTuple_BDT_CR45_merged/"
#include "Reader_newFinal0603_BDT_skimmed_caro.h"

//#define FOLDER_BABYTUPLES "../store/babyTuples_0603_1lepton4jetsMET80/"
//#define FOLDER_BABYTUPLES "../store/babyTuples_0603_withBDT/"
//#include "Reader_newFinal0603_skimmed.h"

// WAS USED BEFORE CARO's MODIFICATION
//#define FOLDER_BABYTUPLES "../babySkimmer/babyTuple_BDT_0618_skimmed/"
//#include "Reader_newFinal0603_BDT_skimmed.h"

#include "common.h" // to get CR45 defined
#include "analysisDefinitions.h"
#include "cutAndCountDefinitions.h"

//required for BDT signal region def
//#include "../AN-14-067/signalRegionDefinitions.h"
#include "BDTDefinition.h"


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

bool goesIn2LControlRegionMTpeak_withSRCuts() { return (goesIn2LControlRegionMTpeak() && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesIn2LControlRegionMTtail_withSRCuts() { return (goesIn2LControlRegionMTtail() && SIGNAL_REGION_CUTS(enableMTCut) ); }
bool goesIn2LControlRegionMTinv_withSRCuts() { return (goesIn2LControlRegionMTinverted() && SIGNAL_REGION_CUTS(disableMTCut)); }

bool goesInVetoControlRegionMTpeak_withSRCuts() { return (goesInVetoControlRegionMTpeak() && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInVetoControlRegionMTtail_withSRCuts() { return (goesInVetoControlRegionMTtail() && SIGNAL_REGION_CUTS(enableMTCut) ); }
bool goesInVetoControlRegionMTinv_withSRCuts() { return (goesInVetoControlRegionMTinverted() && SIGNAL_REGION_CUTS(disableMTCut)); }


bool goesIn2LControlRegionN2_withSRCuts() { return(goesIn2LControlRegionN2() && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesIn2LControlRegionN3_withSRCuts() { return(goesIn2LControlRegionN3() && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesIn2LControlRegionN4_withSRCuts() { return(goesIn2LControlRegionN4() && SIGNAL_REGION_CUTS(disableMTCut)); }

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
            //screwdriver.AddDataset("ttbar_madgraph_mass166-5",   "1ltop",  0, 0);
            //screwdriver.AddDataset("ttbar_madgraph_mass178-5",   "1ltop",  0, 0);
	    screwdriver.AddDataset("ttbar_madgraph_1l",   "1ltop",  0, 0);
            screwdriver.AddDataset("singleTop_st",   "1ltop",  0, 0);

     
     screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",kCyan-3);
            screwdriver.AddDataset("ttbar_madgraph_2l",   "ttbar_2l",  0, 0);
     
     screwdriver.AddProcessClass("W+jets",   "W+jets",                          "background",kOrange-2);
             screwdriver.AddDataset("W+jets",    "W+jets", 0, 0);

     screwdriver.AddProcessClass("rare",   "rare",                              "background",kMagenta-5);
             screwdriver.AddDataset("rare",   "rare", 0, 0);
     
     screwdriver.AddProcessClass("data",   "data",                              "data",COLORPLOT_BLACK);
             screwdriver.AddDataset("SingleElec",   "data", 0, 0);
             screwdriver.AddDataset("SingleMuon",   "data", 0, 0);

             screwdriver.AddDataset("DoubleMuon",   "data", 0, 0);
             screwdriver.AddDataset("MuEl",   "data", 0, 0);
     }



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

     if (CR45) {
     // add 2lepton and 1lepton+antiveto
     screwdriver.AddRegion("2leptons_MTinv",      "2 leptons (MT inv)", &goesIn2LControlRegionMTinv_withSRCuts);
     screwdriver.AddRegion("2leptons_MTpeak",      "2 leptons (MT peak)", &goesIn2LControlRegionMTpeak_withSRCuts);
     screwdriver.AddRegion("2leptons_MTtail",      "2 leptons (MT tail)", &goesIn2LControlRegionMTtail_withSRCuts);

     screwdriver.AddRegion("antiveto_MTinv",      "anti-veto (MT inv)", &goesInVetoControlRegionMTinv_withSRCuts);
     screwdriver.AddRegion("antiveto_MTpeak",      "anti-veto (MT peak)", &goesInVetoControlRegionMTpeak_withSRCuts);
     screwdriver.AddRegion("antiveto_MTtail",      "anti-veto (MT tail)", &goesInVetoControlRegionMTtail_withSRCuts);

     screwdriver.AddRegion("2leptons_N2",      "2 leptons (Njet=1 or 2)", &goesIn2LControlRegionN2_withSRCuts);
     screwdriver.AddRegion("2leptons_N3",      "2 leptons (Njet=3)", &goesIn2LControlRegionN3_withSRCuts);
     screwdriver.AddRegion("2leptons_N4",      "2 leptons (Njet=4 or +)", &goesIn2LControlRegionN4_withSRCuts);
     }

     // ##########################
     // ##   Create Channels    ##
     // ##########################
   
     screwdriver.AddChannel("singleLepton", "e/#mu-channels",         &goesInSingleLeptonChannel);
     screwdriver.AddChannel("singleElec",   "e-channel",              &goesInSingleElecChannel  );
     screwdriver.AddChannel("singleMuon",   "#mu-channel",            &goesInSingleMuonChannel  );
     if (CR45) {
     screwdriver.AddChannel("doubleLepton",   "2l-channel",              &goesInDoubleLeptonChannel  );
     screwdriver.AddChannel("doubleElec",   "ee-channel",              &goesInDoubleElecChannel  );
     screwdriver.AddChannel("doubleMuon",   "#mu#mu-channel",          &goesInDoubleMuonChannel  );
     screwdriver.AddChannel("emu",   "e#mu-channel",            &goesInMuonElecChannel  );
     }

     
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
     string namefile;
     if (CR45) {
       namefile=string(FOLDER_BABYTUPLES2)+currentDataset+".root";
     }
     else {
       namefile=string(FOLDER_BABYTUPLES)+currentDataset+".root";
     }
     TFile f(namefile.c_str());
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

	  if(CR4_2j) if(myEvent.nJets<2) continue; 
	  if(CR4_3j) if(myEvent.nJets<3) continue; 
	  if(CR4_4j || CR4_4j_50evts || CR4_4j_100evts || CR4_4j_150evts) if(myEvent.nJets<4) continue; 

          float weight = getWeight();

          // Split 1-lepton ttbar and 2-lepton ttbar
          string currentProcessClass_ = currentProcessClass;
          //if ((currentDataset == "ttbar_powheg") && (myEvent.numberOfGenLepton == 2)) 
          //    currentProcessClass_ = "ttbar_2l";
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

  vector<string> regions1 = { "preveto_MTpeak",      "preveto_MTtail",      
                             "signalRegion_MTpeak", "signalRegion_MTtail", 
                             "0btag_MTpeak",        "0btag_MTtail",  "signalRegion_MTtail_secondLeptonInAcceptance"      };
  
  vector<string> regions2 = { "preveto_MTpeak",      "preveto_MTtail",      
                             "signalRegion_MTpeak", "signalRegion_MTtail", 
                             "0btag_MTpeak",        "0btag_MTtail",        
                             "antiveto_MTinv", "antiveto_MTpeak",     "antiveto_MTtail"};
  vector<string> regions3 = { "2leptons_MTinv",  "2leptons_MTpeak",     "2leptons_MTtail",
                              "2leptons_N2","2leptons_N3","2leptons_N4"};
  
  vector<string> regions;
  if (CR45) {
    regions=regions2;
  }
  else {
    regions=regions1;
  }
  string exportFile = "rawYieldTables/"+string(SIGNAL_REGION_TAG)+".tab";
  TableDataMC(&screwdriver,regions,"singleLepton").Print(exportFile,4);
  if (CR45) {
   string exportFile2 = "rawYieldTables/"+string(SIGNAL_REGION_TAG)+"_2leptons.tab";
   TableDataMC(&screwdriver,regions3,"doubleLepton").Print(exportFile2,4);
  }
  
  printBoxedMessage("Table generation completed");
  
  return (0);
}


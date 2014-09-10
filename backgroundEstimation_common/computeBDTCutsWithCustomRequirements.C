#include "common.h"
#include "quantiles.h"

#ifndef SIGNAL_REGION_CUTS
    #error SIGNAL_REGION_CUTS need to be defined.
#endif
#ifndef SIGNAL_REGION_TAG
    #error SIGNAL_REGION_TAG need to be defined.
#endif

bool goesInPreVetoSelectionMTpeak_withSRCuts()   { return (goesInPreVetoSelectionMTpeak()   && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInPreVetoSelectionMTtail_withSRCuts()   { return (goesInPreVetoSelectionMTtail()   && SIGNAL_REGION_CUTS(enableMTCut) ); }

bool goesInPreselectionMTpeak_withSRCuts()       { return (goesInPreselectionMTpeak()       && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInPreselectionMTtail_withSRCuts()       { return (goesInPreselectionMTtail()       && SIGNAL_REGION_CUTS(enableMTCut) ); }

bool goesIn0BtagControlRegionMTpeak_withSRCuts() { return (goesIn0BtagControlRegionMTpeak() && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesIn0BtagControlRegionMTtail_withSRCuts() { return (goesIn0BtagControlRegionMTtail() && SIGNAL_REGION_CUTS(enableMTCut) ); }
bool goesIn0BtagControlRegionNoMT_withSRCuts()   { return (goesIn0BtagControlRegion()       && SIGNAL_REGION_CUTS(disableMTCut) ); }

bool goesInDileptonControlRegionMTpeak_withSRCuts() { return (goesInDileptonControlRegionMTpeak() && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInDileptonControlRegionMTtail_withSRCuts() { return (goesInDileptonControlRegionMTtail() && SIGNAL_REGION_CUTS(enableMTCut) ); }
bool goesInDileptonControlRegionNoMT_withSRCuts()   { return (goesInDileptonControlRegion()       && SIGNAL_REGION_CUTS(disableMTCut)); }

bool goesInVetoControlRegionMTpeak_withSRCuts() { return (goesInVetoControlRegionMTpeak() && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInVetoControlRegionMTtail_withSRCuts() { return (goesInVetoControlRegionMTtail() && SIGNAL_REGION_CUTS(enableMTCut) ); }
bool goesInVetoControlRegionNoMT_withSRCuts()   { return (goesInVetosControlRegion()      && SIGNAL_REGION_CUTS(disableMTCut)); }

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

     #ifdef BDT_OUTPUT_AVAILABLE
         screwdriver.AddVariable("BDT_T2tt-1",      "BDT output T2tt-1",     "",   100,-0.5,0.5, &(myEvent.BDT_output_t2tt_R1   ));
         screwdriver.AddVariable("BDT_T2tt-2",      "BDT output T2tt-2",     "",   100,-0.5,0.5, &(myEvent.BDT_output_t2tt_R2   ));
         screwdriver.AddVariable("BDT_T2tt-5",      "BDT output T2tt-5",     "",   100,-0.5,0.5, &(myEvent.BDT_output_t2tt_R5   ));
         screwdriver.AddVariable("BDT_T2bw075-1",   "BDT output T2bw075-1",  "",   100,-0.5,0.5, &(myEvent.BDT_output_t2bw075_R1));
         screwdriver.AddVariable("BDT_T2bw075-2",   "BDT output T2bw075-2",  "",   100,-0.5,0.5, &(myEvent.BDT_output_t2bw075_R2));
         screwdriver.AddVariable("BDT_T2bw075-3",   "BDT output T2bw075-3",  "",   100,-0.5,0.5, &(myEvent.BDT_output_t2bw075_R3));
         screwdriver.AddVariable("BDT_T2bw075-5",   "BDT output T2bw075-5",  "",   100,-0.5,0.5, &(myEvent.BDT_output_t2bw075_R5));
         screwdriver.AddVariable("BDT_T2bw050-1",   "BDT output T2bw050-1",  "",   100,-0.5,0.5, &(myEvent.BDT_output_t2bw050_R1));
         screwdriver.AddVariable("BDT_T2bw050-3",   "BDT output T2bw050-3",  "",   100,-0.5,0.5, &(myEvent.BDT_output_t2bw050_R3));
         screwdriver.AddVariable("BDT_T2bw050-4",   "BDT output T2bw050-4",  "",   100,-0.5,0.5, &(myEvent.BDT_output_t2bw050_R4));
         screwdriver.AddVariable("BDT_T2bw050-5",   "BDT output T2bw050-5",  "",   100,-0.5,0.5, &(myEvent.BDT_output_t2bw050_R5));
         screwdriver.AddVariable("BDT_T2bw050-6",   "BDT output T2bw050-6",  "",   100,-0.5,0.5, &(myEvent.BDT_output_t2bw050_R6));
         screwdriver.AddVariable("BDT_T2bw025-1",   "BDT output T2bw025-1",  "",   100,-0.5,0.5, &(myEvent.BDT_output_t2bw025_R1));
         screwdriver.AddVariable("BDT_T2bw025-3",   "BDT output T2bw025-3",  "",   100,-0.5,0.5, &(myEvent.BDT_output_t2bw025_R3));
         screwdriver.AddVariable("BDT_T2bw025-4",   "BDT output T2bw025-4",  "",   100,-0.5,0.5, &(myEvent.BDT_output_t2bw025_R4));
         screwdriver.AddVariable("BDT_T2bw025-6",   "BDT output T2bw025-6",  "",   100,-0.5,0.5, &(myEvent.BDT_output_t2bw025_R6));
     #endif

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     screwdriver.AddProcessClass("1ltop", "1l top",                             "background",kRed-7);
            screwdriver.AddDataset("ttbar_powheg",   "1ltop",  0, 0);
            screwdriver.AddDataset("singleTop_st",   "1ltop",  0, 0);

     screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",kCyan-3);

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

     screwdriver.AddRegion("0btag",          ";0 b-tag Control Region",             &goesIn0BtagControlRegion          );
     screwdriver.AddRegion("2leptonsMTtail",";2 leptons  (no MT tail)",              &goesInDileptonControlRegionMTtail );
     screwdriver.AddRegion("antiveto", "reversed 2nd lepton veto (no MT cut)",    &goesInVetosControlRegion          );

     // ##########################
     // ##   Create Channels    ##
     // ##########################

     screwdriver.AddChannel("singleLepton", "e/#mu-channels",   &goesInSingleLeptonChannel);
     screwdriver.AddChannel("doubleLepton", "2l-channel",       &goesInDoubleLeptonChannel);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     // Create histograms
     screwdriver.Create1DHistos();

     // Schedule plots
     screwdriver.SchedulePlots("1DDataMCComparison");

     // Config plots

     screwdriver.SetGlobalStringOption("Plot", "infoTopRight", "CMS Internal");
     screwdriver.SetGlobalStringOption("Plot", "infoTopLeft",  "#sqrt{s} = 8 TeV, L = 19.5 fb^{-1}");

     screwdriver.SetGlobalBoolOption("Plot", "exportPdf", false);
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

      int nEntries = theTree->GetEntries();
      for (int i = 0 ; i < nEntries ; i++)
      {
          if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          float weight = getWeight();

          // Split 1-lepton ttbar and 2-lepton ttbar
          string currentProcessClass_ = currentProcessClass;
          if ((currentDataset == "ttbar_powheg") && (myEvent.numberOfGenLepton == 2))
              currentProcessClass_ = "ttbar_2l";

          if (myEvent.nJets < 2) continue;
          //if (myEvent.nJets < 3) continue;
          //if (myEvent.nJets < 4) continue;


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

  // ###################################
  // ##   Quantiles of all BDTS       ##
  // ###################################

  vector<string> BDTTrainings =
  {
      "BDT_T2tt-1",
      "BDT_T2tt-2",
      "BDT_T2tt-5",
      "BDT_T2bw075-1",
      "BDT_T2bw075-2",
      "BDT_T2bw075-3",
      "BDT_T2bw075-5",
      "BDT_T2bw050-1",
      "BDT_T2bw050-3",
      "BDT_T2bw050-4",
      "BDT_T2bw050-5",
      "BDT_T2bw050-6",
      "BDT_T2bw025-1",
      "BDT_T2bw025-3",
      "BDT_T2bw025-4",
      "BDT_T2bw025-6"
  };
  vector<string> processClasses = {"1ltop","ttbar_2l","W+jets","rare"};


  vector<string> numberOfEvents = {"30","50","100","150","200"};
  vector<int> MinNofEvts = {30,50,100,150,200};

  string output_prefix = "BDTCuts_";

  //CR4 BDT cuts
  Table tableCR4(numberOfEvents, BDTTrainings);
  string channel = "doubleLepton";
  string region = "2leptonsMTtail";
  for(unsigned int i=0;i<BDTTrainings.size();i++)
  for(unsigned int j=0;j<MinNofEvts.size();j++)
  {
      Figure bdtCut(MinNofEvtsVariabelCut(screwdriver, MinNofEvts[j], BDTTrainings[i], processClasses, region, channel),0);
      tableCR4.Set(numberOfEvents[j],BDTTrainings[i],bdtCut);
  }
  tableCR4.Print(output_prefix+"CR4.dat",3,"noError");

  //CR5 BDT cuts
  Table tableCR5(numberOfEvents, BDTTrainings);
  region = "antiveto";
  for(unsigned int i=0;i<BDTTrainings.size();i++)
  for(unsigned int j=0;j<MinNofEvts.size();j++)
  {
      Figure bdtCut(MinNofEvtsVariabelCut(screwdriver, MinNofEvts[j], BDTTrainings[i], processClasses, region, channel),0);
      tableCR5.Set(numberOfEvents[j],BDTTrainings[i],bdtCut);
  }
  tableCR5.Print(output_prefix+"CR5.dat",3,"noError");

  //CR1 -  BDT cuts
  numberOfEvents.clear(); numberOfEvents = {"value"};
  Table tableCR1(numberOfEvents, BDTTrainings);
  channel = "singleLepton";
  region = "0btag";
  float quantile = 0.75;//0.90
  for(unsigned int i=0;i<BDTTrainings.size();i++)
  {
          Figure bdtCut(QuantileVariabelCut(screwdriver, quantile, BDTTrainings[i], processClasses, region, channel),0);
          tableCR1.Set("value",BDTTrainings[i],bdtCut);
  }
  tableCR1.Print(output_prefix+"CR1.dat",3,"noError");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################

  printBoxedMessage("Program done.");
  return (0);
}


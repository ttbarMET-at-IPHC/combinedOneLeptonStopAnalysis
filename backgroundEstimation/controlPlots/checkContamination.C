#include "../common/common.h"

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

     float binning[3] = {0,100,400};
     screwdriver.AddVariable("MT",      "M_{T}",   "GeV",    40,0,400,  &(myEvent.MT),  "logY");
     screwdriver.AddVariable("MET",     "MET",     "GeV",    40,0,400,  &(myEvent.MET), "logY");
     screwdriver.AddVariable("Mlb",    "M'(lb)",  "GeV",    26,0,520,  &(myEvent.Mlb),    "");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

        screwdriver.AddProcessClass("1ltop", "1l top",                             "background",kRed-7);
                screwdriver.AddDataset("ttbar_powheg",                "1ltop",  0, 0);
                screwdriver.AddDataset("singleTop_st",                    "1ltop",  0, 0);

     screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",kCyan-3);

     screwdriver.AddProcessClass("W+jets",   "W+jets",                          "background",kOrange-2);
             screwdriver.AddDataset("W+jets",    "W+jets", 0, 0);

     screwdriver.AddProcessClass("rare",   "rare",                              "background",kMagenta-5);
             screwdriver.AddDataset("rare",   "rare", 0, 0);

     screwdriver.AddProcessClass("data",   "data",                              "data",COLORPLOT_BLACK);
             screwdriver.AddDataset("SingleElec",   "data", 0, 0);
             screwdriver.AddDataset("SingleMuon",   "data", 0, 0);

     screwdriver.AddProcessClass("T2tt_175_25", "T2tt (175/25)", "signal", COLORPLOT_GREEN );
            screwdriver.AddDataset("T2tt_175_25",   "T2tt_175_25",   0, 0);

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     screwdriver.AddRegion("preselection",          "Preselection;no M_{T} cut",                            &goesInPreselection);
     screwdriver.AddRegion("preselection_MTtail",   "Preselection;M_{T} > 100 GeV",                         &goesInPreselectionMTtail);
     screwdriver.AddRegion("0tag",                  "Preselection;0 b-tag Control Region, no M_{T} cut",    &goesIn0BtagControlRegion);
     screwdriver.AddRegion("0tag_MTtail",           "Preselection;0 b-tag Control Region, M_{T} > 100 GeV", &goesIn0BtagControlRegionMTtail);

     // ##########################
     // ##   Create Channels    ##
     // ##########################

     screwdriver.AddChannel("singleLepton", "e/#mu-channels",   &goesInSingleLeptonChannel);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     // Create histograms
     screwdriver.Create1DHistos();

     // Schedule plots
     screwdriver.SchedulePlots("1DDataMCComparison");

     // Config plots

     screwdriver.SetGlobalStringOption("DataMCComparison",  "includeSignal",                    "stack");
     screwdriver.SetGlobalFloatOption ("DataMCComparison",  "factorSignal",                     1.0    );
     screwdriver.SetGlobalBoolOption  ("DataMCRatio",       "includeSignal",                    true   );
     screwdriver.SetGlobalFloatOption ("DataMCRatio",       "min",                              0.8    );
     screwdriver.SetGlobalFloatOption ("DataMCRatio",       "max",                              1.5    );

     screwdriver.SetGlobalStringOption("Plot", "infoTopRight", "CMS Preliminary");
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

     sampleName = currentDataset;
     sampleType = screwdriver.GetProcessClassType(currentProcessClass);

     // Open the tree
     TFile f((string(FOLDER_BABYTUPLES)+currentDataset+".root").c_str());
     TTree* theTree = (TTree*) f.Get("babyTuple");

     intermediatePointers pointers;
     InitializeBranchesForReading(theTree,&myEvent,&pointers);

     if (sampleType == "signal")
     {
        theTree->SetBranchAddress("mStop",       &(myEvent.mStop));
        theTree->SetBranchAddress("mNeutralino", &(myEvent.mNeutralino));
     }
     else
     {
         myEvent.mStop       = -1;
         myEvent.mNeutralino = -1;
     }
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

  system("mkdir -p ./plots/");
  screwdriver.WritePlots("./plots/");

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################

  printBoxedMessage("Program done.");
  return (0);
}


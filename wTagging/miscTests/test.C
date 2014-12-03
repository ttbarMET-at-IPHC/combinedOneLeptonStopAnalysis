#include "../common.h"

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
     SonicScrewdriver s;

     // ##########################
     // ##   Create Variables   ##
     // ##########################

     s.AddVariable("MET",             "MET",                   "GeV",        16,50,530,      &(myEvent.MET),                   "logY");
     s.AddVariable("MT",              "M_{T}",                 "GeV",        20,0,400,       &(myEvent.MT),                    "logY");
     s.AddVariable("MT2W",            "M_{T2}^{W}",            "GeV",        25,0,500,       &(myEvent.MT2W),                  "");

     int nWTag = -1;
     s.AddVariable("nWTag",           "# of W-tagged jets",    "",           2,0,1,          &nWTag,                           "");
     s.AddVariable("leadingWjetPt",   "p_{T}(W-tagged jet)",   "GeV",        10,150,400,     &(myEvent.leadingWjetPt),         "noUnderflowInFirstBin");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     s.AddProcessClass("1ltop", "1l top",                             "background", kRed-7);
            s.AddDataset("ttbar_powheg",                "1ltop",  0, 0);
            s.AddDataset("singleTop_st",                "1ltop",  0, 0);

     s.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background", kCyan-3);

     s.AddProcessClass("W+jets",   "W+jets",                          "background", kOrange-2);
             s.AddDataset("W+jets",    "W+jets", 0, 0);

     s.AddProcessClass("rare",   "rare",                              "background", kMagenta-5);
             s.AddDataset("rare",   "rare", 0, 0);

     s.AddProcessClass("T2tt_1",   "T2tt (400/50)",                     "signal",     kSpring-1);
             s.AddDataset("T2tt_400_50",   "T2tt_1", 0, 0);

     s.AddProcessClass("T2tt_2",   "T2tt (700/50)",                     "signal",     kMagenta-5);
             s.AddDataset("T2tt_700_50",   "T2tt_2", 0, 0);

/*
     s.AddProcessClass("data",   "data",                              "data",COLORPLOT_BLACK);
            s.AddDataset("SingleElec",   "data", 0, 0);
            s.AddDataset("SingleMuon",   "data", 0, 0);
            s.AddDataset("DoubleElec",   "data", 0, 0);
            s.AddDataset("DoubleMuon",   "data", 0, 0);
            s.AddDataset("MuEl",         "data", 0, 0);
*/

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     // Preselection
     s.AddRegion("preselection",                "Preselection",                            &goesInPreselection       );

     // MT peak and MT tail inherited from preselection
     vector<Cut> MTpeakCuts = { Cut("MT", '>', 50),
                                Cut("MT", '<', 80) };

     s.AddRegion("preselMTpeak",                "Preselection;M_{T} peak Control Region",  "preselection",  MTpeakCuts);

     vector<Cut> MTtailCuts = { Cut("MT",'>',100) };

     s.AddRegion("preselMTtail",                "Preselection;M_{T} tail",                 "preselection",  MTtailCuts);

     // Example of high deltaM signal region inherited from MT tail
     vector<Cut> signalRegionCuts = { Cut("MT",  '>', 130),
                                      Cut("MET", '>', 150),
                                      Cut("MT2W",'>', 170) };

     s.AddRegion("highDeltaMSignalRegion",      "High #Delta m signal region",             "preselMTtail", signalRegionCuts);

     // ##########################
     // ##   Create Channels    ##
     // ##########################

     s.AddChannel("singleLepton", "e/#mu-channels",            &goesInSingleLeptonChannel);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     // Create histograms
     s.Create1DHistos();

     // Schedule plots
     s.SchedulePlots("1DSuperimposed");
     s.SchedulePlots("1DStack");
     //s.SchedulePlots("1DDataMCComparison");

     // Config plots

     s.SetGlobalBoolOption  ("1DSuperimposed",    "includeSignal",          true   );
     s.SetGlobalStringOption("1DStack",           "includeSignal",          "stack");
     s.SetGlobalFloatOption ("1DStack",           "factorSignal",           1.0    );
     s.SetGlobalStringOption("DataMCComparison",  "includeSignal",          "stack");
     s.SetGlobalFloatOption ("DataMCComparison",  "factorSignal",           1.0    );

     s.SetGlobalStringOption("Plot", "infoTopRight", "CMS Work in progress");
     s.SetGlobalStringOption("Plot", "infoTopLeft",  "#sqrt{s} = 8 TeV, L = 19.5 fb^{-1}");

     s.SetGlobalBoolOption("Plot", "exportPdf", true);
     s.SetGlobalBoolOption("Plot", "exportEps", false);
     s.SetGlobalBoolOption("Plot", "exportPng", false);

  // ########################################
  // ##       Run over the datasets        ##
  // ########################################

  vector<string> datasetsList;
  s.GetDatasetList(&datasetsList);

  cout << "   > Reading datasets... " << endl;
  cout << endl;

  for (unsigned int d = 0 ; d < datasetsList.size() ; d++)
  {
     string currentDataset = datasetsList[d];
     string currentProcessClass = s.GetProcessClass(currentDataset);

     sampleName = currentDataset;
     sampleType = s.GetProcessClassType(currentProcessClass);

     // Open the tree
     TFile f((string(FOLDER_BABYTUPLES)+currentDataset+".root").c_str());
     TTree* theTree = (TTree*) f.Get("babyTuple");

     intermediatePointers pointers;
     InitializeBranchesForReading(theTree,&myEvent,&pointers);

     myEvent.mStop       = -1;
     myEvent.mNeutralino = -1;

  // ########################################
  // ##        Run over the events         ##
  // ########################################

     bool ttbarDatasetToBeSplitted = false;
      if (findSubstring(currentDataset,"ttbar")
      && (currentDataset != "ttbar_madgraph_1l")
      && (currentDataset != "ttbar_madgraph_2l"))
          ttbarDatasetToBeSplitted = true;

      int nEntries = theTree->GetEntries();
      for (int i = 0 ; i < nEntries ; i++)
      {
          if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          nWTag = myEvent.nWTag;
          float weight = getWeight();

          // Split 1-lepton ttbar and 2-lepton ttbar
          string currentProcessClass_ = currentProcessClass;
          if (ttbarDatasetToBeSplitted && (myEvent.numberOfGenLepton == 2))
              currentProcessClass_ = "ttbar_2l";

          s.AutoFillProcessClass(currentProcessClass_,weight);
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
  s.MakePlots();
  cout << "   > Saving plots..." << endl;
  s.WritePlots("./plots/");

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################

  // ...

  printBoxedMessage("Program done.");
  return (0);
}


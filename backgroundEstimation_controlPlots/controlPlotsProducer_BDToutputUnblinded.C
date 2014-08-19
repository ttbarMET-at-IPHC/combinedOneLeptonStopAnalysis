#include "../backgroundEstimation_common/common.h"

#ifndef SIGNAL_REGION_TAG
    #error SIGNAL_REGION_TAG need to be defined.
#endif

// #########################################################################
//                              Main function
// #########################################################################

int main (int argc, char *argv[])
{
  loadBDTSignalRegions();

  string BDTSignalRegionTag = SIGNAL_REGION_TAG;

  if (BDTSignalRegionTag.find("BDT_") == 0)
      BDTSignalRegionTag = BDTSignalRegionTag.substr(4,BDTSignalRegionTag.size());

  string BDTSignalRegionLabel = BDTlabel(BDTSignalRegionTag, "root");
  float  BDTSignalRegionCut   = BDTcut(BDTSignalRegionTag);

  printBoxedMessage("Starting plot generation");

  // ####################
  // ##   Init tools   ##
  // ####################

     // Create a sonic Screwdriver
      SonicScrewdriver screwdriver;

     // ##########################
     // ##   Create Variables   ##
     // ##########################

     float numberOfBinsAfter  = 2.0;
     float numberOfBinsBefore = 3.0;
     float lowerBound = BDTSignalRegionCut - (0.5 - BDTSignalRegionCut) / numberOfBinsAfter * numberOfBinsBefore;

     float BDToutputValue;
     screwdriver.AddVariable("BDToutput_full", "BDT output (full)", "", 20,-0.5,       0.5, &BDToutputValue, "");
     screwdriver.AddVariable("BDToutput",      "BDT output",        "", 5, lowerBound, 0.5, &BDToutputValue, "noUnderflowInFirstBin");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

        screwdriver.AddProcessClass("1ltop", "1l top",                             "background",kRed-7);
            #ifdef USING_TTBAR_POWHEG
                screwdriver.AddDataset("ttbar_powheg",                "1ltop",  0, 0);
            #endif
            #ifdef USING_TTBAR_MADGRAPH
                screwdriver.AddDataset("ttbar_madgraph_1l",           "1ltop",  0, 0);
            #endif
            screwdriver.AddDataset("singleTop_st",                    "1ltop",  0, 0);

     screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",kCyan-3);
            #ifdef USING_TTBAR_MADGRAPH
                screwdriver.AddDataset("ttbar_madgraph_2l",   "ttbar_2l",  0, 0);
            #endif

     screwdriver.AddProcessClass("W+jets",   "W+jets",                          "background",kOrange-2);
             screwdriver.AddDataset("W+jets",    "W+jets", 0, 0);

     screwdriver.AddProcessClass("rare",   "rare",                              "background",kMagenta-5);
             screwdriver.AddDataset("rare",   "rare", 0, 0);

     screwdriver.AddProcessClass("data",   "data",                              "data",COLORPLOT_BLACK);
             screwdriver.AddDataset("SingleElec",   "data", 0, 0);
             screwdriver.AddDataset("SingleMuon",   "data", 0, 0);

     /*
     screwdriver.AddProcessClass("T2tt",     "T2tt",                       "signal",0,  "no1DPlots");
             screwdriver.AddDataset("T2tt",     "T2tt",   0, 0);
     screwdriver.AddProcessClass("signal_300_100",  "T2tt (300/100)",      "signal", COLORPLOT_BLUE );
     screwdriver.AddProcessClass("signal_650_100",  "T2tt (650/100)",      "signal", COLORPLOT_GREEN);
     */


     // ##########################
     // ##    Create Regions    ##
     // ##########################

     screwdriver.AddRegion("preselection",      "Preselection",                       &goesInPreselectionMTtail);
     screwdriver.AddRegion("signalRegion",      BDTSignalRegionLabel, "preselection", { Cut("BDToutput",      '>', BDTSignalRegionCut) }, "showCuts");

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
     screwdriver.SchedulePlots("2D");

     // Config plots

     screwdriver.SetGlobalStringOption("DataMCComparison",  "includeSignal",                    "stack");
     screwdriver.SetGlobalFloatOption ("DataMCComparison",  "factorSignal",                     1.0    );

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

     sampleName = currentDataset;
     sampleType = screwdriver.GetProcessClassType(currentProcessClass);

     // Open the tree
     TFile f((string(FOLDER_BABYTUPLES)+currentDataset+".root").c_str());
     TTree* theTree = (TTree*) f.Get("babyTuple");

     intermediatePointers pointers;
     InitializeBranchesForReading(theTree,&myEvent,&pointers);

     if (currentDataset == "T2tt")
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

          BDToutputValue = BDToutput(BDTSignalRegionTag);

          float weight = getWeight();

          // Split 1-lepton ttbar and 2-lepton ttbar
          string currentProcessClass_ = currentProcessClass;
          if ((currentDataset == "ttbar_powheg") && (myEvent.numberOfGenLepton == 2))
              currentProcessClass_ = "ttbar_2l";

          /*
          if (currentDataset == "T2tt")
          {
              if ((myEvent.mStop == 300) && (myEvent.mNeutralino == 100)) screwdriver.AutoFillProcessClass("signal_300_100",weight);
              if ((myEvent.mStop == 650) && (myEvent.mNeutralino == 100)) screwdriver.AutoFillProcessClass("signal_650_100",weight);
          }
          else
          {
          */
            screwdriver.AutoFillProcessClass(currentProcessClass_,weight);
          //}
      }

      printProgressBar(nEntries,nEntries,currentDataset);
      cout << endl;
      f.Close();

  }

    // #############################
    // ##   Apply scale factors   ##
    // #############################

    Table scaleFactors = Table(string("../backgroundEstimation_prediction/scaleFactors/BDT_")+BDTSignalRegionTag+".tab");

    Figure SF_pre           = scaleFactors.Get("value","SF_pre");
    Figure SF_post          = scaleFactors.Get("value","SF_post");
    Figure SF_MTtail_1ltop  = scaleFactors.Get("value","SF_MTtail_1ltop");
    Figure SF_MTtail_Wjets  = scaleFactors.Get("value","SF_MTtail_Wjets");

    vector<string> channelsOnWhichToApplyScaleFactors = { "singleLepton" };
    for (unsigned int c = 0 ; c < channelsOnWhichToApplyScaleFactors.size() ; c++)
    {
        string channel = channelsOnWhichToApplyScaleFactors[c];

      //  screwdriver.ApplyScaleFactor("ttbar_2l",  "preselection", channel, );
        screwdriver.ApplyScaleFactor("1ltop",     "preselection", channel, SF_MTtail_1ltop);
        screwdriver.ApplyScaleFactor("W+jets",    "preselection", channel, SF_MTtail_Wjets);

        screwdriver.ApplyScaleFactor("ttbar_2l",  "signalRegion", channel, SF_pre);
        screwdriver.ApplyScaleFactor("1ltop",     "signalRegion", channel, SF_post * SF_MTtail_1ltop);
        screwdriver.ApplyScaleFactor("W+jets",    "signalRegion", channel, SF_post * SF_MTtail_Wjets);
    }

  // ###################################
  // ##   Make plots and write them   ##
  // ###################################

  cout << endl;
  cout << "   > Making plots..." << endl;
  screwdriver.MakePlots();
  cout << "   > Saving plots..." << endl;

  system((string("mkdir -p ./plots/")+BDTSignalRegionTag).c_str());
  screwdriver.WritePlots(string("./plots/")+BDTSignalRegionTag);

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################

  printBoxedMessage("Program done.");
  return (0);
}


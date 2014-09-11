#include "../common/common.h"

bool goesInAnyChannel()                          { return (goesInSingleLeptonChannel() || goesInDoubleLeptonChannel());                  }

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

     screwdriver.AddVariable("Mlb",            "M'(lb)",                  "GeV",    26,0,520,       &(myEvent.Mlb),                  "");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

        screwdriver.AddProcessClass("1ltop", "1l top",                             "background",kRed-7);
            #ifdef USING_TTBAR_POWHEG
                screwdriver.AddDataset("ttbar_powheg",                "1ltop",  0, 0);
            #endif
            #ifdef USING_TTBAR_MADGRAPH
                screwdriver.AddDataset("ttbar_madgraph_1l",             "1ltop",  0, 0);
            #endif
            screwdriver.AddDataset("singleTop_st",                  "1ltop",  0, 0);

     screwdriver.AddProcessClass("W+jets",   "W+jets",                          "background",kOrange-2);
             screwdriver.AddDataset("W+jets",    "W+jets", 0, 0);

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     screwdriver.AddRegion("0btag_MTpeak",        "0 b-tag Control Region;M_{T} peak",   &goesIn0BtagControlRegionMTpeak);
     screwdriver.AddRegion("0btag_MTtail",        "0 b-tag Control Region;M_{T} tail",   &goesIn0BtagControlRegionMTtail);

     // ##########################
     // ##   Create Channels    ##
     // ##########################

     screwdriver.AddChannel("singleLepton", "e/#mu-channels",   &goesInSingleLeptonChannel);
     screwdriver.AddChannel("allChannels",  "",                 &goesInAnyChannel         );

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     // Create histograms
     screwdriver.Create1DHistos();

     // Schedule plots
     screwdriver.SchedulePlots("1DSuperimposed");

     // Config plots
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
          //if (i > nEntries * 0.03) break;
          if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          float weight = getWeight();

          // Split 1-lepton ttbar and 2-lepton ttbar
          string currentProcessClass_ = currentProcessClass;
          if (ttbarDatasetToBeSplitted && (myEvent.numberOfGenLepton == 2))
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

  screwdriver.WritePlots(string("./plots/MlbShapeComparison/"));

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################

  printBoxedMessage("Program done.");
  return (0);
}


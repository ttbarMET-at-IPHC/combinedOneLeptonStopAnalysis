

#include "../backgroundEstimation/common/common.h"

// #########################################################################
//                              Main function
// #########################################################################

bool goesInPreselectionNoBTag()
{
    if (myEvent.MET < MET_CUT) return false;
    if (myEvent.numberOfLepton != NLEP_CUT) return false;
    if (myEvent.nJets < NJET_CUT)  return false;
    if ((!myEvent.isolatedTrackVeto) || (!myEvent.tauVeto)) return false;

    return true;
}


int main (int argc, char *argv[])
{

  printBoxedMessage("Starting plot generation");

  // ####################
  // ##   Init tools   ##
  // ####################

     // Create a sonic screwdriver
      SonicScrewdriver s;

     // ##########################
     // ##   Create Variables   ##
     // ##########################

     s.AddVariable("MET",     "MET",                     "GeV",    16,50,530,      &(myEvent.MET),                   "logY");
     s.AddVariable("MT",      "M_{T}",                   "GeV",    20,0,400,       &(myEvent.MT),                    "logY");
     s.AddVariable("Mlb",     "M'(lb)",                  "GeV",    15,0,300,       &(myEvent.Mlb),                   "");

     int nJets, nBtag;
     s.AddVariable("nJets",   "Number of selected jets",              "",       11,0,10,        &(nJets),           "");
     s.AddVariable("nBtag",   "Number of selected b-tagged jets",     "",       5, 0,4,         &(nBtag),           "");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     s.AddProcessClass("1ltop", "1l top",                             "background", kRed-7);
            s.AddDataset("ttbar_powheg",                "1ltop",  0, 0);
            s.AddDataset("singleTop_st",                  "1ltop",  0, 0);

     //s.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background", kCyan-3);

     s.AddProcessClass("W+jets",   "W+jets",                          "background", kOrange-2);
             s.AddDataset("W+jets",    "W+jets", 0, 0);

    // s.AddProcessClass("rare",   "rare",                              "background", kMagenta-5);
    //         s.AddDataset("rare",   "rare", 0, 0);

     s.AddProcessClass("T2tt",     "T2tt (500/200)",                   "signal",     COLORPLOT_GREEN);
     s.AddProcessClass("T2tt_R",   "T2tt (500/200) right",             "signal",     kRed);
     s.AddProcessClass("T2tt_L",   "T2tt (500/200) left",              "signal",     kBlue);
             s.AddDataset("T2tt_500_200",   "T2tt", 0, 0);

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

     s.AddRegion("preselection_nobtag",  "Preselection;No M_{T} cut, no b-tag requirement",        &goesInPreselectionNoBTag      );
     s.AddRegion("preselection",         "Preselection;No M_{T} cut",                              &goesInPreselection            );
     s.AddRegion("0btag",                "Preselection;0 b-tag Control Region, MT tail",           &goesIn0BtagControlRegionMTtail);

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

     // Config plots
     s.SchedulePlots("1DSuperimposed");
     s.SchedulePlots("1DStack");
     //s.SchedulePlots("1DDataMCComparison");

     // Config plots

     s.SetGlobalBoolOption  ("1DSuperimposed",    "includeSignal",          true   );
     s.SetGlobalStringOption("1DStack",           "includeSignal",          "superimposed");
     s.SetGlobalFloatOption ("1DStack",           "factorSignal",           1.0    );

     s.SetGlobalStringOption("Plot", "infoTopRight", "CMS Preliminary");
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

          nJets = myEvent.nJets;
          nBtag = myEvent.nBTag;

          float weight = getWeight();

          // Split 1-lepton ttbar and 2-lepton ttbar
          string currentProcessClass_ = currentProcessClass;
          if (ttbarDatasetToBeSplitted && (myEvent.numberOfGenLepton == 2))
              //currentProcessClass_ = "ttbar_2l";
              continue;

          s.AutoFillProcessClass(currentProcessClass_,weight);

          if (currentProcessClass == "T2tt")
          {
             s.AutoFillProcessClass("T2tt_R",weight*myEvent.weightT2ttRightHanded);
             s.AutoFillProcessClass("T2tt_L",weight*myEvent.weightT2ttLeftHanded);
          }

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

  printBoxedMessage("Program done.");
  return (0);
}


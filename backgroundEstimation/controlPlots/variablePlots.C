#include "../common/common.h"

bool goesInDileptonControlRegion_() { return goesInDileptonControlRegion(); }

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

     screwdriver.AddVariable("MET",            "MET",                     "GeV",    16,50,530,      &(myEvent.MET),                  "logY");
     screwdriver.AddVariable("METPhi",         "METPhi",                  "GeV",    16,50,530,      &(myEvent.METPhi),               "");

     screwdriver.AddVariable("MT",             "M_{T}",                   "GeV",    40,0,400,       &(myEvent.MT),                   "logY");
     screwdriver.AddVariable("MTpeak",         "M_{T}",                   "GeV",    20,0,100,       &(myEvent.MT),                   "noOverflowInLastBin");
     screwdriver.AddVariable("MTtail",         "M_{T}",                   "GeV",    30,100,400,     &(myEvent.MT),                   "logY,noUnderflowInFirstBin");

     screwdriver.AddVariable("deltaPhiMETJets","#Delta#Phi(MET,j_{1,2})", "rad",    16,0,3.2,       &(myEvent.deltaPhiMETJets),      "");
     screwdriver.AddVariable("MT2W",           "M_{T2}^{W}",              "GeV",    20,0,500,       &(myEvent.MT2W),                 "");
     screwdriver.AddVariable("HTratio",        "H_{T}^{ratio}",           "",       20,0,1.2,       &(myEvent.HTRatio),              "");
     screwdriver.AddVariable("HadronicChi2",   "Hadronic #chi^{2}",       "",       20,0,20,        &(myEvent.hadronicChi2),         "logY");
     screwdriver.AddVariable("HT",             "H_{T}",                   "",       46,120,1500,    &(myEvent.HT),                   "");
     screwdriver.AddVariable("leadingBPt",     "p_{T}(leading b-jet)",    "GeV",    20,0,400,       &(myEvent.leadingBPt),           "");
     screwdriver.AddVariable("leadingJetPt",   "p_{T}(leading jet)",      "GeV",    20,0,600,       &(myEvent.leadingJetPt),         "");
     screwdriver.AddVariable("leptonPt",       "p_{T}(lepton)",           "GeV",    28,20,300,      &(myEvent.leadingLeptonPt),      "");
     screwdriver.AddVariable("Mlb",            "M'(lb)",                  "GeV",    20,0,500,       &(myEvent.Mlb),                  "");
     screwdriver.AddVariable("Mlb_hemi",       "M(lb)",             "GeV",          20,0,500,       &(myEvent.Mlb_hemi),             "");
     screwdriver.AddVariable("M3b",            "M3b",                     "GeV",    20,0,1000,      &(myEvent.M3b),                  "");
     screwdriver.AddVariable("deltaRLeptonB",  "#DeltaR(l,leading b)",    "",       20,0,5,         &(myEvent.deltaRLeptonLeadingB), "");
     screwdriver.AddVariable("HTLeptonPtMET",  "HT + MET + p_{T}(lepton)","GeV",    20,100,2100,    &(myEvent.HTPlusLeptonPtPlusMET),"");
     screwdriver.AddVariable("METoverSqrtHT",  "MET / #sqrt{H_{T}}",      "",       32,0,32,        &(myEvent.METoverSqrtHT),        "");

     float leadingNonBPtN4;
     screwdriver.AddVariable("leadingNonBPt",     "p_{T}(leading nonb-jet)",    "GeV",    20,0,500,       &(leadingNonBPtN4),           "");
     float leadingNonBPtN5;
     screwdriver.AddVariable("leadingNonBPtN5",     "p_{T}(leading nonb-jet) (Njet>4)",    "GeV",    20,0,500,       &(leadingNonBPtN5),           "noUnderflowInFirstBin");

     int nJets, nBtag;

     screwdriver.AddVariable("nJets",          "Number of selected jets",           "",       11,0,10,        &(nJets),              "");
     screwdriver.AddVariable("nBtag",          "Number of selected b-tagged jets",  "",       5, 0,4,         &(nBtag),              "");

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
            //screwdriver.AddDataset("ttbar_madgraph_scaledown",    "1ltop",  0, 0);
            //screwdriver.AddDataset("ttbar_madgraph_scaleup",      "1ltop",  0, 0);
            //screwdriver.AddDataset("ttbar_madgraph_matchingdown", "1ltop",  0, 0);
            //screwdriver.AddDataset("ttbar_madgraph_matchingup",   "1ltop",  0, 0);
            //screwdriver.AddDataset("ttbar_madgraph_mass166-5",    "1ltop",  0, 0);
            //screwdriver.AddDataset("ttbar_madgraph_mass178-5",    "1ltop",  0, 0);

            screwdriver.AddDataset("singleTop_st",                  "1ltop",  0, 0);

     screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",kCyan-3);
            #ifdef USING_TTBAR_MADGRAPH
                screwdriver.AddDataset("ttbar_madgraph_2l",   "ttbar_2l",  0, 0);
            #endif

     screwdriver.AddProcessClass("W+jets",   "W+jets",                          "background",kOrange-2);
             screwdriver.AddDataset("W+jets",    "W+jets", 0, 0);

     screwdriver.AddProcessClass("rare",   "rare",                              "background",kMagenta-5);
             screwdriver.AddDataset("rare",   "rare", 0, 0);

     screwdriver.AddProcessClass("T2tt", "T2tt (550, 50)",  "signal", COLORPLOT_GREEN );
            screwdriver.AddDataset("T2tt", "T2tt",   0, 0);

     screwdriver.AddProcessClass("T2bw", "T2bw, x = 0.50 (550, 50)",  "signal", COLORPLOT_MAGENTA );
            screwdriver.AddDataset("T2bw-050", "T2bw",   0, 0);

     screwdriver.AddProcessClass("data",   "data",                              "data",COLORPLOT_BLACK);
            screwdriver.AddDataset("SingleElec",   "data", 0, 0);
            screwdriver.AddDataset("SingleMuon",   "data", 0, 0);
            screwdriver.AddDataset("DoubleElec",   "data", 0, 0);
            screwdriver.AddDataset("DoubleMuon",   "data", 0, 0);
            screwdriver.AddDataset("MuEl",         "data", 0, 0);

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     screwdriver.AddRegion("preselection",                  "Preselection",               &goesInPreselection          );
     screwdriver.AddRegion("preselection_MTtail",           "Preselection;M_{T} tail",    &goesInPreselectionMTtail    );
     screwdriver.AddRegion("preselection_MTtail_withCorr",  "Preselection;M_{T} tail",    &goesInPreselectionMTtail    );

     // ##########################
     // ##   Create Channels    ##
     // ##########################

     screwdriver.AddChannel("singleLepton", "e/#mu-channels",            &goesInSingleLeptonChannel);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     // Create histograms
     screwdriver.Create1DHistos();

     // Schedule plots
     screwdriver.SchedulePlots("1DDataMCComparison");
     screwdriver.SchedulePlots("1DStack");

     // Config plots

     screwdriver.SetGlobalStringOption("DataMCComparison",  "includeSignal",    "superimposed");
     screwdriver.SetGlobalFloatOption ("DataMCComparison",  "factorSignal",     100.0    );

     screwdriver.SetGlobalStringOption("1DStack",           "includeSignal",    "superimposed");
     screwdriver.SetGlobalFloatOption ("1DStack",           "factorSignal",     100.0    );

     screwdriver.SetGlobalFloatOption ("DataMCRatio",       "min",              0.0    );
     screwdriver.SetGlobalFloatOption ("DataMCRatio",       "max",              2.0    );

     screwdriver.SetGlobalStringOption("Plot",              "infoTopRight",     "CMS Preliminary");
     screwdriver.SetGlobalStringOption("Plot",              "infoTopLeft",      "#sqrt{s} = 8 TeV, L = 19.5 fb^{-1}");

     screwdriver.SetGlobalBoolOption  ("Plot",              "exportPdf",        true);
     screwdriver.SetGlobalBoolOption  ("Plot",              "exportEps",        false);
     screwdriver.SetGlobalBoolOption  ("Plot",              "exportPng",        false);

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
              currentProcessClass_ = "ttbar_2l";

          #ifdef LEADING_NON_B_PT_ALREADY_COMPUTED
          leadingNonBPtN4= myEvent.leadingNonBPt;
          #else
          leadingNonBPtN4= leadingNonBPt();
          #endif
          leadingNonBPtN5=-1;
          if (nJets>4) leadingNonBPtN5= leadingNonBPtN4;

          if ((currentProcessClass_ == "T2tt") && ((myEvent.mStop != 550) || (myEvent.mNeutralino != 50))) continue;
          if ((currentProcessClass_ == "T2bw") && ((myEvent.mStop != 550) || (myEvent.mNeutralino != 50))) continue;

          screwdriver.AutoFillProcessClass(currentProcessClass_,weight);
      }

      printProgressBar(nEntries,nEntries,currentDataset);
      cout << endl;
      f.Close();

  }

  // #############################
  // ##   Apply scale factors   ##
  // #############################

    Table scaleFactors = Table("../prediction/scaleFactors/preselection.tab");

    Figure SF_pre           = scaleFactors.Get("value","SF_pre");
    Figure SF_post          = scaleFactors.Get("value","SF_post");
    Figure SF_0btag         = scaleFactors.Get("value","SF_0btag");
    Figure SF_vetopeak      = scaleFactors.Get("value","SF_vetopeak");
    Figure SF_MTtail_1ltop  = scaleFactors.Get("value","SF_MTtail_1ltop");
    Figure SF_MTtail_Wjets  = scaleFactors.Get("value","SF_MTtail_Wjets");

    screwdriver.ApplyScaleFactor("ttbar_2l", "preselection_MTtail_withCorr",  "singleLepton", SF_pre);
    screwdriver.ApplyScaleFactor("1ltop",    "preselection_MTtail_withCorr",  "singleLepton", SF_post * SF_MTtail_1ltop);
    screwdriver.ApplyScaleFactor("W+jets",   "preselection_MTtail_withCorr",  "singleLepton", SF_post * SF_MTtail_Wjets);

  // ###################################
  // ##   Make plots and write them   ##
  // ###################################

  cout << endl;
  cout << "   > Making plots..." << endl;
  screwdriver.MakePlots();
  cout << "   > Saving plots..." << endl;
  screwdriver.WritePlots("./plots/variablePlots/");

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################

  printBoxedMessage("Program done.");
  return (0);
}


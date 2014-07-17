#include "common.h"

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

bool goesInPreselectionNoMT_withSRCuts()       
{ 
    bool iok = goesInPreselection()       && SIGNAL_REGION_CUTS(disableMTCut);
    if (myEvent.MT>MT_CUT && (sampleType == "data") )  iok=0;
    return iok;
}


bool goesIn0BtagControlRegionMTpeak_withSRCuts() { return (goesIn0BtagControlRegionMTpeak() && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesIn0BtagControlRegionMTtail_withSRCuts() { return (goesIn0BtagControlRegionMTtail() && SIGNAL_REGION_CUTS(enableMTCut) ); }
bool goesIn0BtagControlRegionNoMT_withSRCuts()   { return (goesIn0BtagControlRegion()       && SIGNAL_REGION_CUTS(disableMTCut) ); }

bool goesInDileptonControlRegionMTpeak_withSRCuts() { return (goesInDileptonControlRegionMTpeak() && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInDileptonControlRegionMTtail_withSRCuts() { return (goesInDileptonControlRegionMTtail() && SIGNAL_REGION_CUTS(enableMTCut) ); }
bool goesInDileptonControlRegionNoMT_withSRCuts()   { return (goesInDileptonControlRegion()       && SIGNAL_REGION_CUTS(disableMTCut)); }

bool goesInVetoControlRegionMTpeak_withSRCuts() { return (goesInVetoControlRegionMTpeak() && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInVetoControlRegionMTtail_withSRCuts() { return (goesInVetoControlRegionMTtail() && SIGNAL_REGION_CUTS(enableMTCut) ); }
bool goesInVetoControlRegionNoMT_withSRCuts()   { return (goesInVetosControlRegion()      && SIGNAL_REGION_CUTS(disableMTCut)); }

bool goesInAnyChannel()                             { return (goesInSingleLeptonChannel() || goesInDoubleLeptonChannel());                  }

// #########################################################################
//                              Main function
// #########################################################################

int main (int argc, char *argv[])
{
  string signalRegionLabel_ = signalRegionLabel(SIGNAL_REGION_TAG);
  if (signalRegionLabel_ == "") 
  { 
      DEBUG_MSG << "Please define the signal region label associated to tag '" << SIGNAL_REGION_TAG << "'" << endl; 
      return -1; 
  }

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
     screwdriver.AddVariable("MT",             "M_{T}",                   "GeV",    40,0,400,       &(myEvent.MT),                   "logY");
     screwdriver.AddVariable("mStop",          "m_{#tilde{t}}",           "GeV",    28,112.5,812.5,  &(myEvent.mStop),               "");
     screwdriver.AddVariable("mNeutralino",    "m_{#chi^{0}}",            "GeV",    16,-12.5,387.5,  &(myEvent.mNeutralino),         "");

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
             screwdriver.AddDataset("DoubleElec",   "data", 0, 0);
             screwdriver.AddDataset("DoubleMuon",   "data", 0, 0);
             screwdriver.AddDataset("MuEl",   "data", 0, 0);

     screwdriver.AddProcessClass("T2tt",     "T2tt",                       "signal",kViolet-1,"no1Dplots");
             screwdriver.AddDataset("T2tt",     "T2tt",   0, 0);

     screwdriver.AddProcessClass("signal_250_100",  "T2tt (250/100)",      "signal", COLORPLOT_BLUE  );
     screwdriver.AddProcessClass("signal_350_100",  "T2tt (350/100)",      "signal", COLORPLOT_BLACK );
     screwdriver.AddProcessClass("signal_450_100",  "T2tt (450/100)",      "signal", COLORPLOT_GREEN );
     screwdriver.AddProcessClass("signal_650_100",  "T2tt (650/100)",      "signal", COLORPLOT_GREEN2);

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     string rebinning;
     if (string(SIGNAL_REGION_TAG) == "preselection") rebinning="rebin=1";
     else                                             rebinning="rebin=2";

     screwdriver.AddRegion("signalRegion",         signalRegionLabel_,                                            &goesInPreselectionMTtail_withSRCuts,"blinded");
     screwdriver.AddRegion("MTpeak",               signalRegionLabel_+";MT peak Control Region",                  &goesInPreselectionMTpeak_withSRCuts);
     screwdriver.AddRegion("0btag",                signalRegionLabel_+";0 b-tag Control Region",                  &goesIn0BtagControlRegionMTtail_withSRCuts,    rebinning);
     screwdriver.AddRegion("2leptons",             signalRegionLabel_+";2 leptons Control Region",                &goesInDileptonControlRegionMTtail_withSRCuts, rebinning);
     screwdriver.AddRegion("reversedVeto",         signalRegionLabel_+";reversed 2nd lepton veto Control Region", &goesInVetoControlRegionMTtail_withSRCuts,     rebinning);
     
     screwdriver.AddRegion("signalRegion_noMTCut", signalRegionLabel_+"; (no MT cut)",                            &goesInPreselectionNoMT_withSRCuts);
     screwdriver.AddRegion("0btag_noMTCut",        signalRegionLabel_+";0 b-tag (no MT cut)",                     &goesIn0BtagControlRegionNoMT_withSRCuts,    rebinning);
     screwdriver.AddRegion("2leptons_noMTCut",     signalRegionLabel_+";2 leptons (no MT cut)",                   &goesInDileptonControlRegionNoMT_withSRCuts, rebinning);
     screwdriver.AddRegion("reversedVeto_noMTCut", signalRegionLabel_+";reversed 2nd lepton veto (no MT cut)",    &goesInVetoControlRegionNoMT_withSRCuts,     rebinning);

     // ##########################
     // ##   Create Channels    ##
     // ##########################
   
     screwdriver.AddChannel("singleLepton", "e/#mu-channels",   &goesInSingleLeptonChannel);
     screwdriver.AddChannel("doubleLepton", "2l-channel",       &goesInDoubleLeptonChannel);
     screwdriver.AddChannel("allChannels",  "",                 &goesInAnyChannel         );

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     // Create histograms
     screwdriver.Create1DHistos();
     screwdriver.Add2DHisto("mStop","mNeutralino");

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
          //if (i > nEntries * 0.03) break;
          if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          float weight = getWeight();

          // Split 1-lepton ttbar and 2-lepton ttbar
          string currentProcessClass_ = currentProcessClass;
          if ((currentDataset == "ttbar_powheg") && (myEvent.numberOfGenLepton == 2)) 
              currentProcessClass_ = "ttbar_2l";

          if (currentDataset == "T2tt")
          {
              if ((myEvent.mStop == 250) && (myEvent.mNeutralino == 100)) screwdriver.AutoFillProcessClass("signal_250_100",weight); 
              if ((myEvent.mStop == 350) && (myEvent.mNeutralino == 100)) screwdriver.AutoFillProcessClass("signal_350_100",weight); 
              if ((myEvent.mStop == 450) && (myEvent.mNeutralino == 100)) screwdriver.AutoFillProcessClass("signal_450_100",weight); 
              if ((myEvent.mStop == 650) && (myEvent.mNeutralino == 100)) screwdriver.AutoFillProcessClass("signal_650_100",weight); 
          }
          else
          {
            screwdriver.AutoFillProcessClass(currentProcessClass_,weight);
          }

      } 
      printProgressBar(nEntries,nEntries,currentDataset);
      cout << endl;
      f.Close();

  }

    // #############################
    // ##   Apply scale factors   ##
    // #############################

    Table scaleFactors = Table(string("scaleFactors/")+SIGNAL_REGION_TAG+".tab");

    Figure SF_pre       = scaleFactors.Get("value","SF_pre");
    Figure SF_post      = scaleFactors.Get("value","SF_post");
    Figure SF_0btag     = scaleFactors.Get("value","SF_0btag");
    Figure SFR_Wjets    = scaleFactors.Get("value","SFR_W+jets");
    Figure SF_2l        = scaleFactors.Get("value","SF_2l");
    Figure SF_2ltail    = scaleFactors.Get("value","SF_2ltail");
    Figure SF_vetotail  = scaleFactors.Get("value","SF_vetotail");
    Figure SF_vetopeak  = scaleFactors.Get("value","SF_vetopeak");

    screwdriver.ApplyScaleFactor("ttbar_2l",    "MTpeak",               "singleLepton",SF_pre);
    screwdriver.ApplyScaleFactor("1ltop",       "MTpeak",               "singleLepton",SF_post);
    screwdriver.ApplyScaleFactor("W+jets",      "MTpeak",               "singleLepton",SF_post);

    screwdriver.ApplyScaleFactor("W+jets",      "0btag",                "singleLepton",SF_0btag);
    screwdriver.ApplyScaleFactor("1ltop",       "0btag",                "singleLepton",SF_0btag);
    screwdriver.ApplyScaleFactor("W+jets",      "0btag",                "singleLepton",SFR_Wjets);
    //screwdriver.ApplyScaleFactor("1ltop",       "0btag",                "singleLepton",SFR_Wjets);

    //screwdriver.ApplyScaleFactor("ttbar_2l","signalRegion_noMTCut", "singleLepton",SF_pre);
    //screwdriver.ApplyScaleFactor("1ltop",   "signalRegion_noMTCut", "singleLepton",SF_post);
    //screwdriver.ApplyScaleFactor("W+jets",  "signalRegion_noMTCut", "singleLepton",SF_post);

    //screwdriver.ApplyScaleFactor("W+jets",  "0btag_noMTCut",        "singleLepton",SF_0btag);
    //screwdriver.ApplyScaleFactor("1ltop",   "0btag_noMTCut",        "singleLepton",SF_0btag);

    screwdriver.ApplyScaleFactor("ttbar_2l",    "2leptons",             "doubleLepton", SF_2l);
    screwdriver.ApplyScaleFactor("1ltop",       "2leptons",             "doubleLepton", SF_2l);
    screwdriver.ApplyScaleFactor("W+jets",      "2leptons",             "doubleLepton", SF_2l);

    screwdriver.ApplyScaleFactor("ttbar_2l",    "2leptons_noMTCut",     "doubleLepton", SF_2l);
    screwdriver.ApplyScaleFactor("1ltop",       "2leptons_noMTCut",     "doubleLepton", SF_2l);
    screwdriver.ApplyScaleFactor("W+jets",      "2leptons_noMTCut",     "doubleLepton", SF_2l);

    screwdriver.ApplyScaleFactor("ttbar_2l",    "reversedVeto",         "singleLepton", SF_pre);
    screwdriver.ApplyScaleFactor("1ltop",       "reversedVeto",         "singleLepton", SF_vetopeak);
    screwdriver.ApplyScaleFactor("W+jets",      "reversedVeto",         "singleLepton", SF_vetopeak);

    screwdriver.ApplyScaleFactor("ttbar_2l",    "reversedVeto_noMTCut", "singleLepton", SF_pre);
    screwdriver.ApplyScaleFactor("1ltop",       "reversedVeto_noMTCut", "singleLepton", SF_vetopeak);
    screwdriver.ApplyScaleFactor("W+jets",      "reversedVeto_noMTCut", "singleLepton", SF_vetopeak);

  // ###################################
  // ##   Make plots and write them   ##
  // ###################################
 
  cout << endl;
  cout << "   > Making plots..." << endl;
  screwdriver.MakePlots();
  cout << "   > Saving plots..." << endl;
  screwdriver.WritePlots(string("./controlPlotsDataMC/")+SIGNAL_REGION_TAG);

  printBoxedMessage("Plot generation completed");

  string referenceSignal;

       if (string(SIGNAL_REGION_TAG) == "preselection"                  ) referenceSignal = "signal_250_100";
  else if (string(SIGNAL_REGION_TAG) == "cutAndCount_T2tt_offShellLoose") referenceSignal = "signal_250_100";
  else if (string(SIGNAL_REGION_TAG) == "cutAndCount_T2tt_offShellTight") referenceSignal = "signal_250_100";
  else if (string(SIGNAL_REGION_TAG) == "cutAndCount_T2tt_lowDeltaM"    ) referenceSignal = "signal_350_100";
  else if (string(SIGNAL_REGION_TAG) == "cutAndCount_T2tt_mediumDeltaM" ) referenceSignal = "signal_450_100";
  else if (string(SIGNAL_REGION_TAG) == "cutAndCount_T2tt_highDeltaM"   ) referenceSignal = "signal_650_100";

  vector<string> regions  = { "signalRegion", "MTpeak", "0btag", "2leptons", "reversedVeto" };
  TableBackgroundSignal contaminationCheck(&screwdriver,regions,"allChannels");
  contaminationCheck.Print();
 
  vector<string> regions2 = { "signalRegion_noMTCut", "0btag_noMTCut", "2leptons_noMTCut", "reversedVeto_noMTCut" };
  TableBackgroundSignal contaminationCheck2(&screwdriver,regions2,"allChannels");
  contaminationCheck2.Print();

  vector<string> dummyLabel = { SIGNAL_REGION_TAG };
  Table contaminationSummary(regions,dummyLabel);
  for (unsigned int r = 0 ; r < regions.size() ; r++)
  {
     contaminationSummary.Set(regions[r],SIGNAL_REGION_TAG,  contaminationCheck.Get(regions[r],referenceSignal)
                                                           / contaminationCheck.Get(regions[r],"totalSM"));
  }
  contaminationSummary.Print(2,"noError");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################
  
  printBoxedMessage("Program done.");
  return (0);
}


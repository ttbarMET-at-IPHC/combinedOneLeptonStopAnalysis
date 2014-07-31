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
  // Check the label of the signal region is defined
  
  string signalRegionLabel_ = signalRegionLabel(SIGNAL_REGION_TAG);
  if (signalRegionLabel_ == "") 
  { 
      DEBUG_MSG << "Please define the signal region label associated to tag '" << SIGNAL_REGION_TAG << "'" << endl; 
      return -1; 
  }
 
  // Special region for 2, 3 or 4 jets with 50, 100 or 150 minimum events
  /*
  string ControlRegion; 
  if (argc == 2) 
  { 
      ControlRegion = argv[1];
      NOMINAL_BDT_CUT = false;
      printBoxedMessage("Running on CR = "+ControlRegion);
      LoadBDTCut(ControlRegion);
  }
  if (argc >= 3) { WARNING_MSG << "Too many argument specified" << endl; return -1; }
  */

  bool runningOnBDTRegion = false; 
  if (findSubstring(SIGNAL_REGION_TAG,"BDT")) runningOnBDTRegion = true;

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
     screwdriver.AddVariable("MT_rebin2",      "M_{T}",                   "GeV",    16,0,400,       &(myEvent.MT),                   "logY");
     screwdriver.AddVariable("MT_rebin3",      "M_{T}",                   "GeV",    8,0,400,        &(myEvent.MT),                   "logY");
     screwdriver.AddVariable("MT_rebin4",      "M_{T}",                   "GeV",    4,0,400,        &(myEvent.MT),                   "logY");

     screwdriver.AddVariable("MTpeak",         "M_{T}",                   "GeV",    20,0,100,       &(myEvent.MT),                   "noOverflowInLastBin");
     screwdriver.AddVariable("MTtail",         "M_{T}",                   "GeV",    30,100,400,     &(myEvent.MT),                   "logY,noUnderflowInFirstBin");

     screwdriver.AddVariable("deltaPhiMETJets","#Delta#Phi(MET,j_{1,2})", "rad",    16,0,3.2,       &(myEvent.deltaPhiMETJets),      "");
     screwdriver.AddVariable("MT2W",           "M_{T2}^{W}",              "GeV",    20,0,500,       &(myEvent.MT2W),                 "");
     screwdriver.AddVariable("HTratio",        "H_{T}^{ratio}",           "",       20,0,1.2,       &(myEvent.HTRatio),              "");
     screwdriver.AddVariable("HadronicChi2",   "Hadronic #chi^{2}",       "",       40,0,10,        &(myEvent.hadronicChi2),         "");
     screwdriver.AddVariable("HT",             "H_{T}",                   "",       46,120,1500,    &(myEvent.HT),                   "");
     screwdriver.AddVariable("leadingBPt",     "p_{T}(leading b-jet)",    "GeV",    20,0,400,       &(myEvent.leadingBPt),           "");
     screwdriver.AddVariable("leadingJetPt",   "p_{T}(leading jet)",      "GeV",    20,0,600,       &(myEvent.leadingJetPt),         "");
     screwdriver.AddVariable("leptonPt",       "p_{T}(lepton)",           "GeV",    28,20,300,      &(myEvent.leadingLeptonPt),      "");
     screwdriver.AddVariable("Mlb",            "M'(lb)",                  "GeV",    26,0,520,       &(myEvent.Mlb),                  "");
     screwdriver.AddVariable("Mlb_hemi",       "M(lb)",             "GeV",    26,0,520,       &(myEvent.Mlb_hemi),             "");
     screwdriver.AddVariable("M3b",            "M3b",                     "GeV",    20,50,750,      &(myEvent.M3b),                  "");
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





     #ifdef BDT_OUTPUT_AVAILABLE
         float BDToutputValue;

         if (string(SIGNAL_REGION_TAG) == "preselection")
         {
             screwdriver.AddVariable("BDT_T2tt-1",      "BDT output T2tt-1",     "",   20,-0.5,0.5, &(myEvent.BDT_output_t2tt_R1   ), "");
             screwdriver.AddVariable("BDT_T2tt-2",      "BDT output T2tt-2",     "",   20,-0.5,0.5, &(myEvent.BDT_output_t2tt_R2   ), "");
             screwdriver.AddVariable("BDT_T2tt-5",      "BDT output T2tt-5",     "",   20,-0.5,0.5, &(myEvent.BDT_output_t2tt_R5   ), "");
             screwdriver.AddVariable("BDT_T2bw075-1",   "BDT output T2bw075-1",  "",   20,-0.5,0.5, &(myEvent.BDT_output_t2bw075_R1), "");
             screwdriver.AddVariable("BDT_T2bw075-2",   "BDT output T2bw075-2",  "",   20,-0.5,0.5, &(myEvent.BDT_output_t2bw075_R2), "");
             screwdriver.AddVariable("BDT_T2bw075-3",   "BDT output T2bw075-3",  "",   20,-0.5,0.5, &(myEvent.BDT_output_t2bw075_R3), "");
             screwdriver.AddVariable("BDT_T2bw075-5",   "BDT output T2bw075-5",  "",   20,-0.5,0.5, &(myEvent.BDT_output_t2bw075_R5), "");
             screwdriver.AddVariable("BDT_T2bw050-1",   "BDT output T2bw050-1",  "",   20,-0.5,0.5, &(myEvent.BDT_output_t2bw050_R1), "");
             screwdriver.AddVariable("BDT_T2bw050-3",   "BDT output T2bw050-3",  "",   20,-0.5,0.5, &(myEvent.BDT_output_t2bw050_R3), "");
             screwdriver.AddVariable("BDT_T2bw050-4",   "BDT output T2bw050-4",  "",   20,-0.5,0.5, &(myEvent.BDT_output_t2bw050_R4), "");
             screwdriver.AddVariable("BDT_T2bw050-5",   "BDT output T2bw050-5",  "",   20,-0.5,0.5, &(myEvent.BDT_output_t2bw050_R5), "");
             screwdriver.AddVariable("BDT_T2bw050-6",   "BDT output T2bw050-6",  "",   20,-0.5,0.5, &(myEvent.BDT_output_t2bw050_R6), "");
             screwdriver.AddVariable("BDT_T2bw025-1",   "BDT output T2bw025-1",  "",   20,-0.5,0.5, &(myEvent.BDT_output_t2bw025_R1), "");
             screwdriver.AddVariable("BDT_T2bw025-3",   "BDT output T2bw025-3",  "",   20,-0.5,0.5, &(myEvent.BDT_output_t2bw025_R3), "");
             screwdriver.AddVariable("BDT_T2bw025-4",   "BDT output T2bw025-4",  "",   20,-0.5,0.5, &(myEvent.BDT_output_t2bw025_R4), "");
             screwdriver.AddVariable("BDT_T2bw025-6",   "BDT output T2bw025-6",  "",   20,-0.5,0.5, &(myEvent.BDT_output_t2bw025_R6), "");
         }
         else if (runningOnBDTRegion)
         {
             screwdriver.AddVariable(SIGNAL_REGION_TAG, SIGNAL_REGION_TAG,       "",   20,-0.5,0.5, &BDToutputValue,                  "");
         }
     #endif

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
     
     screwdriver.AddProcessClass("data",   "data",                              "data",COLORPLOT_BLACK);
             screwdriver.AddDataset("SingleElec",   "data", 0, 0);
             screwdriver.AddDataset("SingleMuon",   "data", 0, 0);

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     string rebinning;
     if (string(SIGNAL_REGION_TAG) == "preselection") rebinning="rebin=1";
     else                                             rebinning="rebin=2";

     screwdriver.AddRegion("signalRegion_noMTCut", signalRegionLabel_+"; (no MT cut)",                            &goesInPreselectionNoMT_withSRCuts);
     screwdriver.AddRegion("0btag_noMTCut",        signalRegionLabel_+";0 b-tag (no MT cut)",                     &goesIn0BtagControlRegionNoMT_withSRCuts,    rebinning);
     screwdriver.AddRegion("reversedVeto_noMTCut", signalRegionLabel_+";reversed 2nd lepton veto (no MT cut)",    &goesInVetoControlRegionNoMT_withSRCuts,     rebinning);


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

    Table scaleFactors = Table(string("../backgroundEstimation_prediction/scaleFactors/")+SIGNAL_REGION_TAG+".tab");

    Figure SF_pre           = scaleFactors.Get("value","SF_pre");
    Figure SF_post          = scaleFactors.Get("value","SF_post");
    Figure SF_0btag         = scaleFactors.Get("value","SF_0btag");
    Figure SF_vetopeak      = scaleFactors.Get("value","SF_vetopeak");
    Figure SF_MTtail_1ltop  = scaleFactors.Get("value","SF_MTtail_1ltop");
    Figure SF_MTtail_Wjets  = scaleFactors.Get("value","SF_MTtail_Wjets");

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


          #ifdef BDT_OUTPUT_AVAILABLE
             // Keep BDT tag by keepting substr after "BDT_" in SIGNAL_REGION_TAG
             if (runningOnBDTRegion) BDToutputValue = BDToutput(string(SIGNAL_REGION_TAG).substr(4));
          #endif

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


          if ((myEvent.numberOfGenLepton == 1) && myEvent.MT>MT_CUT &&  currentProcessClass_ == "W+jets") weight*=SF_MTtail_Wjets.value();
          if ((myEvent.numberOfGenLepton == 1) && myEvent.MT>MT_CUT &&  currentProcessClass_  == "1ltop" ) weight*=SF_MTtail_1ltop.value();

          screwdriver.AutoFillProcessClass(currentProcessClass_,weight);
      } 

      printProgressBar(nEntries,nEntries,currentDataset);
      cout << endl;
      f.Close();

  }

    // #############################
    // ##   Apply scale factors   ##
    // #############################



    screwdriver.ApplyScaleFactor("ttbar_2l","signalRegion_noMTCut", "singleLepton",SF_pre);
    screwdriver.ApplyScaleFactor("1ltop",   "signalRegion_noMTCut", "singleLepton",SF_post);
    screwdriver.ApplyScaleFactor("W+jets",  "signalRegion_noMTCut", "singleLepton",SF_post);

    screwdriver.ApplyScaleFactor("W+jets",  "0btag_noMTCut",        "singleLepton",SF_0btag);
    screwdriver.ApplyScaleFactor("1ltop",   "0btag_noMTCut",        "singleLepton",SF_0btag);

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
            
  system((string("mkdir -p ./plots/dirtyMT/")+SIGNAL_REGION_TAG).c_str());
  screwdriver.WritePlots(string("./plots/dirtyMT/")+SIGNAL_REGION_TAG);

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################
  
  printBoxedMessage("Program done.");
  return (0);
}


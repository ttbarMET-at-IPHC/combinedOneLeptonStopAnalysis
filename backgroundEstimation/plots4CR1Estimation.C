#include "common.h"

#define ttbar_sample "ttbar_powheg"


bool goesInPreVetoSelectionMTpeak_withSRCuts()   { return (goesInPreVetoSelectionMTpeak()   && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInPreVetoSelectionMTtail_withSRCuts()   { return (goesInPreVetoSelectionMTtail()   && SIGNAL_REGION_CUTS(enableMTCut) ); }
                                                                      
bool goesInPreselectionMTpeak_withSRCuts()       { return (goesInPreselectionMTpeak()       && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInPreselectionMTtail_withSRCuts()       { return (goesInPreselectionMTtail()       && SIGNAL_REGION_CUTS(enableMTCut) ); }
/*
bool goesInPreselectionNoMT_withSRCuts()       
{ 
    bool iok = goesInPreselection()       && SIGNAL_REGION_CUTS(disableMTCut);
    if (myEvent.MT>MT_CUT && (sampleType == "data") )  iok=0;
    return iok;
}
*/

bool goesIn0BtagControlRegionMTpeak_withSRCuts() { return (goesIn0BtagControlRegionMTpeak() && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesIn0BtagControlRegionMTtail_withSRCuts() { return (goesIn0BtagControlRegionMTtail() && SIGNAL_REGION_CUTS(enableMTCut) ); }
bool goesIn0BtagControlRegionNoMT_withSRCuts()   { return (goesIn0BtagControlRegion()       && SIGNAL_REGION_CUTS(disableMTCut) ); }

bool goesInDileptonControlRegionMTpeak_withSRCuts() { return (goesInDileptonControlRegionMTpeak() && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInDileptonControlRegionMTtail_withSRCuts() { return (goesInDileptonControlRegionMTtail() && SIGNAL_REGION_CUTS(enableMTCut) ); }
bool goesInDileptonControlRegionNoMT_withSRCuts()   { return (goesInDileptonControlRegion()       && SIGNAL_REGION_CUTS(disableMTCut)); }

bool goesInVetoControlRegionMTpeak_withSRCuts() { return (goesInVetoControlRegionMTpeak() && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInVetoControlRegionMTtail_withSRCuts() { return (goesInVetoControlRegionMTtail() && SIGNAL_REGION_CUTS(enableMTCut) ); }
bool goesInVetoControlRegionNoMT_withSRCuts()   { return (goesInVetosControlRegion()      && SIGNAL_REGION_CUTS(disableMTCut)); }


bool LowBDT_T2tt_1         () { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2tt_1"         )>  -0.12); }
bool LowBDT_T2tt_2         () { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2tt_2"         )>  -0.20); }
bool LowBDT_T2tt_5         () { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2tt_5_loose"   )>  -0.19); }
bool LowBDT_T2bw075_1      () { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw075_1"      )>  -0.12); }
bool LowBDT_T2bw075_2      () { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw075_2"      )>  -0.19); }
bool LowBDT_T2bw075_3      () { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw075_3"      )>  -0.20); }
bool LowBDT_T2bw075_5      () { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw075_5"      )>  -0.19); }
bool LowBDT_T2bw050_1      () { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw050_1_loose")>  -0.12); }
bool LowBDT_T2bw050_3      () { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw050_3"      )>  -0.12); }
bool LowBDT_T2bw050_4      () { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw050_4"      )>  -0.14); }
bool LowBDT_T2bw050_5      () { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw050_5"      )>  -0.19); }
bool LowBDT_T2bw050_6      () { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw050_6"      )>  -0.19); }
bool LowBDT_T2bw025_1      () { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw025_1"      )>  -0.12); }
bool LowBDT_T2bw025_3      () { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw025_3"      )>  -0.12); }
bool LowBDT_T2bw025_4      () { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw025_4"      )>  -0.14); }
bool LowBDT_T2bw025_6      () { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw025_6"      )>  -0.22); }
 

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

     
     //Plots used for CR1 estimation
     /*
     float M3b_binning[6]={0,20,180,220,300,1000};
     screwdriver.AddVariable("M3b_small",             "M3b",        "GeV",    5, M3b_binning,       &(myEvent.M3b),      "");
     float M3b_binning2[5]={0,120,250,500,1000};
     screwdriver.AddVariable("M3b_small2",             "M3b",        "GeV",    4, M3b_binning2,       &(myEvent.M3b),      "");
     */
     screwdriver.AddVariable("M3b",             "M3b",        "GeV",    40,0,1000,       &(myEvent.M3b),      "");
     screwdriver.AddVariable("Mlb",             "Mlb",        "GeV",    40,0,600,       &(myEvent.Mlb),      "");
     ///*
     //-- to perform systematic study --//
     screwdriver.AddVariable("M3b_JESup",             "M3b_JESup",        "GeV",    40,0,1000,       &(myEvent.M3b_JESup),      "");
     screwdriver.AddVariable("M3b_JESdown",             "M3b_JESdown",        "GeV",    40,0,1000,       &(myEvent.M3b_JESdown),      "");
     screwdriver.AddVariable("Mlb_JESup",             "Mlb_JESup",        "GeV",    40,0,1000,       &(myEvent.Mlb_JESup),      "");
     screwdriver.AddVariable("Mlb_JESdown",             "Mlb_JESdown",        "GeV",    40,0,1000,       &(myEvent.Mlb_JESdown),      "");
     //*/
     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     screwdriver.AddProcessClass("1ltop", "1l top",                             "background",kRed-7);
            screwdriver.AddDataset(ttbar_sample,   "1ltop",  0, 0);
            screwdriver.AddDataset("singleTop_st",   "1ltop",  0, 0);
     
     screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",kCyan-3);
     	    if(string(ttbar_sample)==string("ttbar_madraph"))
	    	screwdriver.AddDataset("ttbar_madgraph_2l",         "ttbar_2l",  0, 0);

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

     // TODO : add signal here and check contamination in control regions

     // ##########################
     // ##    Create Regions    ##
     // ##########################


     screwdriver.AddRegion("presel_MTpeak",      "Preselection (MT peak)",           &goesInPreselectionMTpeak);
     //screwdriver.AddRegion("0btag",              "0 b-tag Control Region",                  &goesIn0BtagControlRegionMTtail_withSRCuts);
     screwdriver.AddRegion("0btag",              "0 b-tag Control Region",    &goesIn0BtagControlRegion);
     screwdriver.AddRegion("0btag_MTpeak",       "0 b-tag (MT peak)",      &goesIn0BtagControlRegionMTpeak);
     screwdriver.AddRegion("0btag_MTinverted",   "0 b-tag (MT < 100 GeV)", &goesIn0BtagControlRegionMTinverted);
     screwdriver.AddRegion("0btag_MTtail",       "0 b-tag (MT tail)",      &goesIn0BtagControlRegionMTtail);
     screwdriver.AddRegion("presel_MTpeak",      "Preselection (MT peak)",           &goesInPreselectionMTpeak);
     screwdriver.AddRegion("presel_MTinverted",  "Preselection (MT < 100 GeV)",      &goesInPreselectionMTinverted);

     screwdriver.AddRegion("LowBDT_T2tt_1", " LowBDT_T2tt_1      ",  &LowBDT_T2tt_1     );
     screwdriver.AddRegion("LowBDT_T2tt_2", " LowBDT_T2tt_2      ",  &LowBDT_T2tt_2     );
     screwdriver.AddRegion("LowBDT_T2tt_5", " LowBDT_T2tt_5      ",  &LowBDT_T2tt_5     );
     screwdriver.AddRegion("LowBDT_T2bw075_1", " LowBDT_T2bw075_1   ",  &LowBDT_T2bw075_1  );
     screwdriver.AddRegion("LowBDT_T2bw075_2", " LowBDT_T2bw075_2   ",  &LowBDT_T2bw075_2  );
     screwdriver.AddRegion("LowBDT_T2bw075_3", " LowBDT_T2bw075_3   ",  &LowBDT_T2bw075_3  );
     screwdriver.AddRegion("LowBDT_T2bw075_5", " LowBDT_T2bw075_5   ",  &LowBDT_T2bw075_5  );
     screwdriver.AddRegion("LowBDT_T2bw050_1", " LowBDT_T2bw050_1   ",  &LowBDT_T2bw050_1  );
     screwdriver.AddRegion("LowBDT_T2bw050_3", " LowBDT_T2bw050_3   ",  &LowBDT_T2bw050_3  );
     screwdriver.AddRegion("LowBDT_T2bw050_4", " LowBDT_T2bw050_4   ",  &LowBDT_T2bw050_4  );
     screwdriver.AddRegion("LowBDT_T2bw050_5", " LowBDT_T2bw050_5   ",  &LowBDT_T2bw050_5  );
     screwdriver.AddRegion("LowBDT_T2bw050_6", " LowBDT_T2bw050_6   ",  &LowBDT_T2bw050_6  );
     screwdriver.AddRegion("LowBDT_T2bw025_1", " LowBDT_T2bw025_1   ",  &LowBDT_T2bw025_1  );
     screwdriver.AddRegion("LowBDT_T2bw025_3", " LowBDT_T2bw025_3   ",  &LowBDT_T2bw025_3  );
     screwdriver.AddRegion("LowBDT_T2bw025_4", " LowBDT_T2bw025_4   ",  &LowBDT_T2bw025_4  );
     screwdriver.AddRegion("LowBDT_T2bw025_6", " LowBDT_T2bw025_6   ",  &LowBDT_T2bw025_6  );

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
          if ((currentDataset != "ttbar_madgraph") && (myEvent.numberOfGenLepton == 2)) 
              currentProcessClass_ = "ttbar_2l";

          screwdriver.AutoFillProcessClass(currentProcessClass_,weight);

      } 
      printProgressBar(nEntries,nEntries,currentDataset);
      cout << endl;
      f.Close();

  }

    // #############################
    // ##   Apply scale factors   ##
    // #############################

    Table scaleFactors = Table("scaleFactors/preselection.tab");

    Figure SF_pre       = scaleFactors.Get("value","SF_pre");
    Figure SF_post      = scaleFactors.Get("value","SF_post");
    Figure SF_0btag     = scaleFactors.Get("value","SF_0btag");
    Figure SFR_Wjets    = scaleFactors.Get("value","SFR_W+jets");
    Figure SF_2l        = scaleFactors.Get("value","SF_2l");
    Figure SF_2ltail    = scaleFactors.Get("value","SF_2ltail");
    Figure SF_vetotail  = scaleFactors.Get("value","SF_vetotail");
    Figure SF_vetopeak  = scaleFactors.Get("value","SF_vetopeak");
/*
    screwdriver.ApplyScaleFactor("ttbar_2l",    "MTpeak",               "singleLepton",SF_pre);
    screwdriver.ApplyScaleFactor("1ltop",       "MTpeak",               "singleLepton",SF_post);
    screwdriver.ApplyScaleFactor("W+jets",      "MTpeak",               "singleLepton",SF_post);
*//*
    screwdriver.ApplyScaleFactor("W+jets",      "0btag",                "singleLepton",SF_0btag);
    screwdriver.ApplyScaleFactor("1ltop",       "0btag",                "singleLepton",SF_0btag);
    screwdriver.ApplyScaleFactor("W+jets",      "0btag",                "singleLepton",SFR_Wjets);
   */ //screwdriver.ApplyScaleFactor("1ltop",       "0btag",                "singleLepton",SFR_Wjets);


  // ###################################
  // ##   Make plots and write them   ##
  // ###################################
 
  cout << endl;
  cout << "   > Making plots..." << endl;
  screwdriver.MakePlots();
  cout << "   > Saving plots..." << endl;
  screwdriver.WritePlots(string("./plots4CR1/")+ttbar_sample+string("/"));

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################
  
  printBoxedMessage("Program done.");
  return (0);
}


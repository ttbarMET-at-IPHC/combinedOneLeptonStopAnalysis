#include "../common.h"

// Sonic screwdriver headers

#include "interface/Table.h" 
#include "interface/SonicScrewdriver.h" 
#include "interface/tables/TableBackgroundSignal.h" 
#include "interface/tables/TableDataMC.h" 
using namespace theDoctor;

// BabyTuple format and location

//#define FOLDER_BABYTUPLES "../store/babyTuples_0328/"
#define FOLDER_BABYTUPLES "../store/babyTuples_0328_1lepton4jetsMET80/"
#include "analysisDefinitions.h"

babyEvent* myEventPointer;

bool SR()
{
    // Apply MET and MT cuts
    if (myEvent.MET < 150)            return false;

    if (myEvent.deltaPhiMETJets < 0.8) return false;
    if (myEvent.hadronicChi2    > 5)   return false;

    return true; 
}

bool goesInPreVetoSelection_tmp()             { return (goesInPreVetoSelection() && SR()); }
bool goesInPreVetoSelectionMTpeak_tmp()       { return (goesInPreVetoSelectionMTpeak() && SR()); }
bool goesInPreVetoSelectionMTtail_tmp()       { return (goesInPreVetoSelectionMTtail() && SR()); }
bool goesInPreVetoSelectionMTinverted_tmp()   { return (goesInPreVetoSelectionMTinverted() && SR()); }
                                                                          
bool goesInPreselection_tmp()                 { return (goesInPreselection() && SR()); }
bool goesInPreselectionMTpeak_tmp()           { return (goesInPreselectionMTpeak() && SR()); }
bool goesInPreselectionMTtail_tmp()           { return (goesInPreselectionMTtail() && SR()); }
bool goesInPreselectionMTinverted_tmp()       { return (goesInPreselectionMTinverted() && SR()); }
                                                                          
bool goesIn0BtagControlRegion_tmp()           { return (goesIn0BtagControlRegion() && SR()); }
bool goesIn0BtagControlRegionMTpeak_tmp()     { return (goesIn0BtagControlRegionMTpeak() && SR()); }
bool goesIn0BtagControlRegionMTtail_tmp()     { return (goesIn0BtagControlRegionMTtail() && SR()); }
bool goesIn0BtagControlRegionMTinverted_tmp() { return (goesIn0BtagControlRegionMTinverted() && SR()); }


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

     // Pointer to the event
     myEventPointer = &myEvent;
     
     // ##########################
     // ##   Create Variables   ##
     // ##########################

     screwdriver.AddVariable("MET",            "MET",                     "GeV",    16,50,530,      &(myEvent.MET),                  "logY");
     screwdriver.AddVariable("MT",             "M_{T}",                   "GeV",    40,0,400,       &(myEvent.MT),                   "logY");
     screwdriver.AddVariable("MTpeak",         "M_{T}",                   "GeV",    20,0,100,       &(myEvent.MT),                   "noOverflowInLastBin");
     screwdriver.AddVariable("MTtail",         "M_{T}",                   "GeV",    30,100,400,     &(myEvent.MT),                   "logY,noUnderflowInFirstBin");
     screwdriver.AddVariable("deltaPhiMETJets","#Delta#Phi(MET,j_{1,2})", "rad",    16,0,3.2,       &(myEvent.deltaPhiMETJets),      "");
     screwdriver.AddVariable("MT2W",           "M_{T2}^{W}",              "GeV",    20,0,500,       &(myEvent.MT2W),                 "");
     screwdriver.AddVariable("HTratio",        "H_{T}^{ratio}",           "",       20,0,1.2,       &(myEvent.HTRatio),              "");
     screwdriver.AddVariable("HadronicChi2",   "Hadronic #chi^{2}",       "",       40,0,10,        &(myEvent.hadronicChi2),         "");
     screwdriver.AddVariable("HT",             "H_{T}",                   "",       46,120,1500,    &(myEvent.HT),                   "");
     screwdriver.AddVariable("leadingBPt",     "p_{T}(leading b-jet)",    "GeV",    20,0,400,       &(myEvent.leadingBPt),           "");
     screwdriver.AddVariable("leadingJetPt",   "p_{T}(leading jet)",      "GeV",    20,0,600,       &(myEvent.leadingJetPt),         "");
     screwdriver.AddVariable("leptonPt",       "p_{T}(lepton)",           "GeV",    28,20,300,      &(myEvent.leadingLeptonPt),     "");
     screwdriver.AddVariable("Mlb",            "M_{lb}",                  "GeV",    26,0,520,       &(myEvent.Mlb),                  "");
     screwdriver.AddVariable("Mlb_hemi",       "M_{lb}_hemi",             "GeV",    26,0,520,       &(myEvent.Mlb_hemi),             "");
     screwdriver.AddVariable("M3b",            "M3b",                     "GeV",    20,50,750,      &(myEvent.M3b),                 "");
     screwdriver.AddVariable("deltaRLeptonB",  "#DeltaR(l,leading b)",    "",       20,0,5,         &(myEvent.deltaRLeptonLeadingB), "");
     screwdriver.AddVariable("HTLeptonPtMET",  "HT + MET + p_{T}(lepton)","GeV",    20,100,2100,    &(myEvent.HTPlusLeptonPtPlusMET),"");
     screwdriver.AddVariable("METoverSqrtHT",  "MET / #sqrt{H_{T}}",      "",       32,0,32,        &(myEvent.METoverSqrtHT),        "");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     screwdriver.AddProcessClass("1ltop",           "1l top",                            "background",kRed-7);
     screwdriver.AddProcessClass("ttbar_2l",        "t#bar{t} #rightarrow l^{+}l^{-}",   "background",kCyan-3);
            screwdriver.AddDataset("ttbar",         "1ltop",  0, 0);
     
     screwdriver.AddProcessClass("W+jets",          "W+jets",                            "background",kOrange-2);
             screwdriver.AddDataset("Wjets",        "W+jets", 0, 0);
     
     screwdriver.AddProcessClass("rare",          "rare",                            "background",kMagenta-5);
             screwdriver.AddDataset("others",       "rare", 0, 0);
    
     screwdriver.AddProcessClass("data",   "data",                                       "data",COLORPLOT_BLACK);
             screwdriver.AddDataset("SingleElec",   "data", 0, 0);
             screwdriver.AddDataset("SingleMuon",   "data", 0, 0);

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     screwdriver.AddRegion("presel_MTpeak",           "Pre-selection;MT peak",                                     &goesInPreselectionMTpeak);
     screwdriver.AddRegion("presel_0btag",            "Pre-selection;0 b-tag;MT tail (> 120)",                     &goesIn0BtagControlRegionMTtail);

     screwdriver.AddRegion("signalRegion_MTpeak",     "LM150 selection;MT peak Control Region",                    &goesInPreselectionMTpeak_tmp);
     screwdriver.AddRegion("signalRegion_0btag",      "LM150 selection;0 b-tag Control Region;MT tail (> 120)",    &goesIn0BtagControlRegionMTtail_tmp, "rebin=2");

     // ##########################
     // ##   Create Channels    ##
     // ##########################
   
     screwdriver.AddChannel("singleLepton", "e/#mu-channels",         &goesInSingleLeptonChannel);
     //screwdriver.AddChannel("singleElec",   "e-channel",              &goesInSingleElecChannel  );
     //screwdriver.AddChannel("singleMuon",   "#mu-channel",            &goesInSingleMuonChannel  );
     
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
     InitializeBranches(theTree,&myEvent,&pointers);

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
          if ((currentDataset == "ttbar") && (myEvent.numberOfGenLepton == 2)) 
              currentProcessClass_ = "ttbar_2l";

          // Split singletop s and t channels from other
          if ((currentDataset == "rare") 
          && ((myEvent.crossSection == 1.8)
          ||  (myEvent.crossSection == 30.0)
          ||  (myEvent.crossSection == 3.9 )
          ||  (myEvent.crossSection == 55.5))) currentProcessClass_ = "1ltop"; 

          screwdriver.AutoFillProcessClass(currentProcessClass_,weight);

      } 
      printProgressBar(nEntries,nEntries,currentDataset);
      cout << endl;
      f.Close();

  }

  // #############################
  // ##   Apply scale factors   ##
  // #############################

  string signalRegionLabel = "test";

  Table scaleFactors = Table("scaleFactors/"+signalRegionLabel+".tab");

  Figure SF_pre     = scaleFactors.Get("value","SF_pre");
  Figure SF_post    = scaleFactors.Get("value","SF_post");
  Figure SF_0btag   = scaleFactors.Get("value","SF_0btag");
  Figure SFR_Wjets  = scaleFactors.Get("value","SFR_W+jets");

  TableDataMC(&screwdriver,{"signalRegion_MTpeak" },"singleLepton").Print();

  screwdriver.ApplyScaleFactor("ttbar_2l","signalRegion_MTpeak","singleLepton",SF_pre);
  screwdriver.ApplyScaleFactor("1ltop",   "signalRegion_MTpeak","singleLepton",SF_post);
  screwdriver.ApplyScaleFactor("W+jets",  "signalRegion_MTpeak","singleLepton",SF_post);

  screwdriver.ApplyScaleFactor("W+jets","signalRegion_0btag","singleLepton",SF_0btag);
  screwdriver.ApplyScaleFactor("1ltop", "signalRegion_0btag","singleLepton",SF_0btag);
  screwdriver.ApplyScaleFactor("W+jets","signalRegion_0btag","singleLepton",SFR_Wjets);
  
  TableDataMC(&screwdriver,{"signalRegion_MTpeak" },"singleLepton").Print();

  // ###################################
  // ##   Make plots and write them   ##
  // ###################################
 
  cout << endl;
  cout << "   > Making plots..." << endl;
  screwdriver.MakePlots();
  cout << "   > Saving plots..." << endl;
  screwdriver.WritePlots("../plots/backgroundEstimation/");

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################
  
  printBoxedMessage("Program done.");
  return (0);
}


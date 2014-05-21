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

     screwdriver.AddVariable("MT",             "M_{T}",        "GeV",    40,0,400,       &(myEvent.MT),      "logY");
     screwdriver.AddVariable("MTpeak",         "M_{T}",        "GeV",    20,0,100,       &(myEvent.MT),      "noOverflowInLastBin");
     screwdriver.AddVariable("MTtail",         "M_{T}",        "GeV",    30,100,400,     &(myEvent.MT),      "logY,noUnderflowInFirstBin");
     screwdriver.AddVariable("mStop",          "m_{#tilde{t}}",           "GeV",    28,112.5,812.5,  &(myEvent.mStop),               "");
     screwdriver.AddVariable("mNeutralino",    "m_{#chi^{0}}",            "GeV",    16,-12.5,387.5,  &(myEvent.mNeutralino),         "");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     screwdriver.AddProcessClass("ttbar_1l",          "1l top",                            "background",kRed-7);
     screwdriver.AddProcessClass("ttbar_2l",        "t#bar{t} #rightarrow l^{+}l^{-}",   "background",kCyan-3);
            screwdriver.AddDataset("ttbar",         "ttbar_1l",  0, 0);
     
     screwdriver.AddProcessClass("W+jets",          "W+jets",                            "background",kOrange-2);
             screwdriver.AddDataset("Wjets",        "W+jets", 0, 0);
     
     screwdriver.AddProcessClass("singletop",       "singletop",                         "background",kBlue);

     screwdriver.AddProcessClass("others",          "others",                            "background",kMagenta-5);
             screwdriver.AddDataset("others",       "others", 0, 0);
    
     //screwdriver.AddProcessClass("T2tt",     "T2tt",                       "signal",kViolet-1);
     //        screwdriver.AddDataset("T2tt",     "T2tt",   0, 0);

     screwdriver.AddProcessClass("data",   "data",                                       "data",COLORPLOT_BLACK);
             screwdriver.AddDataset("SingleElec",   "data", 0, 0);
             screwdriver.AddDataset("SingleMuon",   "data", 0, 0);

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     screwdriver.AddRegion("preveto",            "Preselection (no MT cut)",         &goesInPreVetoSelection);
     screwdriver.AddRegion("preveto_MTpeak",     "Preselection (MT peak)",           &goesInPreVetoSelectionMTpeak);
     screwdriver.AddRegion("preveto_MTtail",     "Preselection (MT tail)",           &goesInPreVetoSelectionMTtail);
     screwdriver.AddRegion("preveto_MTinverted", "Preselection (MT < 100 GeV)",      &goesInPreVetoSelectionMTinverted);

     screwdriver.AddRegion("presel",             "Preselection (no MT cut)",         &goesInPreselection);
     screwdriver.AddRegion("presel_MTpeak",      "Preselection (MT peak)",           &goesInPreselectionMTpeak);
     screwdriver.AddRegion("presel_MTtail",      "Preselection (MT tail)",           &goesInPreselectionMTtail);
     screwdriver.AddRegion("presel_MTinverted",  "Preselection (MT < 100 GeV)",      &goesInPreselectionMTinverted);

     screwdriver.AddRegion("0btag",              "0 b-tag (no MT cut)",    &goesIn0BtagControlRegion);
     screwdriver.AddRegion("0btag_MTpeak",       "0 b-tag (MT peak)",      &goesIn0BtagControlRegionMTpeak);
     screwdriver.AddRegion("0btag_MTtail",       "0 b-tag (MT tail)",      &goesIn0BtagControlRegionMTtail);
     screwdriver.AddRegion("0btag_MTinverted",   "0 b-tag (MT < 100 GeV)", &goesIn0BtagControlRegionMTinverted);

     // ##########################
     // ##   Create Channels    ##
     // ##########################
   
     screwdriver.AddChannel("singleLepton", "e/#mu-channels",         &goesInSingleLeptonChannel);
     screwdriver.AddChannel("singleElec",   "e-channel",              &goesInSingleElecChannel  );
     screwdriver.AddChannel("singleMuon",   "#mu-channel",            &goesInSingleMuonChannel  );
     
  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     screwdriver.SetLumi(20000);

     // Create histograms
     screwdriver.Create1DHistos();
     screwdriver.Add2DHisto("mStop","mNeutralino");

     screwdriver.SetGlobalBoolOption  ("1DSuperimposed",   "includeSignal",                   true   );

     screwdriver.SetGlobalStringOption("1DStack",          "includeSignal",                   "stack");
     screwdriver.SetGlobalFloatOption ("1DStack",          "factorSignal",                    1.0    );

     screwdriver.SetGlobalStringOption("DataMCComparison", "includeSignal",                   "stack");
     screwdriver.SetGlobalFloatOption ("DataMCComparison", "factorSignal",                    1.0    );

     screwdriver.SetGlobalFloatOption ("FigureOfMerit",    "backgroundSystematicUncertainty", 0.15   );

     // Schedule plots
     screwdriver.SchedulePlots("1DSuperimposed");
     screwdriver.SchedulePlots("1DStack");
     screwdriver.SchedulePlots("1DDataMCComparison");
     screwdriver.SchedulePlots("2D");

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
          if ((currentDataset == "others") 
          && ((myEvent.crossSection == 1.8)
          ||  (myEvent.crossSection == 30.0)
          ||  (myEvent.crossSection == 3.9 )
          ||  (myEvent.crossSection == 55.5))) currentProcessClass_ = "singletop"; 

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
  screwdriver.WritePlots("../plots/backgroundEstimation/");

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################
  
  printBoxedMessage("Now computing misc tests ... ");

  vector<string> tablepreveto = { "preveto", "preveto_MTpeak", "preveto_MTtail", "preveto_MTinverted" };
  TableDataMC(&screwdriver,tablepreveto,"singleLepton").PrintTable();
  
  vector<string> tablepresel = { "presel", "presel_MTpeak", "presel_MTtail", "presel_MTinverted" };
  TableDataMC(&screwdriver,tablepresel,"singleLepton").PrintTable();
  
  vector<string> table0btag = { "0btag", "0btag_MTpeak", "0btag_MTtail", "0btag_MTinverted" };
  TableDataMC(&screwdriver,table0btag,"singleLepton").PrintTable();


  // #################################################

  Figure presel_data = screwdriver->GetYieldAndError("data", processesTags[p],
                                                     regionsTags[r],
                                                     channel);

  printBoxedMessage("Program done.");
  return (0);
}


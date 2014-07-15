#include "../common.h"

// Sonic screwdriver headers

#include "interface/Table.h" 
#include "interface/SonicScrewdriver.h" 
#include "interface/tables/TableBackgroundSignal.h" 
#include "interface/tables/TableDataMC.h" 
using namespace theDoctor;

// BabyTuple format and location

#define FOLDER_BABYTUPLES "../store/babyTuples_testV4vsV5/"
#include "Reader.h"

#include "analysisDefinitions.h"

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
     
     screwdriver.AddVariable("MET",            "MET",                   "GeV",    40,0,400,       &(myEvent.MET),                     "logY");
     screwdriver.AddVariable("MT",             "M_{T}",                 "GeV",    40,0,400,       &(myEvent.MT),                      "logY");
     screwdriver.AddVariable("deltaPhiMETJets","#Delta#Phi(MET,j_{1,2})", "rad",    16,0,3.2,     &(myEvent.deltaPhiMETJets),      "");
     screwdriver.AddVariable("MT2W",           "M_{T2}^{W}",              "GeV",    20,0,500,     &(myEvent.MT2W),                 "");
     screwdriver.AddVariable("HTratio",        "H_{T}^{ratio}",           "",       20,0,1.2,     &(myEvent.HTRatio),              "");
     screwdriver.AddVariable("HadronicChi2",   "Hadronic #chi^{2}",       "",       40,0,10,      &(myEvent.hadronicChi2),         "");
     screwdriver.AddVariable("HT",             "H_{T}",                   "",       46,120,1500,  &(myEvent.HT),                   "");
     screwdriver.AddVariable("leadingBPt",     "p_{T}(leading b-jet)",    "GeV",    20,0,400,     &(myEvent.leadingBPt),           "");
     screwdriver.AddVariable("leadingJetPt",   "p_{T}(leading jet)",      "GeV",    20,0,600,     &(myEvent.leadingJetPt),         "");
     screwdriver.AddVariable("leptonPt",       "p_{T}(lepton)",           "GeV",    28,20,300,    &(myEvent.leadingLeptonPt),      "");
     screwdriver.AddVariable("Mlb",            "M_{lb}",                  "GeV",    26,0,520,     &(myEvent.Mlb),                  "");
     screwdriver.AddVariable("Mlb_hemi",       "M_{lb}_hemi",             "GeV",    26,0,520,     &(myEvent.Mlb_hemi),             "");
     screwdriver.AddVariable("M3b",            "M3b",                     "GeV",    20,50,750,    &(myEvent.M3b),                  "");
     screwdriver.AddVariable("deltaRLeptonB",  "#DeltaR(l,leading b)",    "",       20,0,5,       &(myEvent.deltaRLeptonLeadingB), "");
     screwdriver.AddVariable("HTLeptonPtMET",  "HT + MET + p_{T}(lepton)","GeV",    20,100,2100,  &(myEvent.HTPlusLeptonPtPlusMET),"");
     screwdriver.AddVariable("METoverSqrtHT",  "MET / #sqrt{H_{T}}",      "",       32,0,32,      &(myEvent.METoverSqrtHT),        "");

     screwdriver.AddVariable("mStop",          "m_{#tilde{t}}",         "GeV",    28,112.5,812.5, &(myEvent.mStop),       "");
     screwdriver.AddVariable("mNeutralino",    "m_{#chi^{0}}",          "GeV",    16,-12.5,387.5, &(myEvent.mNeutralino), "noOverflowInLastBin");
     
     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     screwdriver.AddProcessClass("V4",                           "V4 (low stat)",  "signal",COLORPLOT_RED);
          screwdriver.AddDataset("T2tt_V04_skimmed_forLara",     "V4",   0, 0);
   
     screwdriver.AddProcessClass("V5-lowStat",     "V5 (low stat)", "background",COLORPLOT_BLUE);
          screwdriver.AddDataset("T2tt_lowStat",   "V5-lowStat",     0, 0);

     screwdriver.AddProcessClass("V5-newStat",           "V5 (new stat)", "background",COLORPLOT_GREEN);
          screwdriver.AddDataset("T2tt_additionalStat", "V5-newStat",     0, 0);

  // ##########################
  // ##    Create Regions    ##
  // ##########################

     screwdriver.AddRegion("presel_MTtail",     "Preselection, MT tail",     &goesInPreselectionMTtail);

  // ##########################
  // ##   Create Channels    ##
  // ##########################
   
     screwdriver.AddChannel("singleLepton", "e/#mu-channels",         &goesInSingleLeptonChannel);
     
  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     screwdriver.SetLumi(20000);

     // Create histograms
     screwdriver.Create1DHistos();
     screwdriver.Add2DHisto("mStop","mNeutralino");

     screwdriver.SetGlobalBoolOption  ("1DSuperimposed", "includeSignal",    true        );
     screwdriver.SetGlobalStringOption("1DStack",        "includeSignal",    "superimpose");
     screwdriver.SetGlobalFloatOption ("1DStack",        "factorSignal",     1.0          );


     // Schedule plots
     screwdriver.SchedulePlots("1DSuperimposed");
     screwdriver.SchedulePlots("1DStack");
     screwdriver.SchedulePlots("2D");

     // Config plots

     screwdriver.SetGlobalStringOption("Plot", "infoTopRight", "CMS Internal");
     screwdriver.SetGlobalStringOption("Plot", "infoTopLeft",  "#sqrt{s} = 8 TeV");

     screwdriver.SetGlobalBoolOption("Plot", "exportPdf", true);
     screwdriver.SetGlobalBoolOption("Plot", "exportEps", false);
     screwdriver.SetGlobalBoolOption("Plot", "exportPng", false);

  // ########################################
  // ##       Run over the datasets        ##
  // ########################################

  myEvent.leadingLeptonIdEfficiency = 1.0;
  myEvent.leadingLeptonIsoScaleFactor = 1.0;

  vector<string> datasetsList;
  screwdriver.GetDatasetList(&datasetsList);

  cout << "   > Reading datasets... " << endl;
  cout << endl;

  for (unsigned int d = 0 ; d < datasetsList.size() ; d++)
  {
     string currentDataset = datasetsList[d];
     string currentProcessClass = screwdriver.GetProcessClass(currentDataset);

     sampleName = currentDataset;
     sampleType = "signal";
     //screwdriver.GetProcessClassType(currentProcessClass);
    
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

          //if (i > 0.1 * nEntries) break;

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          float weight = getWeight();
          //float weight = 1.0;


          if ((myEvent.mStop == 250) && (myEvent.mNeutralino == 50))
          {
              screwdriver.AutoFillProcessClass(currentProcessClass,weight);
              if (goesInPreselectionMTtail())
                  DEBUG_MSG << endl
                      << " sample = " << currentProcessClass
                      << " ; id = " << myEvent.event << " ; MET = " << myEvent.MET << " ; MT = " << myEvent.MT << " ; weight = " << weight
                      << " ; xsec  = " << myEvent.weightCrossSection
                      << " ; trig  = " << myEvent.weightTriggerEfficiency
                      << " ; lepid = " << myEvent.leadingLeptonIdEfficiency * myEvent.leadingLeptonIsoScaleFactor
                      << " ; pu    = " << myEvent.weightPileUp
                      << " ; ISR   = " << myEvent.weightISRmodeling
                      << endl;
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
  screwdriver.MakePlots();
  cout << "   > Saving plots..." << endl;
  screwdriver.WritePlots("./");

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################
  
  printBoxedMessage("Now computing misc tests ... ");

  printBoxedMessage("Program done.");
  return (0);
}


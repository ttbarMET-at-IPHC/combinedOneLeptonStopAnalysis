#include "../common.h"


// Sonic screwdriver headers

#include "interface/Table.h" 
#include "interface/SonicScrewdriver.h" 
#include "interface/tables/TableBackgroundSignal.h" 
#include "interface/tables/TableDataMC.h" 
using namespace theDoctor;

// BabyTuple format and location

#define FOLDER_BABYTUPLES "../store/babyTuples_0603/"
//#define FOLDER_BABYTUPLES "../store/babyTuples_0328_preSelectionSkimmed/"
#include "Reader.h"


babyEvent* myEventPointer;
string* pCurrentDataset;
string* pCurrentDatasetType;

// #########################################################################
//                          Region selectors
// #########################################################################



bool singleElecChannelSelector() 
{ 
    if (myEventPointer->numberOfLepton != 1) return false;
    if ((*pCurrentDatasetType == "data") && (*pCurrentDataset != "SingleElec")) return false;
    return (abs(myEventPointer->leadingLeptonPDGId) == 11); 
}
bool singleMuonChannelSelector() 
{ 
    if (myEventPointer->numberOfLepton != 1) return false;
    if ((*pCurrentDatasetType == "data") && (*pCurrentDataset != "SingleMuon")) return false;
    return (abs(myEventPointer->leadingLeptonPDGId) == 13); 
}

bool doubleElecChannelSelector() 
{ 
    if (myEventPointer->numberOfLepton != 2) return false;
    if ((*pCurrentDatasetType == "data") && (*pCurrentDataset != "DoubleElec")) return false;
    return ((abs(myEventPointer->leadingLeptonPDGId) == 11) 
         && (abs(myEventPointer->secondLeptonPDGId)  == 11)); 
}

bool doubleMuonChannelSelector() 
{ 
    if (myEventPointer->numberOfLepton != 2) return false;
    if ((*pCurrentDatasetType == "data") && (*pCurrentDataset != "DoubleMuon")) return false;
    return ((abs(myEventPointer->leadingLeptonPDGId) == 13) 
         && (abs(myEventPointer->secondLeptonPDGId)  == 13)); 
}

bool muonElecChannelSelector() 
{ 
    if (myEventPointer->numberOfLepton != 2) return false;
    if ((*pCurrentDatasetType == "data") && (*pCurrentDataset != "MuEl")) return false;
    return   (((abs(myEventPointer->leadingLeptonPDGId) == 13) 
            && (abs(myEventPointer->secondLeptonPDGId)  == 11))
       ||     ((abs(myEventPointer->leadingLeptonPDGId) == 11) 
            && (abs(myEventPointer->secondLeptonPDGId)  == 13)));
}

bool singleLeptonChannelSelector() { return (singleElecChannelSelector() || singleMuonChannelSelector()); }
bool doubleLeptonChannelSelector() { return (doubleElecChannelSelector() 
                                          || doubleMuonChannelSelector()
                                          || muonElecChannelSelector()); }


bool Selector_presel() 
{
    babyEvent myEvent = *myEventPointer;

    // Require nLepton == 1
    if (myEvent.numberOfLepton != 1)                        return false;

    // Require nJets >= 4, nBTag >= 1
    if ((myEvent.nJets <= 3) || (myEvent.nBTag == 0) )      return false; 

    // Apply vetos
    if ((!myEvent.isolatedTrackVeto) || (!myEvent.tauVeto)) return false;

    // Apply MET and MT cuts
    if (myEvent.MET < 80) return false;
    //|| (myEvent.MT > 100))            return false;

    return true; 
}

bool Selector_2leptons() 
{
    babyEvent myEvent = *myEventPointer;

    // Apply double-lepton trigger requirement (for both MC and data)
    if ((!myEvent.triggerMuonElec) 
     && (!myEvent.triggerDoubleMuon)
     && (!myEvent.triggerDoubleElec)) return false;
    
    // Require nLepton == 2
    if (myEvent.numberOfLepton != 2)                      return false;

    // Require nJets >= 4, nBTag >= 1
    if ((myEvent.nJets <= 3) || (myEvent.nBTag == 0))     return false; 

    // Apply vetos
    //if ((!myEvent.isolatedTrackVeto) || (!myEvent.tauVeto))return false;

    // Apply MET and MT cuts
    if ((myEvent.MET < 80))         return false;

    return true; 
}

bool Selector_0bTag() 
{
    babyEvent myEvent = *myEventPointer;

    // Require nLepton == 1
    if (myEvent.numberOfLepton != 1)                      return false;

    // Require nJets >= 4, nBTag >= 1
    if ((myEvent.nJets <= 3) || (myEvent.nBTag != 0))     return false; 

    // Apply vetos
    if ((!myEvent.isolatedTrackVeto) || (!myEvent.tauVeto))return false;

    // Apply MET and MT cuts
    if ((myEvent.MET < 80))       return false;

    return true; 
}

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

       // Create a container for the event
     babyEvent myEvent;
     myEventPointer = &myEvent;

  // ##########################
  // ##   Create Variables   ##
  // ##########################

     screwdriver.AddVariable("MET",            "MET",                     "GeV",    15,50,500,      &(myEvent.MET),                  "logY");
     screwdriver.AddVariable("MT",             "M_{T}",                   "GeV",    40,0,400,       &(myEvent.MT),                   "logY");
     screwdriver.AddVariable("MTpeak",         "M_{T}",                   "GeV",    20,0,100,       &(myEvent.MT),                   "noOverflowInLastBin");
     screwdriver.AddVariable("MTtail",         "M_{T}",                   "GeV",    30,100,400,     &(myEvent.MT),                   "logY,noUnderflowInFirstBin");
     screwdriver.AddVariable("deltaPhiMETJets","#Delta#Phi(MET,j_{1,2})", "rad",    16,0,3.2,       &(myEvent.deltaPhiMETJets),      "");
     screwdriver.AddVariable("MT2W",           "M_{T2}^{W}",              "GeV",    20,0,500,       &(myEvent.MT2W),                 "");
     screwdriver.AddVariable("HTratio",        "H_{T}^{ratio}",           "",       20,0,1.2,       &(myEvent.HTRatio),              "");
     screwdriver.AddVariable("HadronicChi2",   "Hadronic #chi^{2}",       "",       40,0,20,        &(myEvent.hadronicChi2),         "");
     screwdriver.AddVariable("HT",             "H_{T}",                   "",       45,150,1500,    &(myEvent.HT),                   "");
     screwdriver.AddVariable("leadingBPt",     "p_{T}(leading b-jet)",    "GeV",    20,0,200,       &(myEvent.leadingBPt),           "");
     screwdriver.AddVariable("leadingJetPt",   "p_{T}(leading jet)",      "GeV",    20,0,200,       &(myEvent.leadingJetPt),         "");
     screwdriver.AddVariable("leptonPt",       "p_{T}(lepton)",           "GeV",    20,5,205,       &(myEvent.leadingLeptonPt),      "");
     screwdriver.AddVariable("Mlb",            "M_{lb}",                  "GeV",    25,0,500,       &(myEvent.Mlb),                  "");
     screwdriver.AddVariable("Mlb_hemi",       "M_{lb}_hemi",             "GeV",    25,0,500,       &(myEvent.Mlb_hemi),             "");
     screwdriver.AddVariable("M3b",            "M3b",                     "GeV",    25,0,500,       &(myEvent.M3b),                  "");
     screwdriver.AddVariable("deltaRLeptonB",  "#DeltaR(l,leading b)",    "",       25,0,5,         &(myEvent.deltaRLeptonLeadingB), "");
     screwdriver.AddVariable("HTLeptonPtMET",  "HT + MET + p_{T}(lepton)","GeV",    20,100,2100,    &(myEvent.HTPlusLeptonPtPlusMET),"");
     screwdriver.AddVariable("METoverSqrtHT",  "MET / #sqrt{H_{T}}",      "",       32,0,32,        &(myEvent.METoverSqrtHT),        "");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     screwdriver.AddProcessClass("1ltop", "1l top",                          "background",kRed-7);
            screwdriver.AddDataset("ttbar_madgraph_1l",    "1ltop",  0, 0);
            screwdriver.AddDataset("singleTop_st",         "1ltop",  0, 0);
     
     screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",kCyan-3);
            screwdriver.AddDataset("ttbar_madgraph_2l",    "ttbar_2l",  0, 0);
     
     screwdriver.AddProcessClass("W+jets",   "W+jets",                          "background",kOrange-2);
             screwdriver.AddDataset("W+jets",    "W+jets", 0, 0);

     screwdriver.AddProcessClass("rare",   "rare",                              "background",kMagenta-5);
             screwdriver.AddDataset("rare",   "rare", 0, 0);
     
     screwdriver.AddProcessClass("data",   "data",                              "data",COLORPLOT_BLACK);
             screwdriver.AddDataset("SingleElec",   "data", 0, 0);
             screwdriver.AddDataset("SingleMuon",   "data", 0, 0);
             screwdriver.AddDataset("DoubleMuon",   "data", 0, 0);
             screwdriver.AddDataset("DoubleElec",   "data", 0, 0);
             screwdriver.AddDataset("MuEl",         "data", 0, 0);


  // ##########################
  // ##    Create Regions    ##
  // ##########################

     screwdriver.AddRegion("presel",      "Preselection region",            &Selector_presel);
     screwdriver.AddRegion("2leptons",    "2 leptons region",               &Selector_2leptons);
     screwdriver.AddRegion("0bTag",       "0 b-tag region",                 &Selector_0bTag);

  // ##########################
  // ##   Create Channels    ##
  // ##########################
   
     screwdriver.AddChannel("singleLepton", "e/#mu-channels",         &singleLeptonChannelSelector);
     screwdriver.AddChannel("singleElec",   "e-channel",              &singleElecChannelSelector  );
     screwdriver.AddChannel("singleMuon",   "#mu-channel",            &singleMuonChannelSelector  );
     
     screwdriver.AddChannel("doubleLepton", "ee/#mu#mu/e#mu-channels",&doubleLeptonChannelSelector);
     screwdriver.AddChannel("doubleElec",   "ee-channel",             &doubleElecChannelSelector  );
     screwdriver.AddChannel("doubleMuon",   "#mu#mu-channel",         &doubleMuonChannelSelector  );
     screwdriver.AddChannel("elecMuon",     "e#mu-channel",           &muonElecChannelSelector    );

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     screwdriver.SetLumi(20000);

     // Create histograms
     screwdriver.Create1DHistos();

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

     pCurrentDataset     = &currentDataset;
     pCurrentDatasetType = &currentProcessClass;

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

          //if (i > 0.03 * nEntries) break;

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          float weight = 1.0;
          if (currentProcessClass != "data")
          {
              // For MC, apply weights
              float lumi;

                   if (singleElecChannelSelector())  lumi = 19154.0;
              else if (singleMuonChannelSelector())  lumi = 19096.0;
              else if (muonElecChannelSelector()  )  lumi = 19347.0;
              else if (doubleMuonChannelSelector())  lumi = 14595.0;
              else if (doubleElecChannelSelector())  lumi = 19216.0;
              else                                   lumi = 0.0;

              // Normalize to cross section times lumi
              weight *= myEvent.weightCrossSection * lumi;

              // Apply trigger efficiency weights for singleLepton channels
              if (myEvent.numberOfLepton == 1) weight *= myEvent.weightTriggerEfficiency;

              // Apply pile-up weight
              weight *= myEvent.weightPileUp;

              // For ttbar, apply topPt reweighting
              if ((currentDataset == "ttbar_madgraph_1l") 
               || (currentProcessClass == "ttbar_2l")) weight *= myEvent.weightTopPt;
          }
          else
          {
              // For data, apply trigger requirements
              if ((currentDataset == "SingleMuon") && (!myEvent.triggerMuon) && (!myEvent.xtriggerMuon))   continue;
              if ((currentDataset == "SingleElec") && (!myEvent.triggerElec))                              continue;
              if ((currentDataset == "MuEl")       && (!myEvent.triggerMuonElec))                          continue;
              if ((currentDataset == "DoubleMuon") && (!myEvent.triggerDoubleMuon))                        continue;
              if ((currentDataset == "DoubleElec") && (!myEvent.triggerDoubleElec))                        continue;

          }

          screwdriver.AutoFillProcessClass(currentProcessClass,weight);

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
  screwdriver.WritePlots("../plots/dataMCChecks/");

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################
  
  printBoxedMessage("Now computing misc tests ... ");

  vector<string> regions1l = { "presel", "0bTag" };

  TableDataMC(&screwdriver,regions1l,"singleElec").Print();
  TableDataMC(&screwdriver,regions1l,"singleMuon").Print();

  vector<string> regions2l = { "2leptons" };

  TableDataMC(&screwdriver,regions2l,"doubleElec").Print();

  printBoxedMessage("Program done.");
  return (0);
}


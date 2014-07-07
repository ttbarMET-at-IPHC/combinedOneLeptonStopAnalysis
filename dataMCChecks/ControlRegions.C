#include "../common.h"

// Sonic screwdriver headers

#include "interface/Table.h" 
#include "interface/SonicScrewdriver.h" 
#include "interface/tables/TableBackgroundSignal.h" 
#include "interface/tables/TableDataMC.h" 
using namespace theDoctor;

// BabyTuple format and location

#define FOLDER_BABYTUPLES "../store/babyTuples_0603_fix2lepton1or2jets/"
#include "Reader_newFinal0603_skimmed.h"
#include "analysisDefinitions.h"

bool goesInDileptonControlRegion2() 
{
    if (myEvent.numberOfLepton != 2) return false;
    if (myEvent.nBTag < NBJET_CUT)  return false;
    if (myEvent.MET < MET_CUT) return false;

    // Remove same-sign events
    if ((myEvent.leadingLeptonPDGId < 0) && (myEvent.secondLeptonPDGId < 0)) return false;
    if ((myEvent.leadingLeptonPDGId > 0) && (myEvent.secondLeptonPDGId > 0)) return false;
    
    // Remove Z mass peak
    if ( fabs((myEvent.leadingLepton + myEvent.secondLepton).M() - 91) < 15) return false;

    return true; 
}

bool goesInLooseDileptonRegion() 
{
    if (myEvent.numberOfLepton != 2) return false;

    // Remove same-sign events
    if ((myEvent.leadingLeptonPDGId < 0) && (myEvent.secondLeptonPDGId < 0)) return false;
    if ((myEvent.leadingLeptonPDGId > 0) && (myEvent.secondLeptonPDGId > 0)) return false;
    
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

  // ##########################
  // ##   Create Variables   ##
  // ##########################

     screwdriver.AddVariable("MET",            "MET",                     "GeV",    15,50,500,      &(myEvent.MET),                    "logY");
     screwdriver.AddVariable("MT",             "M_{T}",                   "GeV",    40,0,400,       &(myEvent.MT),                     "logY");
     screwdriver.AddVariable("MTpeak",         "M_{T}",                   "GeV",    20,0,100,       &(myEvent.MT),                     "noOverflowInLastBin");
     screwdriver.AddVariable("MTtail",         "M_{T}",                   "GeV",    30,100,400,     &(myEvent.MT),                     "logY,noUnderflowInFirstBin");
     screwdriver.AddVariable("deltaPhiMETJets","#Delta#Phi(MET,j_{1,2})", "rad",    16,0,3.2,       &(myEvent.deltaPhiMETJets),        "");
     screwdriver.AddVariable("MT2W",           "M_{T2}^{W}",              "GeV",    20,0,500,       &(myEvent.MT2W),                   "");
     screwdriver.AddVariable("HTratio",        "H_{T}^{ratio}",           "",       20,0,1.2,       &(myEvent.HTRatio),                "");
     screwdriver.AddVariable("HadronicChi2",   "Hadronic #chi^{2}",       "",       40,0,20,        &(myEvent.hadronicChi2),           "");
     screwdriver.AddVariable("HT",             "H_{T}",                   "",       45,150,1500,    &(myEvent.HT),                     "");
     screwdriver.AddVariable("leadingBPt",     "p_{T}(leading b-jet)",    "GeV",    20,0,200,       &(myEvent.leadingBPt),             "");
     screwdriver.AddVariable("leadingJetPt",   "p_{T}(leading jet)",      "GeV",    20,0,200,       &(myEvent.leadingJetPt),           "");
     screwdriver.AddVariable("leptonPt",       "p_{T}(lepton)",           "GeV",    20,5,205,       &(myEvent.leadingLeptonPt),        "");
     screwdriver.AddVariable("Mlb",            "M_{lb}",                  "GeV",    25,0,500,       &(myEvent.Mlb),                    "");
     screwdriver.AddVariable("Mlb_hemi",       "M_{lb}_hemi",             "GeV",    25,0,500,       &(myEvent.Mlb_hemi),               "");
     screwdriver.AddVariable("M3b",            "M3b",                     "GeV",    25,0,500,       &(myEvent.M3b),                    "");
     screwdriver.AddVariable("deltaRLeptonB",  "#DeltaR(l,leading b)",    "",       25,0,5,         &(myEvent.deltaRLeptonLeadingB),   "");
     screwdriver.AddVariable("HTLeptonPtMET",  "HT + MET + p_{T}(lepton)","GeV",    20,100,2100,    &(myEvent.HTPlusLeptonPtPlusMET),  "");
     screwdriver.AddVariable("METoverSqrtHT",  "MET / #sqrt{H_{T}}",      "",       32,0,32,        &(myEvent.METoverSqrtHT),          "");
     screwdriver.AddVariable("nPV",            "Number of primary vertices","",     50,0,49,        &(myEvent.numberOfPrimaryVertices),"");
     int nJets;
     screwdriver.AddVariable("nJets",          "Number of selected jets",  "",      6,0,5,          &(nJets),                          "");
    
     float invariantMass;
     screwdriver.AddVariable("m_ll",           "m_{ll}",                  "GeV",    50,50,150,      &(invariantMass),        "");


     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     screwdriver.AddProcessClass("1ltop", "1l top",                             "background",COLORPLOT_RED);
            screwdriver.AddDataset("ttbar_powheg",   "1ltop",  0, 0);
//            screwdriver.AddDataset("singleTop_st",   "1ltop",  0, 0);
   
     screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",COLORPLOT_CYAN);
   
     screwdriver.AddProcessClass("W+jets",   "W+jets",                          "background",COLORPLOT_ORANGE);
//             screwdriver.AddDataset("W+jets",    "W+jets", 0, 0);

     screwdriver.AddProcessClass("rare",   "rare",                              "background",COLORPLOT_MAGENTA);
             screwdriver.AddDataset("rare",   "rare", 0, 0);
   
     screwdriver.AddProcessClass("data",   "data",                              "data",COLORPLOT_BLACK);
//             screwdriver.AddDataset("SingleElec",   "data", 0, 0);
//             screwdriver.AddDataset("SingleMuon",   "data", 0, 0);
             screwdriver.AddDataset("DoubleMuon",   "data", 0, 0);
             screwdriver.AddDataset("DoubleElec",   "data", 0, 0);
             screwdriver.AddDataset("MuEl",         "data", 0, 0);


  // ##########################
  // ##    Create Regions    ##
  // ##########################

     //screwdriver.AddRegion("MTpeak",      "Pre-selection;MT peak control region",    &goesInPreselectionMTpeak      );
     //screwdriver.AddRegion("0btag",       "Pre-selection;0 b-tag control region",    &goesIn0BtagControlRegionMTtail);
     screwdriver.AddRegion("2leptons",    "Pre-selection;2l control region",         &goesInDileptonControlRegion2   );
     //screwdriver.AddRegion("lepton+veto", "Pre-selection;1l+veto control region",    &goesInVetosControlRegion      );
     //screwdriver.AddRegion("looseDilepton", "Loose dilepton check",         &goesInLooseDileptonRegion   );

  // ##########################
  // ##   Create Channels    ##
  // ##########################
  
     //screwdriver.AddChannel("singleLepton", "e/#mu-channels",         &goesInSingleLeptonChannel);
     //screwdriver.AddChannel("singleElec",   "e-channel",              &goesInSingleElecChannel  );
     //screwdriver.AddChannel("singleMuon",   "#mu-channel",            &goesInSingleMuonChannel  );
     
     screwdriver.AddChannel("doubleLepton", "ee/#mu#mu/e#mu-channels",&goesInDoubleLeptonChannel);
     screwdriver.AddChannel("doubleElec",   "ee-channel",             &goesInDoubleElecChannel  );
     screwdriver.AddChannel("doubleMuon",   "#mu#mu-channel",         &goesInDoubleMuonChannel  );
     screwdriver.AddChannel("elecMuon",     "e#mu-channel",           &goesInMuonElecChannel    );

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

          //if (i / (float) nEntries > 0.1) break;

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);
          float weight = getWeight();
          nJets = myEvent.nJets;
     
          invariantMass = (myEvent.leadingLepton + myEvent.secondLepton).M();

          // Split 1-lepton ttbar and 2-lepton ttbar
          string currentProcessClass_ = currentProcessClass;
          if ((currentDataset == "ttbar_powheg") && (myEvent.numberOfGenLepton == 2)) 
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
  screwdriver.WritePlots("../plots/dataMCChecks/");

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################
  
  //printBoxedMessage("Now computing misc tests ... ");

  //printBoxedMessage("Program done.");
  return (0);
}


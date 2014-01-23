
// C/C++ headers

#include <cmath>
#include <iomanip>
#include <iostream>
#include <time.h>
#include <set>
using namespace std;

// ROOT headers

#include <TFile.h>
#include <TRandom.h>
#include <TMarker.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLorentzVector.h>
#include <TMVA/Reader.h>

// Sonic Screwdriver headers

#include "interface/Table.h" 
#include "interface/SonicScrewdriver.h" 
using namespace theDoctor;

// BabyTuple format and location

//#define FOLDER_BABYTUPLES "../store/babyTuples_1102/"
#define FOLDER_BABYTUPLES "../store/babyTuples_1102_skimmed/"
#include "Reader.h"
babyEventSkimmed* myEventPointer;

// #########################################################################
//                          Region selectors
// #########################################################################

bool inclusiveChannelSelector() { return true; }

bool Selector_presel() 
{
    babyEventSkimmed myEvent = *myEventPointer;

    // Reject event that don't pass the trigger
    if ((!myEvent.triggerMuon) && (!myEvent.triggerElec)) return false;     // TODO : add xtrigger + switch to trigger efficiency
    
    // Require nLepton == 1
    if (myEvent.numberOfLepton != 1)                      return false;

    // Require nJets >= 4, nBTag >= 1
    if ((myEvent.nJets <= 3) || (myEvent.nBTag == 0))     return false; 

    // Apply vetos
    if ((!myEvent.isolatedTrackVeto) || (!myEvent.tauVeto))return false;

    // Apply MET and MT cuts
    if ((myEvent.MET < 80) || (myEvent.MT < 100))         return false;

    if (myEvent.deltaPhiMETJets < 0.8) return false;
    if (myEvent.hadronicChi2    > 5) return false;

    return true; 
}

bool Selector_bestScenar1_450_100_METoverSqrtHT()
{
    //if (myEventPointer->METoverSqrtHT < 13 ) return false;
    if (myEventPointer->MT            < 140) return false;
    if (myEventPointer->MT2W          < 200) return false;
    return Selector_presel();
}

bool Selector_bestScenar1_450_100_MT()
{
    if (myEventPointer->METoverSqrtHT < 13 ) return false;
    //if (myEventPointer->MT            < 140) return false;
    if (myEventPointer->MT2W          < 200) return false;
    return Selector_presel();
}

bool Selector_bestScenar1_450_100_MT2W()
{
    if (myEventPointer->METoverSqrtHT < 13 ) return false;
    if (myEventPointer->MT            < 140) return false;
    //if (myEventPointer->MT2W          < 200) return false;
    return Selector_presel();
}


bool Selector_SR(int i)
{
    if (myEventPointer->mStop == -1) return true;

    float deltaM = myEventPointer->mStop - myEventPointer->mNeutralino;

    if (deltaM < i*100 + 25) return false;
    if (deltaM > i*100 + 75) return false;

    return true;
}

bool Selector_SR1() { return Selector_SR(1); }
bool Selector_SR2() { return Selector_SR(2); }
bool Selector_SR3() { return Selector_SR(3); }
bool Selector_SR4() { return Selector_SR(4); }
bool Selector_SR5() { return Selector_SR(5); }
bool Selector_SR6() { return Selector_SR(6); }

bool Selector_MTAnalysis()
{
    if (myEventPointer->MT              < 120) return false;
    if (myEventPointer->MET             < 300) return false;
    if (myEventPointer->MT2W            < 200) return false;
    if (myEventPointer->deltaPhiMETJets < 0.8) return false;
    if (myEventPointer->hadronicChi2    > 5) return false;

    return Selector_presel();
}

// #########################################################################
//                          Others tools/stuff
// #########################################################################

void fillMCSignalTable(SonicScrewdriver* screwdriver, vector<string> region, vector<string> process, Table* table);
float stopCrossSection(float inputMass);
void printProgressBar(int current, int max);

/* 
void optimizeCuts(vector< vector<float> > listBackground, vector< vector<float> > listSignal);
void optimizeCutsTriangular(vector< vector<float> > listBackground, vector< vector<float> > listSignal);
void optimizeCutsTriangular2(vector< vector<float> > listBackground, vector< vector<float> > listSignal);
*/

float getYield(vector< vector<float> > listEvent, vector<float> cuts);
vector<float> optimizeCuts(vector< vector<float> > listBackground, vector< vector<float> > listSignal, bool* use, float* bestFOM, float* bestYieldSig, float* bestYieldBkg);



// #########################################################################
//                              Main function
// #########################################################################

int main (int argc, char *argv[])
{

  cout << endl;
  cout << "   ┌──────────────────────────────┐  " << endl;
  cout << "   │   Starting plot generation   │  " << endl;
  cout << "   └──────────────────────────────┘  " << endl; 
  cout << endl;


  // ####################
  // ##   Init tools   ##
  // ####################
  
     // Create a sonic Screwdriver
      SonicScrewdriver screwdriver;

       // Create a container for the event
     babyEventSkimmed myEvent;
     myEventPointer = &myEvent;

  // ##########################
  // ##   Create Variables   ##
  // ##########################

     screwdriver.AddVariable("MET",            "MET",                     "GeV",    15,50,500,      &(myEvent.MET),                  "logY=true");
     screwdriver.AddVariable("MT",             "MT",                      "GeV",    17,0,510,       &(myEvent.MT),                   "logY=true");
     screwdriver.AddVariable("deltaPhiMETJets","#Delta#Phi(MET,j_{1,2})", "rad",    16,0,3.2,       &(myEvent.deltaPhiMETJets),      "");
     screwdriver.AddVariable("MT2W",           "M_{T2}^{W}",              "GeV",    20,0,500,       &(myEvent.MT2W),                 "");
     screwdriver.AddVariable("HT",             "H_{T}",                   "",       45,150,1500,    &(myEvent.HT),                   "");
     screwdriver.AddVariable("HTratio",        "H_{T}^{ratio}",           "",       20,0,1.2,       &(myEvent.HTRatio),              "");
     screwdriver.AddVariable("HadronicChi2",   "Hadronic #chi^{2}",       "",       40,0,20,        &(myEvent.hadronicChi2),         "");
     screwdriver.AddVariable("leadingBPt",     "p_{T}(leading b-jet)",    "GeV",    25,0,500,       &(myEvent.leadingBPt),           "");
     screwdriver.AddVariable("leadingJetPt",   "p_{T}(leading jet)",      "GeV",    25,0,500,       &(myEvent.leadingJetPt),         "");
     screwdriver.AddVariable("leptonPt",       "p_{T}(lepton)",           "GeV",    25,0,500,       &(myEvent.leadingLeptonPt),      "");

     screwdriver.AddVariable("Mlb",            "M_{lb}",                  "GeV",    25,0,500,       &(myEvent.Mlb),                  "");
     screwdriver.AddVariable("Mlb_hemi",       "M_{lb}_hemi",             "GeV",    25,0,500,       &(myEvent.Mlb_hemi),             "");
     screwdriver.AddVariable("M3b",            "M3b",                     "GeV",    25,0,500,       &(myEvent.M3b),                  "");
     screwdriver.AddVariable("deltaRLeptonB",  "#DeltaR(l,leading b)",    "",       25,0,5,         &(myEvent.deltaRLeptonLeadingB), "");
     screwdriver.AddVariable("METoverSqrtHT",  "MET / #sqrt{H_{T}}",      "",       32,0,32,        &(myEvent.METoverSqrtHT),        "");
     float METoverHT;
     screwdriver.AddVariable("METoverHT",      "MET / H_{T}",             "",       20,0,5,         &(METoverHT),                    "");

     screwdriver.AddVariable("HTLeptonPtMET",  "HT + MET + p_{T}(lepton)","GeV",    20,100,2100,    &(myEvent.HTPlusLeptonPtPlusMET),"");

     //screwdriver.AddVariable("nJets",          "# of jets",               "",       4, 2.5,6.5,     &(myEvent.nJets),                "");
     //screwdriver.AddVariable("nBTag",          "# of b-tagged jets",      "",       4,-0.5,3.5,     &(myEvent.nBTag),                "");

     screwdriver.AddVariable("mStop",          "m_{#tilde{t}}",           "GeV",    28,112.5,812.5,  &(myEvent.mStop),                "");
     screwdriver.AddVariable("mNeutralino",    "m_{#chi^{0}}",            "GeV",    16,-12.5,387.5,  &(myEvent.mNeutralino),         "");
     float deltaM;
     screwdriver.AddVariable("deltaM",         "#Delta M",                "GeV",    16,25,825,       &(deltaM),         "");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     screwdriver.AddProcessClass("ttbar_1l", "t#bar{t} #rightarrow l+jets","background",kRed-7);
            screwdriver.AddDataset("ttbar",    "ttbar_1l",  0, 0);
     screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}",    "background",kCyan-3);

     screwdriver.AddProcessClass("W+jets",   "W+jets",                     "background",kOrange-2);
             screwdriver.AddDataset("Wjets",    "W+jets", 0, 0);

     screwdriver.AddProcessClass("others",   "others",                     "background",kMagenta-5);
             screwdriver.AddDataset("others",   "others", 0, 0);

     //screwdriver.AddProcessClass("T2tt",     "T2tt",                       "signal",kViolet-1);
     
             screwdriver.AddDataset("T2tt",     "T2tt",   0, 0);
     screwdriver.AddProcessClass("signal_250_100",  "T2tt (250/100)",             "signal",COLORPLOT_AZURE);
     screwdriver.AddProcessClass("signal_450_100",  "T2tt (450/100)",             "signal",kCyan-3);
     screwdriver.AddProcessClass("signal_650_100",  "T2tt (650/100)",             "signal",COLORPLOT_GREEN);

  // ##########################
  // ##    Create Regions    ##
  // 
  // ##########################

     screwdriver.AddRegion("presel",             "Preselection",               &Selector_presel);
     screwdriver.AddRegion("scenar1_XX_140_200", "Best scenar1 at (450,100)",  &Selector_bestScenar1_450_100_METoverSqrtHT);
     screwdriver.AddRegion("scenar1_13_XXX_200", "Best scenar1 at (450,100)",  &Selector_bestScenar1_450_100_MT);
     screwdriver.AddRegion("scenar1_13_140_XXX", "Best scenar1 at (450,100)",  &Selector_bestScenar1_450_100_MT2W);

  // ##########################
  // ##   Create Channels    ##
  // ##########################
   
     screwdriver.AddChannel("inclusiveChannel","",&inclusiveChannelSelector);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     screwdriver.SetLumi(20000);

     // Create histograms
     screwdriver.Create1DHistos();
     screwdriver.Add2DHisto("mStop","mNeutralino");
     screwdriver.Add2DHisto("deltaM","MET");
     screwdriver.Add2DHisto("deltaM","MT");
     screwdriver.Add2DHisto("deltaM","deltaPhiMETJets");
     screwdriver.Add2DHisto("deltaM","MT2W");
     screwdriver.Add2DHisto("deltaM","HTratio");
     screwdriver.Add2DHisto("deltaM","leadingBPt");
     screwdriver.Add2DHisto("deltaM","METoverSqrtHT");
     screwdriver.Add2DHisto("deltaM","HTLeptonPtMET");
     
     screwdriver.Add2DHisto("METoverSqrtHT","MT");
     screwdriver.Add2DHisto("METoverSqrtHT","MT2W");
     screwdriver.Add2DHisto("METoverSqrtHT","HTLeptonPtMET");
     screwdriver.Add2DHisto("MT","MT2W");
     screwdriver.Add2DHisto("MT","HTLeptonPtMET");
     screwdriver.Add2DHisto("MT2W","HTLeptonPtMET");

     screwdriver.SetGlobalBoolOption  ("1DSuperimposed",   "includeSignal",                   true   );

     screwdriver.SetGlobalStringOption("1DStack",          "includeSignal",                   "stack");
     screwdriver.SetGlobalFloatOption ("1DStack",          "factorSignal",                    1.0    );

     screwdriver.SetGlobalStringOption("DataMCComparison", "includeSignal",                   "stack");
     screwdriver.SetGlobalFloatOption ("DataMCComparison", "factorSignal",                    1.0    );

     screwdriver.SetGlobalFloatOption ("FigureOfMerit",    "backgroundSystematicUncertainty", 0.15   );

     // Schedule plots
     screwdriver.SchedulePlots("1DSuperimposed");
     screwdriver.SchedulePlots("1DStack");
     screwdriver.SchedulePlots("2D");
     screwdriver.SchedulePlots("2DSuperimposed");
     screwdriver.SchedulePlots("1DFigureOfMerit","var=METoverSqrtHT,cutType=keepHighValues");
     screwdriver.SchedulePlots("1DFigureOfMerit","var=METoverHT,cutType=keepHighValues");
     screwdriver.SchedulePlots("1DFigureOfMerit","var=MET,cutType=keepHighValues");
     screwdriver.SchedulePlots("1DFigureOfMerit","var=HT,cutType=keepHighValues");

     // Config plots

     screwdriver.SetGlobalStringOption("Plot", "infoTopRight", "CMS Internal");
     screwdriver.SetGlobalStringOption("Plot", "infoTopLeft",  "#sqrt{s} = 8 TeV, L = 20 fb^{-1}");

     screwdriver.SetGlobalBoolOption("Plot", "exportPdf", true);
     screwdriver.SetGlobalBoolOption("Plot", "exportEps", false);
     screwdriver.SetGlobalBoolOption("Plot", "exportPng", false);

  // ########################################
  // ##       Run over the datasets        ##
  // ########################################

  vector<string> datasetsList;
  screwdriver.GetDatasetList(&datasetsList);

  cout << "   > Running on dataset : " << endl;

  vector< vector<float> > listBackground;
  vector< vector<float> > listSignal;

  float yieldSignalMTAnalysis = 0.0; 
  float yieldBackgroundMTAnalysis = 0.0; 

  for (unsigned int d = 0 ; d < datasetsList.size() ; d++)
  {
     string currentDataset = datasetsList[d];
     string currentProcessClass = screwdriver.GetProcessClass(currentDataset); 
     // Open the tree
     TFile f((string(FOLDER_BABYTUPLES)+currentDataset+".root").c_str());
     TTree* theTree = (TTree*) f.Get("babyTuple"); 

     intermediatePointersSkimmed pointers;
     InitializeBranches(theTree,&myEvent,&pointers);

     cout << "                    " << currentDataset << endl; 

  // ########################################
  // ##        Run over the events         ##
  // ########################################

      for (int i = 0 ; i < theTree->GetEntries() ; i++)
      //for (int i = 0 ; i < 20000; i++)
      {
          if (i % (theTree->GetEntries() / 50) == 0) 
              printProgressBar(i,theTree->GetEntries());

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          // Keep only events that pass preselection
          if (!Selector_presel()) continue;
  
          float weight = myEvent.weightCrossSection * screwdriver.GetLumi();

          // Don't take x-sec into account for signal
          //if ((currentDataset == "T2tt") || (currentDataset == "T2bw-050"))
          //    weight = 0.01 * screwdriver.GetLumi() / myEvent.numberOfInitialEvents;

          // Split 1-lepton ttbar and 2-lepton ttbar
          string currentProcessClass_ = currentProcessClass;
          if ((currentDataset == "ttbar") && (myEvent.numberOfGenLepton == 2)) 
              currentProcessClass_ = "ttbar_2l";

          if (myEvent.hadronicChi2  >= 20) myEvent.hadronicChi2  = 19.99;
          if (myEvent.METoverSqrtHT >= 32) myEvent.METoverSqrtHT = 31.99;

          deltaM = myEvent.mStop - myEvent.mNeutralino;

          METoverHT = myEvent.MET / myEvent.HT;

          // Fill all the variables with autoFill-mode activated
          if (currentDataset != "T2tt")
              screwdriver.AutoFillProcessClass(currentProcessClass_,weight);

          // Store stuff for cut optimization
          vector<float> values;
          values.push_back(myEvent.METoverSqrtHT);
          values.push_back(myEvent.MT);
          values.push_back(myEvent.MT2W);
          values.push_back(myEvent.HTPlusLeptonPtPlusMET);
          values.push_back(myEvent.HTRatio);
          values.push_back(weight);

          float stopMassForTest = 450;
          float neutralinoMassForTest = 100;

          if ((currentDataset == "T2tt") && (myEvent.mStop == stopMassForTest) && (myEvent.mNeutralino == neutralinoMassForTest)) listSignal.push_back(values);
          else if  (currentDataset != "T2tt")                       listBackground.push_back(values);

          if ((myEvent.mStop == 250) && (myEvent.mNeutralino == 100))
              screwdriver.AutoFillProcessClass("signal_250_100",weight);
          if ((myEvent.mStop == 450) && (myEvent.mNeutralino == 100))
              screwdriver.AutoFillProcessClass("signal_450_100",weight);
          if ((myEvent.mStop == 650) && (myEvent.mNeutralino == 100))
              screwdriver.AutoFillProcessClass("signal_650_100",weight);

          if (Selector_MTAnalysis())
          {
                if ((currentDataset == "T2tt") && (myEvent.mStop == stopMassForTest) && (myEvent.mNeutralino == neutralinoMassForTest))
                    yieldSignalMTAnalysis += weight;
           else if (currentDataset != "T2tt")
                    yieldBackgroundMTAnalysis += weight;

          }
      } 
      
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
  screwdriver.WritePlots("../plots/cutAndCount/");

  cout << endl;
  cout << "   ┌──────────────────────────────┐  " << endl;
  cout << "   │   Plot generation completed  │  " << endl;
  cout << "   └──────────────────────────────┘  " << endl; 
  cout << endl;

  float FOMMTAnalysis = yieldSignalMTAnalysis / sqrt(yieldBackgroundMTAnalysis + 0.15*0.15*yieldBackgroundMTAnalysis*yieldBackgroundMTAnalysis);
  if (yieldBackgroundMTAnalysis < 1) FOMMTAnalysis = yieldSignalMTAnalysis / sqrt(1.0 + 0.15*0.15);
  //if (yieldSignalMTAnalysis < 3) FOMMTAnalysis = 0;

  cout << "MT analysis yields (sig, bkg) : (" << yieldSignalMTAnalysis << "," << yieldBackgroundMTAnalysis << ")" << endl;
  cout << "                        FOM   :  " << FOMMTAnalysis << endl;

  //optimizeCuts(listBackground,listSignal);
  //optimizeCutsTriangular(listBackground,listSignal);
  //optimizeCutsTriangular2(listBackground,listSignal);
 
  float bestFOM, bestYieldSig, bestYieldBkg;
/*
  bool scenario_1_1[5]   = {1,0,0,0,0}; vector<float> cuts_1_1   = optimizeCuts(listBackground, listSignal, scenario_1_1, &bestFOM, &bestYieldSig, &bestYieldBkg  );
  cout << "MET/sqrt(HT) - " << cuts_1_1[0] << "  -  FOM,yieldSig,yieldBkg - " << bestFOM << " - " << bestYieldSig << " ; " << bestYieldBkg << endl; 
  
  bool scenario_2_12[5]  = {1,1,0,0,0}; vector<float> cuts_2_12  = optimizeCuts(listBackground, listSignal, scenario_2_12, &bestFOM, &bestYieldSig, &bestYieldBkg   );
  cout << "MET/sqrt(HT),MT - " << cuts_2_12[0] << " ; " << cuts_2_12[1] << "  -  FOM,yieldSig,yieldBkg - " << bestFOM << " - " << bestYieldSig << " ; " << bestYieldBkg << endl; 
  
  bool scenario_2_13[5]  = {1,0,1,0,0}; vector<float> cuts_2_13  = optimizeCuts(listBackground, listSignal, scenario_2_13, &bestFOM, &bestYieldSig, &bestYieldBkg   );
  cout << "MET/sqrt(HT),MT2W - " << cuts_2_13[0] << " ; " << cuts_2_13[2] << "  -  FOM,yieldSig,yieldBkg - " << bestFOM << " - " << bestYieldSig << " ; " << bestYieldBkg << endl; 
  
  bool scenario_2_14[5]  = {1,0,0,1,0}; vector<float> cuts_2_14  = optimizeCuts(listBackground, listSignal, scenario_2_14, &bestFOM, &bestYieldSig, &bestYieldBkg   );
  cout << "MET/sqrt(HT),HT+pT(lep)+MET - " << cuts_2_14[0] << " ; " << cuts_2_14[3] << "  -  FOM,yieldSig,yieldBkg - " << bestFOM << " - " << bestYieldSig << " ; " << bestYieldBkg << endl; 
*/
  bool scenario_3_123[5] = {1,1,1,0,0}; vector<float> cuts_3_123 = optimizeCuts(listBackground, listSignal, scenario_3_123, &bestFOM, &bestYieldSig, &bestYieldBkg  );
  cout << "MET/sqrt(HT),MT,MT2W - " << cuts_3_123[0] << " ; " << cuts_3_123[1] << " ; " << cuts_3_123[2] << "  -  FOM,yieldSig,yieldBkg - " << bestFOM << " - " << bestYieldSig << " ; " << bestYieldBkg << endl; 
  /*
  bool scenario_3_134[5] = {1,0,1,1,0}; vector<float> cuts_3_134 = optimizeCuts(listBackground, listSignal, scenario_3_134, &bestFOM, &bestYieldSig, &bestYieldBkg  );
  cout << "MET/sqrt(HT),MT2W,HT+pT(lep)+MET - " << cuts_3_134[0] << " ; " << cuts_3_134[2] << " ; " << cuts_3_134[3] << "  -  FOM,yieldSig,yieldBkg - " << bestFOM << " - " << bestYieldSig << " ; " << bestYieldBkg << endl; 
  
  bool scenario_3_145[5] = {1,0,0,1,1}; vector<float> cuts_3_145 = optimizeCuts(listBackground, listSignal, scenario_3_145, &bestFOM, &bestYieldSig, &bestYieldBkg  );
  cout << "MET/sqrt(HT),HT+pT(lep)+MET,HTRatio - " << cuts_3_145[0] << " ; " << cuts_3_145[3] << " ; " << cuts_3_145[4] << "  -  FOM,yieldSig,yieldBkg - " << bestFOM << " - " << bestYieldSig << " ; " << bestYieldBkg << endl; 
*/
  return (0);
}

vector<float> optimizeCuts(vector< vector<float> > listBackground, vector< vector<float> > listSignal, bool* use, float* bestFOM, float* bestYieldSig, float* bestYieldBkg)
{
    vector<float> bestCuts;
    (*bestFOM)      = 0.0;
    (*bestYieldSig) = 0.0;
    (*bestYieldBkg) = 0.0;

    vector<float> cuts;
    cuts.push_back(0);
    cuts.push_back(0);
    cuts.push_back(0);
    cuts.push_back(0);
    cuts.push_back(0);
    bestCuts = cuts;

    /*
    for (cuts[0] = 13  ; cuts[0] <= 13  ; cuts[0] += 1  ) {
    for (cuts[1] = 140 ; cuts[1] <= 140 ; cuts[1] += 40 ) {
    for (cuts[2] = 200 ; cuts[2] <= 200 ; cuts[2] += 50 ) {
    for (cuts[3] = 100 ; cuts[3] <= 100 ; cuts[3] += 200) {
    for (cuts[4] = 1.2 ; cuts[4] >= 1.2 ; cuts[4] -= 0.2)
   */ 
    for (cuts[0] = 5   ; cuts[0] <= (use[0] ? 20   : 0  ) ; cuts[0] += 1  ) {
    for (cuts[1] = 100 ; cuts[1] <= (use[1] ? 400  : 100) ; cuts[1] += 40 ) {
    for (cuts[2] = 0   ; cuts[2] <= (use[2] ? 400  : 0  ) ; cuts[2] += 50 ) {
    for (cuts[3] = 100 ; cuts[3] <= (use[3] ? 1100 : 100) ; cuts[3] += 200) {
    for (cuts[4] = 1.2 ; cuts[4] >= (use[4] ? 0    : 1.2) ; cuts[4] -= 0.2)
    {
        float yieldBackground = getYield(listBackground,cuts);
        float yieldSignal     = getYield(listSignal,cuts);

        float FOM = yieldSignal / sqrt(yieldBackground + 0.15*0.15*yieldBackground*yieldBackground);
        if (yieldBackground < 1) FOM = yieldSignal / sqrt(1.0 + 0.15*0.15);
        if (yieldSignal < 3) FOM = 0;

        if (FOM > *bestFOM)
        { 
            bestCuts        = cuts; 
            (*bestFOM)      = FOM;
            (*bestYieldSig) = yieldSignal;
            (*bestYieldBkg) = yieldBackground;
        }
        
    } } } } }
//    } } }
        
    return bestCuts;
}

float getYield(vector< vector<float> > listEvent, vector<float> cuts)
{
    float yield = 0.0;

    for (unsigned int evt = 0 ; evt < listEvent.size() ; evt++)
    {
        vector<float> event = listEvent[evt];

        bool flagPassSelection = true;
        for (unsigned int i = 0 ; i < event.size()-1 ; i++)
        {
            if (i < event.size() - 2)
            {    if (event[i] < cuts[i]) { flagPassSelection = false; break; } }
            else
            {    if (event[i] > cuts[i]) { flagPassSelection = false; break; } }
        }

        if (flagPassSelection) yield += event[event.size() - 1];
    }

    return yield;
}



/*
void optimizeCutsTriangular(vector< vector<float> > listBackground, vector< vector<float> > listSignal)
{
 
  float bestCut_METoverSqrtHT = 0.0;
  float bestCut_HTLeptonPtMET  = 0.0;
  float bestYieldBackground   = 0.0;
  float bestYieldSignal       = 0.0;
  float bestFOM               = 0.0;

  for (float cut_METoverSqrtHT = 0   ; cut_METoverSqrtHT < 32   ; cut_METoverSqrtHT += 1 ) 
  for (float cut_HTLeptonPtMET = 100 ; cut_HTLeptonPtMET < 2100 ; cut_HTLeptonPtMET += 100)
  {
      float yieldBackground = 0;
      float yieldSignal = 0;

      for (unsigned int evt_bkg = 0 ; evt_bkg < listBackground.size() ; evt_bkg++)
      {
          float METoverSqrtHT = listBackground[evt_bkg][0];
          float MT            = listBackground[evt_bkg][1];
          float MT2W          = listBackground[evt_bkg][2];
          float HTLeptonPtMET = listBackground[evt_bkg][3];
          float w             = listBackground[evt_bkg][5];

          if ((METoverSqrtHT > cut_METoverSqrtHT) && (HTLeptonPtMET > cut_HTLeptonPtMET)) yieldBackground += w;
      }

      for (unsigned int evt_sig = 0 ; evt_sig < listSignal.size() ; evt_sig++)
      {
          float METoverSqrtHT = listSignal[evt_sig][0];
          float MT            = listSignal[evt_sig][1];
          float MT2W          = listSignal[evt_sig][2];
          float HTLeptonPtMET = listSignal[evt_sig][3];
          float w             = listSignal[evt_sig][5];

          if ((METoverSqrtHT > cut_METoverSqrtHT) && (HTLeptonPtMET > cut_HTLeptonPtMET)) yieldSignal += w;
      }

      float FOM = yieldSignal / sqrt(yieldBackground + 0.15*0.15*yieldBackground*yieldBackground);
    
      if (yieldBackground < 1) FOM = yieldSignal / sqrt(1.0 + 0.15*0.15);
      if (yieldSignal < 3) FOM = 0;

      cout << "( METoverSqrtHT ; HTLeptonPtMET ) > ( " << cut_METoverSqrtHT << " ; " << cut_HTLeptonPtMET << " )"
       << " |  ( sig ; bkg ; FOM ) = ( " << yieldSignal << " ; " << yieldBackground << " ; " << FOM << " )" << endl;

      if ((yieldSignal < 3) || (yieldBackground <= 1)) break;

      if (FOM > bestFOM)
      {
          bestCut_METoverSqrtHT = cut_METoverSqrtHT;
          bestCut_HTLeptonPtMET = cut_HTLeptonPtMET;
          bestYieldSignal       = yieldSignal;
          bestYieldBackground   = yieldBackground;
          bestFOM               = FOM;
      }

  }

  cout << "bestCut : " << endl;
  cout << "( METoverSqrtHT ; HTLeptonPtMET ) > ( " << bestCut_METoverSqrtHT << " ; " << bestCut_HTLeptonPtMET << " )"
       << " |  ( sig ; bkg ; FOM ) = ( " << bestYieldSignal << " ; " << bestYieldBackground << " ; " << bestFOM << " )" << endl;


}

void optimizeCutsTriangular2(vector< vector<float> > listBackground, vector< vector<float> > listSignal)
{
 
  float bestCut_a = 0.0;
  float bestCut_b = 0.0;
  float bestYieldBackground   = 0.0;
  float bestYieldSignal       = 0.0;
  float bestFOM               = 0.0;

  for (float b = 2000 ; b < 5000 ; b += 100) 
  for (float a = -350 ; a < -30  ; a += 4)
  {
      float yieldBackground = 0;
      float yieldSignal = 0;

      for (unsigned int evt_bkg = 0 ; evt_bkg < listBackground.size() ; evt_bkg++)
      {
          float METoverSqrtHT = listBackground[evt_bkg][0];
          float MT            = listBackground[evt_bkg][1];
          float MT2W          = listBackground[evt_bkg][2];
          float HTLeptonPtMET = listBackground[evt_bkg][3];
          float w             = listBackground[evt_bkg][5];

          if (a * METoverSqrtHT + b < HTLeptonPtMET) yieldBackground += w;
      }

      for (unsigned int evt_sig = 0 ; evt_sig < listSignal.size() ; evt_sig++)
      {
          float METoverSqrtHT = listSignal[evt_sig][0];
          float MT            = listSignal[evt_sig][1];
          float MT2W          = listSignal[evt_sig][2];
          float HTLeptonPtMET = listSignal[evt_sig][3];
          float w             = listSignal[evt_sig][5];

          if (a * METoverSqrtHT + b < HTLeptonPtMET) yieldSignal += w;
      }

      float FOM = yieldSignal / sqrt(yieldBackground + 0.15*0.15*yieldBackground*yieldBackground);
    
      if (yieldBackground < 1) FOM = yieldSignal / sqrt(1.0 + 0.15*0.15);
      if (yieldSignal < 3) FOM = 0;

      //cout << "( a ; b ) > ( " << a << " ; " << b << " )"
      // << " |  ( sig ; bkg ; FOM ) = ( " << yieldSignal << " ; " << yieldBackground << " ; " << FOM << " )" << endl;

//      if ((yieldSignal < 3) || (yieldBackground <= 1)) break;

      if (FOM > bestFOM)
      {
          bestCut_a = a;
          bestCut_b = b;
          bestYieldSignal       = yieldSignal;
          bestYieldBackground   = yieldBackground;
          bestFOM               = FOM;
      }

  }

  cout << "bestCut : " << endl;
  cout << "( a ; b) > ( " << bestCut_a << " ; " << bestCut_b << " )"
       << " |  ( sig ; bkg ; FOM ) = ( " << bestYieldSignal << " ; " << bestYieldBackground << " ; " << bestFOM << " )" << endl;


}

void optimizeCuts(vector< vector<float> > listBackground, vector< vector<float> > listSignal)
{
 
  float bestCut_METoverSqrtHT = 0.0;
  float bestCut_MT            = 0.0;
  float bestCut_MT2W          = 0.0;
  float bestYieldBackground   = 0.0;
  float bestYieldSignal       = 0.0;
  float bestFOM               = 0.0;

  for (float cut_METoverSqrtHT = 0   ; cut_METoverSqrtHT < 32  ; cut_METoverSqrtHT += 1 ) 
  for (float cut_MT            = 100 ; cut_MT            < 500 ; cut_MT            += 20) 
  for (float cut_MT2W          = 0   ; cut_MT2W < 500          ; cut_MT2W          += 25)
  {
      float yieldBackground = 0;
      float yieldSignal = 0;

      for (unsigned int evt_bkg = 0 ; evt_bkg < listBackground.size() ; evt_bkg++)
      {
          float METoverSqrtHT = listBackground[evt_bkg][0];
          float MT            = listBackground[evt_bkg][1];
          float MT2W          = listBackground[evt_bkg][2];
          float HTLeptonPtMET = listBackground[evt_bkg][3];
          float w             = listBackground[evt_bkg][5];

          if ((METoverSqrtHT > cut_METoverSqrtHT) && (MT > cut_MT) && (MT2W > cut_MT2W)) yieldBackground += w;
      }

      for (unsigned int evt_sig = 0 ; evt_sig < listSignal.size() ; evt_sig++)
      {
          float METoverSqrtHT = listSignal[evt_sig][0];
          float MT            = listSignal[evt_sig][1];
          float MT2W          = listSignal[evt_sig][2];
          float HTLeptonPtMET = listSignal[evt_sig][3];
          float w             = listSignal[evt_sig][5];

          if ((METoverSqrtHT > cut_METoverSqrtHT) && (MT > cut_MT) && (MT2W > cut_MT2W)) yieldSignal += w;
      }

      float FOM = yieldSignal / sqrt(yieldBackground + 0.15*0.15*yieldBackground*yieldBackground);
    
      if (yieldBackground < 1) FOM = yieldSignal / sqrt(1.0 + 0.15*0.15);
      if (yieldSignal < 3) FOM = 0;

      //cout << "( METoverSqrtHT ; MT ; MT2W ) > ( " << cut_METoverSqrtHT << " ; " << cut_MT << " ; " << cut_MT2W << " )"
      // << " |  ( sig ; bkg ; FOM ) = ( " << yieldSignal << " ; " << yieldBackground << " ; " << FOM << " )" << endl;

      if ((yieldSignal < 3) || (yieldBackground <= 1)) break;

      if (FOM > bestFOM)
      {
          bestCut_METoverSqrtHT = cut_METoverSqrtHT;
          bestCut_MT            = cut_MT;
          bestCut_MT2W          = cut_MT2W;
          bestYieldSignal       = yieldSignal;
          bestYieldBackground   = yieldBackground;
          bestFOM               = FOM;
      }

  }

  cout << "bestCut : " << endl;
  cout << "( METoverSqrtHT ; MT ; MT2W ) > ( " << bestCut_METoverSqrtHT << " ; " << bestCut_MT << " ; " << bestCut_MT2W << " )"
       << " |  ( sig ; bkg ; FOM ) = ( " << bestYieldSignal << " ; " << bestYieldBackground << " ; " << bestFOM << " )" << endl;




}
*/
void fillMCSignalTable(SonicScrewdriver* screwdriver, vector<string> region, vector<string> process, Table* table)
{
    string varUsedToGetYields = "BDTOutputAdaBoostNoWTag";
    string channelUsedToGetYields = "inclusiveChannel";

    for (unsigned int r = 0 ; r < region.size()          ; r++)
    {
        Figure tmpTotal(0.0,0.0);
        for (unsigned int p = 0 ; p < process.size() ; p++)
        {
            if (process[p] == "total") continue;
            table->Set(region[r],
                      process[p],
                      screwdriver->GetYieldAndError(varUsedToGetYields,
                                               process[p],
                                               region[r],
                                               channelUsedToGetYields));
            
            if ((process[p] != "signal_550_25") && (process[p] != "signal_650_25") && (process[p] != "signal_750_25"))
                tmpTotal += screwdriver->GetYieldAndError(varUsedToGetYields,
                                                     process[p],
                                                     region[r],
                                                     channelUsedToGetYields);
        }
        table->Set(region[r],"total",tmpTotal);
    }

}

void printProgressBar(int current, int max)
{
    std::string bar;
    int percent = 100 * (float) current / (float) max;

    for(int i = 0; i < 50; i++)
    {
        if( i < (percent/2))       bar.replace(i,1,"=");
        else if( i == (percent/2)) bar.replace(i,1,">");
        else                       bar.replace(i,1," ");
    }

    std::cout << "  [Progress]  ";
    std::cout << "[" << bar << "] ";
    std::cout.width( 3 );
    std::cout << percent << "%     ";
    std::cout << "(" << current << " / " << max << ")" << "\r" << std::flush;
}


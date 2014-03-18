
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

// Misc

#include "../common.h"

// BabyTuple format and location

#define FOLDER_BABYTUPLES "../store/babyTuples_0219_preSelectionSkimmed/"
#include "Reader.h"
babyEvent* myEventPointer;

// #########################################################################
//                          Region selectors
// #########################################################################

bool inclusiveChannelSelector() { return true; }

bool Selector_presel() 
{
    babyEvent myEvent = *myEventPointer;

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
    //if (myEvent.hadronicChi2    > 5) return false;

   /* 
    if (myEventPointer->nJets < 5) return false;

    bool foundISRJet = false;
    for (unsigned int i = 0 ; i < myEventPointer->jets.size() ; i++)
    {
       // Check jet is high-pt
      if ((myEventPointer->jets)[i].Pt() < 200) continue;
       // Check jet isn't b-tagged
      if ((myEventPointer->jets_CSV_reshaped)[i] > 0.679) continue;

      foundISRJet = true;
    }
    if (foundISRJet == false) return false;
    */

    return true; 
}

// #########################################################################
//                          Others tools/stuff
// #########################################################################

float getYield(vector< vector<float> > listEvent, vector<float> cuts);
vector<float> optimizeCuts(vector< vector<float> > listBackground,  vector< vector<float> > listSignal, bool* use, float* bestFOM, float* bestYieldSig, float* bestYieldBkg);

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

     /*
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
     screwdriver.AddVariable("HTLeptonPtMET",  "HT + MET + p_{T}(lepton)","GeV",    20,100,2100,    &(myEvent.HTPlusLeptonPtPlusMET),"");
     */

     screwdriver.AddVariable("mStop",          "m_{#tilde{t}}",           "GeV",    28,112.5,812.5,  &(myEvent.mStop),                "");
     screwdriver.AddVariable("mNeutralino",    "m_{#chi^{0}}",            "GeV",    16,-12.5,387.5,  &(myEvent.mNeutralino),         "");

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
/*
     screwdriver.AddProcessClass("T2bw-025",     "T2bw (x=0.25)",          "signal",COLORPLOT_AZURE);
          screwdriver.AddDataset("T2bw-025",     "T2bw-025",   0, 0);
*/
     screwdriver.AddProcessClass("T2bw-050",     "T2bw (x=0.50)",          "signal",kCyan-3);
          screwdriver.AddDataset("T2bw-050",     "T2bw-050",   0, 0);
/*
     screwdriver.AddProcessClass("T2bw-075",     "T2bw (x=0.75)",          "signal",COLORPLOT_GREEN);
          screwdriver.AddDataset("T2bw-075",     "T2bw-075",   0, 0);
*/

  // ##########################
  // ##    Create Regions    ##
  // ##########################

     screwdriver.AddRegion("presel",             "Preselection",                 &Selector_presel);

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

     // Config plots

     screwdriver.SetGlobalStringOption("Plot", "infoTopRight", "CMS Internal");
     screwdriver.SetGlobalStringOption("Plot", "infoTopLeft",  "#sqrt{s} = 8 TeV, L = 20 fb^{-1}");

     screwdriver.SetGlobalBoolOption("Plot", "exportPdf", true);
     screwdriver.SetGlobalBoolOption("Plot", "exportEps", false);
     screwdriver.SetGlobalBoolOption("Plot", "exportPng", true);

  // ########################################
  // ##       Run over the datasets        ##
  // ########################################

  vector<string> datasetsList;
  screwdriver.GetDatasetList(&datasetsList);

  cout << "   > Running on dataset : " << endl;

  vector< vector<float> > listBackground;
  vector< vector<float> > listSignal;

  for (unsigned int d = 0 ; d < datasetsList.size() ; d++)
  {
     string currentDataset = datasetsList[d];
     string currentProcessClass = screwdriver.GetProcessClass(currentDataset); 
     // Open the tree
     TFile f((string(FOLDER_BABYTUPLES)+currentDataset+".root").c_str());
     TTree* theTree = (TTree*) f.Get("babyTuple"); 

     intermediatePointers pointers;
     InitializeBranches(theTree,&myEvent,&pointers);

     cout << "                    " << currentDataset << endl; 

  // ########################################
  // ##        Run over the events         ##
  // ########################################

      for (int i = 0 ; i < theTree->GetEntries() ; i++)
      //for (int i = 0 ; i < min(200000, (int) theTree->GetEntries()); i++)
      {
          if (i % (theTree->GetEntries() / 50) == 0) 
              printProgressBar(i,theTree->GetEntries());

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          // Keep only events that pass preselection
          if (!Selector_presel()) continue;

          // Weight to lumi and apply trigger efficiency
          float weight = myEvent.weightCrossSection * screwdriver.GetLumi() * myEvent.weightTriggerEfficiency;
          
          // Apply PU weight except for signal
          if  ((currentDataset != "T2bw-025")
            && (currentDataset != "T2bw-050")
            && (currentDataset != "T2bw-075")) weight *= myEvent.weightPileUp;
          
          // For ttbar, apply topPt reweighting
          if (currentDataset == "ttbar") weight *= myEvent.weightTopPt;

          // For signal, apply isr reweighting
          if  ((currentDataset == "T2bw-025")
            || (currentDataset == "T2bw-050")
            || (currentDataset == "T2bw-075")) weight *= myEvent.weightISRmodeling;

          // Split 1-lepton ttbar and 2-lepton ttbar
          string currentProcessClass_ = currentProcessClass;
          if ((currentDataset == "ttbar") && (myEvent.numberOfGenLepton == 2)) 
              currentProcessClass_ = "ttbar_2l";

          // Dirty overflow management
          if (myEvent.hadronicChi2  >= 20) myEvent.hadronicChi2  = 19.99;
          if (myEvent.METoverSqrtHT >= 32) myEvent.METoverSqrtHT = 31.99;

          // Fill all the variables with autoFill-mode activated
          //if (currentDataset != "T2tt")
          
          screwdriver.AutoFillProcessClass(currentProcessClass_,weight);

          // Store stuff for cut optimization
          
          vector<float> values;
          values.push_back(myEvent.METoverSqrtHT);
          values.push_back(myEvent.MET);
          values.push_back(myEvent.MT);
          values.push_back(myEvent.MT2W);
          values.push_back(myEvent.leadingBPt);
          values.push_back(myEvent.M3b);
          values.push_back(myEvent.Mlb);
          values.push_back(myEvent.Mlb_hemi);
          values.push_back(weight);

          float stopMassForTest = 300;
          float neutralinoMassForTest = 25;

          if ((currentDataset == "T2bw-050") && (myEvent.mStop == stopMassForTest) && (myEvent.mNeutralino == neutralinoMassForTest)) listSignal.push_back(values);
          else if  (currentDataset != "T2bw-050")  listBackground.push_back(values);

          /*
          if ((myEvent.mStop == 250) && (myEvent.mNeutralino == 100))
              screwdriver.AutoFillProcessClass("signal_250_100",weight);
          if ((myEvent.mStop == 450) && (myEvent.mNeutralino == 100))
              screwdriver.AutoFillProcessClass("signal_450_100",weight);
          if ((myEvent.mStop == 650) && (myEvent.mNeutralino == 100))
              screwdriver.AutoFillProcessClass("signal_650_100",weight);
          */
      } 
      
      cout << endl;
      f.Close();

  }

  // ###################################
  // ##   Make plots and write them   ##
  // ###################################
 
  cout << endl;
  cout << "   > Making plots..." << endl;
  //screwdriver.MakePlots();
  cout << "   > Saving plots..." << endl;
  //screwdriver.WritePlots("../plots/cutAndCount_T2tt/");

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################
  
  printBoxedMessage("Now computing misc tests ... ");
  
  // #####################################
  // ##   Other optimization stuff...   ##
  // #####################################
 
   {
      float bestFOM, bestYieldSig, bestYieldBkg;
      bool scenario[8] = {1,0,1,1,1,0,0,0};
      vector<float> cuts =  optimizeCuts(listBackground, listSignal, scenario, &bestFOM, &bestYieldSig, &bestYieldBkg  );
  
      cout << "  METsig   MET   MT   MT2W   BPt   M3b   Mlb   Mlbhemi" << endl;
      cout << "   "    << scenario[0] << "        " << scenario[1] << "    "   << scenario[2] << "     " << scenario[3] 
           << "      " << scenario[4] << "     "    << scenario[5] << "      " << scenario[6] << "     " << scenario[7] << endl;
      cout << "   " << cuts[0]     << "       " << cuts[1]     << "   "  << cuts[2]     << "   "  << cuts[3]     
          << "    " << cuts[4]     << "    "    << cuts[5]     << "    " << cuts[6]     << "    " << cuts[7]     << endl;

      cout << " FOM,yieldSig,yieldBkg - [" << bestFOM << "] - " << bestYieldSig << " ; " << bestYieldBkg << endl;
      cout << endl;
  }   

  printBoxedMessage("Program done.");
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
    cuts.push_back(0);
    cuts.push_back(0);
    cuts.push_back(0);
    bestCuts = cuts;

    for (cuts[0] = 0   ; cuts[0] <= (use[0] ? 20   : 0  ) ; cuts[0] += 1   ) {  // MET / sqrt(HT)
    for (cuts[1] = 100 ; cuts[1] <= (use[1] ? 400  : 100) ; cuts[1] += 25  ) {  // MET
    for (cuts[2] = 100 ; cuts[2] <= (use[2] ? 300  : 100) ; cuts[2] += 25  ) {  // MT
    for (cuts[3] = 100 ; cuts[3] <= (use[3] ? 300  : 100) ; cuts[3] += 25  ) {  // MT2W
    for (cuts[4] = 30  ; cuts[4] <= (use[4] ? 330  : 30 ) ; cuts[4] += 25  ) {  // leadingBPt
    for (cuts[5] = 0   ; cuts[5] <= (use[5] ? 300  : 0 )  ; cuts[5] += 25  ) {  // M3b
    for (cuts[6] = 0   ; cuts[6] <= (use[6] ? 300  : 0 )  ; cuts[6] += 25  ) {  // Mlb
    for (cuts[7] = 0   ; cuts[7] <= (use[7] ? 300  : 0 )  ; cuts[7] += 25  ) {  // Mlb_hemi
        /*
    for (cuts[0] = 0   ; cuts[0] <= (use[0] ? 20   : 0  ) ; cuts[0] += 2   ) {  // MET / sqrt(HT)
    for (cuts[1] = 100 ; cuts[1] <= (use[1] ? 400  : 100) ; cuts[1] += 50  ) {  // MET
    for (cuts[2] = 100 ; cuts[2] <= (use[2] ? 400  : 100) ; cuts[2] += 50  ) {  // MT
    for (cuts[3] = 100 ; cuts[3] <= (use[3] ? 300  : 100) ; cuts[3] += 50  ) {  // MT2W
    for (cuts[4] = 30  ; cuts[4] <= (use[4] ? 430  : 30 ) ; cuts[4] += 50  ) {  // leadingBPt
    for (cuts[5] = 0   ; cuts[5] <= (use[5] ? 500  : 0 )  ; cuts[5] += 50  ) {  // M3b
    for (cuts[6] = 0   ; cuts[6] <= (use[6] ? 500  : 0 )  ; cuts[6] += 50  ) {  // Mlb
    for (cuts[7] = 0   ; cuts[7] <= (use[7] ? 500  : 0 )  ; cuts[7] += 50  ) {  // Mlb_hemi
    */
    {
        float yieldBackground  = getYield(listBackground,cuts);
        float yieldSignal      = getYield(listSignal,cuts);

        if (yieldBackground < 1.0) yieldBackground = 1.0;

        float FOM = yieldSignal / sqrt(yieldBackground + 0.15*0.15*yieldBackground*yieldBackground);
        if (yieldSignal < 3) FOM = 0;

        if (FOM > *bestFOM)
        { 
            bestCuts        = cuts; 
            (*bestFOM)      = FOM;
            (*bestYieldSig) = yieldSignal;
            (*bestYieldBkg) = yieldBackground;
        }
        
    } } } } } } } } }
        
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
            if (event[i] < cuts[i]) { flagPassSelection = false; break; }
        }

        if (flagPassSelection) yield += event[event.size() - 1];
    }

    return yield;
}



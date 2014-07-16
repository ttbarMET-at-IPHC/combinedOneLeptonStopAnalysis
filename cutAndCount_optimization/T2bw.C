#include "common.h"
// ##################
// Others tools/stuff
// ##################

bool additionalCuts()
{
    //if (myEvent.deltaPhiMETJets < 0.2) return false;
    if (myEvent.deltaPhiMETJets < 0.8) return false;

    if (myEvent.MT < 120) return false;
    //if (myEvent.ISRJet == false) return false;

    return true;
}

bool goesInPreselectionMTtail_withAdditionalCuts() { return (goesInPreselectionMTtail() && additionalCuts());     } 

/*
 * Reference :
                                                                                    // MET METsig   MT              MT2W BPt dPhi ISRjet
bool cutAndCount_T2bw025_veryOffShell_loose(bool applyMTCut) { return cutAndCount_T2bw(-1,   9,    120 * applyMTCut, -1,  -1,  0.2, true ); }
bool cutAndCount_T2bw025_offShell_loose(bool applyMTCut)     { return cutAndCount_T2bw(-1,   7,    120 * applyMTCut, 200, 150, 0.8, false); }
bool cutAndCount_T2bw025_lowDeltaM_tight(bool applyMTCut)    { return cutAndCount_T2bw(-1,   6,    120 * applyMTCut, 200, 180, 0.8, false); }
bool cutAndCount_T2bw025_highDeltaM(bool applyMTCut)         { return cutAndCount_T2bw(-1,  10,    140 * applyMTCut, 200, 180, 0.8, false); }

                                                                                    // MET METsig   MT              MT2W BPt dPhi ISRjet
bool cutAndCount_T2bw050_offShell_loose(bool applyMTCut)     { return cutAndCount_T2bw(-1,   9,   120 * applyMTCut, -1,  -1, 0.2, true ); }
bool cutAndCount_T2bw050_lowMass(bool applyMTCut)            { return cutAndCount_T2bw(-1,   6,   120 * applyMTCut, 200,100, 0.8, false); }
bool cutAndCount_T2bw050_mediumDeltaM_loose(bool applyMTCut) { return cutAndCount_T2bw(-1,   7,   150 * applyMTCut, 200,150, 0.8, false); }
bool cutAndCount_T2bw050_highDeltaM(bool applyMTCut)         { return cutAndCount_T2bw(-1,  10,   160 * applyMTCut, 200,180, 0.8, false); }

                                                                                    // MET METsig   MT              MT2W BPt dPhi ISRjet
bool cutAndCount_T2bw075_lowDeltaM_tight(bool applyMTCut)    { return cutAndCount_T2bw(-1,  12,    120 * applyMTCut, -1,  -1, 0.8, true);  }
bool cutAndCount_T2bw075_mediumDeltaM(bool applyMTCut)       { return cutAndCount_T2bw(-1,  10,    140 * applyMTCut, 180, -1, 0.8, false); }
bool cutAndCount_T2bw075_highDeltaM(bool applyMTCut)         { return cutAndCount_T2bw(320, -1,    160 * applyMTCut, 200, -1, 0.8, false); }

*/
/*
#define T2BW_x025

#define SF_1ltop_and_Wjets          2
#define SF_allOthers                1.3
*/
/*
#define BENCHMARK_STOP_MASS         250
#define BENCHMARK_NEUTRALINO_MASS   100
                   // METsig  MT, MT2W, MET BPt
#define REFERENCE_CUTS {9,   120, -1,   -1, -1}
#define SYST_UNCERTAINTY            0.2
*/
/*
#define BENCHMARK_STOP_MASS         450
#define BENCHMARK_NEUTRALINO_MASS   225
                    // METsig  MT, MT2W, MET BPt
#define REFERENCE_CUTS {7,   120, 200,   -1, 150}
#define SYST_UNCERTAINTY            0.2
*/
/*
#define BENCHMARK_STOP_MASS         500
#define BENCHMARK_NEUTRALINO_MASS   50
                    // METsig  MT, MT2W, MET BPt
#define REFERENCE_CUTS {6,   120, 200,   -1, 180}
#define SYST_UNCERTAINTY            0.2
*/
/*
#define BENCHMARK_STOP_MASS         650
#define BENCHMARK_NEUTRALINO_MASS   150
                    // METsig  MT, MT2W, MET BPt
#define REFERENCE_CUTS {10,   150, 200,   -1, 180}
#define SYST_UNCERTAINTY            0.4
*/

#define T2BW_x050

#define SF_1ltop_and_Wjets          1.5
#define SF_allOthers                1.1

/*
#define BENCHMARK_STOP_MASS         300
#define BENCHMARK_NEUTRALINO_MASS   150
                   // METsig  MT, MT2W, MET BPt
#define REFERENCE_CUTS {9,   120, -1,   -1, -1}
#define SYST_UNCERTAINTY            0.20
*/
/*
#define BENCHMARK_STOP_MASS         300
#define BENCHMARK_NEUTRALINO_MASS    50
                   // METsig  MT, MT2W, MET BPt
#define REFERENCE_CUTS {6,   120, 200,  -1, 100}
#define SYST_UNCERTAINTY            0.20
*/
/*
#define BENCHMARK_STOP_MASS         500
#define BENCHMARK_NEUTRALINO_MASS   150
                   // METsig  MT, MT2W, MET BPt
#define REFERENCE_CUTS {7,   150, 200,  -1, 150}
#define SYST_UNCERTAINTY            0.25
*/

#define BENCHMARK_STOP_MASS         650
#define BENCHMARK_NEUTRALINO_MASS    50
                   // METsig  MT, MT2W, MET BPt
#define REFERENCE_CUTS {10,   150, 200,  -1, 180}
#define SYST_UNCERTAINTY            0.4


float getYield(vector< vector<float> > listEvent, vector<float> cuts);
void fillTable(Table* results, string label, bool* use, vector<float> cuts, float bestFOM, float bestYieldSig, float bestYieldBkg);

vector<float> optimizeCuts(bool* use, float* bestFOM, float* bestYieldSig, float* bestYieldBkg, string mode);

vector< vector<float> > listBackground;
vector< vector<float> > listSignal;

// #############
// Main function
// #############

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

     screwdriver.AddVariable("mStop",          "m_{#tilde{t}}",   "GeV",    28,112.5,812.5,  &(myEvent.mStop),               "");
     screwdriver.AddVariable("mNeutralino",    "m_{#chi^{0}}",    "GeV",    16,-12.5,387.5,  &(myEvent.mNeutralino),         "");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     screwdriver.AddProcessClass("1ltop",                        "1l top", "background",kRed-7);
         #ifdef USING_TTBAR_POWHEG
             screwdriver.AddDataset("ttbar_powheg",              "1ltop",  0, 0);
         #endif
         #ifdef USING_TTBAR_MADGRAPH
             screwdriver.AddDataset("ttbar_madgraph_1l",         "1ltop",  0, 0);
         #endif
         screwdriver.AddDataset("singleTop_st",                  "1ltop",  0, 0);


     screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",kCyan-3);
         #ifdef USING_TTBAR_MADGRAPH
             screwdriver.AddDataset("ttbar_madgraph_2l",   "ttbar_2l",  0, 0);
         #endif

     screwdriver.AddProcessClass("W+jets",         "W+jets",                          "background", kOrange-2);
             screwdriver.AddDataset("W+jets",      "W+jets", 0, 0);
                                                   
     screwdriver.AddProcessClass("rare",           "rare",                            "background", kMagenta-5);
             screwdriver.AddDataset("rare",        "rare", 0, 0);
                                                   
     #ifdef T2BW_x025
     screwdriver.AddProcessClass("T2bw-025",     "T2bw (x=0.25)",          "signal",kViolet-1);
          screwdriver.AddDataset("T2bw-025",     "T2bw-025",   0, 0);
     #endif
     #ifdef T2BW_x050
     screwdriver.AddProcessClass("T2bw-050",     "T2bw (x=0.50)",          "signal",kViolet-1);
          screwdriver.AddDataset("T2bw-050",     "T2bw-050",   0, 0);
     #endif
     #ifdef T2BW_x075
     screwdriver.AddProcessClass("T2bw-075",     "T2bw (x=0.75)",          "signal",kViolet-1);
          screwdriver.AddDataset("T2bw-075",     "T2bw-075",   0, 0);
     #endif

  // ##########################
  // ##    Create Regions    ##
  // ##########################

     screwdriver.AddRegion("presel", "Preselection", &goesInPreselectionMTtail_withAdditionalCuts);

  // ##########################
  // ##   Create Channels    ##
  // ##########################
   
     screwdriver.AddChannel("singleLepton", "e/#mu-channels",  &goesInSingleLeptonChannel);

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
     screwdriver.SetGlobalBoolOption("Plot", "exportPng", false);

  // ########################################
  // ##       Run over the datasets        ##
  // ########################################

  vector<string> datasetsList;
  screwdriver.GetDatasetList(&datasetsList);

  cout << "   > Reading datasets " << endl;

  for (unsigned int d = 0 ; d < datasetsList.size() ; d++)
  {
      string currentDataset = datasetsList[d];
      string currentProcessClass = screwdriver.GetProcessClass(currentDataset); 
      // Open the tree
      TFile f((string(FOLDER_BABYTUPLES)+currentDataset+".root").c_str());
      TTree* theTree = (TTree*) f.Get("babyTuple"); 

      intermediatePointers pointers;
      InitializeBranchesForReading(theTree,&myEvent,&pointers);

      if (findSubstring(currentDataset,"T2"))
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

      for (int i = 0 ; i < theTree->GetEntries() ; i++)
      {
          
          //if (i >  theTree->GetEntries() * 0.1) break; 

          if (i % (theTree->GetEntries() / 50) == 0) 
              printProgressBar(i,theTree->GetEntries(),currentDataset);

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          // Split 1-lepton ttbar and 2-lepton ttbar
          string currentProcessClass_ = currentProcessClass;
          if ((currentDataset == "ttbar_powheg") && (myEvent.numberOfGenLepton == 2)) 
              currentProcessClass_ = "ttbar_2l";

          screwdriver.AutoFillProcessClass(currentProcessClass_,getWeight());

          // Store stuff for cut optimization

          if (goesInPreselectionMTtail_withAdditionalCuts())
          {
              vector<float> values;
              values.push_back(myEvent.METoverSqrtHT);
              values.push_back(myEvent.MT);
              values.push_back(myEvent.MT2W);
              values.push_back(myEvent.MET);
              values.push_back(myEvent.leadingBPt);

              if (findSubstring(currentDataset,"T2"))
              {
                  if ((myEvent.mStop       == BENCHMARK_STOP_MASS     ) 
                   && (myEvent.mNeutralino == BENCHMARK_NEUTRALINO_MASS))
                   {
                      values.push_back(getWeight());
                      listSignal.push_back(values);
                   }
              }
              else
              {
                 if ((currentProcessClass_ == "1ltop") 
                  || (currentProcessClass_ == "W+jets")) values.push_back(getWeight() * SF_1ltop_and_Wjets);
                 else                                    values.push_back(getWeight() * SF_allOthers);
                 
                 listBackground.push_back(values);
              }
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
  //screwdriver.WritePlots("../plots/cutAndCount_T2tt/");

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################
  
  printBoxedMessage("Now computing misc tests ... ");
  
  // #####################################
  // ##   Other optimization stuff...   ##
  // #####################################

  bool scenario_1235[5] = {1,1,1,0,1};
  bool scenario_2345[5] = {0,1,1,1,1}; 
  bool scenario_125 [5] = {1,1,0,0,1}; 
  bool scenario_245 [5] = {0,1,0,1,1}; 

  vector<bool*> scenarios =
  {
      scenario_1235,
      scenario_2345,
      scenario_125,
      scenario_245,
  };

  vector<string> columns = { "MET/sqrt(HT)", "MET", "MT", "MT2W", "BPt", "FOM", "S", "B" };
  vector<string> lines = 
  {
      "1235/discovery",
      "2345/discovery",
      "125/discovery",
      "245/discovery",

      "1235/azimov",
      "2345/azimov",
      "125/azimov",
      "245/azimov",
      
      "1235/exclusion",
      "2345/exclusion",
      "125/exclusion",
      "245/exclusion",
  };
  Table optimizationResult(columns,lines);

  float bestFOM, bestYieldSig, bestYieldBkg;
  vector<float> cuts;
  for (unsigned int s = 0 ; s < scenarios.size() ; s++)
  {
      printProgressBar(s*3,scenarios.size()*3);
      cuts = optimizeCuts(scenarios[s], &bestFOM, &bestYieldSig, &bestYieldBkg, "discovery");
      fillTable(&optimizationResult, lines[s                  ], scenarios[s],cuts,bestFOM,bestYieldSig,bestYieldBkg);
      
      printProgressBar(s*3+1,scenarios.size()*3);
      cuts = optimizeCuts(scenarios[s], &bestFOM, &bestYieldSig, &bestYieldBkg, "azimov");
      fillTable(&optimizationResult, lines[s+  scenarios.size()], scenarios[s],cuts,bestFOM,bestYieldSig,bestYieldBkg);
      
      printProgressBar(s*3+2,scenarios.size()*3);
      cuts = optimizeCuts(scenarios[s], &bestFOM, &bestYieldSig, &bestYieldBkg, "exclusion");
      fillTable(&optimizationResult, lines[s+2*scenarios.size()], scenarios[s],cuts,bestFOM,bestYieldSig,bestYieldBkg);
  }


  optimizationResult.Print(3,"noError");

  float yieldBackgroundRef  = getYield(listBackground,REFERENCE_CUTS);
  float yieldSignalRef      = getYield(listSignal,REFERENCE_CUTS);

  cout << "FOM for Reference cuts : " << figureOfMerit(yieldSignalRef, yieldBackgroundRef, "discovery") << " (discovery)"
                                      << figureOfMerit(yieldSignalRef, yieldBackgroundRef, "exclusion") << " (exclusion)"
      << " S, B = " << yieldSignalRef << ", " << yieldBackgroundRef<< endl; 

  printBoxedMessage("Program done.");
  return (0);
}
  
void fillTable(Table* results, string label, bool* use, vector<float> cuts, float bestFOM, float bestYieldSig, float bestYieldBkg)
{
    results->Set("MET/sqrt(HT)", label, use[0] ? cuts[0] : -1);
    results->Set("MET",          label, use[3] ? cuts[3] : -1);
    results->Set("MT",           label, use[1] ? cuts[1] : -1);
    results->Set("MT2W",         label, use[2] ? cuts[2] : -1);
    results->Set("BPt",          label, use[4] ? cuts[4] : -1);
    results->Set("FOM",          label, bestFOM);
    results->Set("S",            label, bestYieldSig);
    results->Set("B",            label, bestYieldBkg);
}

vector<float> optimizeCuts(bool* use, float* bestFOM, float* bestYieldSig, float* bestYieldBkg, string mode)
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

    for (cuts[0] = (use[0] ? 6   : -1) ; cuts[0] <= (use[0] ? 15  : -1) ; cuts[0] += 1  ) {  // MET / sqrt(HT)
    for (cuts[1] = (use[1] ? 120 : -1) ; cuts[1] <= (use[1] ? 140 : -1) ; cuts[1] += 5  ) {  // MT
    for (cuts[2] = (use[2] ? 100 : -1) ; cuts[2] <= (use[2] ? 200 : -1) ; cuts[2] += 10 ) {  // MT2W
    for (cuts[3] = (use[3] ? 100 : -1) ; cuts[3] <= (use[3] ? 400 : -1) ; cuts[3] += 50 ) {  // MET
    for (cuts[4] = (use[4] ? 80  : -1) ; cuts[4] <= (use[4] ? 200 : -1) ; cuts[4] += 20 ) {  // bPt 

        float yieldBackground  = getYield(listBackground,cuts);
        float yieldSignal      = getYield(listSignal,cuts);

        //DEBUG_MSG << "S, B = " << yieldSignal << " ; " << yieldBackground << endl;

        float FOM = figureOfMerit(yieldSignal, yieldBackground, mode); 

        if (FOM > *bestFOM)
        { 
            bestCuts        = cuts; 
            (*bestFOM)      = FOM;
            (*bestYieldSig) = yieldSignal;
            (*bestYieldBkg) = yieldBackground;
        }
        
    } } } } }
        
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



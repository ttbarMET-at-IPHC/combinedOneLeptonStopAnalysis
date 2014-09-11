#include "common.h"
// ##################
// Others tools/stuff
// ##################

bool additionalCuts()
{
    //if (myEvent.deltaPhiMETJets < 0.8) return false;
    //if (myEvent.hadronicChi2    > 3)   return false;

    if (myEvent.MT < 120) return false;
    //if (myEvent.ISRJet == false) return false;

    return true;
}

bool goesInPreselectionMTtail_withAdditionalCuts() { return (goesInPreselectionMTtail() && additionalCuts());     } 

/*
 * Reference :
                                                                            // MET METsig   MT             MT2W dPhi Chi2    ISRjet
bool cutAndCount_T2tt_offShellLoose(bool applyMTCut) { return cutAndCount_T2tt( 200, -1, 120 * applyMTCut, -1,  -1,  999999, true ); }
bool cutAndCount_T2tt_offShellTight(bool applyMTCut) { return cutAndCount_T2tt( -1,  10, 140 * applyMTCut, -1,  -1,  999999, true ); }
bool cutAndCount_T2tt_lowDeltaM    (bool applyMTCut) { return cutAndCount_T2tt( 130, -1, 130 * applyMTCut, -1,  0.8, 5,      false); }
bool cutAndCount_T2tt_mediumDeltaM (bool applyMTCut) { return cutAndCount_T2tt( -1,  10, 140 * applyMTCut, 180, 0.8, 3,      false); }
bool cutAndCount_T2tt_highDeltaM   (bool applyMTCut) { return cutAndCount_T2tt( -1,  15, 190 * applyMTCut, 240, -1,  999999, false); }
*/

#define SF_1ltop                    5
#define SF_allOthers                5

/*
#define BENCHMARK_STOP_MASS         250
#define BENCHMARK_NEUTRALINO_MASS   100

                     // METsig  MT, MT2W, MET
#define REFERENCE_CUTS {-1,    120, -1, 200}
#define SYST_UNCERTAINTY            0.17
*/
/*
#define BENCHMARK_STOP_MASS         350
#define BENCHMARK_NEUTRALINO_MASS   200

                     // METsig  MT, MT2W, MET
#define REFERENCE_CUTS {10,    140, -1, -1}
#define SYST_UNCERTAINTY            0.21
*/
/*
#define BENCHMARK_STOP_MASS         250
#define BENCHMARK_NEUTRALINO_MASS   25

                     // METsig  MT, MT2W, MET
#define REFERENCE_CUTS {-1,    130, -1, 130}
#define SYST_UNCERTAINTY            0.22
*/
/*
#define BENCHMARK_STOP_MASS         450
#define BENCHMARK_NEUTRALINO_MASS   100

                     // METsig  MT, MT2W, MET
#define REFERENCE_CUTS {10,    140, 180, -1}
#define SYST_UNCERTAINTY            0.37
*/

#define BENCHMARK_STOP_MASS         600
#define BENCHMARK_NEUTRALINO_MASS   50

                     // METsig  MT, MT2W, MET
#define REFERENCE_CUTS {15,    190, 240, -1}
#define SYST_UNCERTAINTY            0.37


float getYield(vector< vector<float> > listEvent, vector<float> cuts);
void fillTable(Table* results, string label, bool* use, vector<float> cuts, float bestFOM, float bestYieldSig, float bestYieldBkg);

vector<float> optimizeCuts(bool* use, float* bestFOM, 
                           float* bestYieldSig, float* bestYieldBkg, string mode);

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
                                                   
     screwdriver.AddProcessClass("T2tt",           "T2tt",                            "signal",     kViolet-1);
             screwdriver.AddDataset("T2tt",        "T2tt",   0, 0);

     //screwdriver.AddProcessClass("signal_250_100",  "T2tt (250/100)",             "signal",COLORPLOT_AZURE);
     //screwdriver.AddProcessClass("signal_450_100",  "T2tt (450/100)",             "signal",kCyan-3);
     //screwdriver.AddProcessClass("signal_650_100",  "T2tt (650/100)",             "signal",COLORPLOT_GREEN);

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

      if (currentDataset == "T2tt")
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

              if (currentDataset == "T2tt") 
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
                 if (currentProcessClass_ == "1ltop") values.push_back(getWeight() * SF_1ltop);
                 else                                 values.push_back(getWeight() * SF_allOthers);
                 
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
  
  bool scenario_3_123[4] = {1,1,1,0}; 
  bool scenario_3_234[4] = {0,1,1,1};
  bool scenario_2_12[4]  = {1,1,0,0};
  bool scenario_2_24[4]  = {0,1,0,1}; 
  
  float bestFOM, bestYieldSig, bestYieldBkg;

  vector<string> columns = { "MET/sqrt(HT)", "MET", "MT", "MT2W", "FOM", "S", "B" };
  vector<string> lines = 
  {
      "123/discovery",
      "234/discovery",
      "12/discovery",
      "24/discovery",
      
      "123/azimov",
      "234/azimov",
      "12/azimov",
      "24/azimov",
      
      "123/exclusion",
      "234/exclusion",
      "12/exclusion",
      "24/exclusion",

  };
  Table optimizationResult(columns,lines);

  vector<float> cuts;
  
  cuts = optimizeCuts(scenario_3_123, &bestFOM, &bestYieldSig, &bestYieldBkg, "discovery");
  fillTable(&optimizationResult, "123/discovery",scenario_3_123,cuts,bestFOM,bestYieldSig,bestYieldBkg);
  cuts = optimizeCuts(scenario_3_123, &bestFOM, &bestYieldSig, &bestYieldBkg, "exclusion");
  fillTable(&optimizationResult, "123/exclusion",scenario_3_123,cuts,bestFOM,bestYieldSig,bestYieldBkg);
  cuts = optimizeCuts(scenario_3_123, &bestFOM, &bestYieldSig, &bestYieldBkg, "azimov");
  fillTable(&optimizationResult, "123/azimov",   scenario_3_123,cuts,bestFOM,bestYieldSig,bestYieldBkg);

  cuts = optimizeCuts(scenario_3_234, &bestFOM, &bestYieldSig, &bestYieldBkg, "discovery");
  fillTable(&optimizationResult, "234/discovery",scenario_3_234,cuts,bestFOM,bestYieldSig,bestYieldBkg);                  
  cuts = optimizeCuts(scenario_3_234, &bestFOM, &bestYieldSig, &bestYieldBkg, "exclusion");
  fillTable(&optimizationResult, "234/exclusion",scenario_3_234,cuts,bestFOM,bestYieldSig,bestYieldBkg);                  
  cuts = optimizeCuts(scenario_3_234, &bestFOM, &bestYieldSig, &bestYieldBkg, "azimov");
  fillTable(&optimizationResult, "234/azimov",   scenario_3_234,cuts,bestFOM,bestYieldSig,bestYieldBkg);
 
  cuts = optimizeCuts(scenario_2_12, &bestFOM, &bestYieldSig, &bestYieldBkg, "discovery");
  fillTable(&optimizationResult, "12/discovery",scenario_2_12,cuts,bestFOM,bestYieldSig,bestYieldBkg);                  
  cuts = optimizeCuts(scenario_2_12, &bestFOM, &bestYieldSig, &bestYieldBkg, "exclusion");
  fillTable(&optimizationResult, "12/exclusion",scenario_2_12,cuts,bestFOM,bestYieldSig,bestYieldBkg);                  
  cuts = optimizeCuts(scenario_2_12, &bestFOM, &bestYieldSig, &bestYieldBkg, "azimov");
  fillTable(&optimizationResult, "12/azimov",   scenario_2_12,cuts,bestFOM,bestYieldSig,bestYieldBkg); 
 
  cuts = optimizeCuts(scenario_2_24, &bestFOM, &bestYieldSig, &bestYieldBkg, "discovery");
  fillTable(&optimizationResult, "24/discovery",scenario_2_24,cuts,bestFOM,bestYieldSig,bestYieldBkg);                  
  cuts = optimizeCuts(scenario_2_24, &bestFOM, &bestYieldSig, &bestYieldBkg, "exclusion");
  fillTable(&optimizationResult, "24/exclusion",scenario_2_24,cuts,bestFOM,bestYieldSig,bestYieldBkg);                  
  cuts = optimizeCuts(scenario_2_24, &bestFOM, &bestYieldSig, &bestYieldBkg, "azimov");
  fillTable(&optimizationResult, "24/azimov",   scenario_2_24,cuts,bestFOM,bestYieldSig,bestYieldBkg);  
    

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
    bestCuts = cuts;

    for (cuts[0] = (use[0] ? 6 : -1)   ; cuts[0] <= (use[0] ? 15  : -1) ; cuts[0] += 1  ) {  // MET / sqrt(HT)
        printProgressBar(cuts[0]-6,9);
    for (cuts[1] = (use[1] ? 120 : -1) ; cuts[1] <= (use[1] ? 140 : -1) ; cuts[1] += 5  ) {  // MT
    for (cuts[2] = (use[2] ? 100 : -1) ; cuts[2] <= (use[2] ? 200 : -1) ; cuts[2] += 10 ) {  // MT2W
    for (cuts[3] = (use[3] ? 100 : -1) ; cuts[3] <= (use[3] ? 400 : -1) ; cuts[3] += 50 ) {  // MET

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
        
    } } } }
        
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



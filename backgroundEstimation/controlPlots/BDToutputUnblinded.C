#include "../backgroundEstimation_common/common.h"

#ifndef SIGNAL_REGION_TAG
    #error SIGNAL_REGION_TAG need to be defined.
#endif

typedef struct
{
    string type;
    int stopMass;
    int neutralinoMass;
}
SignalBenchmark;

// #########################################################################
//                              Main function
// #########################################################################

int main (int argc, char *argv[])
{
  loadBDTSignalRegions();

  string BDTSignalRegionTag = SIGNAL_REGION_TAG;

  if (BDTSignalRegionTag.find("BDT_") == 0)
      BDTSignalRegionTag = BDTSignalRegionTag.substr(4,BDTSignalRegionTag.size());

  string BDTSignalRegionLabel = BDTlabel(BDTSignalRegionTag, "root");
  replace(BDTSignalRegionLabel, "BDT T2tt-",     "T2tt, BDT ");
  replace(BDTSignalRegionLabel, "BDT T2bw025-",  "T2bw x=0.25, BDT ");
  replace(BDTSignalRegionLabel, "BDT T2bw050-",  "T2bw x=0.50, BDT ");
  replace(BDTSignalRegionLabel, "BDT T2bw075-",  "T2bw x=0.75, BDT ");

  float  BDTSignalRegionCut   = BDTcut(BDTSignalRegionTag);

  SignalBenchmark referenceSignal;
  SignalBenchmark referenceSignal2;

  float forceMin  = -99;
  float forceMax  = -99;
  float forceBins = -1;

  referenceSignal2 = { "", 0, 0 };

       if (string(SIGNAL_REGION_TAG) == "BDT_T2tt_1_lowLSP"    ) referenceSignal = { "T2tt",     175, 25  };
       if (string(SIGNAL_REGION_TAG) == "BDT_T2tt_1_mediumLSP" ) referenceSignal = { "T2tt",     250, 100 };
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2tt_1_highLSP"   ) referenceSignal = { "T2tt",     300, 150 };
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2tt_2"           ) referenceSignal = { "T2tt",     300, 50  };
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2tt_5_lowDM"     ) referenceSignal = { "T2tt",     400, 50  };
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2tt_5_highDM"    ) referenceSignal = { "T2tt",     600, 50  };
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw025_1"        ) referenceSignal = { "T2bw-025", 200, 50  };
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw025_3"        ) referenceSignal = { "T2bw-025", 300, 50  };
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw025_4_lowLSP" ) referenceSignal = { "T2bw-025", 500, 50  };
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw025_4_highLSP") referenceSignal = { "T2bw-025", 600, 150 };
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw025_6"        ) referenceSignal = { "T2bw-025", 675, 50  };
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw050_1_lowDM_lowLSP" ) referenceSignal = { "T2bw-050", 150, 50  };
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw050_1_lowDM_highLSP") referenceSignal = { "T2bw-050", 250, 150 };
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw050_1_highDM" ) referenceSignal = { "T2bw-050", 275, 100 };
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw050_3"        ) referenceSignal = { "T2bw-050", 350, 50  };
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw050_4"        ) referenceSignal = { "T2bw-050", 500, 50  };
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw050_5"        ) referenceSignal = { "T2bw-050", 600, 50  };
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw050_6"        ) referenceSignal = { "T2bw-050", 700, 50  };
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw075_1"        ) { referenceSignal = { "T2bw-075", 300, 200  }; referenceSignal2 = { "T2bw-075", 500, 200  }; }
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw075_2"        ) { referenceSignal = { "T2bw-075", 300, 200  }; referenceSignal2 = { "T2bw-075", 500, 200  }; }
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw075_3"        ) { referenceSignal = { "T2bw-075", 300, 200  }; referenceSignal2 = { "T2bw-075", 500, 200  }; }
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw075_5_lowDM"  ) { referenceSignal = { "T2bw-075", 300, 200  }; referenceSignal2 = { "T2bw-075", 500, 200  }; forceMin = 0; forceMax = 0.4; forceBins = 8; }
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw075_5_highDM" ) { referenceSignal = { "T2bw-075", 300, 200  }; referenceSignal2 = { "T2bw-075", 500, 200  }; forceMin = 0; forceMax = 0.4; forceBins = 8; }
/*
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw075_1"        ) referenceSignal = { "T2bw-075", 200, 50  };
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw075_2"        ) referenceSignal = { "T2bw-075", 300, 50  };
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw075_3"        ) referenceSignal = { "T2bw-075", 450, 50  };
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw075_5_lowDM"  ) referenceSignal = { "T2bw-075", 550, 50  };
  else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw075_5_highDM" ) referenceSignal = { "T2bw-075", 650, 50  };
*/

  printBoxedMessage("Starting plot generation");

  // ####################
  // ##   Init tools   ##
  // ####################

     // Create a sonic Screwdriver
      SonicScrewdriver screwdriver;

     // ##########################
     // ##   Create Variables   ##
     // ##########################

     float min = BDTSignalRegionCut - 0.20;
     float max = BDTSignalRegionCut + 0.10;
     float bins = 9;

     if (forceMin  != -99) min  = forceMin;
     if (forceMax  != -99) max  = forceMax;
     if (forceBins != -1)  bins = forceBins;

     float BDToutputValue;
     screwdriver.AddVariable("BDToutput_full", "BDT output (full)", "", 20,-0.5,       0.5, &BDToutputValue, "");
     screwdriver.AddVariable("BDToutput",      "BDT output",        "", bins, min, max, &BDToutputValue, "noUnderflowInFirstBin,logY");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

        screwdriver.AddProcessClass("1ltop", "1l top",                             "background",kRed-7);
            #ifdef USING_TTBAR_POWHEG
                screwdriver.AddDataset("ttbar_powheg",                "1ltop",  0, 0);
            #endif
            #ifdef USING_TTBAR_MADGRAPH
                screwdriver.AddDataset("ttbar_madgraph_1l",           "1ltop",  0, 0);
            #endif
            screwdriver.AddDataset("singleTop_st",                    "1ltop",  0, 0);

     screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",kCyan-3);
            #ifdef USING_TTBAR_MADGRAPH
                screwdriver.AddDataset("ttbar_madgraph_2l",   "ttbar_2l",  0, 0);
            #endif

     screwdriver.AddProcessClass("W+jets",   "W+jets",                          "background",kOrange-2);
             screwdriver.AddDataset("W+jets",    "W+jets", 0, 0);

     screwdriver.AddProcessClass("rare",   "rare",                              "background",kMagenta-5);
             screwdriver.AddDataset("rare",   "rare", 0, 0);

     screwdriver.AddProcessClass("data",   "data",                              "data",COLORPLOT_BLACK);
             screwdriver.AddDataset("SingleElec",   "data", 0, 0);
             screwdriver.AddDataset("SingleMuon",   "data", 0, 0);

     string label;
     if (findSubstring(referenceSignal.type,"T2bw-025")) label = "T2bw x=0.25, ("+intToString(referenceSignal.stopMass)+"/"+intToString(referenceSignal.neutralinoMass)+")";
     if (findSubstring(referenceSignal.type,"T2bw-050")) label = "T2bw x=0.50, ("+intToString(referenceSignal.stopMass)+"/"+intToString(referenceSignal.neutralinoMass)+")";
     if (findSubstring(referenceSignal.type,"T2bw-075")) label = "T2bw x=0.75, ("+intToString(referenceSignal.stopMass)+"/"+intToString(referenceSignal.neutralinoMass)+")";
     if (findSubstring(referenceSignal.type,"T2tt")    ) label = "T2tt ("+intToString(referenceSignal.stopMass)+"/"+intToString(referenceSignal.neutralinoMass)+")";

     screwdriver.AddProcessClass(referenceSignal.type+"_example1",
                                 label, "signal", COLORPLOT_GREEN );
            screwdriver.AddDataset(referenceSignal.type,    referenceSignal.type+"_example1",   0, 0);

     if (referenceSignal2.type != "")
     {
         string label2;
         if (findSubstring(referenceSignal2.type,"T2bw-025")) label2 = "T2bw x=0.25, ("+intToString(referenceSignal2.stopMass)+"/"+intToString(referenceSignal2.neutralinoMass)+")";
         if (findSubstring(referenceSignal2.type,"T2bw-050")) label2 = "T2bw x=0.50, ("+intToString(referenceSignal2.stopMass)+"/"+intToString(referenceSignal2.neutralinoMass)+")";
         if (findSubstring(referenceSignal2.type,"T2bw-075")) label2 = "T2bw x=0.75, ("+intToString(referenceSignal2.stopMass)+"/"+intToString(referenceSignal2.neutralinoMass)+")";
         if (findSubstring(referenceSignal2.type,"T2tt")    ) label2 = "T2tt ("+intToString(referenceSignal2.stopMass)+"/"+intToString(referenceSignal2.neutralinoMass)+")";

        screwdriver.AddProcessClass(referenceSignal2.type+"_example2",
                                    label2, "signal", COLORPLOT_MAGENTA );
     }

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     screwdriver.AddRegion("preselection",      "Preselection",                       &goesInPreselectionMTtail);
     screwdriver.AddRegion("signalRegion",      BDTSignalRegionLabel, "preselection", { Cut("BDToutput",      '>', BDTSignalRegionCut) }, "showCuts");

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
     screwdriver.SchedulePlots("2D");

     // Config plots

     screwdriver.SetGlobalStringOption("DataMCComparison",  "includeSignal",                    "stack");
     screwdriver.SetGlobalFloatOption ("DataMCComparison",  "factorSignal",                     1.0    );
     screwdriver.SetGlobalBoolOption  ("DataMCRatio",       "includeSignal",                    false  );
     screwdriver.SetGlobalFloatOption ("DataMCRatio",       "min",                              0.0    );
     screwdriver.SetGlobalFloatOption ("DataMCRatio",       "max",                              2.0    );

     screwdriver.SetGlobalStringOption("Plot", "infoTopRight", "CMS Preliminary");
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

     if (currentDataset == referenceSignal.type)
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

     int nEntries = theTree->GetEntries();
     for (int i = 0 ; i < nEntries ; i++)
     {
         //if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

         // Get the i-th entry
         ReadEvent(theTree,i,&pointers,&myEvent);

         BDToutputValue = BDToutput(BDTSignalRegionTag);

         float weight = getWeight();

         // Split 1-lepton ttbar and 2-lepton ttbar
         string currentProcessClass_ = currentProcessClass;
         if ((currentDataset == "ttbar_powheg") && (myEvent.numberOfGenLepton == 2))
             currentProcessClass_ = "ttbar_2l";

         if (currentDataset == referenceSignal.type)
         {
             if ((myEvent.mStop == referenceSignal.stopMass) && (myEvent.mNeutralino == referenceSignal.neutralinoMass))
                 screwdriver.AutoFillProcessClass(referenceSignal.type+"_example1",weight);
             else if ((myEvent.mStop == referenceSignal2.stopMass) && (myEvent.mNeutralino == referenceSignal2.neutralinoMass))
                 screwdriver.AutoFillProcessClass(referenceSignal.type+"_example2",weight);
         }
         else
         {
             screwdriver.AutoFillProcessClass(currentProcessClass_,weight);
         }

         if (currentProcessClass_ == "data")
         {
             if ((goesInPreselectionMTtail()) && (BDToutputValue >  BDTSignalRegionCut))
             {
                cout << "[dataset, run, lumi, event, BDToutput] = " << currentDataset << " " << myEvent.run << " " << myEvent.lumi << " " << myEvent.event << " " << BDToutputValue << endl;
             }
         }
     }

      //printProgressBar(nEntries,nEntries,currentDataset);
      cout << endl;
      f.Close();

  }

    // #############################
    // ##   Apply scale factors   ##
    // #############################

    Table scaleFactors = Table(string("../backgroundEstimation_prediction/scaleFactors/BDT_")+BDTSignalRegionTag+".tab");

    Figure SF_pre           = scaleFactors.Get("value","SF_pre");
    Figure SF_post          = scaleFactors.Get("value","SF_post");
    Figure SF_MTtail_1ltop  = scaleFactors.Get("value","SF_MTtail_1ltop");
    Figure SF_MTtail_Wjets  = scaleFactors.Get("value","SF_MTtail_Wjets");

    vector<string> channelsOnWhichToApplyScaleFactors = { "singleLepton" };
    for (unsigned int c = 0 ; c < channelsOnWhichToApplyScaleFactors.size() ; c++)
    {
        string channel = channelsOnWhichToApplyScaleFactors[c];

      //  screwdriver.ApplyScaleFactor("ttbar_2l",  "preselection", channel, );
        screwdriver.ApplyScaleFactor("1ltop",     "preselection", channel, SF_MTtail_1ltop);
        screwdriver.ApplyScaleFactor("W+jets",    "preselection", channel, SF_MTtail_Wjets);

        screwdriver.ApplyScaleFactor("ttbar_2l",  "signalRegion", channel, SF_pre);
        screwdriver.ApplyScaleFactor("1ltop",     "signalRegion", channel, SF_post * SF_MTtail_1ltop);
        screwdriver.ApplyScaleFactor("W+jets",    "signalRegion", channel, SF_post * SF_MTtail_Wjets);
    }

  // ###################################
  // ##   Make plots and write them   ##
  // ###################################

  cout << endl;
  cout << "   > Making plots..." << endl;
  screwdriver.MakePlots();
  cout << "   > Saving plots..." << endl;

  system((string("mkdir -p ./plots/")+BDTSignalRegionTag).c_str());
  screwdriver.WritePlots(string("./plots/")+BDTSignalRegionTag);

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################

  printBoxedMessage("Program done.");
  return (0);
}


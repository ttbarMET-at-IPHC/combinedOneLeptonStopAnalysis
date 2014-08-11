#include "../backgroundEstimation_common/common.h"

bool goesInPreselectionMTpeak_withSRCuts()          { return (goesInPreselectionMTpeak()       && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesIn0BtagControlRegionMTpeak_withSRCuts()    { return (goesIn0BtagControlRegionMTpeak() && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesIn0BtagControlRegionMTtail_withSRCuts()    { return (goesIn0BtagControlRegionMTtail() && SIGNAL_REGION_CUTS(enableMTCut) ); }
bool goesInDileptonControlRegionNoMT_withSRCuts()   { return (goesInDileptonControlRegion(4)   && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInVetoControlRegionMTpeak_withSRCuts()     { return (goesInVetoControlRegionMTpeak()  && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInVetoControlRegionMTtail_withSRCuts()     { return (goesInVetoControlRegionMTtail()  && SIGNAL_REGION_CUTS(enableMTCut) ); }

bool goesInAnyChannel()                             { return (goesInSingleLeptonChannel() || goesInDoubleLeptonChannel());           }

// #########################################################################
//                              Main function
// #########################################################################

typedef struct
{
    string type;
    int stopMass;
    int neutralinoMass;
}
SignalBenchmark;

int main (int argc, char *argv[])
{
    SignalBenchmark referenceSignal;

    #ifndef SIGNAL_REGION_TAG
        referenceSignal = {"T2tt", 450, 100};
    #else
             if (string(SIGNAL_REGION_TAG) == "preselection"                    ) referenceSignal = { "T2tt",     450, 100 };
        else if (string(SIGNAL_REGION_TAG) == "cutAndCount_T2tt_offShellLoose"  ) referenceSignal = { "T2tt",     225, 75  };
        else if (string(SIGNAL_REGION_TAG) == "cutAndCount_T2tt_offShellTight"  ) referenceSignal = { "T2tt",     325, 200 };
        else if (string(SIGNAL_REGION_TAG) == "cutAndCount_T2tt_lowDeltaM"      ) referenceSignal = { "T2tt",     250, 25  };
        else if (string(SIGNAL_REGION_TAG) == "cutAndCount_T2tt_mediumDeltaM"   ) referenceSignal = { "T2tt",     400, 100 };
        else if (string(SIGNAL_REGION_TAG) == "cutAndCount_T2tt_highDeltaM"     ) referenceSignal = { "T2tt",     600, 100 };
        else if (string(SIGNAL_REGION_TAG) == "cutAndCount_T2bw025_offShell"    ) referenceSignal = { "T2bw-025", 250, 100 };
        else if (string(SIGNAL_REGION_TAG) == "cutAndCount_T2bw025_lowMasses"   ) referenceSignal = { "T2bw-025", 400, 100 };
        else if (string(SIGNAL_REGION_TAG) == "cutAndCount_T2bw025_highMasses"  ) referenceSignal = { "T2bw-025", 650, 100 };
        else if (string(SIGNAL_REGION_TAG) == "cutAndCount_T2bw050_offShell"    ) referenceSignal = { "T2bw-050", 250, 100 };
        else if (string(SIGNAL_REGION_TAG) == "cutAndCount_T2bw050_lowMasses"   ) referenceSignal = { "T2bw-050", 250, 25  };
        else if (string(SIGNAL_REGION_TAG) == "cutAndCount_T2bw050_mediumDeltaM") referenceSignal = { "T2bw-050", 400, 100 };
        else if (string(SIGNAL_REGION_TAG) == "cutAndCount_T2bw050_highDeltaM"  ) referenceSignal = { "T2bw-050", 600, 100 };
        else if (string(SIGNAL_REGION_TAG) == "cutAndCount_T2bw075_lowDeltaM"   ) referenceSignal = { "T2bw-075", 250, 100 };
        else if (string(SIGNAL_REGION_TAG) == "cutAndCount_T2bw075_mediumDeltaM") referenceSignal = { "T2bw-075", 350, 50  };
        else if (string(SIGNAL_REGION_TAG) == "cutAndCount_T2bw075_highDeltaM"  ) referenceSignal = { "T2bw-075", 600, 100 };
        /*
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2tt_1_lowLSP"               ) referenceSignal = { "T2tt",    };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2tt_1_highLSP"              ) referenceSignal = { "T2tt",    };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2tt_2_lowDM"                ) referenceSignal = { "T2tt",    };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2tt_2_highDM"               ) referenceSignal = { "T2tt",    };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2tt_5_lowDM"                ) referenceSignal = { "T2tt",    };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2tt_5_mediumDM"             ) referenceSignal = { "T2tt",    };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2tt_5_highDM"               ) referenceSignal = { "T2tt",    };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw025_1"                   ) referenceSignal = { "T2bw-025", };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw025_3_lowDM"             ) referenceSignal = { "T2bw-025", };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw025_3_highDM"            ) referenceSignal = { "T2bw-025", };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw025_4"                   ) referenceSignal = { "T2bw-025", };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw025_6"                   ) referenceSignal = { "T2bw-025", };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw050_1_lowLSP"            ) referenceSignal = { "T2bw-050", };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw050_1_highLSP"           ) referenceSignal = { "T2bw-050", };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw050_3"                   ) referenceSignal = { "T2bw-050", };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw050_4"                   ) referenceSignal = { "T2bw-050", };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw050_5_lowDM"             ) referenceSignal = { "T2bw-050", };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw050_5_highDM"            ) referenceSignal = { "T2bw-050", };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw050_6"                   ) referenceSignal = { "T2bw-050", };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw075_1"                   ) referenceSignal = { "T2bw-075", };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw075_2"                   ) referenceSignal = { "T2bw-075", };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw075_3"                   ) referenceSignal = { "T2bw-075", };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw075_5_lowDM"             ) referenceSignal = { "T2bw-075", };
        else if (string(SIGNAL_REGION_TAG) == "BDT_T2bw075_5_highDM"            ) referenceSignal = { "T2bw-075", };
        */
    #endif

  printBoxedMessage("Starting plot generation");

  // ####################
  // ##   Init tools   ##
  // ####################

     // Create a sonic Screwdriver
      SonicScrewdriver screwdriver;

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     screwdriver.AddProcessClass("1ltop", "1l top",                             "background",kRed-7);
            screwdriver.AddDataset("ttbar_powheg",   "1ltop",  0, 0);
            screwdriver.AddDataset("singleTop_st",   "1ltop",  0, 0);

     screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",kCyan-3);

     screwdriver.AddProcessClass("W+jets",   "W+jets",                          "background",kOrange-2);
             screwdriver.AddDataset("W+jets",    "W+jets", 0, 0);

     screwdriver.AddProcessClass("rare",   "rare",                              "background",kMagenta-5);
             screwdriver.AddDataset("rare",   "rare", 0, 0);

     screwdriver.AddProcessClass(referenceSignal.type,     referenceSignal.type,        "signal",   kViolet-1);
             screwdriver.AddDataset(referenceSignal.type,  referenceSignal.type,   0, 0);

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     screwdriver.AddRegion("MTpeak",               "MT peak Control Region",                            &goesInPreselectionMTpeak_withSRCuts        );
     screwdriver.AddRegion("0btag_MTpeak",         "0 b-tag Control Region, MT peak",                   &goesIn0BtagControlRegionMTpeak_withSRCuts  );
     screwdriver.AddRegion("0btag_MTtail",         "0 b-tag Control Region, MT tail",                   &goesIn0BtagControlRegionMTtail_withSRCuts  );
     screwdriver.AddRegion("2leptons",             "2 leptons Control Region",                          &goesInDileptonControlRegionNoMT_withSRCuts );
     screwdriver.AddRegion("reversedVeto_MTpeak",  "Reversed 2nd lepton veto Control Region, MT peak",  &goesInVetoControlRegionMTpeak_withSRCuts   );
     screwdriver.AddRegion("reversedVeto_MTtail",  "Reversed 2nd lepton veto Control Region, MT tail",  &goesInVetoControlRegionMTpeak_withSRCuts   );

     // ##########################
     // ##   Create Channels    ##
     // ##########################

     screwdriver.AddChannel("allChannels",  "",                 &goesInAnyChannel         );

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     // Create histograms
     screwdriver.Create1DHistos();
     screwdriver.SchedulePlots("1DStack");

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
         //if (i > nEntries * 0.03) break;
         if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

         // Get the i-th entry
         ReadEvent(theTree,i,&pointers,&myEvent);

         float weight = getWeight();

         // Split 1-lepton ttbar and 2-lepton ttbar
         string currentProcessClass_ = currentProcessClass;
         if ((currentDataset == "ttbar_powheg") && (myEvent.numberOfGenLepton == 2))
             currentProcessClass_ = "ttbar_2l";

         if (currentDataset == referenceSignal.type)
         {
             if ((myEvent.mStop       == referenceSignal.stopMass)
              && (myEvent.mNeutralino == referenceSignal.neutralinoMass))
                 screwdriver.AutoFillProcessClass(currentProcessClass_,weight);
         }
         else
         {
             screwdriver.AutoFillProcessClass(currentProcessClass_,weight);
         }

     }
     printProgressBar(nEntries,nEntries,currentDataset);
     cout << endl;
     f.Close();

  }

  // ###################################
  // ##   Make plots and write them   ##
  // ###################################

  screwdriver.MakePlots();

  printBoxedMessage("Plot generation completed");

  vector<string> regions = { "MTpeak", "0btag_MTpeak", "0btag_MTtail", "2leptons", "reversedVeto_MTpeak", "reversedVeto_MTtail" };
  TableBackgroundSignal contaminationCheck(&screwdriver,regions,"allChannels");
  contaminationCheck.Print();

  vector<string> dummyLabel = { SIGNAL_REGION_TAG };
  Table contaminationSummary(regions,dummyLabel);
  for (unsigned int r = 0 ; r < regions.size() ; r++)
  {
     contaminationSummary.Set(regions[r],SIGNAL_REGION_TAG,  contaminationCheck.Get(regions[r],referenceSignal.type)
                                                           / contaminationCheck.Get(regions[r],"totalSM"));
  }
  contaminationSummary.Print(string("rawTables/")+SIGNAL_REGION_TAG+".tab",3);

  // #############################
  // ##   Post-plotting tests   ##
  // #############################

  printBoxedMessage("Program done.");
  return (0);
}


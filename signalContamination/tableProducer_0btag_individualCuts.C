#include "../backgroundEstimation_common/common.h"

bool goesIn0BtagControlRegionMTtail_withSRCuts()    { return (goesIn0BtagControlRegionMTtail() && SIGNAL_REGION_CUTS(enableMTCut) ); }
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
    printBoxedMessage("Starting plot generation");

  // ####################
  // ##   Init tools   ##
  // ####################

     // Create a sonic Screwdriver
      SonicScrewdriver screwdriver;

     // ####################
     // ##   Init tools   ##
     // ####################

     screwdriver.AddVariable("MET",            "MET",                     "GeV",    16,50,530,      &(myEvent.MET),                  "logY");
     screwdriver.AddVariable("MT",             "M_{T}",                   "GeV",    40,0,400,       &(myEvent.MT),                   "logY");
     screwdriver.AddVariable("MT2W",           "M_{T2}^{W}",              "GeV",    20,0,500,       &(myEvent.MT2W),                 "");
     screwdriver.AddVariable("METoverSqrtHT",  "MET / #sqrt{H_{T}}",      "",       32,0,32,        &(myEvent.METoverSqrtHT),        "");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     screwdriver.AddProcessClass("1ltop", "1l top",                             "background", kRed-7);
            screwdriver.AddDataset("ttbar_powheg",   "1ltop",  0, 0);
            screwdriver.AddDataset("singleTop_st",   "1ltop",  0, 0);

     screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background", kCyan-3);

     screwdriver.AddProcessClass("W+jets",   "W+jets",                          "background", kOrange-2);
             screwdriver.AddDataset("W+jets",    "W+jets", 0, 0);

     screwdriver.AddProcessClass("rare",   "rare",                              "background", kMagenta-5);
             screwdriver.AddDataset("rare",   "rare", 0, 0);

             screwdriver.AddDataset("T2tt",  "T2tt",   0, 0);
     screwdriver.AddProcessClass("T2tt_225_75" , "T2tt (225/75)" ,              "signal", 0);
     screwdriver.AddProcessClass("T2tt_325_200", "T2tt (325/200)",              "signal", 0);
     screwdriver.AddProcessClass("T2tt_250_25" , "T2tt (250/25)" ,              "signal", 0);
     screwdriver.AddProcessClass("T2tt_400_100", "T2tt (400/100)",              "signal", 0);
     screwdriver.AddProcessClass("T2tt_600_100", "T2tt (600/100)",              "signal", 0);

             screwdriver.AddDataset("T2bw-025",  "T2bw025",   0, 0);
     screwdriver.AddProcessClass("T2bw025_250_100", "T2bw (250/100/0.25)",      "signal", 0);
     screwdriver.AddProcessClass("T2bw025_400_100", "T2bw (400/100/0.25)",      "signal", 0);
     screwdriver.AddProcessClass("T2bw025_650_100", "T2bw (650/100/0.25)",      "signal", 0);

             screwdriver.AddDataset("T2bw-050",  "T2bw050",   0, 0);
     screwdriver.AddProcessClass("T2bw050_250_25",  "T2bw (250/25 /0.50)" ,     "signal", 0);
     screwdriver.AddProcessClass("T2bw050_250_100", "T2bw (250/100/0.50)",      "signal", 0);
     screwdriver.AddProcessClass("T2bw050_400_100", "T2bw (400/100/0.50)",      "signal", 0);
     screwdriver.AddProcessClass("T2bw050_600_100", "T2bw (600/100/0.50)",      "signal", 0);

             screwdriver.AddDataset("T2bw-075",  "T2bw075",   0, 0);
     screwdriver.AddProcessClass("T2bw075_250_100", "T2bw (250/100/0.75)",      "signal", 0);
     screwdriver.AddProcessClass("T2bw075_350_50",  "T2bw (350/50 /0.75)",      "signal", 0);
     screwdriver.AddProcessClass("T2bw075_600_100", "T2bw (600/100/0.75)",      "signal", 0);

     vector<string> referenceSignals =
     {
         "T2tt_225_75" ,
         "T2tt_325_200",
         "T2tt_250_25" ,
         "T2tt_400_100",
         "T2tt_600_100",
         "T2bw025_250_100",
         "T2bw025_400_100",
         "T2bw025_650_100",
         "T2bw050_250_25",
         "T2bw050_250_100",
         "T2bw050_400_100",
         "T2bw050_600_100",
         "T2bw075_250_100",
         "T2bw075_350_50",
         "T2bw075_600_100",
     };

     vector<string> cutAndCountRegionTags =
     {
        "cutAndCount_T2tt_offShellLoose",
        "cutAndCount_T2tt_offShellTight",
        "cutAndCount_T2tt_lowDeltaM",
        "cutAndCount_T2tt_mediumDeltaM",
        "cutAndCount_T2tt_highDeltaM",
        "cutAndCount_T2bw025_offShell",
        "cutAndCount_T2bw025_lowMasses",
        "cutAndCount_T2bw025_highMasses",
        "cutAndCount_T2bw050_offShell",
        "cutAndCount_T2bw050_lowMasses",
        "cutAndCount_T2bw050_mediumDeltaM",
        "cutAndCount_T2bw050_highDeltaM",
        "cutAndCount_T2bw075_lowDeltaM",
        "cutAndCount_T2bw075_mediumDeltaM",
        "cutAndCount_T2bw075_highDeltaM"
     };

     vector<float> MTcutUsed =
     {
        125,
        130,
        140,
        140,
        130,
        120,
        120,
        120,
        120,
        135,
        140,
        120,
        120,
        130,
        140
     };

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     vector<Cut> individualCuts =
     {
        Cut("MT",   '>', 100),
        Cut("MT",   '>', 115),
        Cut("MT",   '>', 110),
        Cut("MT",   '>', 120),
        Cut("MT",   '>', 125),
        Cut("MT",   '>', 130),
        Cut("MT",   '>', 135),
        Cut("MT",   '>', 140),

        Cut("MET",  '>', 100),
        Cut("MET",  '>', 200),
        Cut("MET",  '>', 300),

        Cut("METoverSqrtHT",  '>', 5),
        Cut("METoverSqrtHT",  '>', 7),
        Cut("METoverSqrtHT",  '>', 9)
     };

     vector<string> targetRegions = { "0btag_MTtail" };

     screwdriver.AddRegion("0btag_MTtail",         "0 b-tag Control Region, MT tail",                   &goesIn0BtagControlRegionMTtail_withSRCuts  );

     for (unsigned int r = 0 ; r < targetRegions.size() ; r++)
     for (unsigned int i = 0 ; i < individualCuts.size() ; i++)
     {
        string tag   = individualCuts[i].getTargetTag()
               + "_" + floatToString(individualCuts[i].getValue());

        string label = individualCuts[i].getTargetTag()
               + " " + individualCuts[i].getType()
               + " " + floatToString(individualCuts[i].getValue());

        vector<Cut> individualCut = { individualCuts[i] };
        screwdriver.AddRegion(targetRegions[r]+"_"+tag, label, targetRegions[r], individualCut);
     }

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

         if (findSubstring(currentDataset,"T2"))
         {
                  if ((currentDataset == "T2tt")     && (myEvent.mStop == 450) && (myEvent.mNeutralino == 100)) screwdriver.AutoFillProcessClass("T2tt_450_100",    weight);
             else if ((currentDataset == "T2tt")     && (myEvent.mStop == 225) && (myEvent.mNeutralino == 75 )) screwdriver.AutoFillProcessClass("T2tt_225_75" ,    weight);
             else if ((currentDataset == "T2tt")     && (myEvent.mStop == 325) && (myEvent.mNeutralino == 200)) screwdriver.AutoFillProcessClass("T2tt_325_200",    weight);
             else if ((currentDataset == "T2tt")     && (myEvent.mStop == 250) && (myEvent.mNeutralino == 25 )) screwdriver.AutoFillProcessClass("T2tt_250_25" ,    weight);
             else if ((currentDataset == "T2tt")     && (myEvent.mStop == 400) && (myEvent.mNeutralino == 100)) screwdriver.AutoFillProcessClass("T2tt_400_100",    weight);
             else if ((currentDataset == "T2tt")     && (myEvent.mStop == 600) && (myEvent.mNeutralino == 100)) screwdriver.AutoFillProcessClass("T2tt_600_100",    weight);
             else if ((currentDataset == "T2bw-025") && (myEvent.mStop == 250) && (myEvent.mNeutralino == 100)) screwdriver.AutoFillProcessClass("T2bw025_250_100", weight);
             else if ((currentDataset == "T2bw-025") && (myEvent.mStop == 400) && (myEvent.mNeutralino == 100)) screwdriver.AutoFillProcessClass("T2bw025_400_100", weight);
             else if ((currentDataset == "T2bw-025") && (myEvent.mStop == 650) && (myEvent.mNeutralino == 100)) screwdriver.AutoFillProcessClass("T2bw025_650_100", weight);
             else if ((currentDataset == "T2bw-050") && (myEvent.mStop == 250) && (myEvent.mNeutralino == 25 )) screwdriver.AutoFillProcessClass("T2bw050_250_25" , weight);
             else if ((currentDataset == "T2bw-050") && (myEvent.mStop == 250) && (myEvent.mNeutralino == 100)) screwdriver.AutoFillProcessClass("T2bw050_250_100", weight);
             else if ((currentDataset == "T2bw-050") && (myEvent.mStop == 400) && (myEvent.mNeutralino == 100)) screwdriver.AutoFillProcessClass("T2bw050_400_100", weight);
             else if ((currentDataset == "T2bw-050") && (myEvent.mStop == 600) && (myEvent.mNeutralino == 100)) screwdriver.AutoFillProcessClass("T2bw050_600_100", weight);
             else if ((currentDataset == "T2bw-075") && (myEvent.mStop == 250) && (myEvent.mNeutralino == 100)) screwdriver.AutoFillProcessClass("T2bw075_250_100", weight);
             else if ((currentDataset == "T2bw-075") && (myEvent.mStop == 350) && (myEvent.mNeutralino == 50 )) screwdriver.AutoFillProcessClass("T2bw075_350_50" , weight);
             else if ((currentDataset == "T2bw-075") && (myEvent.mStop == 600) && (myEvent.mNeutralino == 100)) screwdriver.AutoFillProcessClass("T2bw075_600_100", weight);
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

  for (unsigned int t = 0 ; t < targetRegions.size() ; t++)
  {
      vector<string> regions;
      for (unsigned int i = 0 ; i < individualCuts.size() ; i++)
      {
          string tag   = individualCuts[i].getTargetTag()
              + "_" + floatToString(individualCuts[i].getValue());

          regions.push_back(targetRegions[t]+"_"+tag);
      }

      TableBackgroundSignal contaminationCheck(&screwdriver,regions,"allChannels");
      contaminationCheck.Print();

      Table contaminationSummary({ "MTcut" },cutAndCountRegionTags);
      for (unsigned int s = 0 ; s < referenceSignals.size() ; s++)
      {
          string regionLabel = "0btag_MTtail_MT_"+floatToString(MTcutUsed[s]);

          contaminationSummary.Set("MTcut", cutAndCountRegionTags[s], contaminationCheck.Get(regionLabel,referenceSignals[s])
                                                                    / contaminationCheck.Get(regionLabel,"totalSM"          ));
      }

      contaminationSummary.Print("rawTables/0btag.tab",3);
  }

  // #############################
  // ##   Post-plotting tests   ##
  // #############################

  printBoxedMessage("Program done.");
  return (0);
}


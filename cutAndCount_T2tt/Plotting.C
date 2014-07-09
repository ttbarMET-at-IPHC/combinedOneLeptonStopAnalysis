
#define SYST_UNCERTAINTY            0.15

#include "common.h"

void formatAndWriteMapPlot(SonicScrewdriver* screwdriver, TH2F* theHisto, string name, string comment, bool enableText);

// #########################################################################
//                          Region selectors
// #########################################################################

bool Selector_cutAndCount(float cutMEToverSqrtHT, float cutMT, float cutMT2W, float cutMET, float cutDeltaPhi, float cutHadronicChi2, bool enableISRJetRequirement)
{
    if (myEvent.METoverSqrtHT   < cutMEToverSqrtHT) return false;
    if (myEvent.MT              < cutMT)            return false;
    if (myEvent.MT2W            < cutMT2W)          return false;
    if (myEvent.MET             < cutMET)           return false;
    if (myEvent.deltaPhiMETJets < cutDeltaPhi)      return false;
    if (myEvent.hadronicChi2    > cutHadronicChi2)  return false;
    if ((enableISRJetRequirement) && (!myEvent.ISRJet))     return false;

    return goesInPreselectionMTtail();
}

/*
                                                                    // METsig MT  MT2W  MET  dPhi  Chi2   ISRjet
*/
/*
bool Selector_cutAndCount_offShellLoose() { return Selector_cutAndCount(-1,   120, -1,  200, -1,  999999, true ); }
bool Selector_cutAndCount_offShellTight() { return Selector_cutAndCount(10,   140, -1,  -1,  -1,  999999, true ); }
bool Selector_cutAndCount_lowDeltaM()     { return Selector_cutAndCount(-1,   130, -1,  130, 0.8, 5,      false); }
bool Selector_cutAndCount_mediumDeltaM()  { return Selector_cutAndCount(10,   140, 180, -1,  0.8, 3,      false); }
bool Selector_cutAndCount_highDeltaM()    { return Selector_cutAndCount(15,   190, 240, -1,  -1,  999999, false); }
*/

bool Selector_cutAndCount_offShellLoose() { return Selector_cutAndCount(8,    120, -1,  -1,  -1,  999999, true ); }
bool Selector_cutAndCount_offShellTight() { return Selector_cutAndCount(10,   140, -1,  -1,  -1,  999999, true ); }
bool Selector_cutAndCount_lowDeltaM()     { return Selector_cutAndCount(8,    140, -1,  -1,  0.8, 5,      false); }
bool Selector_cutAndCount_mediumDeltaM()  { return Selector_cutAndCount(10,   160, 180, -1,  0.8, 3,      false); }
bool Selector_cutAndCount_highDeltaM()    { return Selector_cutAndCount(15,   190, 240, -1,  -1,  999999, false); }


bool Selector_MTAnalysis(float METcut, bool useMT2Wcut)
{
    // Don't consider muon with pT < 25 for MT analysis
    if ((abs(myEvent.leadingLeptonPDGId) == 13) 
         && (myEvent.leadingLepton.Pt()   < 25)) return false;

    if (myEvent.deltaPhiMETJets < 0.8) return false;
    if (myEvent.hadronicChi2    > 5) return false;
    if (myEvent.MT              < 120) return false;
    if (myEvent.MET             < METcut) return false;
    if (useMT2Wcut && (myEvent.MT2W < 200)) return false;

    return goesInPreselectionMTtail();
}

bool Selector_MTAnalysis_LM150() { return Selector_MTAnalysis(150,false); }
bool Selector_MTAnalysis_LM200() { return Selector_MTAnalysis(200,false); }
bool Selector_MTAnalysis_LM250() { return Selector_MTAnalysis(250,false); }
bool Selector_MTAnalysis_LM300() { return Selector_MTAnalysis(300,false); }
bool Selector_MTAnalysis_HM150() { return Selector_MTAnalysis(150,true);  }
bool Selector_MTAnalysis_HM200() { return Selector_MTAnalysis(200,true);  }
bool Selector_MTAnalysis_HM250() { return Selector_MTAnalysis(250,true);  }
bool Selector_MTAnalysis_HM300() { return Selector_MTAnalysis(300,true);  }

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

     screwdriver.AddVariable("MET",            "MET",                     "GeV",    15,50,500,      &(myEvent.MET),                  "logY=true");
/*
     screwdriver.AddVariable("MT",             "MT",                      "GeV",    17,0,510,       &(myEvent.MT),                   "logY=true");
     screwdriver.AddVariable("deltaPhiMETJets","#Delta#Phi(MET,j_{1,2})", "rad",    16,0,3.2,       &(myEvent.deltaPhiMETJets),      "");
     screwdriver.AddVariable("MT2W",           "M_{T2}^{W}",              "GeV",    20,0,500,       &(myEvent.MT2W),                 "");
     screwdriver.AddVariable("HTratio",        "H_{T}^{ratio}",           "",       20,0,1.2,       &(myEvent.HTRatio),              "");
     screwdriver.AddVariable("HadronicChi2",   "Hadronic #chi^{2}",       "",       40,0,20,        &(myEvent.hadronicChi2),         "");
     screwdriver.AddVariable("HT",             "H_{T}",                   "",       45,150,1500,    &(myEvent.HT),                   "");
     screwdriver.AddVariable("leadingBPt",     "p_{T}(leading b-jet)",    "GeV",    25,0,500,       &(myEvent.leadingBPt),           "");
     screwdriver.AddVariable("leadingJetPt",   "p_{T}(leading jet)",      "GeV",    25,0,500,       &(myEvent.leadingJetPt),         "");
     screwdriver.AddVariable("leptonPt",       "p_{T}(lepton)",           "GeV",    25,0,500,       &(myEvent.leadingLeptonPt),      "");
     screwdriver.AddVariable("Mlb",            "M_{lb}",                  "GeV",    25,0,500,       &(myEvent.Mlb),                  "");
     screwdriver.AddVariable("Mlb_hemi",       "M_{lb}_hemi",             "GeV",    25,0,500,       &(myEvent.Mlb_hemi),             "");
     screwdriver.AddVariable("M3b",            "M3b",                     "GeV",    25,0,500,       &(myEvent.M3b),                  "");
     screwdriver.AddVariable("deltaRLeptonB",  "#DeltaR(l,leading b)",    "",       25,0,5,         &(myEvent.deltaRLeptonLeadingB), "");
     screwdriver.AddVariable("HTLeptonPtMET",  "HT + MET + p_{T}(lepton)","GeV",    20,100,2100,    &(myEvent.HTPlusLeptonPtPlusMET),"");
     screwdriver.AddVariable("METoverSqrtHT",  "MET / #sqrt{H_{T}}",      "",       32,0,32,        &(myEvent.METoverSqrtHT),        "");
*/
     
     screwdriver.AddVariable("mStop",          "m_{#tilde{t}}",           "GeV",    28,112.5,812.5,  &(myEvent.mStop),               "");
     screwdriver.AddVariable("mNeutralino",    "m_{#chi^{0}}",            "GeV",    16,-12.5,387.5,  &(myEvent.mNeutralino),         "noOverflowInLastBin");
     
     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     screwdriver.AddProcessClass("1ltop", "1l top",                             "background",kRed-7);
            screwdriver.AddDataset("ttbar_powheg",   "1ltop",  0, 0);
            screwdriver.AddDataset("singleTop_st",   "1ltop",  0, 0);
     
     screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",kCyan-3);

     screwdriver.AddProcessClass("W+jets",         "W+jets",                          "background", kOrange-2);
             screwdriver.AddDataset("W+jets",      "W+jets", 0, 0);
                                                   
     screwdriver.AddProcessClass("rare",           "rare",                            "background", kMagenta-5);
             screwdriver.AddDataset("rare",        "rare", 0, 0);
                                                   
     screwdriver.AddProcessClass("T2tt",           "T2tt",                            "signal",     kViolet-1);
             screwdriver.AddDataset("T2tt",        "T2tt",   0, 0);

     screwdriver.AddProcessClass("signal_250_100",  "T2tt (250/100)",      "signal",COLORPLOT_BLUE   );
     screwdriver.AddProcessClass("signal_450_100",  "T2tt (450/100)",      "signal",COLORPLOT_GREEN2 ); 
     screwdriver.AddProcessClass("signal_650_100",  "T2tt (650/100)",      "signal",COLORPLOT_GREEN  );

  // ##########################
  // ##    Create Regions    ##
  // ##########################

     screwdriver.AddRegion("presel",            "Preselection",              &goesInPreselectionMTtail);

     screwdriver.AddRegion("CC_offShell_Loose", "Cut&Count;Off-shell Loose", &Selector_cutAndCount_offShellLoose);
     screwdriver.AddRegion("CC_offShell_Tight", "Cut&Count;Off-shell tight", &Selector_cutAndCount_offShellTight);
     screwdriver.AddRegion("CC_lowDM",          "Cut&Count;Low #DeltaM",     &Selector_cutAndCount_lowDeltaM    );
     screwdriver.AddRegion("CC_mediumDM",       "Cut&Count;Medium #DeltaM",  &Selector_cutAndCount_mediumDeltaM );
     screwdriver.AddRegion("CC_highDM",         "Cut&Count;High #DeltaM",    &Selector_cutAndCount_highDeltaM   );
   

     /*
     screwdriver.AddRegion("MT_LM150",          "MT analysis (LM 150)",      &Selector_MTAnalysis_LM150);
     screwdriver.AddRegion("MT_LM200",          "MT analysis (LM 200)",      &Selector_MTAnalysis_LM200);
     screwdriver.AddRegion("MT_LM250",          "MT analysis (LM 250)",      &Selector_MTAnalysis_LM250);
     screwdriver.AddRegion("MT_LM300",          "MT analysis (LM 300)",      &Selector_MTAnalysis_LM300);
     screwdriver.AddRegion("MT_HM150",          "MT analysis (HM 150)",      &Selector_MTAnalysis_HM150);
     screwdriver.AddRegion("MT_HM200",          "MT analysis (HM 200)",      &Selector_MTAnalysis_HM200);
     screwdriver.AddRegion("MT_HM250",          "MT analysis (HM 250)",      &Selector_MTAnalysis_HM250);
     screwdriver.AddRegion("MT_HM300",          "MT analysis (HM 300)",      &Selector_MTAnalysis_HM300);
     */

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

  cout << "   > Reading datasets... " << endl;
  cout << endl;

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

      int nEntries = theTree->GetEntries();
      for (int i = 0 ; i < nEntries ; i++)
      //for (int i = 0 ; i < min(200000, (int) theTree->GetEntries()); i++)
      {
          if (i % (theTree->GetEntries() / 50) == 0) 
              printProgressBar(i,nEntries,currentDataset);

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          // Split 1-lepton ttbar and 2-lepton ttbar
          string currentProcessClass_ = currentProcessClass;
          if ((currentDataset == "ttbar_powheg") && (myEvent.numberOfGenLepton == 2)) 
              currentProcessClass_ = "ttbar_2l";

          screwdriver.AutoFillProcessClass(currentProcessClass_,getWeight());

          if ((myEvent.mStop == 250) && (myEvent.mNeutralino == 100))
              screwdriver.AutoFillProcessClass("signal_250_100",getWeight());
          if ((myEvent.mStop == 450) && (myEvent.mNeutralino == 100))
              screwdriver.AutoFillProcessClass("signal_450_100",getWeight());
          if ((myEvent.mStop == 650) && (myEvent.mNeutralino == 100))
              screwdriver.AutoFillProcessClass("signal_650_100",getWeight());
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
  screwdriver.WritePlots("../plots/cutAndCount_T2tt/");

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################
  
  printBoxedMessage("Now computing misc tests ... ");

  // ##########################
  // ##   Compute FOM maps   ##
  // ##########################

  vector<string> cutAndCountRegions =
  {
      "CC_offShell_Loose",
      "CC_offShell_Tight",
      "CC_lowDM",
      "CC_mediumDM",
      "CC_highDM"
  };


  vector<float> oneLepTopScaleFactor =
  {
      2, 
      10,
      4,
      10,
      5,
  };

  vector<float> ttbar2lScaleFactor =
  {
      1.5,
      1.3,
      1.5,
      2,
      8,
  };

  vector<float> globalBackgroundUncertainty =
  {
      0.2,
      0.23,
      0.2,
      0.33,
      0.43
  };

  vector<TH2F*> signalMaps;
  //vector<TH2F*> backgroundMaps;
  vector<TH2F*> FOMMaps;
  vector<TH2F*> efficiencies;

  int nBinsX = -1;
  int nBinsY = -1;

  TH2F* signalMapPresel  = screwdriver.get2DHistoClone("mStop","mNeutralino","T2tt","presel","singleLepton");
  TH2F* backgroundPresel = screwdriver.get2DCompositeHistoClone("mStop","mNeutralino","2DSumBackground","presel","singleLepton","");

  if (nBinsX == -1) nBinsX = signalMapPresel->GetNbinsX();
  if (nBinsY == -1) nBinsY = signalMapPresel->GetNbinsY();

  // Store background eff in (mStop,mLSP) = (200,300)
  int backgroundBin = signalMapPresel->FindBin(200,300);
  float backgroundYieldPresel = backgroundPresel->Integral(0,nBinsX+1,0,nBinsY+1);

  for (unsigned int i = 0 ; i < cutAndCountRegions.size() ; i++)
  {
      signalMaps.push_back(screwdriver.get2DHistoClone("mStop","mNeutralino","T2tt",cutAndCountRegions[i],"singleLepton"));
      signalMaps[i]->SetName((string("signalMap_")+cutAndCountRegions[i]).c_str());
      //backgroundMaps.push_back(screwdriver.get2DCompositeHistoClone("mStop","mNeutralino","2DSumBackground",cutAndCountRegions[i],"singleLepton",""));

      float B =   screwdriver.GetYieldAndError("1ltop",    cutAndCountRegions[i],"singleLepton").value() * oneLepTopScaleFactor[i]
                + screwdriver.GetYieldAndError("ttbar_2l", cutAndCountRegions[i],"singleLepton").value() * ttbar2lScaleFactor[i]
                + screwdriver.GetYieldAndError("W+jets",   cutAndCountRegions[i],"singleLepton").value() 
                + screwdriver.GetYieldAndError("rare",     cutAndCountRegions[i],"singleLepton").value();

      //float B = backgroundMaps[i]->Integral(0,nBinsX+1,0,nBinsY+1);

      // Apply scale factor from background prediction
      float f_B = globalBackgroundUncertainty[i];
      //float f_B = 0.15;

      if (B < 1.0) B = 1.0;
 
      efficiencies.push_back((TH2F*) signalMaps[i]->Clone());
      efficiencies[i]->SetName((string("eff_")+cutAndCountRegions[i]).c_str());
      efficiencies[i]->Divide(signalMapPresel);
      efficiencies[i]->SetBinContent(backgroundBin,B/backgroundYieldPresel);
      
      FOMMaps.push_back((TH2F*) signalMaps[i]->Clone());
      FOMMaps[i]->SetName((string("FOM_")+cutAndCountRegions[i]).c_str());

      for (int x = 1 ; x <= nBinsX ; x++)
      for (int y = 1 ; y <= nBinsY ; y++)
      {
          float S = signalMaps[i]->GetBinContent(x,y);

          float FOM = figureOfMerit(S,B,"discovery",false,f_B);
          FOMMaps[i]->SetBinContent(x,y,FOM);
      }

  }

  // ################################
  // ##   Compute "best" FOM map   ##
  // ################################

  TH2F* bestFOMMap = (TH2F*) signalMaps[0]->Clone();  bestFOMMap->SetName("bestFOM");
  TH2F* bestSetMap = (TH2F*) signalMaps[0]->Clone();  bestSetMap->SetName("bestSet");
  TH2F* bestSigEff = (TH2F*) signalMaps[0]->Clone();  bestSigEff->SetName("bestSigEff");
  TH2F* bestBkgEff = (TH2F*) signalMaps[0]->Clone();  bestBkgEff->SetName("bestBkgEff");
  
  for (int x = 1 ; x <= nBinsX ; x++)
  for (int y = 1 ; y <= nBinsY ; y++)
  {
      float bestFOM = -1.0;
      int bestSet = 0;
      float bestSigEff_ = -1.0;
      float bestBkgEff_ = -1.0;
      for (unsigned int i = 0 ; i < cutAndCountRegions.size() ; i++)
      {
          float FOM = FOMMaps[i]->GetBinContent(x,y);
          if (bestFOM < FOM)
          {
              bestFOM = FOM;
              if (bestFOM > 0) bestSet = i+1;
              bestSigEff_ = efficiencies[i]->GetBinContent(x,y);
              bestBkgEff_ = efficiencies[i]->GetBinContent(backgroundBin);
          }
      }
      bestFOMMap->SetBinContent(x,y,bestFOM);
      bestSetMap->SetBinContent(x,y,bestSet);
      bestSigEff->SetBinContent(x,y,bestSigEff_);
      bestBkgEff->SetBinContent(x,y,bestBkgEff_);
  }

  // #####################################
  // ##   Compute map for MT analysis   ##
  // #####################################

  /*
  vector<string> cutAndCountRegions_MTanalysis;
  cutAndCountRegions_MTanalysis.push_back("MT_LM150");
  cutAndCountRegions_MTanalysis.push_back("MT_LM200");
  cutAndCountRegions_MTanalysis.push_back("MT_LM250");
  cutAndCountRegions_MTanalysis.push_back("MT_LM300");
  cutAndCountRegions_MTanalysis.push_back("MT_HM150");
  cutAndCountRegions_MTanalysis.push_back("MT_HM200");
  cutAndCountRegions_MTanalysis.push_back("MT_HM250");
  cutAndCountRegions_MTanalysis.push_back("MT_HM300");

  vector<TH2F*> signalMaps_MTanalysis;
  vector<TH2F*> backgroundMaps_MTanalysis;
  vector<TH2F*> FOMMaps_MTanalysis;

  for (unsigned int i = 0 ; i < cutAndCountRegions_MTanalysis.size() ; i++)
  {
      signalMaps_MTanalysis.push_back(screwdriver.get2DHistoClone("mStop","mNeutralino","T2tt",cutAndCountRegions_MTanalysis[i],"singleLepton"));
      backgroundMaps_MTanalysis.push_back(screwdriver.get2DCompositeHistoClone("mStop","mNeutralino","2DSumBackground",cutAndCountRegions_MTanalysis[i],"singleLepton",""));

      FOMMaps_MTanalysis.push_back((TH2F*) signalMaps_MTanalysis[i]->Clone());
      FOMMaps_MTanalysis[i]->SetName((string("FOM_")+cutAndCountRegions_MTanalysis[i]).c_str());

      float B = backgroundMaps_MTanalysis[i]->Integral(0,nBinsX+1,0,nBinsY+1);
      if (B < 1.0) B = 1.0;

      for (int x = 1 ; x <= nBinsX ; x++)
      for (int y = 1 ; y <= nBinsY ; y++)
      {
          float S = signalMaps_MTanalysis[i]->GetBinContent(x,y);
          float FOM = figureOfMerit(S,B,"discovery",false);
          FOMMaps_MTanalysis[i]->SetBinContent(x,y,FOM);
      }
  }
  
 
  
  TH2F* bestFOMMap_MTanalysis = (TH2F*) FOMMaps_MTanalysis[0]->Clone();
  bestFOMMap_MTanalysis->SetName("bestFOMMap_MTanalysis");
  TH2F* ratio_newCC_MTanalysisCC = (TH2F*) FOMMaps_MTanalysis[0]->Clone();
  ratio_newCC_MTanalysisCC->SetName("ratio_newCC_MTanalysisCC");
  
  TFile bestSetMap_MTanalysis_File("CC_MTanalysis_mapBestSet.root","READ");
  TH2F* bestSetMap_MTanalysis;
  bestSetMap_MTanalysis_File.GetObject("map",bestSetMap_MTanalysis);
  bestSetMap_MTanalysis->SetName("bestSetMap_MTanalysis");

  for (int x = 1 ; x <= nBinsX ; x++)
  for (int y = 1 ; y <= nBinsY ; y++)
  {
      int bestSet = bestSetMap_MTanalysis->GetBinContent(x,y);
      if (bestSet == 0) continue;
      float bestFOM = FOMMaps_MTanalysis[bestSet-1]->GetBinContent(x,y);
      bestFOMMap_MTanalysis->SetBinContent(x,y,bestFOM);
      
      if (bestFOM == 0)
        ratio_newCC_MTanalysisCC->SetBinContent(x,y,0.0);
      else
        ratio_newCC_MTanalysisCC->SetBinContent(x,y,bestFOMMap->GetBinContent(x,y)/bestFOM);
  }
  */
  //bestSetMap_MTanalysis_File.Close();

  // #################################
  // ##   Make ratio map with BDT   ##
  // #################################

  /*
  TH2F* ratio_newCC_newBDT = (TH2F*) FOMMaps_MTanalysis[0]->Clone();
  ratio_newCC_newBDT->SetName("ratio_newCC_newBDT");
  for (int x = 1 ; x <= nBinsX ; x++)
  for (int y = 1 ; y <= nBinsY ; y++)
  {
      ratio_newCC_newBDT->SetBinContent(x,y,0.0);
  }


  TFile bestFOMMap_BDT_File("BDT_mapFOM.root","READ");
  TH2D* bestFOMMap_BDT;
  bestFOMMap_BDT_File.GetObject("twodplot",bestFOMMap_BDT);

  for (int x = 3 ; x <= nBinsX ; x++)
  for (int y = 2 ; y <= nBinsY ; y++)
  {
      float bestFOM = bestFOMMap_BDT->GetBinContent(x-2,y-1);
      if (bestFOM == 0)
        ratio_newCC_newBDT->SetBinContent(x,y,0.0);
      else 
      {
        float ratio_ = bestFOMMap->GetBinContent(x,y)/bestFOM;
        if (ratio_ >= 2) ratio_ = 2;
        ratio_newCC_newBDT->SetBinContent(x,y,ratio_);
      }
  }
  */
 
  // #########################
  // ##   Save those maps   ##
  // #########################

  TFile fOutput("../plots/cutAndCount_T2tt/custom.root","RECREATE");
  gStyle->SetPaintTextFormat("4.0f");
  formatAndWriteMapPlot(&screwdriver,bestSetMap,bestSetMap->GetName(),"T2tt;Best set of cuts",true);
  gStyle->SetPaintTextFormat("4.1f");
  for (unsigned int i = 0 ; i < cutAndCountRegions.size() ; i++)
  {
      FOMMaps[i]->SetMaximum(5.0);
      formatAndWriteMapPlot(&screwdriver,FOMMaps[i],FOMMaps[i]->GetName(),string("FOM for ")+cutAndCountRegions[i], true);
      formatAndWriteMapPlot(&screwdriver,efficiencies[i],efficiencies[i]->GetName(),string("Efficiencies for")+cutAndCountRegions[i], true);
  }
  bestFOMMap->SetMaximum(5.0);
  //bestFOMMap_MTanalysis->SetMaximum(5.0);
  //ratio_newCC_MTanalysisCC->SetMaximum(2.0);
  //ratio_newCC_newBDT->SetMaximum(2.0);
  formatAndWriteMapPlot(&screwdriver,bestFOMMap,bestFOMMap->GetName(),"T2tt;Best FOM",true);
  formatAndWriteMapPlot(&screwdriver,bestSigEff,bestSigEff->GetName(),"T2tt;Best signal efficiency",true);
  formatAndWriteMapPlot(&screwdriver,bestBkgEff,bestBkgEff->GetName(),"T2tt;Best backgr efficiency",true);
  //formatAndWriteMapPlot(&screwdriver,bestFOMMap_MTanalysis,bestFOMMap_MTanalysis->GetName(),"T2tt;Best FOM;from MT analysis",true);
  //formatAndWriteMapPlot(&screwdriver,ratio_newCC_MTanalysisCC,ratio_newCC_MTanalysisCC->GetName(),"T2tt;FOM gain;wrt MT analysis",true);
  //formatAndWriteMapPlot(&screwdriver,ratio_newCC_newBDT,ratio_newCC_newBDT->GetName(),"T2tt;FOM gain;wrt BDT",true);
  fOutput.Close();

  printBoxedMessage("Program done.");
  return (0);
}

void formatAndWriteMapPlot(SonicScrewdriver* screwdriver, TH2F* theHisto, string name, string comment, bool enableText)
{
    Plot thePlot(name,"custom",screwdriver->GetGlobalOptions());
    thePlot.SetParameter("name",name);
    thePlot.AddToInPlotInfo(comment);

    thePlot.getCanvas()->SetRightMargin(0.1);
	theHisto->GetXaxis()->SetTitle("m_{#tilde{t}} [GeV]");
	theHisto->GetXaxis()->SetTitleFont(PLOTDEFAULTSTYLES_FONT+2);
	theHisto->GetXaxis()->SetTitleSize(0.05);
	theHisto->GetXaxis()->SetTitleOffset(0.9);
	theHisto->GetXaxis()->SetLabelFont(PLOTDEFAULTSTYLES_FONT+3);
	theHisto->GetXaxis()->SetLabelSize(22);
	theHisto->GetYaxis()->SetTitle("m_{#chi^{0}} [GeV]");
	theHisto->GetYaxis()->SetTitleFont(PLOTDEFAULTSTYLES_FONT+2);
	theHisto->GetYaxis()->SetTitleSize(0.05);
	theHisto->GetYaxis()->SetTitleOffset(0.9);
	theHisto->GetYaxis()->SetLabelFont(PLOTDEFAULTSTYLES_FONT+3);
	theHisto->GetYaxis()->SetLabelSize(22);
    theHisto->SetTitle("");
    theHisto->SetStats(0);

    if (!enableText) theHisto->Draw("COLZ");
    else             theHisto->Draw("COLZ TEXT");
    thePlot.Update();
    TPaletteAxis *pal = (TPaletteAxis*) theHisto->GetListOfFunctions()->FindObject("palette");
    if (pal != 0) 
    {
        pal->SetX1NDC(0.901);
        pal->SetY1NDC(0.1);
        pal->SetX2NDC(0.93);
        pal->SetY2NDC(1.0-thePlot.getCanvas()->GetTopMargin());
    }

    TLine* line1 = new TLine(160,-12.5,560,387.5);
    line1->SetLineWidth(2);
    line1->SetLineStyle(2);
    line1->Draw();
    thePlot.Write("../plots/cutAndCount_T2tt/","custom",screwdriver->GetGlobalOptions());
}

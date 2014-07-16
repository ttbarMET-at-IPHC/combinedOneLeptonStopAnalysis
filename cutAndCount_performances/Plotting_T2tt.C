
#include "common.h"


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

                                                                    // METsig MT  MT2W  MET  dPhi  Chi2   ISRjet
/*
bool Selector_cutAndCount_offShellLoose() { return Selector_cutAndCount(-1,   120, -1,  200, -1,  999999, true ); }
bool Selector_cutAndCount_offShellTight() { return Selector_cutAndCount(10,   140, -1,  -1,  -1,  999999, true ); }
bool Selector_cutAndCount_lowDeltaM()     { return Selector_cutAndCount(-1,   130, -1,  130, 0.8, 5,      false); }
bool Selector_cutAndCount_mediumDeltaM()  { return Selector_cutAndCount(10,   140, 180, -1,  0.8, 3,      false); }
bool Selector_cutAndCount_highDeltaM()    { return Selector_cutAndCount(15,   190, 240, -1,  -1,  999999, false); }
*/

bool Selector_cutAndCount_offShellLoose() { return Selector_cutAndCount(8,    125, -1,  -1,   -1, 999999, true ); }
bool Selector_cutAndCount_offShellTight() { return Selector_cutAndCount(-1,   130, -1,  300,  -1, 999999, true ); }
bool Selector_cutAndCount_lowDeltaM()     { return Selector_cutAndCount(8,    140, -1,  -1,  0.8, 5,      false); }
bool Selector_cutAndCount_mediumDeltaM()  { return Selector_cutAndCount(-1,   140, 180, 200, 0.8, 3,      false); }
bool Selector_cutAndCount_highDeltaM()    { return Selector_cutAndCount(-1,   130, 190, 350,  -1, 999999, false); }

// #########################################################################
//                              Main function
// #########################################################################

int main (int argc, char *argv[])
{

  printBoxedMessage("Starting plot generation");

  // ####################
  // ##   Init tools   ##
  // ####################
 
     string signalCategory = "T2tt";

     // Create a sonic Screwdriver
      SonicScrewdriver screwdriver;

  // ##########################
  // ##   Create Variables   ##
  // ##########################

     screwdriver.AddVariable("METoverSqrtHT",  "MET / #sqrt{H_{T}}",      "",       32,0,32,         &(myEvent.METoverSqrtHT),       "");
     screwdriver.AddVariable("MET",            "MET",                     "GeV",    15,50,500,       &(myEvent.MET),                 "logY=true");
     screwdriver.AddVariable("MT",             "MT",                      "GeV",    20,0,400,        &(myEvent.MT),                  "logY=true");
     screwdriver.AddVariable("MT2W",           "M_{T2}^{W}",              "GeV",    20,0,500,        &(myEvent.MT2W),                "");
     screwdriver.AddVariable("deltaPhiMETJets","#Delta#Phi(MET,j_{1,2})", "rad",    16,0,3.2,        &(myEvent.deltaPhiMETJets),     "");
     screwdriver.AddVariable("HadronicChi2",   "Hadronic #chi^{2}",       "",       40,0,20,         &(myEvent.hadronicChi2),        "");
     
     screwdriver.AddVariable("mStop",          "m_{#tilde{t}}",           "GeV",    28,112.5,812.5,  &(myEvent.mStop),               "");
     screwdriver.AddVariable("mNeutralino",    "m_{#chi^{0}}",            "GeV",    16,-12.5,387.5,  &(myEvent.mNeutralino),         "noOverflowInLastBin");
     
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

     screwdriver.AddProcessClass("W+jets",         "W+jets",                    "background", kOrange-2);
             screwdriver.AddDataset("W+jets",      "W+jets", 0, 0);
                                                   
     screwdriver.AddProcessClass("rare",           "rare",                      "background", kMagenta-5);
             screwdriver.AddDataset("rare",        "rare", 0, 0);
                                                   
     screwdriver.AddProcessClass(signalCategory,    signalCategory,             "signal",     kViolet-1);
             screwdriver.AddDataset(signalCategory, signalCategory,   0, 0);

     screwdriver.AddProcessClass("signal_250_100",  signalCategory+" (250/100)",      "signal",COLORPLOT_BLUE   );
     screwdriver.AddProcessClass("signal_450_100",  signalCategory+" (450/100)",      "signal",COLORPLOT_GREEN2 ); 
     screwdriver.AddProcessClass("signal_650_100",  signalCategory+" (650/100)",      "signal",COLORPLOT_GREEN  );

  // ##########################
  // ##    Create Regions    ##
  // ##########################

     screwdriver.AddRegion("presel",            "Preselection",              &goesInPreselectionMTtail);

     screwdriver.AddRegion("CC_offShell_Loose", "Cut&Count;Off-shell Loose", &Selector_cutAndCount_offShellLoose);
     screwdriver.AddRegion("CC_offShell_Tight", "Cut&Count;Off-shell tight", &Selector_cutAndCount_offShellTight);
     screwdriver.AddRegion("CC_lowDM",          "Cut&Count;Low #DeltaM",     &Selector_cutAndCount_lowDeltaM    );
     screwdriver.AddRegion("CC_mediumDM",       "Cut&Count;Medium #DeltaM",  &Selector_cutAndCount_mediumDeltaM );
     screwdriver.AddRegion("CC_highDM",         "Cut&Count;High #DeltaM",    &Selector_cutAndCount_highDeltaM   );
   
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

     // Schedule plots
     screwdriver.SchedulePlots("1DSuperimposed");
     screwdriver.SchedulePlots("1DStack");
     screwdriver.SchedulePlots("2D");
     screwdriver.SchedulePlots("2DSuperimposed");

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

     if (currentDataset == signalCategory)
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
  screwdriver.WritePlots("../plots/cutAndCount_performances/"+signalCategory+"/");

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

  float SF_1ltop = 5;  

  vector<float> globalBackgroundUncertainty =
  {
      0.17,
      0.21,
      0.22,
      0.37,
      0.37
  };
  
  vector<TH2F*> signalMaps;
  vector<TH2F*> FOMdiscoveryMaps;
  vector<TH2F*> FOMexclusionMaps;
  vector<TH2F*> efficiencies;

  int nBinsX = -1;
  int nBinsY = -1;

  TH2F* signalMapPresel  = screwdriver.get2DHistoClone("mStop","mNeutralino",signalCategory,"presel","singleLepton");
  TH2F* backgroundPresel = screwdriver.get2DCompositeHistoClone("mStop","mNeutralino","2DSumBackground","presel","singleLepton","");

  if (nBinsX == -1) nBinsX = signalMapPresel->GetNbinsX();
  if (nBinsY == -1) nBinsY = signalMapPresel->GetNbinsY();

  // Store background eff in (mStop,mLSP) = (200,300)
  int backgroundBin = signalMapPresel->FindBin(200,300);
  float backgroundYieldPresel = backgroundPresel->Integral(0,nBinsX+1,0,nBinsY+1);

  for (unsigned int i = 0 ; i < cutAndCountRegions.size() ; i++)
  {
      signalMaps.push_back(screwdriver.get2DHistoClone("mStop","mNeutralino",signalCategory,cutAndCountRegions[i],"singleLepton"));
      signalMaps[i]->SetName((string("signalMap_")+cutAndCountRegions[i]).c_str());

      float B =   screwdriver.GetYieldAndError("1ltop",    cutAndCountRegions[i],"singleLepton").value() //* SF_1ltop
                + screwdriver.GetYieldAndError("ttbar_2l", cutAndCountRegions[i],"singleLepton").value()
                + screwdriver.GetYieldAndError("W+jets",   cutAndCountRegions[i],"singleLepton").value() 
                + screwdriver.GetYieldAndError("rare",     cutAndCountRegions[i],"singleLepton").value();

      // Apply scale factor from background prediction
      //float f_B = globalBackgroundUncertainty[i];
      float f_B = 0.15;

      if (B < 1.0) B = 1.0;
 
      efficiencies.push_back((TH2F*) signalMaps[i]->Clone());
      efficiencies[i]->SetName((string("eff_")+cutAndCountRegions[i]).c_str());
      efficiencies[i]->Divide(signalMapPresel);
      efficiencies[i]->SetBinContent(backgroundBin,B/backgroundYieldPresel);
      
      FOMdiscoveryMaps.push_back((TH2F*) signalMaps[i]->Clone());
      FOMdiscoveryMaps[i]->SetName((string("FOMdisco_")+cutAndCountRegions[i]).c_str());

      FOMexclusionMaps.push_back((TH2F*) signalMaps[i]->Clone());
      FOMexclusionMaps[i]->SetName((string("FOMexclu_")+cutAndCountRegions[i]).c_str());

      for (int x = 1 ; x <= nBinsX ; x++)
      for (int y = 1 ; y <= nBinsY ; y++)
      {
          float S = signalMaps[i]->GetBinContent(x,y);

          float FOMdiscovery = figureOfMerit(S,B,"discovery",false,f_B);
          FOMdiscoveryMaps[i]->SetBinContent(x,y,FOMdiscovery);
          
          float FOMexclusion = figureOfMerit(S,B,"exclusion",false,f_B);
          FOMexclusionMaps[i]->SetBinContent(x,y,FOMexclusion);
      }

  }

  // ################################
  // ##   Compute "best" FOM map   ##
  // ################################

  TH2F* bestDiscoFOMMap = (TH2F*) signalMaps[0]->Clone();  bestDiscoFOMMap->SetName("bestDiscoFOM");
  TH2F* bestDiscoSetMap = (TH2F*) signalMaps[0]->Clone();  bestDiscoSetMap->SetName("bestDiscoSet");
  TH2F* bestDiscoSigEff = (TH2F*) signalMaps[0]->Clone();  bestDiscoSigEff->SetName("bestDiscoSigEff");
  TH2F* bestDiscoBkgEff = (TH2F*) signalMaps[0]->Clone();  bestDiscoBkgEff->SetName("bestDiscoBkgEff");
  
  TH2F* bestExcluFOMMap = (TH2F*) signalMaps[0]->Clone();  bestExcluFOMMap->SetName("bestExcluFOM");
  TH2F* bestExcluSetMap = (TH2F*) signalMaps[0]->Clone();  bestExcluSetMap->SetName("bestExcluSet");
  TH2F* bestExcluSigEff = (TH2F*) signalMaps[0]->Clone();  bestExcluSigEff->SetName("bestExcluSigEff");
  TH2F* bestExcluBkgEff = (TH2F*) signalMaps[0]->Clone();  bestExcluBkgEff->SetName("bestExcluBkgEff");
  
  for (int x = 1 ; x <= nBinsX ; x++)
  for (int y = 1 ; y <= nBinsY ; y++)
  {
      float bestDiscoFOM = -1.0;
      int   bestDiscoSet = 0;
      float bestDiscoSigEff_ = -1.0;
      float bestDiscoBkgEff_ = -1.0;
      for (unsigned int i = 0 ; i < cutAndCountRegions.size() ; i++)
      {
          float DiscoFOM = FOMdiscoveryMaps[i]->GetBinContent(x,y);
          if (bestDiscoFOM < DiscoFOM)
          {
              bestDiscoFOM = DiscoFOM;
              if (bestDiscoFOM > 0) bestDiscoSet = i+1;
              bestDiscoSigEff_ = efficiencies[i]->GetBinContent(x,y);
              bestDiscoBkgEff_ = efficiencies[i]->GetBinContent(backgroundBin);
          }
      }
      bestDiscoFOMMap->SetBinContent(x,y,bestDiscoFOM);
      bestDiscoSetMap->SetBinContent(x,y,bestDiscoSet);
      bestDiscoSigEff->SetBinContent(x,y,bestDiscoSigEff_);
      bestDiscoBkgEff->SetBinContent(x,y,bestDiscoBkgEff_);

      float bestExcluFOM = -1.0;
      int   bestExcluSet = 0;
      float bestExcluSigEff_ = -1.0;
      float bestExcluBkgEff_ = -1.0;
      for (unsigned int i = 0 ; i < cutAndCountRegions.size() ; i++)
      {
          float ExcluFOM = FOMexclusionMaps[i]->GetBinContent(x,y);
          if (bestExcluFOM < ExcluFOM)
          {
              bestExcluFOM = ExcluFOM;
              if (bestExcluFOM > 0) bestExcluSet = i+1;
              bestExcluSigEff_ = efficiencies[i]->GetBinContent(x,y);
              bestExcluBkgEff_ = efficiencies[i]->GetBinContent(backgroundBin);
          }
      }
      bestExcluFOMMap->SetBinContent(x,y,bestExcluFOM);
      bestExcluSetMap->SetBinContent(x,y,bestExcluSet);
      bestExcluSigEff->SetBinContent(x,y,bestExcluSigEff_);
      bestExcluBkgEff->SetBinContent(x,y,bestExcluBkgEff_);

  }

  // #########################
  // ##   Save those maps   ##
  // #########################

  TFile fOutput(("../plots/cutAndCount_performances/"+signalCategory+"/custom.root").c_str(),"RECREATE");
  string pathExport = "../plots/cutAndCount_performances/"+signalCategory+"/";
  gStyle->SetPaintTextFormat("4.0f");
  formatAndWriteMapPlot(&screwdriver,bestDiscoSetMap,bestDiscoSetMap->GetName(),signalCategory+";Best set of cuts;(for discovery)",pathExport);
  formatAndWriteMapPlot(&screwdriver,bestExcluSetMap,bestExcluSetMap->GetName(),signalCategory+";Best set of cuts;(for exclusion)",pathExport);
  gStyle->SetPaintTextFormat("4.1f");
  for (unsigned int i = 0 ; i < cutAndCountRegions.size() ; i++)
  {
      FOMdiscoveryMaps[i]->SetMaximum(5.0);
      formatAndWriteMapPlot(&screwdriver,FOMdiscoveryMaps[i],FOMdiscoveryMaps[i]->GetName(),string("Discovery FOM for ")+cutAndCountRegions[i], pathExport);
      formatAndWriteMapPlot(&screwdriver,    efficiencies[i],    efficiencies[i]->GetName(),string("Efficiencies for " )+cutAndCountRegions[i], pathExport);
  }
  bestDiscoFOMMap->SetMaximum(5.0);
  bestExcluFOMMap->SetMaximum(5.0);
  formatAndWriteMapPlot(&screwdriver,bestDiscoFOMMap,bestDiscoFOMMap->GetName(),signalCategory+";Best FOM;(for discovery)"              ,pathExport);
  formatAndWriteMapPlot(&screwdriver,bestDiscoSigEff,bestDiscoSigEff->GetName(),signalCategory+";Best signal efficiency;(for discovery)",pathExport);
  formatAndWriteMapPlot(&screwdriver,bestDiscoBkgEff,bestDiscoBkgEff->GetName(),signalCategory+";Best backgr efficiency;(for discovery)",pathExport);
  formatAndWriteMapPlot(&screwdriver,bestExcluFOMMap,bestExcluFOMMap->GetName(),signalCategory+";Best FOM;(for exclusion)"              ,pathExport);
  formatAndWriteMapPlot(&screwdriver,bestExcluSigEff,bestExcluSigEff->GetName(),signalCategory+";Best signal efficiency;(for exclusion)",pathExport);
  formatAndWriteMapPlot(&screwdriver,bestExcluBkgEff,bestExcluBkgEff->GetName(),signalCategory+";Best backgr efficiency;(for exclusion)",pathExport);
  fOutput.Close();

  printBoxedMessage("Program done.");
  return (0);
}


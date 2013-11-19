
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

#define FOLDER_BABYTUPLES "../store/babyTuples_1102/"
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
    if ((!myEvent.isolatedTrackVeto) || (!myEvent.tauVeto)) return false;     // TODO : check this is the good way to do this

    // Apply MET and MT cuts
    if ((myEvent.MET < 80) || (myEvent.MT < 100))         return false;

    return true; 
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

// #########################################################################
//                          Others tools/stuff
// #########################################################################

void fillMCSignalTable(SonicScrewdriver* screwdriver, vector<string> region, vector<string> process, Table* table);
float stopCrossSection(float inputMass);
void printProgressBar(int current, int max);

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
	 babyEvent myEvent;
     myEventPointer = &myEvent;

  // ##########################
  // ##   Create Variables   ##
  // ##########################


    Float_t HTPlusLeptonPtPlusMET;              // HT + pT(leading lepton) + MET


 	 screwdriver.AddVariable("MET",            "MET",                     "GeV",    15,50,500,      &(myEvent.MET),                  "logY=true");
 	 screwdriver.AddVariable("MT",             "MT",                      "GeV",    17,0,510,       &(myEvent.MT),                   "logY=true");
 	 screwdriver.AddVariable("deltaPhiMETJets","#Delta#Phi(MET,j_{1,2})", "rad",    16,0,3.2,       &(myEvent.deltaPhiMETJets),      "");
 	 screwdriver.AddVariable("MT2W",           "M_{T2}^{W}",              "GeV",    20,0,500,       &(myEvent.MT2W),                 "");
 	 screwdriver.AddVariable("HT",             "H_{T}",                   "",       40,0,400,       &(myEvent.HT),                   "");
 	 screwdriver.AddVariable("HTratio",        "H_{T}^{ratio}",           "",       20,0,1.2,       &(myEvent.HTRatio),              "");
 	 screwdriver.AddVariable("HadronicChi2",   "Hadronic #chi^{2}",       "",       40,0,20,        &(myEvent.hadronicChi2),         "");
 	 screwdriver.AddVariable("leadingBPt",     "p_{T}(leading b-jet)",    "GeV",    25,0,500,       &(myEvent.leadingBPt),           "");
 	 screwdriver.AddVariable("leadingJetPt",   "p_{T}(leading jet)",      "GeV",    25,0,500,       &(myEvent.leadingJetPt),         "");
 	 screwdriver.AddVariable("leptonPt",       "p_{T}(lepton)",           "GeV",    25,0,500,       &(myEvent.leadingLeptonPt),      "");
   
 	 screwdriver.AddVariable("Mlb",            "M_{lb}",                  "GeV",    25,0,500,       &(myEvent.Mlb),                  "");
 	 screwdriver.AddVariable("Mlb_hemi",       "M_{lb}_hemi",             "GeV",    25,0,500,       &(myEvent.Mlb_hemi),             "");
 	 screwdriver.AddVariable("M3b",            "M3b",                     "GeV",    25,0,500,       &(myEvent.M3b),                  "");
 	 screwdriver.AddVariable("deltaRLeptonB",  "#DeltaR(l,leading b)",    "",       25,0,5,         &(myEvent.deltaRLeptonLeadingB), "");
 	 screwdriver.AddVariable("METoverSqrtHT",  "MET / #sqrt{H_{T}}",      "",       40,0,40,        &(myEvent.METoverSqrtHT),        "");

     float METoverSqrtHT_JESup;
     float METoverSqrtHT_JESdown;

 	 screwdriver.AddVariable("METoverSqrtHT_JESup",  "MET / #sqrt{H_{T}} (JES up)",   "",    40,0,40, &(METoverSqrtHT_JESup  ),      ""); 
     screwdriver.AddVariable("METoverSqrtHT_JESdown","MET / #sqrt{H_{T}} (JES down)", "",    40,0,40, &(METoverSqrtHT_JESdown),      "");
 	 screwdriver.AddVariable("HTLeptonPtMET",  "HT + MET + p_{T}(lepton)","GeV",    20,100,2100,    &(myEvent.HTPlusLeptonPtPlusMET),"");

     //screwdriver.AddVariable("nJets",          "# of jets",               "",       4, 2.5,6.5,     &(myEvent.nJets),                "");
     //screwdriver.AddVariable("nBTag",          "# of b-tagged jets",      "",       4,-0.5,3.5,     &(myEvent.nBTag),                "");
     
     screwdriver.AddVariable("mStop",          "m_{#tilde{t}}",           "GeV",    28,112.5,812.5,  &(myEvent.mStop),                "");
 	 screwdriver.AddVariable("mNeutralino",    "m_{#chi^{0}}",            "GeV",    16,-12.5,387.5,  &(myEvent.mNeutralino),         "");

  // #########################################################
  // ##   Create ProcessClasses (and associated datasets)   ##
  // #########################################################
      
     screwdriver.AddProcessClass("ttbar_1l", "t#bar{t} #rightarrow l+jets","background",kRed-7);
          screwdriver.AddDataset("ttbar",    "ttbar_1l",  0, 0);
     screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow ll",    "background",kCyan-3);

     screwdriver.AddProcessClass("W+jets",   "W+jets",                     "background",kOrange-2);
          screwdriver.AddDataset("Wjets",    "W+jets", 0, 0);

     screwdriver.AddProcessClass("others",   "others",                     "background",kMagenta-5);
          screwdriver.AddDataset("others",   "others", 0, 0);

     screwdriver.AddProcessClass("T2tt",     "T2tt",                       "signal",COLORPLOT_AZURE);
          screwdriver.AddDataset("T2tt",     "T2tt",   0, 0);

     //screwdriver.AddProcessClass("T2bw-050", "T2bw-050",                   "signal",COLORPLOT_GREEN);
     //     screwdriver.AddDataset("T2bw-050", "T2bw-050",   0, 0);

  // ##########################
  // ##    Create Regions    ##
  // 
  // ##########################

     screwdriver.AddRegion("presel",               "Preselection",             &Selector_presel);
     screwdriver.AddRegion("SR1",                  "Signal region 1",          &Selector_SR1);
     screwdriver.AddRegion("SR2",                  "Signal region 2",          &Selector_SR2);
     screwdriver.AddRegion("SR3",                  "Signal region 3",          &Selector_SR3);
     screwdriver.AddRegion("SR4",                  "Signal region 4",          &Selector_SR4);
     screwdriver.AddRegion("SR5",                  "Signal region 5",          &Selector_SR5);
     screwdriver.AddRegion("SR6",                  "Signal region 6",          &Selector_SR6);
 
  // ##########################
  // ##   Create Channels    ##
  // ##########################
   
	 screwdriver.AddChannel("inclusiveChannel","e/#mu channel",&inclusiveChannelSelector);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################
  
     screwdriver.SetLumi(20000);

	 // Create histograms
  	 screwdriver.Create1DHistos();
  	 screwdriver.Add2DHisto("mStop","mNeutralino");
  	 
  	 // Schedule plots
  	 screwdriver.SchedulePlots("1DSuperpRenorm","includeSignal=true");
  	 screwdriver.SchedulePlots("1DStack","includeSignal=true,includeSignalHow=stack,factorSignal=1.0");
  	 screwdriver.SchedulePlots("2D");
     
     screwdriver.SchedulePlots("1DCutSignificance","signal=T2tt,var=MET,cutType=keepHighValues,backgroundSystematicUncertainty=0.15");
     screwdriver.SchedulePlots("1DCutSignificance","signal=T2tt,var=MT,cutType=keepHighValues,backgroundSystematicUncertainty=0.15");
     screwdriver.SchedulePlots("1DCutSignificance","signal=T2tt,var=deltaPhiMETJets,cutType=keepHighValues,backgroundSystematicUncertainty=0.15");
     screwdriver.SchedulePlots("1DCutSignificance","signal=T2tt,var=MT2W,cutType=keepHighValues,backgroundSystematicUncertainty=0.15");
     screwdriver.SchedulePlots("1DCutSignificance","signal=T2tt,var=HTratio,cutType=keepLowValues,backgroundSystematicUncertainty=0.15");
     screwdriver.SchedulePlots("1DCutSignificance","signal=T2tt,var=HadronicChi2,cutType=keepLowValues,backgroundSystematicUncertainty=0.15");
     screwdriver.SchedulePlots("1DCutSignificance","signal=T2tt,var=leadingBPt,cutType=keepHighValues,backgroundSystematicUncertainty=0.15");
     screwdriver.SchedulePlots("1DCutSignificance","signal=T2tt,var=leadingJetPt,cutType=keepHighValues,backgroundSystematicUncertainty=0.15");
     screwdriver.SchedulePlots("1DCutSignificance","signal=T2tt,var=leptonPt,cutType=keepLowValues,backgroundSystematicUncertainty=0.15");
     screwdriver.SchedulePlots("1DCutSignificance","signal=T2tt,var=Mlb_hemi,cutType=keepHighValues,backgroundSystematicUncertainty=0.15");
     screwdriver.SchedulePlots("1DCutSignificance","signal=T2tt,var=M3b,cutType=keepHighValues,backgroundSystematicUncertainty=0.15");
     screwdriver.SchedulePlots("1DCutSignificance","signal=T2tt,var=deltaRLeptonB,cutType=keepLowValues,backgroundSystematicUncertainty=0.15");
     screwdriver.SchedulePlots("1DCutSignificance","signal=T2tt,var=METoverSqrtHT,cutType=keepHighValues,backgroundSystematicUncertainty=0.15");
     screwdriver.SchedulePlots("1DCutSignificance","signal=T2tt,var=METoverSqrtHT_JESup,cutType=keepHighValues,backgroundSystematicUncertainty=0.15");
     screwdriver.SchedulePlots("1DCutSignificance","signal=T2tt,var=METoverSqrtHT_JESdown,cutType=keepHighValues,backgroundSystematicUncertainty=0.15");
     screwdriver.SchedulePlots("1DCutSignificance","signal=T2tt,var=HTLeptonPtMET,cutType=keepHighValues,backgroundSystematicUncertainty=0.15");

  // ########################################
  // ##       Run over the datasets        ##
  // ########################################

  vector<string> datasetsList;
  screwdriver.GetDatasetList(&datasetsList);

  cout << "   > Running on dataset : " << endl;

  for (unsigned int d = 0 ; d < datasetsList.size() ; d++)
  {
     string currentDataset = datasetsList[d];
     string currentProcessClass = screwdriver.GetProcessClass(currentDataset); 
     // Open the tree
     TFile f((string(FOLDER_BABYTUPLES)+"babyTuple_"+currentDataset+".root").c_str());
     TTree* theTree = (TTree*) f.Get("babyTuple"); 

     intermediatePointers pointers;
     InitializeBranches(theTree,&myEvent,&pointers);

     cout << "                    " << currentDataset << endl; 

  // ########################################
  // ##        Run over the events         ##
  // ########################################

      float weight_lumi = 0.0;
      for (int i = 0 ; i < theTree->GetEntries() ; i++)
      {
          if (i % (theTree->GetEntries() / 50) == 0) 
              printProgressBar(i,theTree->GetEntries());

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          // Keep only events that pass preselection
          if (!Selector_presel()) continue;
  
          float weight = myEvent.weightCrossSection * screwdriver.GetLumi();

          // Don't take x-sec into account for signal
          if ((currentDataset == "T2tt") || (currentDataset == "T2bw-050"))
              weight = 0.01 * screwdriver.GetLumi() / myEvent.numberOfInitialEvents;

          // Split 1-lepton ttbar and 2-lepton ttbar
          string currentProcessClass_ = currentProcessClass;
          if ((currentDataset == "ttbar") && (myEvent.numberOfGenLepton == 2)) 
              currentProcessClass_ = "ttbar_2l";

          if (myEvent.hadronicChi2  >= 20) myEvent.hadronicChi2  = 19.99;
          if (myEvent.METoverSqrtHT >= 40) myEvent.METoverSqrtHT = 39.99;

          METoverSqrtHT_JESup   = myEvent.MET_JESup   / sqrt(myEvent.HT_JESup)  ;
          METoverSqrtHT_JESdown = myEvent.MET_JESdown / sqrt(myEvent.HT_JESdown);

          // Fill all the variables with autoFill-mode activated
          screwdriver.AutoFillProcessClass(currentProcessClass_,weight);

      } 
      
      cout << endl;
      f.Close();

  }

  // ###################################
  // ##   Make plots and write them   ##
  // ###################################
 
  std::ostringstream lumiInString;
  lumiInString << screwdriver.GetLumi() / 1000;
  string infoText("CMS Internal      #sqrt{s} = 8 TeV, ");
  infoText += string("L = ")+lumiInString.str()+string(" fb^{-1}, ");
  
  cout << endl;
  cout << "   > Making plots..." << endl;
  screwdriver.MakePlots();
  cout << "   > Saving plots..." << endl;
  screwdriver.WritePlots("../plots/cutAndCount/",infoText);  //"exportPngAndEps=true");

  cout << endl;
  cout << "   ┌──────────────────────────────┐  " << endl;
  cout << "   │   Plot generation completed  │  " << endl;
  cout << "   └──────────────────────────────┘  " << endl; 
  cout << endl;

  /*
  cout << endl;
  cout << "   ┌──────────────────────────────┐  " << endl;
  cout << "   │  Starting tables generation  │  " << endl;
  cout << "   └──────────────────────────────┘  " << endl; 
  cout << endl;
;
      vector<string> processClassList;
      screwdriver.GetProcessClassList(&processClassList);
      processClassList.pop_back();
      processClassList.pop_back();
      processClassList.pop_back();
      processClassList.push_back("total");
      processClassList.push_back("signal_550_25");
      processClassList.push_back("signal_650_25");
      processClassList.push_back("signal_750_25");
      
      vector<string> regions;
      regions.push_back("signalRegion");
        
      Table table(regions,processClassList);
      fillMCSignalTable(&screwdriver,regions,processClassList,&table);
      table.PrintTable();

  cout << endl;
  cout << "   ┌───────────────────────────────┐  " << endl;
  cout << "   │  Tables generation completed  │  " << endl;
  cout << "   └───────────────────────────────┘  " << endl; 
  cout << endl;
  */

  return (0);
}

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
        else					   bar.replace(i,1," ");
    }

    std::cout << "  [Progress]  ";
    std::cout << "[" << bar << "] ";
    std::cout.width( 3 );
    std::cout << percent << "%     ";
    std::cout << "(" << current << " / " << max << ")" << "\r" << std::flush;
}


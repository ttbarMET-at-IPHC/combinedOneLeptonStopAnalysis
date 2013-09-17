
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


// MicroTuple format and location

#define FOLDER_MICROTUPLES "../store/microTuples_MVA0904/"
#include "../microTupling/MicroTuple_Format_MVA0808.h" 
microEvent* myEventPointer;

// Variables for region selector

float* AdaBoostBDTOutputNoWTag_Pointer;
float* AdaBoostBDTOutputWithWTag_Pointer;

float* BDTOutputNoWTag_Pointer;
float* BDTOutputWithWTag_Pointer;


// #########################################################################
//                          Region selectors
// #########################################################################

bool inclusiveChannelSelector() { return true; }

bool Selector_presel() { return true; }
bool Selector_signalRegion() 
{ 
    return  (
               (myEventPointer->MET   > 80) 
            && (myEventPointer->MT    > 100)
            && (myEventPointer->nJets >= 4 )
            ); 
}
bool Selector_afterBDTNoWTagCut() 
{ 
     return (Selector_signalRegion() && (*BDTOutputNoWTag_Pointer > 0.5)); 
}

bool Selector_afterBDTWithWTagCut() 
{ 
     return (Selector_signalRegion() && (*BDTOutputWithWTag_Pointer > 0.5)); 
}

// #########################################################################
//                          Others tools/stuff
// #########################################################################

void fillMCSignalTable(SonicScrewdriver* mySonic, vector<string> region, vector<string> process, Table* table);
float stopCrossSection(float inputMass);

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
 	 SonicScrewdriver mySonic;

  	 // Create a container for the event
	 microEvent myEvent;
	 myEventPointer = &myEvent;

  // ##########################
  // ##   Create Variables   ##
  // ##########################

 	 mySonic.AddVariable("MET",            "MET",                     "GeV",    15,50,500,      &(myEvent.MET),           "logY=true");
 	 mySonic.AddVariable("MT",             "MT",                      "GeV",    17,0,510,       &(myEvent.MT),            "logY=true");
 	 mySonic.AddVariable("dPhiMETjet",     "#Delta#Phi(MET,j_{1,2})", "rad",    16,0,3.2,       &(myEvent.dPhiMETjet),    "logY=true");
 	 mySonic.AddVariable("MT2W",           "M_{T2}^{W}",               "GeV",    20,0,500,      &(myEvent.MT2W),          "");
 	 mySonic.AddVariable("HTratio",        "HT_{ratio}",               "",       20,0,1.2,      &(myEvent.HTratio),       "");
 	 mySonic.AddVariable("HadronicChi2",   "Hadronic #chi^{2}",        "",       20,0,5,        &(myEvent.HadronicChi2),  "");

     mySonic.AddVariable("nJets",          "# of jets",              "",        4, 2.5,6.5,     &(myEvent.nJets),         "");
    
     mySonic.AddVariable("nBTag",          "# of b-tagged jets",     "",        4,-0.5,3.5,     &(myEvent.nBTag),         "");
     mySonic.AddVariable("nWTag",          "# of W-tagged jets",     "",        3,-0.5,2.5,     &(myEvent.nWTag),         "");

 	 mySonic.AddVariable("mStop",          "m_{#tilde{t}}",          "GeV",     14,112.5,812.5, &(myEvent.mStop),         "");
 	 mySonic.AddVariable("mNeutralino",    "m_{#chi^{0}}",           "GeV",     8,-12.5,387.5,  &(myEvent.mNeutralino),   "");

     float AdaBoostBDTOutputNoWTag;   AdaBoostBDTOutputNoWTag_Pointer   = &AdaBoostBDTOutputNoWTag;
     mySonic.AddVariable("BDTOutputAdaBoostNoWTag",  "(AdaBoost) BDT output (no WTag)",   "", 44,-1.1,1.1, &(AdaBoostBDTOutputNoWTag),   "logY=true");
     
     float AdaBoostBDTOutputWithWTag; AdaBoostBDTOutputWithWTag_Pointer = &AdaBoostBDTOutputWithWTag;
     mySonic.AddVariable("BDTOutputAdaBoostWithWTag","(AdaBoost) BDT output (with WTag)", "", 44,-1.1,1.1, &(AdaBoostBDTOutputWithWTag), "logY=true");

     float BDTOutputNoWTag;           BDTOutputNoWTag_Pointer = &BDTOutputNoWTag;
     mySonic.AddVariable("BDTOutputNoWTag",          "BDT output (no WTag)",              "", 40,-1,1,     &(BDTOutputNoWTag),           "logY=true");
 
     float BDTOutputWithWTag;         BDTOutputWithWTag_Pointer = &BDTOutputWithWTag;
     mySonic.AddVariable("BDTOutputWithWTag",        "BDT output (with WTag)",            "", 40,-1,1,     &(BDTOutputWithWTag),         "");
    
  // #########################################################
  // ##   Create ProcessClasses (and associated datasets)   ##
  // #########################################################
      
      // ttbar -> l + jets
      mySonic.AddProcessClass("ttbar",    "t#bar{t}",     "background",COLORPLOT_RED);
          mySonic.AddDataset("ttbar",     "ttbar", 100,225.2);

      // Wjets
      mySonic.AddProcessClass("W+jets",   "W+jets",       "background",kOrange-2);
          //mySonic.AddDataset("W2Jets",    "W+jets",100,2159);
          //mySonic.AddDataset("W3Jets",    "W+jets",100,640);
          mySonic.AddDataset("W4Jets",    "W+jets",100,264);

     // Signal
     mySonic.AddProcessClass("signal_550_25", "T2tt (550/25)",  "signal",COLORPLOT_AZURE);
          mySonic.AddDataset("signal1", "signal_550_25", 100,1);

     mySonic.AddProcessClass("signal_650_25", "T2tt (650/25)",  "signal",kCyan-3);
          mySonic.AddDataset("signal2", "signal_650_25", 100,1);

     mySonic.AddProcessClass("signal_750_25", "T2tt (750/25)",  "signal",COLORPLOT_GREEN);
          mySonic.AddDataset("signal3", "signal_750_25", 100,1);
          

  // ##########################
  // ##    Create Regions    ##
  // ##########################

     mySonic.AddRegion("presel",                "Preselection",             &Selector_presel);
     mySonic.AddRegion("signalRegion",          "Signal region",            &Selector_signalRegion);
     //mySonic.AddRegion("afterBDTNoWTagCut",     "After BDT cut (no WTag)",  &Selector_afterBDTNoWTagCut);
     //mySonic.AddRegion("afterBDTWithWTagCut",   "After BDT cut (with WTag)",&Selector_afterBDTWithWTagCut);
 
  // ##########################
  // ##   Create Channels    ##
  // ##########################
   
	 mySonic.AddChannel("inclusiveChannel","",&inclusiveChannelSelector);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################
  
     mySonic.SetLumi(20000);
     //mySonic.SetLumi(19500);

	 // Create histograms
  	 mySonic.Create1DHistos();
  	 mySonic.Add2DHisto("mStop","mNeutralino");
  	 mySonic.Add2DHisto("BDTOutputNoWTag","BDTOutputWithWTag");
  	 
  	 // Schedule plots
  	 mySonic.SchedulePlots("1DSuperpRenorm","includeSignal=true");
  	 mySonic.SchedulePlots("1DStack","includeSignal=true,includeSignalHow=stack,factorSignal=1.0");
  	 mySonic.SchedulePlots("2D");

     mySonic.SchedulePlots("1DCutSignificance","signal=signal_650_25,var=BDTOutputNoWTag,cutType=keepHighValues,backgroundSystematicUncertainty=0.3");
  	 mySonic.SchedulePlots("1DCutSignificance","signal=signal_650_25,var=BDTOutputWithWTag,cutType=keepHighValues,backgroundSystematicUncertainty=0.3");
  	 mySonic.SchedulePlots("1DCutSignificance","signal=signal_650_25,var=AdaBoostBDTOutputNoWTag,cutType=keepHighValues,backgroundSystematicUncertainty=0.3");
  	 mySonic.SchedulePlots("1DCutSignificance","signal=signal_650_25,var=AdaBoostBDTOutputWithWTag,cutType=keepHighValues,backgroundSystematicUncertainty=0.3");

  // ##########################
  // ##  Set up MVA reader   ##
  // ########################## 

  TMVA::Reader* MVAReader_AdaBoost_noWTag = new TMVA::Reader( "" );
  MVAReader_AdaBoost_noWTag->AddVariable( "MET",           &(myEvent.MET)          );
  MVAReader_AdaBoost_noWTag->AddVariable( "MT2W",          &(myEvent.MT2W)         );
  MVAReader_AdaBoost_noWTag->AddVariable( "dPhiMETjet",    &(myEvent.dPhiMETjet)   );
  MVAReader_AdaBoost_noWTag->AddVariable( "HTratio",       &(myEvent.HTratio)      );
  MVAReader_AdaBoost_noWTag->AddVariable( "HadronicChi2",  &(myEvent.HadronicChi2) );
  MVAReader_AdaBoost_noWTag->BookMVA("BDT","../WTagTMVATraining/weights_AdaBoost_withWeights_noWTag/TMVAClassification_BDT.weights.xml");

  TMVA::Reader* MVAReader_AdaBoost_withWTag = new TMVA::Reader( "" );
  MVAReader_AdaBoost_withWTag->AddVariable( "MET",           &(myEvent.MET)          );
  MVAReader_AdaBoost_withWTag->AddVariable( "MT2W",          &(myEvent.MT2W)         );
  MVAReader_AdaBoost_withWTag->AddVariable( "dPhiMETjet",    &(myEvent.dPhiMETjet)   );
  MVAReader_AdaBoost_withWTag->AddVariable( "HTratio",       &(myEvent.HTratio)      );
  MVAReader_AdaBoost_withWTag->AddVariable( "HadronicChi2",  &(myEvent.HadronicChi2) );
  MVAReader_AdaBoost_withWTag->AddVariable( "nWTag",         &(myEvent.nWTag)        );
  MVAReader_AdaBoost_withWTag->BookMVA("BDT","../WTagTMVATraining/weights_AdaBoost_withWeights_withWTag/TMVAClassification_BDT.weights.xml");

  TMVA::Reader* MVAReader_noWTag = new TMVA::Reader( "" );
  MVAReader_noWTag->AddVariable( "MET",           &(myEvent.MET)          );
  MVAReader_noWTag->AddVariable( "MT2W",          &(myEvent.MT2W)         );
  MVAReader_noWTag->AddVariable( "dPhiMETjet",    &(myEvent.dPhiMETjet)   );
  MVAReader_noWTag->AddVariable( "HTratio",       &(myEvent.HTratio)      );
  MVAReader_noWTag->AddVariable( "HadronicChi2",  &(myEvent.HadronicChi2) );
  MVAReader_noWTag->BookMVA("BDT","../WTagTMVATraining/weights_noWTag/TMVAClassification_BDT.weights.xml");

  TMVA::Reader* MVAReader_withWTag = new TMVA::Reader( "" );
  MVAReader_withWTag->AddVariable( "MET",           &(myEvent.MET)          );
  MVAReader_withWTag->AddVariable( "MT2W",          &(myEvent.MT2W)         );
  MVAReader_withWTag->AddVariable( "dPhiMETjet",    &(myEvent.dPhiMETjet)   );
  MVAReader_withWTag->AddVariable( "HTratio",       &(myEvent.HTratio)      );
  MVAReader_withWTag->AddVariable( "HadronicChi2",  &(myEvent.HadronicChi2) );
  MVAReader_withWTag->AddVariable( "nWTag",         &(myEvent.nWTag)        );
  MVAReader_withWTag->BookMVA("BDT","../WTagTMVATraining/weights_withWTag/TMVAClassification_BDT.weights.xml");

  // ########################################
  // ##       Run over the datasets        ##
  // ########################################

  vector<string> datasetsList;
  mySonic.GetDatasetList(&datasetsList);

  cout << "   > Running on dataset : " << endl;

  for (unsigned int d = 0 ; d < datasetsList.size() ; d++)
  {
     string currentDataset = datasetsList[d];
     string currentProcessClass = mySonic.GetProcessClass(currentDataset);

     // Open the tree
     TFile f((string(FOLDER_MICROTUPLES)+currentDataset+".root").c_str());
     TTree* theTree = (TTree*) f.Get("microTuple"); 
     theTree->SetBranchAddress("microEvents",&myEvent);
          
     theTree->GetEntry(0);
     mySonic.SetTrueNumberOfEvents(currentDataset,myEvent.trueNumberOfEvents);
    
     if ((currentDataset == "signal1")
      || (currentDataset == "signal2")
      || (currentDataset == "signal3"))
        mySonic.SetTrueNumberOfEvents(currentDataset,55000);

     cout << "                    " << currentDataset << endl; 

  // ########################################
  // ##        Run over the events         ##
  // ########################################

      float weight_lumi = 0.0;
      for (int i = 0 ; i < theTree->GetEntries() ; i++)
      {

          // Get the i-th entry
          theTree->GetEntry(i);
  
          float weight = mySonic.GetDatasetLumiWeight(currentDataset);

          if (myEvent.mStop != -1)
          {
            // Filter weird signal events (bugged mNeutralino = 0 events ?)
            if (myEvent.mStop       - ((int) myEvent.mStop      ) != 0) continue;
            if (myEvent.mNeutralino - ((int) myEvent.mNeutralino) != 0) continue;
           
            float deltaM = myEvent.mStop - myEvent.mNeutralino;
            // Sort events according to deltaM or masses
            if ((currentDataset == "signal1") && ((myEvent.mStop != 550) || (myEvent.mNeutralino != 25))) continue;
            if ((currentDataset == "signal2") && ((myEvent.mStop != 650) || (myEvent.mNeutralino != 25))) continue;
            if ((currentDataset == "signal3") && ((myEvent.mStop != 750) || (myEvent.mNeutralino != 25))) continue;
   
            weight *= stopCrossSection(myEvent.mStop);
          }

          AdaBoostBDTOutputNoWTag   = 0; //MVAReader_AdaBoost_noWTag->EvaluateMVA("BDT");
          AdaBoostBDTOutputWithWTag = 0; //MVAReader_AdaBoost_withWTag->EvaluateMVA("BDT");
          BDTOutputNoWTag           = MVAReader_noWTag->EvaluateMVA("BDT");
          BDTOutputWithWTag         = MVAReader_withWTag->EvaluateMVA("BDT");
          
          // Fill all the variables with autoFill-mode activated
          mySonic.AutoFillProcessClass(currentProcessClass,weight);

      } 
      
  f.Close();

  }

  // ###################################
  // ##   Make plots and write them   ##
  // ###################################
 
  std::ostringstream lumiInString;
  lumiInString << mySonic.GetLumi() / 1000;
  string infoText("CMS Internal      #sqrt{s} = 8 TeV, ");
  infoText += string("L = ")+lumiInString.str()+string(" fb^{-1}, ");
  
  cout << endl;
  cout << "   > Making plots..." << endl;
  mySonic.MakePlots();
  cout << "   > Saving plots..." << endl;
  mySonic.WritePlots("../plots/WTagWithMVA/",infoText);  //"exportPngAndEps=true");

  cout << endl;
  cout << "   ┌──────────────────────────────┐  " << endl;
  cout << "   │   Plot generation completed  │  " << endl;
  cout << "   └──────────────────────────────┘  " << endl; 
  cout << endl;
 
  cout << endl;
  cout << "   ┌──────────────────────────────┐  " << endl;
  cout << "   │  Starting tables generation  │  " << endl;
  cout << "   └──────────────────────────────┘  " << endl; 
  cout << endl;

      vector<string> processClassList;
      mySonic.GetProcessClassList(&processClassList);
      processClassList.pop_back();
      processClassList.pop_back();
      processClassList.pop_back();
      processClassList.push_back("total");
      processClassList.push_back("signal_550_25");
      processClassList.push_back("signal_650_25");
      processClassList.push_back("signal_750_25");
      
      vector<string> regions;
      regions.push_back("presel");
      regions.push_back("signalRegion");
      regions.push_back("afterBDTNoWTagCut");
      regions.push_back("afterBDTWithWTagCut");
        
      Table table(regions,processClassList);
      fillMCSignalTable(&mySonic,regions,processClassList,&table);
      table.PrintTable();

  cout << endl;
  cout << "   ┌───────────────────────────────┐  " << endl;
  cout << "   │  Tables generation completed  │  " << endl;
  cout << "   └───────────────────────────────┘  " << endl; 
  cout << endl;

  cout << endl;
  cout << "   ┌───────────────────────────┐  " << endl;
  cout << "   │  Generating Eric's plots  │  " << endl;
  cout << "   └───────────────────────────┘  " << endl; 
  cout << endl;
    
    vector<Histo2D>* Histos2D = mySonic.Get2DHistoList();

    TH2F* BDTOutputsTTbar;
    TH2F* BDTOutputsWJets;
    TH2F* BDTOutputsSignal550;
    TH2F* BDTOutputsSignal650;
    TH2F* BDTOutputsSignal750;

    float WeightTTbar = mySonic.GetDatasetLumiWeight("ttbar");
    float WeightWJets = mySonic.GetDatasetLumiWeight("W4Jets");
    // Dont forget to change stopCrossSection argument if you change the signal !!!
    float WeightSignal550 = mySonic.GetDatasetLumiWeight("signal1") * stopCrossSection(550);
    float WeightSignal650 = mySonic.GetDatasetLumiWeight("signal2") * stopCrossSection(650);
    float WeightSignal750 = mySonic.GetDatasetLumiWeight("signal3") * stopCrossSection(750);

    for (int i = 0 ; i < Histos2D->size() ; i++)
    {
        if (((*Histos2D)[i].getRegionTag()    == "signalRegion")
        &&  ((*Histos2D)[i].getVariableXTag() == "BDTOutputNoWTag")
        &&  ((*Histos2D)[i].getVariableYTag() == "BDTOutputWithWTag"))
        {
               if ((*Histos2D)[i].getProcessClassTag() == "ttbar")
              BDTOutputsTTbar = (*Histos2D)[i].getClone();
          else if ((*Histos2D)[i].getProcessClassTag() == "W+jets")
              BDTOutputsWJets = (*Histos2D)[i].getClone();
          else if ((*Histos2D)[i].getProcessClassTag() == "signal_550_25")
              BDTOutputsSignal550 = (*Histos2D)[i].getClone();
          else if ((*Histos2D)[i].getProcessClassTag() == "signal_650_25")
              BDTOutputsSignal650 = (*Histos2D)[i].getClone();
          else if ((*Histos2D)[i].getProcessClassTag() == "signal_750_25")
              BDTOutputsSignal750 = (*Histos2D)[i].getClone();

        }
    }

     int  nBins = BDTOutputsSignal550->GetNbinsX(); 
    float xMin  = BDTOutputsSignal550->GetXaxis()->GetXmin();
    float xMax  = BDTOutputsSignal550->GetXaxis()->GetXmax();
    
    TH1F* signal550_00 = new TH1F("signal550_00","",nBins,xMin,xMax);
    TH1F* signal550_10 = new TH1F("signal550_10","",nBins,xMin,xMax);
    TH1F* signal550_01 = new TH1F("signal550_01","",nBins,xMin,xMax);
    TH1F* signal550_11 = new TH1F("signal550_11","",nBins,xMin,xMax);
    TH1F* signal650_00 = new TH1F("signal650_00","",nBins,xMin,xMax);
    TH1F* signal650_10 = new TH1F("signal650_10","",nBins,xMin,xMax);
    TH1F* signal650_01 = new TH1F("signal650_01","",nBins,xMin,xMax);
    TH1F* signal650_11 = new TH1F("signal650_11","",nBins,xMin,xMax);
    TH1F* signal750_00 = new TH1F("signal750_00","",nBins,xMin,xMax);
    TH1F* signal750_10 = new TH1F("signal750_10","",nBins,xMin,xMax);
    TH1F* signal750_01 = new TH1F("signal750_01","",nBins,xMin,xMax);
    TH1F* signal750_11 = new TH1F("signal750_11","",nBins,xMin,xMax);
    TH1F* ttbar_00 = new TH1F("ttbar_00","",nBins,xMin,xMax);
    TH1F* ttbar_10 = new TH1F("ttbar_10","",nBins,xMin,xMax);
    TH1F* ttbar_01 = new TH1F("ttbar_01","",nBins,xMin,xMax);
    TH1F* ttbar_11 = new TH1F("ttbar_11","",nBins,xMin,xMax);
    TH1F* wjets_00 = new TH1F("wjets_00","",nBins,xMin,xMax);
    TH1F* wjets_10 = new TH1F("wjets_10","",nBins,xMin,xMax);
    TH1F* wjets_01 = new TH1F("wjets_01","",nBins,xMin,xMax);
    TH1F* wjets_11 = new TH1F("wjets_11","",nBins,xMin,xMax);

    for (int i = 1 ; i <= nBins ; i++)
    {
        signal550_00->SetBinContent(i,BDTOutputsSignal550->Integral(0,i,0,i)                 / WeightSignal550);
        signal550_10->SetBinContent(i,BDTOutputsSignal550->Integral(i+1,nBins+1,0,i)         / WeightSignal550);
        signal550_01->SetBinContent(i,BDTOutputsSignal550->Integral(0,i,i+1,nBins+1)         / WeightSignal550);
        signal550_11->SetBinContent(i,BDTOutputsSignal550->Integral(i+1,nBins+1,i+1,nBins+1) / WeightSignal550);
        
        signal650_00->SetBinContent(i,BDTOutputsSignal650->Integral(0,i,0,i)                 / WeightSignal650);
        signal650_10->SetBinContent(i,BDTOutputsSignal650->Integral(i+1,nBins+1,0,i)         / WeightSignal650);
        signal650_01->SetBinContent(i,BDTOutputsSignal650->Integral(0,i,i+1,nBins+1)         / WeightSignal650);
        signal650_11->SetBinContent(i,BDTOutputsSignal650->Integral(i+1,nBins+1,i+1,nBins+1) / WeightSignal650);
        
        signal750_00->SetBinContent(i,BDTOutputsSignal750->Integral(0,i,0,i)                 / WeightSignal750);
        signal750_10->SetBinContent(i,BDTOutputsSignal750->Integral(i+1,nBins+1,0,i)         / WeightSignal750);
        signal750_01->SetBinContent(i,BDTOutputsSignal750->Integral(0,i,i+1,nBins+1)         / WeightSignal750);
        signal750_11->SetBinContent(i,BDTOutputsSignal750->Integral(i+1,nBins+1,i+1,nBins+1) / WeightSignal750);

        ttbar_00->SetBinContent(i,BDTOutputsTTbar->Integral(0,i,0,i)                / WeightTTbar);
        ttbar_10->SetBinContent(i,BDTOutputsTTbar->Integral(i+1,nBins+1,0,i)        / WeightTTbar);
        ttbar_01->SetBinContent(i,BDTOutputsTTbar->Integral(0,i,i+1,nBins+1)        / WeightTTbar);
        ttbar_11->SetBinContent(i,BDTOutputsTTbar->Integral(i+1,nBins+1,i+1,nBins+1)/ WeightTTbar);

        wjets_00->SetBinContent(i,BDTOutputsWJets->Integral(0,i,0,i)                / WeightWJets);
        wjets_10->SetBinContent(i,BDTOutputsWJets->Integral(i+1,nBins+1,0,i)        / WeightWJets);
        wjets_01->SetBinContent(i,BDTOutputsWJets->Integral(0,i,i+1,nBins+1)        / WeightWJets);
        wjets_11->SetBinContent(i,BDTOutputsWJets->Integral(i+1,nBins+1,i+1,nBins+1)/ WeightWJets);

    }

    TFile outputEric("BDTyields.root","RECREATE");
      
    BDTOutputsTTbar->Write();
    BDTOutputsWJets->Write();
    BDTOutputsSignal550->Write();
    BDTOutputsSignal650->Write();
    BDTOutputsSignal750->Write();

    signal550_00->Write();
    signal550_10->Write();
    signal550_01->Write();
    signal550_11->Write();
    signal650_00->Write();
    signal650_10->Write();
    signal650_01->Write();
    signal650_11->Write();
    signal750_00->Write();
    signal750_10->Write();
    signal750_01->Write();
    signal750_11->Write();
    ttbar_00->Write();
    ttbar_10->Write();
    ttbar_01->Write();
    ttbar_11->Write();
    wjets_00->Write();
    wjets_10->Write();
    wjets_01->Write();
    wjets_11->Write();

    cout << "weightTTbar = " << WeightTTbar << endl;
    cout << "weightWJets = " << WeightWJets << endl;
    cout << "weightSignal550 = " << WeightSignal550 << endl;
    cout << "weightSignal650 = " << WeightSignal650 << endl;
    cout << "weightSignal750 = " << WeightSignal750 << endl;

    outputEric.Close();

  cout << endl;
  cout << "   ┌──────────────────────────┐  " << endl;
  cout << "   │  Eric's plots generated  │  " << endl;
  cout << "   └──────────────────────────┘  " << endl; 
  cout << endl;




  return (0);
}

void fillMCSignalTable(SonicScrewdriver* mySonic, vector<string> region, vector<string> process, Table* table)
{
    string varUsedToGetYields = "BDTOutputNoWTag";
    string channelUsedToGetYields = "inclusiveChannel";

    for (unsigned int r = 0 ; r < region.size()          ; r++)
    {
        Figure tmpTotal(0.0,0.0);
        for (unsigned int p = 0 ; p < process.size() ; p++)
        {
            if (process[p] == "total") continue;
            table->Set(region[r],
                      process[p],
                      mySonic->GetYieldAndError(varUsedToGetYields,
                                               process[p],
                                               region[r],
                                               channelUsedToGetYields));
            
            if ((process[p] != "signal_550_25") && (process[p] != "signal_650_25") && (process[p] != "signal_750_25"))
                tmpTotal += mySonic->GetYieldAndError(varUsedToGetYields,
                                                     process[p],
                                                     region[r],
                                                     channelUsedToGetYields);
        }
        table->Set(region[r],"total",tmpTotal);
    }

}





float stopCrossSection(float inputMass)
{
         if (abs(inputMass - 170) <= 5) return 42.6441;
    else if (abs(inputMass - 180) <= 5) return 31.8695;
    else if (abs(inputMass - 190) <= 5) return 24.1585;
    else if (abs(inputMass - 200) <= 5) return 18.5245;
    else if (abs(inputMass - 210) <= 5) return 14.3201;
    else if (abs(inputMass - 220) <= 5) return 11.1808;
    else if (abs(inputMass - 230) <= 5) return 8.78125;
    else if (abs(inputMass - 240) <= 5) return 6.96892;
    else if (abs(inputMass - 250) <= 5) return 5.57596;
    else if (abs(inputMass - 260) <= 5) return 4.48773;
    else if (abs(inputMass - 270) <= 5) return 3.63085;
    else if (abs(inputMass - 280) <= 5) return 2.95613;
    else if (abs(inputMass - 290) <= 5) return 2.42299;
    else if (abs(inputMass - 300) <= 5) return 1.99608;
    else if (abs(inputMass - 310) <= 5) return 1.64956;
    else if (abs(inputMass - 320) <= 5) return 1.3733;
    else if (abs(inputMass - 330) <= 5) return 1.14277;
    else if (abs(inputMass - 340) <= 5) return 0.959617;
    else if (abs(inputMass - 350) <= 5) return 0.807323;
    else if (abs(inputMass - 360) <= 5) return 0.681346;
    else if (abs(inputMass - 370) <= 5) return 0.576882;
    else if (abs(inputMass - 380) <= 5) return 0.489973;
    else if (abs(inputMass - 390) <= 5) return 0.4176;
    else if (abs(inputMass - 400) <= 5) return 0.35683;
    else if (abs(inputMass - 410) <= 5) return 0.305512;
    else if (abs(inputMass - 420) <= 5) return 0.262683;
    else if (abs(inputMass - 430) <= 5) return 0.226367;
    else if (abs(inputMass - 440) <= 5) return 0.195812;
    else if (abs(inputMass - 450) <= 5) return 0.169668;
    else if (abs(inputMass - 460) <= 5) return 0.147492;
    else if (abs(inputMass - 470) <= 5) return 0.128326;
    else if (abs(inputMass - 480) <= 5) return 0.112241;
    else if (abs(inputMass - 490) <= 5) return 0.0977878;
    else if (abs(inputMass - 500) <= 5) return 0.0855847;
    else if (abs(inputMass - 510) <= 5) return 0.0751004;
    else if (abs(inputMass - 520) <= 5) return 0.0660189;
    else if (abs(inputMass - 530) <= 5) return 0.0580348;
    else if (abs(inputMass - 540) <= 5) return 0.0511747;
    else if (abs(inputMass - 550) <= 5) return 0.0452067;
    else if (abs(inputMass - 560) <= 5) return 0.0399591;
    else if (abs(inputMass - 570) <= 5) return 0.0354242;
    else if (abs(inputMass - 580) <= 5) return 0.0313654;
    else if (abs(inputMass - 590) <= 5) return 0.0279395;
    else if (abs(inputMass - 600) <= 5) return 0.0248009;
    else if (abs(inputMass - 610) <= 5) return 0.0220672;
    else if (abs(inputMass - 620) <= 5) return 0.0196331;
    else if (abs(inputMass - 630) <= 5) return 0.0175075;
    else if (abs(inputMass - 640) <= 5) return 0.0155809;
    else if (abs(inputMass - 650) <= 5) return 0.0139566;
    else if (abs(inputMass - 660) <= 5) return 0.0125393;
    else if (abs(inputMass - 670) <= 5) return 0.0112223;
    else if (abs(inputMass - 680) <= 5) return 0.0100516;
    else if (abs(inputMass - 690) <= 5) return 0.0090306;
    else if (abs(inputMass - 700) <= 5) return 0.0081141;
    else if (abs(inputMass - 710) <= 5) return 0.00730084;
    else if (abs(inputMass - 720) <= 5) return 0.00656729;
    else if (abs(inputMass - 730) <= 5) return 0.00591771;
    else if (abs(inputMass - 740) <= 5) return 0.00532605;
    else if (abs(inputMass - 750) <= 5) return 0.00480639;
    else if (abs(inputMass - 760) <= 5) return 0.00433688;
    else if (abs(inputMass - 770) <= 5) return 0.00391839;
    else if (abs(inputMass - 780) <= 5) return 0.00354211;
    else if (abs(inputMass - 790) <= 5) return 0.00320476;
    else if (abs(inputMass - 800) <= 5) return 0.00289588;
    else if (abs(inputMass - 810) <= 5) return 0.0026184;
    else if (abs(inputMass - 820) <= 5) return 0.00237168;
    else if (abs(inputMass - 830) <= 5) return 0.00214607;
    else if (abs(inputMass - 840) <= 5) return 0.00195172;
    else if (abs(inputMass - 850) <= 5) return 0.00176742;
    else if (abs(inputMass - 860) <= 5) return 0.00160403;
    else if (abs(inputMass - 870) <= 5) return 0.00145772;
    else if (abs(inputMass - 880) <= 5) return 0.00132077;
    else if (abs(inputMass - 890) <= 5) return 0.00120568;
    else if (abs(inputMass - 900) <= 5) return 0.00109501;
    else if (abs(inputMass - 910) <= 5) return 0.000996193;
    else if (abs(inputMass - 920) <= 5) return 0.000907494;
    else if (abs(inputMass - 930) <= 5) return 0.000826533;
    else if (abs(inputMass - 940) <= 5) return 0.000753768;
    else if (abs(inputMass - 950) <= 5) return 0.000687022;
    else if (abs(inputMass - 960) <= 5) return 0.000626876;
    else if (abs(inputMass - 970) <= 5) return 0.000571551;
    else if (abs(inputMass - 980) <= 5) return 0.000522495;
    else if (abs(inputMass - 990) <= 5) return 0.000476255;
    else if (abs(inputMass - 1000) <= 5) return 0.000435488;
    else return 0.0;
}


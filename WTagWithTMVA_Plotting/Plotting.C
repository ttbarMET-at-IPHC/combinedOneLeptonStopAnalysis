
#include <cmath>
#include <iomanip>
#include <iostream>
#include <time.h>
#include <set>


#include <TFile.h>
#include <TRandom.h>
#include <TMarker.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLorentzVector.h>
#include <TMVA/Reader.h>

using namespace std;

#include "../microTupling/MicroTuple_Format_MVA0726.h" 
microEvent* myEventPointer;
float* NNOutputPointer;

#include "interface/Table.h" 
#include "interface/SonicScrewdriver.h" 
using namespace theDoctor;

#define FOLDER_MICROTUPLES "../store/microTuples_MVA0726/"


// #########################################################################
//                          Region selectors
// #########################################################################

bool inclusiveChannelSelector() { return true; }

bool Selector_presel() { return true; }
bool Selector_signalRegion() 
{ 
    return  (
               (myEventPointer->MET   > 100) 
            && (myEventPointer->MT    > 120)
            && (myEventPointer->nJets >= 4 )
            ); 
}
bool Selector_afterNNCut() 
{ 
     return (Selector_signalRegion() && (*NNOutputPointer > 0.95)); 
}

// #########################################################################
//                          Others tools/stuff
// #########################################################################

void fillMCSignalTable(SonicScrewdriver* mySonic, vector<string> region, vector<string> process, Table* table);

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
 	 //mySonic.AddVariable("HadronicChi2",   "had. #Chi^{2}",           "",       20,0,20,        &(myEvent.HadronicChi2),  "");
 	 mySonic.AddVariable("MT2W",           "M_{T2}^{W}",              "GeV",    20,0,500,       &(myEvent.MT2W),          "");
 	 mySonic.AddVariable("HTratio",        "HT_{ratio}",              "",       20,0,1.2,       &(myEvent.HTratio),       "");
 	 mySonic.AddVariable("HadronicChi2",   "Hadronic Chi2",           "",       20,0,5,         &(myEvent.HadronicChi2),  "");

     mySonic.AddVariable("nJets",          "# of jets",              "",        4, 2.5,6.5,     &(myEvent.nJets),         "");
     mySonic.AddVariable("nBTag",          "# of b-tagged jets",     "",        4,-0.5,3.5,     &(myEvent.nBTag),         "");
     mySonic.AddVariable("nWTag",          "# of W-tagged jets",     "",        3,-0.5,2.5,     &(myEvent.nWTag),         "");

 	 mySonic.AddVariable("mStop",          "m_{#tilde{t}}",          "GeV",     14,112.5,812.5, &(myEvent.mStop),         "");
 	 mySonic.AddVariable("mNeutralino",    "m_{#chi^{0}}",           "GeV",     8,-12.5,387.5,  &(myEvent.mNeutralino),   "");
 	 
     float NNOutput;    NNOutputPointer = &NNOutput;
     mySonic.AddVariable("NNOutput",       "NN output",              "",        20,0.7,1.2,     &(NNOutput),              "");
     
  // #########################################################
  // ##   Create ProcessClasses (and associated datasets)   ##
  // #########################################################
      
      // ttbar -> l + jets
      mySonic.AddProcessClass("ttbar",    "t#bar{t}",     "background",COLORPLOT_RED);
          
          mySonic.AddDataset("ttbar",     "ttbar", 100,234);

      // Wjets
      mySonic.AddProcessClass("W+jets",   "W+jets",       "background",kOrange-2);
      
          mySonic.AddDataset("W2Jets",    "W+jets",100,2159);
          mySonic.AddDataset("W3Jets",    "W+jets",100,640);
          mySonic.AddDataset("W4Jets",    "W+jets",100,264);

     // Signal
     mySonic.AddProcessClass("signal_dm500-600", "signal (#DeltaM500-600)",  "signal",COLORPLOT_AZURE);
          mySonic.AddDataset("signal1", "signal_dm500-600", 100,1);
          set< pair<float,float> > signalPoints1;

     mySonic.AddProcessClass("signal_dm600-700", "signal (#DeltaM600-700)",  "signal",COLORPLOT_GREEN);
          mySonic.AddDataset("signal2", "signal_dm600-700", 100,1);
          set< pair<float,float> > signalPoints2;

     mySonic.AddProcessClass("signal_dm700-800", "signal (#DeltaM700-800)",  "signal",kCyan-3);
          mySonic.AddDataset("signal3", "signal_dm700-800", 100,1);
          set< pair<float,float> > signalPoints3;
          
          

  // ##########################
  // ##    Create Regions    ##
  // ##########################

     mySonic.AddRegion("presel",                "Preselection",             &Selector_presel);
     mySonic.AddRegion("signalRegion",          "Signal region",            &Selector_signalRegion);
     mySonic.AddRegion("afterNNCut",            "After NN cut",             &Selector_afterNNCut);
 
  // ##########################
  // ##   Create Channels    ##
  // ##########################
   
	 mySonic.AddChannel("inclusiveChannel","",&inclusiveChannelSelector);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################
  
     mySonic.SetLumi(20000);

	 // Create histograms
  	 mySonic.Create1DHistos();
  	 mySonic.Add2DHisto("mStop","mNeutralino");
  	 
  	 // Schedule plots
  	 mySonic.SchedulePlots("1DSuperpRenorm");
  	 mySonic.SchedulePlots("1DStack");
  	 mySonic.SchedulePlots("2D");
 
  // ##########################
  // ##  Set up MVA reader   ##
  // ########################## 
  
  TMVA::Reader* MVAReader = new TMVA::Reader( "" );

  //MVAReader->AddVariable( "nJets",      &(myEvent.nJets)     );
  //MVAReader->AddVariable( "nWTag",      &(myEvent.nWTag)     );
  MVAReader->AddVariable( "MET",           &(myEvent.MET)          );
  MVAReader->AddVariable( "MT2W",          &(myEvent.MT2W)         );
  MVAReader->AddVariable( "dPhiMETjet",    &(myEvent.dPhiMETjet)   );
  MVAReader->AddVariable( "HTratio",       &(myEvent.HTratio)      );
  MVAReader->AddVariable( "HadronicChi2",  &(myEvent.HadronicChi2) );
 
  MVAReader->BookMVA("MLP","../WTagWithTMVA/weights_noWTag/TMVAClassification_MLP.weights.xml");

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
    
     cout << "                    " << currentDataset << endl; 

  // ########################################
  // ##        Run over the events         ##
  // ########################################

      float weight_lumi = 0.0;
      for (int i = 0 ; i < theTree->GetEntries() ; i++)
      {

          // Get the i-th entry
          theTree->GetEntry(i);
  
          float deltaM = myEvent.mStop - myEvent.mNeutralino;

          if (myEvent.mStop != -1)
          {
            pair<float,float> signalPoint(myEvent.mStop,myEvent.mNeutralino);

            // Filter weird signal events (bugged mNeutralino = 0 events ?)
            if (myEvent.mStop       - ((int) myEvent.mStop      ) != 0) continue;
            if (myEvent.mNeutralino - ((int) myEvent.mNeutralino) != 0) continue;
            
            if (myEvent.mNeutralino > 50) continue;

            // Sort events according to deltaM
            if ((currentDataset == "signal1") && ((deltaM < 500) || (deltaM > 600))) continue;
            else signalPoints1.insert(signalPoint);
            
            if ((currentDataset == "signal2") && ((deltaM < 600) || (deltaM > 700))) continue;
            else signalPoints2.insert(signalPoint);
            
            if ((currentDataset == "signal3") && ((deltaM < 700) || (deltaM > 800))) continue;
            else signalPoints3.insert(signalPoint);

          }

          NNOutput = MVAReader->EvaluateMVA("MLP");


          // Fill all the variables with autoFill-mode activated
          mySonic.AutoFillProcessClass(currentProcessClass,myEvent.weight);

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

      for (set< pair<float,float> >::iterator it  = signalPoints1.begin() ;
                                              it != signalPoints1.end()   ;
                                              it++)
          cout << " " << (*it).first << ";" << (*it).second;
      cout << endl;


      vector<string> processClassList;
      mySonic.GetProcessClassList(&processClassList);
      processClassList.pop_back();
      processClassList.pop_back();
      processClassList.pop_back();
      processClassList.push_back("total");
      processClassList.push_back("signal_dm500-600");
      processClassList.push_back("signal_dm600-700");
      processClassList.push_back("signal_dm700-800");
      
      vector<string> regions;
      regions.push_back("presel");
      regions.push_back("signalRegion");
      regions.push_back("afterNNCut");
        
      Table table(regions,processClassList);
      fillMCSignalTable(&mySonic,regions,processClassList,&table);
      
      for (unsigned int r = 0 ; r < regions.size() ; r++)
      {
          table.Set(regions[r],"signal_dm500-600",table.Get(regions[r],"signal_dm500-600") / ((float) signalPoints1.size()));
          table.Set(regions[r],"signal_dm600-700",table.Get(regions[r],"signal_dm600-700") / ((float) signalPoints2.size()));
          table.Set(regions[r],"signal_dm700-800",table.Get(regions[r],"signal_dm700-800") / ((float) signalPoints3.size()));
      }

      table.PrintTable();

  cout << endl;
  cout << "   ┌───────────────────────────────┐  " << endl;
  cout << "   │  Tables generation completed  │  " << endl;
  cout << "   └───────────────────────────────┘  " << endl; 
  cout << endl;

  return (0);
}

void fillMCSignalTable(SonicScrewdriver* mySonic, vector<string> region, vector<string> process, Table* table)
{
    string varUsedToGetYields = "NNOutput";
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
            
            if ((process[p] != "signal_dm500-600") && (process[p] != "signal_dm600-700") && (process[p] != "signal_dm700-800"))
                tmpTotal += mySonic->GetYieldAndError(varUsedToGetYields,
                                                     process[p],
                                                     region[r],
                                                     channelUsedToGetYields);
        }
        table->Set(region[r],"total",tmpTotal);
    }

}


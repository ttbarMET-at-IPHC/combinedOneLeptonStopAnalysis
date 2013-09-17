#include <cstdlib>
#include <iostream> 
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TPluginManager.h"

//#include "TMVAGui.C"

#ifndef __CINT__
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
#endif

#define MICROTUPLES_FOLDER "../store/microTuples_MVA0904/"
#include "../microTupling/MicroTuple_Format_MVA0808.h" 

using namespace TMVA;
using namespace std;

float getNumberOfEvent(TTree* theTree);

int main() 
{
   // this loads the library
   TMVA::Tools::Instance();

   //---------------------------------------------------------------
   // default MVA methods to be trained + tested
   
   std::map<std::string,int> Use;
   Use["Cuts"]            =1;
   Use["BDT"]             =1;
   
   // ---------------------------------------------------------------

   std::cout << std::endl;
   std::cout << "==> Start TMVAClassification" << std::endl;

   // Create a new root output file.
   TString outfileName( "TMVA_output.root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

   TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile, 
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D" );

    // Add the variables you want to consider 
    
    //factory->AddVariable( "MT := MT",  'F' );
    //factory->AddVariable( "nJets := nJets",  'F' );
    factory->AddVariable( "MET := MET",  'F' );
    factory->AddVariable( "MT2W := MT2W",  'F' );
    factory->AddVariable( "dPhiMETjet := dPhiMETjet",  'F' );
    factory->AddVariable( "HTratio := HTratio",  'F' );
    factory->AddVariable( "HadronicChi2 := HadronicChi2",  'F' );
    //factory->AddVariable( "nWTag := nWTag",  'F' );
    
    // Open samples
    
    TFile* f_signal = TFile::Open((string(MICROTUPLES_FOLDER)+"signal.root").c_str());
    TFile* f_ttbar  = TFile::Open((string(MICROTUPLES_FOLDER)+"ttbar.root" ).c_str());
    TFile* f_W2Jets = TFile::Open((string(MICROTUPLES_FOLDER)+"W2Jets.root").c_str());
    TFile* f_W3Jets = TFile::Open((string(MICROTUPLES_FOLDER)+"W3Jets.root").c_str());
    TFile* f_W4Jets = TFile::Open((string(MICROTUPLES_FOLDER)+"W4Jets.root").c_str());
    
    TTree* signal = (TTree*) f_signal->Get("microTuple");
    TTree* ttbar  = (TTree*) f_ttbar ->Get("microTuple");
    TTree* W2Jets = (TTree*) f_W2Jets->Get("microTuple");
    TTree* W3Jets = (TTree*) f_W3Jets->Get("microTuple");
    TTree* W4Jets = (TTree*) f_W4Jets->Get("microTuple");

    // Register the trees
        
    factory->AddSignalTree    ( signal, 1.0 * 20000.0 / 55000);
    factory->AddBackgroundTree( ttbar,  225.2 * 20000.0 / getNumberOfEvent(ttbar) );

    /*
    cout << " signal ; w = " << 1.0   * 20000.0 / getNumberOfEvent(signal) << endl;
    factory->AddSignalTree    ( signal, 1.0   * 20000.0 / getNumberOfEvent(signal));
    cout << " ttbar ; w = "  << 225.2 * 20000.0 / getNumberOfEvent(ttbar) << endl;
    factory->AddBackgroundTree( ttbar,  234.0 * 20000.0 / getNumberOfEvent(ttbar));
    cout << " W2Jets ; w = " << 2159  * 20000.0 / getNumberOfEvent(W2Jets) << endl;
    factory->AddBackgroundTree( W2Jets, 2159  * 20000.0 / getNumberOfEvent(W2Jets));
    cout << " W3Jets ; w = " << 640   * 20000.0 / getNumberOfEvent(W3Jets) << endl;
    factory->AddBackgroundTree( W3Jets, 640   * 20000.0 / getNumberOfEvent(W3Jets));
    cout << " W4Jets ; w = " << 264   * 20000.0 / getNumberOfEvent(W4Jets) << endl;
    factory->AddBackgroundTree( W4Jets, 264   * 20000.0 / getNumberOfEvent(W4Jets));
    */

    // Add preselection cuts
   
    std::string preselectionCutsSig("nJets > 4 && MET > 80 && MT > 100");
    std::string preselectionCutsBkg("nJets > 4 && MET > 80 && MT > 100");

    // Prepare the training

    factory->PrepareTrainingAndTestTree( preselectionCutsSig.c_str(), preselectionCutsBkg.c_str(),
                    "nTrain_Signal=40000:nTrain_Background=300000:nTest_Signal=40000:nTest_Background=300000:SplitMode=Random:NormMode=EqualNumEvents:!V" );

    // Cut optimisation
    //if (Use["Cuts"])     factory->BookMethod( TMVA::Types::kCuts, "Cuts",
    //                     "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );
    if (Use["BDT"])      factory->BookMethod( TMVA::Types::kBDT, "BDT", 
                         "!H:!V:NTrees=400:nEventsMin=400:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" ); 
   // --------------------------------------------------------------

   // Train MVAs using the set of training events
   factory->TrainAllMethodsForClassification();

   // Evaluate all MVAs using the set of test events
   factory->TestAllMethods();

   // Evaluate and compare performance of all configured MVAs
   factory->EvaluateAllMethods();    

   // --------------------------------------------------------------
   
   // Save the output
   outputFile->Close();

   std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
   std::cout << "==> TMVAClassification is done!" << std::endl;      

   delete factory;

   // Launch the GUI for the root macros
   //if (!gROOT->IsBatch()) TMVAGui( outfileName );
}


float getNumberOfEvent(TTree* theTree)
{
   microEvent myEvent;
   theTree->SetBranchAddress("microEvents",&myEvent);
   theTree->GetEntry(0);
   return myEvent.trueNumberOfEvents;

}

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

#define FOLDER_BABYTUPLES "../store/babyTuples_1102/"

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
   TString outfileName( "output.root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

   TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile, 
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D" );

    // Add the variables you want to consider 
   
    factory->AddVariable("METoverSqrtHT := METoverSqrtHT", 'F' );
    factory->AddVariable("MET := MET", 'F' );
    factory->AddVariable("MT := MT", 'F' );
    factory->AddVariable("MT2W := MT2W", 'F' );
    factory->AddVariable("HTPlusLeptonPtPlusMET := HTPlusLeptonPtPlusMET", 'F' );
    factory->AddVariable("HTRatio := HTRatio", 'F' );
    factory->AddVariable("deltaPhiMETJets := deltaPhiMETJets", 'F' );
    factory->AddVariable("leadingBPt := leadingBPt", 'F' );
    factory->AddVariable("deltaRLeptonLeadingB := deltaRLeptonLeadingB", 'F' );
    factory->AddVariable("hadronicChi2 := hadronicChi2", 'F' );
    factory->AddVariable("leadingJetPt := leadingJetPt", 'F' );
    factory->AddVariable("Mlb_hemi := Mlb_hemi", 'F' );
    factory->AddVariable("HT := HT", 'F' );
    factory->AddVariable("leadingLeptonPt := leadingLeptonPt", 'F' );
    factory->AddVariable("Mlb := Mlb", 'F' );
    factory->AddVariable("M3b := M3b", 'F' );
    
    // Open samples
    
    TFile* f_signal = TFile::Open((string(FOLDER_BABYTUPLES)+"babyTuple_T2tt.root").c_str());
    TFile* f_ttbar  = TFile::Open((string(FOLDER_BABYTUPLES)+"babyTuple_ttbar.root" ).c_str());
    
    TTree* signal = (TTree*) f_signal->Get("babyTuple");
    TTree* ttbar  = (TTree*) f_ttbar ->Get("babyTuple");

    // Register the trees

    float weightSignal     = 1.0;
    float weightBackground = 1.0;

    factory->AddSignalTree    ( signal, weightSignal    );
    factory->AddBackgroundTree( ttbar,  weightBackground);

    // Add preselection cuts
   
    std::string preselection("(triggerMuon == 1 || triggerElec == 1) && numberOfLepton == 1 && isolatedTrackVeto == 1 && tauVeto == 1 && nJets >= 4 && nBTag >= 1 && MET > 80 && MT > 100 && ((mStop == -1) || (((mStop - mNeutralino) > 125) && ((mStop - mNeutralino) < 175)))");

    // Prepare the training

    factory->PrepareTrainingAndTestTree( preselection.c_str(), preselection.c_str(),
                    "nTrain_Signal=500000:nTrain_Background=500000:nTest_Signal=40000:nTest_Background=300000:SplitMode=Random:NormMode=EqualNumEvents:!V" );

    // Cut optimisation
    //if (Use["Cuts"])     factory->BookMethod( TMVA::Types::kCuts, "Cuts",
    //                     "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );
    if (Use["BDT"])      factory->BookMethod( TMVA::Types::kBDT, "BDT", 
                         "!H:!V:NTrees=400:nEventsMin=400:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning");
   
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


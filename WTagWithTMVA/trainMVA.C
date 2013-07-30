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

using namespace TMVA;

int main() 
{
   // this loads the library
   TMVA::Tools::Instance();

   //---------------------------------------------------------------
   // default MVA methods to be trained + tested
   std::map<std::string,int> Use;

   Use["Cuts"]            =1;
   Use["CutsD"]           =0;
   Use["CutsPCA"]         =0;
   Use["CutsGA"]          =0;
   Use["CutsSA"]          =0;
   // ---
   Use["MLP"]             =1; // this is the recommended ANN
   Use["MLPBFGS"]         =0; // recommended ANN with optional training method
   Use["CFMlpANN"]        =0; // *** missing
   Use["TMlpANN"]         =0; 
   // ---
   Use["SVM"]             =0;
   // ---
   Use["BDT"]             =1;
   Use["BDTD"]            =0;
   Use["BDTG"]            =0;
   Use["BDTB"]            =0;
   // ---
   Use["RuleFit"]         =0;
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
    //factory->AddVariable( "nWTag := nWTag",  'F' );
    factory->AddVariable( "MET := MET",  'F' );
    factory->AddVariable( "MT2W := MT2W",  'F' );
    factory->AddVariable( "dPhiMETjet := dPhiMETjet",  'F' );
    factory->AddVariable( "HTratio := HTratio",  'F' );
    factory->AddVariable( "HadronicChi2 := HadronicChi2",  'F' );
    
    // Open samples
    
    TFile* f_signal = TFile::Open("../store/microTuples_MVA0726/signal.root");
    TFile* f_ttbar  = TFile::Open("../store/microTuples_MVA0726/ttbar.root" );
    TFile* f_W2Jets = TFile::Open("../store/microTuples_MVA0726/W2Jets.root");
    TFile* f_W3Jets = TFile::Open("../store/microTuples_MVA0726/W3Jets.root");
    TFile* f_W4Jets = TFile::Open("../store/microTuples_MVA0726/W4Jets.root");
    
    TTree *signal = (TTree*) f_signal->Get("microTuple");
    TTree *ttbar  = (TTree*) f_ttbar ->Get("microTuple");
    TTree *W2Jets = (TTree*) f_W2Jets->Get("microTuple");
    TTree *W3Jets = (TTree*) f_W3Jets->Get("microTuple");
    TTree *W4Jets = (TTree*) f_W4Jets->Get("microTuple");

    // Register the trees

    factory->AddSignalTree    ( signal, 1.0 );
    factory->AddBackgroundTree( ttbar,  1.0 );
    factory->AddBackgroundTree( W2Jets, 1.0 );
    factory->AddBackgroundTree( W3Jets, 1.0 );
    factory->AddBackgroundTree( W4Jets, 1.0 );

    factory->SetSignalWeightExpression("weight");
    factory->SetBackgroundWeightExpression("weight");

    // Add preselection cuts
   
    std::string preselectionCutsSig("MET > 100 && MT > 120");
    std::string preselectionCutsBkg("MET > 100 && MT > 120");

    // Prepare the training

    factory->PrepareTrainingAndTestTree( preselectionCutsSig.c_str(), preselectionCutsBkg.c_str(),
                    "nTrain_Signal=4000:nTrain_Background=5000:SplitMode=Random:NormMode=NumEvents:!V" );

    // Cut optimisation
    if (Use["Cuts"])     factory->BookMethod( TMVA::Types::kCuts, "Cuts",
                         "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );
    if (Use["CutsD"])    factory->BookMethod( TMVA::Types::kCuts, "CutsD", 
                         "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=Decorrelate" );
    if (Use["CutsPCA"])  factory->BookMethod( TMVA::Types::kCuts, "CutsPCA", 
                         "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=PCA" );
    if (Use["CutsGA"])   factory->BookMethod( TMVA::Types::kCuts, "CutsGA",  
                         "H:!V:FitMethod=GA:CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95" );
    if (Use["CutsSA"])   factory->BookMethod( TMVA::Types::kCuts, "CutsSA",
                         "!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );
    if (Use["MLP"])      factory->BookMethod( TMVA::Types::kMLP, "MLP", 
                         "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5" );
    if (Use["MLPBFGS"])  factory->BookMethod( TMVA::Types::kMLP, "MLPBFGS", 
                         "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS" );
    if (Use["CFMlpANN"]) factory->BookMethod( TMVA::Types::kCFMlpANN, "CFMlpANN", 
                         "!H:!V:NCycles=2000:HiddenLayers=N+1,N"  ); 
    if (Use["TMlpANN"])  factory->BookMethod( TMVA::Types::kTMlpANN, "TMlpANN", 
                         "!H:!V:NCycles=200:HiddenLayers=N+1,N:LearningMethod=BFGS:ValidationFraction=0.3"  ); 
    if (Use["SVM"])      factory->BookMethod( TMVA::Types::kSVM, "SVM", 
                         "Gamma=0.25:Tol=0.001:VarTransform=Norm" );
    if (Use["BDT"])      factory->BookMethod( TMVA::Types::kBDT, "BDT", 
                         "!H:!V:NTrees=400:nEventsMin=400:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );
    // Gradient Boost
    if (Use["BDTG"])     factory->BookMethod( TMVA::Types::kBDT, "BDTG", 
                         "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.30:UseBaggedGrad:GradBaggingFraction=0.6:SeparationType=GiniIndex:nCuts=20:NNodesMax=5" );
    // Bagging
    if (Use["BDTB"])     factory->BookMethod( TMVA::Types::kBDT, "BDTB", 
                         "!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );
    // Decorrelation + Adaptive Boost
    if (Use["BDTD"])     factory->BookMethod( TMVA::Types::kBDT, "BDTD", 
                        "!H:!V:NTrees=400:nEventsMin=400:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning:VarTransform=Decorrelate" );
    // RuleFit -- TMVA implementation of Friedman's method
    if (Use["RuleFit"])  factory->BookMethod( TMVA::Types::kRuleFit, "RuleFit",
                         "H:!V:RuleFitModule=RFTMVA:Model=ModRuleLinear:MinImp=0.001:RuleMinDist=0.001:NTrees=20:fEventsMin=0.01:fEventsMax=0.5:GDTau=-1.0:GDTauPrec=0.01:GDStep=0.01:GDNSteps=10000:GDErrScale=1.02" );

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

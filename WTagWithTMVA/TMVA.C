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

#include "TMVAGui.C"

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
   // ---
   Use["Plugin"]          =0;
   // ---------------------------------------------------------------

   std::cout << std::endl;
   std::cout << "==> Start TMVAClassification" << std::endl;

   // Create a new root output file.
   TString outfileName( "TMVA_output.root" );
   TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

   TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile, 
                                               "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D" );

    // Add the variables you want to consider 
    
    factory->AddVariable( "nJets := nJets",  'F' );
    factory->AddVariable( "MT := MT",  'F' );
    factory->AddVariable( "MET := MET",  'F' );
    factory->AddVariable( "MT2W := MT2W",  'F' );
    factory->AddVariable( "dPhiMETjet := dPhiMETjet",  'F' );
    factory->AddVariable( "HTratio := HTratio",  'F' );
    
    // Open samples
    
    TFile* ttbarv1 = TFile::Open("../store/microTuples_MVANoWTag0723/ttbar-v1.root")
    TFile* ttbarv2 = TFile::Open("../store/microTuples_MVANoWTag0723/ttbar-v2.root")
    TFile* W2Jets  = TFile::Open("../store/microTuples_MVANoWTag0723/W2Jets.root")
    TFile* W3Jets  = TFile::Open("../store/microTuples_MVANoWTag0723/W3Jets.root")
    TFile* W4Jets  = TFile::Open("../store/microTuples_MVANoWTag0723/W4Jets.root")
    
    TTree *signal      = (TTree*)f0->Get("theTree2");
    TTree *signal2     = (TTree*)f3->Get("theTree2");
    TTree *background  = (TTree*)f1->Get("theTree2");
    TTree *background2 = (TTree*)f2->Get("theTree2");
    cout<<"trees: "<<signal<<" "<<background<<endl;

    Double_t signalWeight     = 0.2057/209434;
    Double_t signalWeight2     = 0.23/185338;
    Double_t backgroundWeight = 234.*0.33*0.33/9982625;
    Double_t backgroundWeight2 = 27.59/5828292;
    /*
    Double_t signalWeight     = 0.2057/6841;
    Double_t backgroundWeight = 234.*0.33*0.33/304512;
    Double_t backgroundWeight2 = 27.59/205690;
    */
    // ====== register trees ====================================================
    //
    // the following method is the prefered one:
    // you can add an arbitrary number of signal or background trees

    factory->AddSignalTree    ( signal,     signalWeight     );
    factory->AddBackgroundTree( background, backgroundWeight );
    factory->AddBackgroundTree( background2, backgroundWeight2 );

    factory->SetSignalWeightExpression("weight1*weight2");
    factory->SetBackgroundWeightExpression("weight1*weight2");


    factory->PrepareTrainingAndTestTree( mycuts, mycutb,
                    "nTrain_Signal=4000:nTrain_Background=5000:SplitMode=Random:NormMode=NumEvents:!V" );

    // If no numbers of events are given, half of the events in the tree are used for training, and 
    // the other half for testing:
    //    factory->PrepareTrainingAndTestTree( mycut, "SplitMode=random:!V" );  
    // To also specify the number of testing events, use:
    //    factory->PrepareTrainingAndTestTree( mycut, 
    //                                         "NSigTrain=3000:NBkgTrain=3000:NSigTest=3000:NBkgTest=3000:SplitMode=Random:!V" );  

    // ---- Book MVA methods
    //
    // please lookup the various method configuration options in the corresponding cxx files, eg:
    // src/MethoCuts.cxx, etc, or here: http://tmva.sourceforge.net/optionRef.html
    // it is possible to preset ranges in the option string in which the cut optimisation should be done:
    // "...:CutRangeMin[2]=-1:CutRangeMax[2]=1"...", where [2] is the third input variable

    // Cut optimisation
    if (Use["Cuts"])
    factory->BookMethod( TMVA::Types::kCuts, "Cuts", 
               "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart" );

    if (Use["CutsD"])
    factory->BookMethod( TMVA::Types::kCuts, "CutsD", 
               "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=Decorrelate" );

    if (Use["CutsPCA"])
    factory->BookMethod( TMVA::Types::kCuts, "CutsPCA", 
               "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart:VarTransform=PCA" );

    if (Use["CutsGA"])
    factory->BookMethod( TMVA::Types::kCuts, "CutsGA",
               "H:!V:FitMethod=GA:CutRangeMin[0]=-10:CutRangeMax[0]=10:VarProp[1]=FMax:EffSel:Steps=30:Cycles=3:PopSize=400:SC_steps=10:SC_rate=5:SC_factor=0.95" );

    if (Use["CutsSA"])
    factory->BookMethod( TMVA::Types::kCuts, "CutsSA",
               "!H:!V:FitMethod=SA:EffSel:MaxCalls=150000:KernelTemp=IncAdaptive:InitialTemp=1e+6:MinTemp=1e-6:Eps=1e-10:UseDefaultScale" );

    // TMVA ANN: MLP (recommended ANN) -- all ANNs in TMVA are Multilayer Perceptrons
    if (Use["MLP"])
       factory->BookMethod( TMVA::Types::kMLP, "MLP", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5" );

    if (Use["MLPBFGS"])
       factory->BookMethod( TMVA::Types::kMLP, "MLPBFGS", "H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:TestRate=5:TrainingMethod=BFGS" );

    // CF(Clermont-Ferrand)ANN
    if (Use["CFMlpANN"])
        factory->BookMethod( TMVA::Types::kCFMlpANN, "CFMlpANN", "!H:!V:NCycles=2000:HiddenLayers=N+1,N"  ); // n_cycles:#nodes:#nodes:...  
  
    // Tmlp(Root)ANN
    if (Use["TMlpANN"])
        factory->BookMethod( TMVA::Types::kTMlpANN, "TMlpANN", "!H:!V:NCycles=200:HiddenLayers=N+1,N:LearningMethod=BFGS:ValidationFraction=0.3"  ); // n_cycles:#nodes:#nodes:...
  
    // Support Vector Machine
    if (Use["SVM"])
        factory->BookMethod( TMVA::Types::kSVM, "SVM", "Gamma=0.25:Tol=0.001:VarTransform=Norm" );
   
    // Boosted Decision Trees
    if (Use["BDTG"]) // Gradient Boost
        factory->BookMethod( TMVA::Types::kBDT, "BDTG", 
                           "!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.30:UseBaggedGrad:GradBaggingFraction=0.6:SeparationType=GiniIndex:nCuts=20:NNodesMax=5" );

    if (Use["BDT"])  // Adaptive Boost
        f->BookMethod( TMVA::Types::kBDT, "BDT", 
                           "!H:!V:NTrees=400:nEventsMin=400:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );
   
   if (Use["BDTB"]) // Bagging
      factory->BookMethod( TMVA::Types::kBDT, "BDTB", 
                           "!H:!V:NTrees=400:BoostType=Bagging:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning" );

   if (Use["BDTD"]) // Decorrelation + Adaptive Boost
      factory->BookMethod( TMVA::Types::kBDT, "BDTD", 
                           "!H:!V:NTrees=400:nEventsMin=400:MaxDepth=3:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=NoPruning:VarTransform=Decorrelate" );
   
   // RuleFit -- TMVA implementation of Friedman's method
   if (Use["RuleFit"])
      factory->BookMethod( TMVA::Types::kRuleFit, "RuleFit",
                           "H:!V:RuleFitModule=RFTMVA:Model=ModRuleLinear:MinImp=0.001:RuleMinDist=0.001:NTrees=20:fEventsMin=0.01:fEventsMax=0.5:GDTau=-1.0:GDTauPrec=0.01:GDStep=0.01:GDNSteps=10000:GDErrScale=1.02" );

   // --------------------------------------------------------------------------------------------------

   // As an example how to use the ROOT plugin mechanism, book BDT via
   // plugin mechanism
   if (Use["Plugin"]) {
         //
         // first the plugin has to be defined, which can happen either through the following line in the local or global .rootrc:
         //
         // # plugin handler          plugin name(regexp) class to be instanciated library        constructor format
         // Plugin.TMVA@@MethodBase:  ^BDT                TMVA::MethodBDT          TMVA.1         "MethodBDT(TString,TString,DataSet&,TString)"
         // 
         // or by telling the global plugin manager directly
      gPluginMgr->AddHandler("TMVA@@MethodBase", "BDT", "TMVA::MethodBDT", "TMVA.1", "MethodBDT(TString,TString,DataSet&,TString)");
      factory->BookMethod( TMVA::Types::kPlugins, "BDT",
                           "!H:!V:NTrees=400:BoostType=AdaBoost:SeparationType=GiniIndex:nCuts=20:PruneMethod=CostComplexity:PruneStrength=50" );
   }

   // --------------------------------------------------------------------------------------------------
   // ---- Now you can tell the factory to train, test, and evaluate the MVAs

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
   if (!gROOT->IsBatch()) TMVAGui( outfileName );
}

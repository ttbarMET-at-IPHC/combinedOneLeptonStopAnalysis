
// C / C++ headers
#include <cmath>
#include <iostream>
using namespace std;

// ROOT headers
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TBranch.h>
#include <TLorentzVector.h>

#include "../common.h"

// Define format
#include "Format.h"
babyEvent myEvent;
string sampleType;

// Define input and output folders

    // Standard babyTuples
#define FOLDER_INPUT  "../store/babyTuples_0603_withBDT/"
//#define FOLDER_OUTPUT "../store/babyTuples_0603_withBDT_skim/1lepton4jetsMET80/"
    
    // Fix for dilepton channels with 1 and 2 jets bins
//#define FOLDER_INPUT  "../store/babyTuples_0603_fix2leptonAtLeast1jet_withBDT/"
#define FOLDER_OUTPUT "../store/babyTuples_0603_withBDT_skim/2leptons1jetMET50/"

// Files needed to compute on-fly variables
#include "../AN-14-067/SecondLeptonInAcceptance.h"
#include "../AN-14-067/cutAndCountDefinitions.h"

// ###################
// #  Main function  #
// ###################

int main (int argc, char *argv[])
{
    if (argc <= 1)
    {
        cout << "No dataset specified in argument" << endl;
        return -1;
    }

    string dataset = argv[1]; 

    // ################################
    // ##       Open the tree        ##
    // ################################

    // Input tree   
    TFile fInput((FOLDER_INPUT+dataset+".root").c_str(),"READ");
    TTree* theInputTree = (TTree*) fInput.Get("babyTuple"); 
    intermediatePointers pointers;
    InitializeBranchesForReading(theInputTree,&myEvent,&pointers);

    // Output tree
    TFile fOutput((FOLDER_OUTPUT+dataset+".root").c_str(),"RECREATE");
    TTree* theOutputTree = new TTree("babyTuple","");
    InitializeBranchesForWriting(theOutputTree,&myEvent);

    // ########################################
    // ##        Run over the events         ##
    // ########################################

    for (int i = 0 ; i < theInputTree->GetEntries() ; i++)
    {

        if (i % (theInputTree->GetEntries() / 100) == 0) 
            printProgressBar(i,theInputTree->GetEntries(),dataset);

        // Read event
        ReadEvent(theInputTree,i,&pointers,&myEvent);

        // Apply selection
        
        /*
        if (myEvent.numberOfLepton != 1) continue;
        if (myEvent.nJets < 4)           continue;
        if (myEvent.MET < 80)            continue;
        */
         
        if (myEvent.numberOfLepton != 2) continue;
        if (myEvent.nJets < 1)           continue;
        if (myEvent.MET < 50)            continue;
        

        if ((dataset == "DoubleMuon") 
         || (dataset == "DoubleElec") 
         || (dataset == "MuEl"      ) 
         || (dataset == "SingleMuon") 
         || (dataset == "SingleElec"))
            sampleType = "data";

        // Compute some variables before storing them
        myEvent.secondLeptonInAcceptance = EventHasSecondGeneratedLeptonInAcceptance();
        myEvent.ISRJet                   = findISRJet();
        myEvent.leadingNonBPt            = leadingNonBPt();

        // Fill tree

        theOutputTree->Fill();
    }           

    cout << endl;
    theOutputTree->Write(0, TObject::kOverwrite); 

    return (0); 
}             



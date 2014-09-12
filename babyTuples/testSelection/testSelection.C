// C++ headers
#include <iomanip>
#include <iostream>
#include <time.h>
using namespace std;

// ROOT headers
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>

// IPHC headers
#include "NTFormat/interface/NTEvent.h"
#include "Selection/interface/combined1LeptonStopSelection.h"
#include "Tools/interface/Dataset.h"
#include "Tools/interface/AnalysisEnvironmentLoader.h"
using namespace IPHCTree;

#include "../../common.h"

// #########################################################################""
// #########################################################################""
// #########################################################################""

int main (int argc, char *argv[])
{

  printBoxedMessage("Starting test");

  // ############################
  // #  Initializing variables  #
  // ############################

  string xmlFileName("config.xml");
  int verbosity = 3;
  vector < Dataset > datasets;
  combined1LeptonStopSelection sel;
  IPHCTree::NTEvent * event = 0;

  // #############################
  // #   Loading configuration   #
  // #############################

  cout << endl;
  DEBUG_MSG << "Loading configuration..." << endl;
  cout << "        (config : " << xmlFileName << ")" << endl;

  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known
  anaEL.LoadSelection (sel);    // now the parameters for the selection are given to the selection // no specific TTbarMET parameters

  // ###########################
  // #   Loading corrections   #
  // ###########################

  sel.setBTagReshapingInput(     anaEL.GetInfo("Analysis","Corrections","bTagReshaping")          );
  sel.setMCJetCorrectionsInput(  anaEL.GetInfo("Analysis","Corrections","jetEnergyCorrectionMC")  );
  sel.setDataJetCorrectionsInput(anaEL.GetInfo("Analysis","Corrections","jetEnergyCorrectionData"));
  sel.setPileUpReweightingInput( anaEL.GetInfo("Analysis","Corrections","pileUp")                 );

  sel.loadCorrections();

  // ####################################
  // #   Start loop over the datasets   #
  // ####################################

  if (verbosity > 0) printBoxedMessage("Starting loop over datasets");

  for (unsigned int datasetId = 0; datasetId < datasets.size (); datasetId++)
  {

    // ########################
    // #   Load the dataset   #
    // ########################

    DEBUG_MSG << "Loading next dataset..." << endl;

    datasets[datasetId].eventTree()->SetBranchAddress ("NTEvent", &event);

    if (verbosity > 2)
    {
        cout << endl;
        cout << "         [ Dataset n°" << datasetId+1 << " ]" << endl;
        cout << "         " << datasets[datasetId].Name() << endl;
        cout << endl;
        DEBUG_MSG << "   NEvents total    : " << static_cast<unsigned int>(datasets[datasetId].eventTree()->GetEntries ()) << endl;
        DEBUG_MSG << "NEvents to run over : " << datasets[datasetId].NofEvtsToRunOver() << endl;
        cout << endl;
    }

    bool runningOnData = datasets[datasetId].isData();

    // ############################
    // #   Loop over the events   #
    // ############################

    if (verbosity > 0) printBoxedMessage("Starting loop over events");

    for (int ievt = 0; ievt < datasets[datasetId].NofEvtsToRunOver(); ievt++)
    {

        if (ievt % 100000 == 0) printProgressBar(ievt,datasets[datasetId].NofEvtsToRunOver());

        // Load the event

        datasets[datasetId].eventTree()->GetEntry(ievt);
        IPHCTree::NTTransient::InitializeAfterReading(event);
        int eventId = event->general.eventNb;
        sel.LoadEvent(event);

        /*
        cout    << "run   = " << event->general.runNb
             << " ; lumi  = " << event->general.lumiblock
             << " ; event = " << event->general.eventNb << endl;
             */

        // Apply selection
        if (sel.PassEventSelection(runningOnData) == false) continue;

        TLorentzVector leadingLepton      = sel.GetTheLeadingLepton();
        Short_t        leadingLeptonPDGId = sel.GetTheLeadingLeptonPDGId();

        float leptonCharge;
        if (leadingLeptonPDGId > 0) leptonCharge = -1.0;
        else                        leptonCharge = +1.0;

        bool isolatedTrackVeto = sel.Get1LeptonStopIsolatedTrackVeto(leadingLepton,leptonCharge);
        bool tauVeto           = sel.Get1LeptonStopTauVeto(leadingLepton,leptonCharge);

        const std::vector<IPHCTree::NTJet>& goodJets = sel.GetGoodJets();

        for(unsigned int i=0 ; i < goodJets.size() ; i++)
        {

            //cout << "i = " << i << " ; " << goodJets[i].p4.Pt() << goodJets[i].p4.Eta() << endl;
            //cout << "i = " << i << " ; " << goodJets[i].p4Gen.Pt() << goodJets[i].p4Gen.Eta() << endl;

        }


    }   // End event loop
  }     // End dataset loop


  printBoxedMessage("Program completed");
  return (0);
}



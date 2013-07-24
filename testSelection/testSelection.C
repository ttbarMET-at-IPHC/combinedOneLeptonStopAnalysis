#include <iomanip>
#include <iostream>
#include <time.h>
#include "NTFormat/interface/NTEvent.h"

#include "Selection/interface/TTbarMetSelection.h"
#include "Selection/interface/SelectionTable.h"
#include "Tools/interface/Dataset.h"
#include "Tools/interface/AnalysisEnvironmentLoader.h"
#include "Tools/interface/PUWeighting.h"
#include "Tools/interface/LumiReweightingStandAlone.h"
#include "Tools/interface/JetCorrector.h"

#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>

#include "miscTools.h"

using namespace IPHCTree;
using namespace std;

// #########################################################################""
// #########################################################################""
// #########################################################################""

int main (int argc, char *argv[])
{
  cout << endl;
  cout << "   ,-----------------------," << endl;
  cout << "   |   Starting analysis   |" << endl;
  cout << "   `-----------------------`" << endl;
  cout << endl;

  // ##################################
  // #	Some parameters for the user  #
  // ##################################
 
  bool doHistoManager = false;		// False will not produce histos
  bool doLatexOutput = true;		// False will not produce latex table
  bool doPileUpReweigthing = false;	// False will not use pileup reweighting

  // (can be overwritten by argv[1])
  string defaultConfiguration("config.xml");
  // (can be overwritten by argv[2])
  string defaultLatexTableName("CutflowTable");
  // (can be overwritten by argv[3])
  string defaultRootOutputName("TTbarMETanalysis.root");

  // ############################
  // #	Initializing variables  #
  // ############################
  
  INFO1_MSG << "Initializing variables..." << endl;
 
  vector < Dataset > datasets;
  TTbarMetSelection sel;
  
  float Luminosity = 0;
  float LumiError = 0.;
  int DataType = 0;				// DataType : 0: MC - 1: Data - 2 Data & MC
  int verbosity = -1;

  reweight::LumiReWeighting *LumiWeights;
  IPHCTree::NTEvent * event = 0;

  // Reading parameters from argv
  // 	-> configuration file
  string xmlFileName;
  if (argc > 1) xmlFileName = string(argv[1]);
  else          xmlFileName = defaultConfiguration;
  // 	-> root output name
  string rootOutputName;
  if (argc > 2) rootOutputName = string(argv[2]);
  else 			rootOutputName = defaultRootOutputName;
  // 	-> latex table name
  string latexTableName;
  if (argc > 3) latexTableName = string(argv[3]);
  else latexTableName = defaultLatexTableName;
  
  // #############################
  // # 	 Loading configuration   #
  // #############################

  cout << endl;
  INFO1_MSG << "Loading configuration..." << endl;
  cout << "        (config : " << xmlFileName << ")" << endl;
  
 
  AnalysisEnvironmentLoader anaEL (xmlFileName);
  anaEL.LoadSamples (datasets);	// now the list of datasets written in the xml file is known
  anaEL.LoadSelection (sel);	// now the parameters for the selection are given to the selection // no specific TTbarMET parameters
  anaEL.LoadGeneralInfo (DataType, Luminosity, LumiError, verbosity);

  // ##############################
  // # 	 Printing general infos   #
  // ##############################
  
  cout << endl;
  cout << "   -----------------------------" << endl;
  cout << "     Verbosity mode : " << verbosity << endl;
  cout << "     Luminosity : " << Luminosity << endl;
  cout << "     DataType (whole config) : ";
  switch (DataType) 
  {
  	case 0: { cout << "MC" << endl;   break; }
  	case 1: { cout << "Data" << endl; break; }
  	case 2: { cout << "Data & MC" << endl; break; }
  	default: { cout << "Unknwon" << endl; break; }
  }
  cout << "   -----------------------------" << endl;
  
  // #########################################
  // # 	 Start loop over the dataset infos   #
  // #########################################
  if (verbosity > 0)
  {
  	cout << endl;
  	cout << "   ,---------------------------------," << endl;
  	cout << "   |   Starting loop over datasets   |" << endl;
  	cout << "   `---------------------------------`" << endl;
  	cout << endl;
  }

  //sel.InitSUSYstopJEC("../store/JECinput/GR_P_V39_AN3");
  //sel.InitSUSYstopJEC("../store/JECinput/START53_V7E");

  int nEvents_tot;
  for (unsigned int datasetId = 0; datasetId < datasets.size (); datasetId++) 
  {

	// ########################
	// #   Load the dataset   #
	// ########################
   
    INFO1_MSG << "Loading next dataset..." << endl;

    datasets[datasetId].eventTree()->SetBranchAddress ("NTEvent", &event);
    unsigned int nEvents = static_cast<unsigned int>(datasets[datasetId].eventTree()->GetEntries ());
    nEvents_tot = nEvents;

    if (verbosity > 2)
    {
        cout << endl;
        cout << "         [ Dataset n°" << datasetId+1 << " ]" << endl;
        cout << "         " << datasets[datasetId].Name() << endl;
        cout << endl;
        INFO1_MSG << "   NEvents total    : " << nEvents << endl;
        INFO1_MSG << "NEvents to run over : " << datasets[datasetId].NofEvtsToRunOver() << endl;
        cout << endl;
    }

	// ########################
	// #   Load the pile-up   #
	// ########################
	
	INFO1_MSG << "Loading pile-up informations..." << endl;
   
    // PU from JL's code
	if (datasets[datasetId].isData() == false) {
      string datafile = "/opt/sbg/data/data1/cms/ccollard/CMSSW/CMSSW_4_2_8_patch7/src/NTuple/NTupleAnalysis/macros/data/default73.5mb.root";
      string mcfile   = "/opt/sbg/data/data1/cms/ccollard/CMSSW/CMSSW_4_2_8_patch7/src/NTuple/NTupleAnalysis/macros/data/PU3DMC.root";

      LumiWeights    = new reweight::LumiReWeighting(mcfile, datafile, "histoMCPU", "pileup" );
      LumiWeights->weight3D_init( 1. );
    }
 
	// ############################
	// #   Loop over the events   #
	// ############################
			  
	if (verbosity > 0)
	{
 		cout << endl;
 	 	cout << "   ,-------------------------------," << endl;
 	 	cout << "   |   Starting loop over events   |" << endl;
  		cout << "   `-------------------------------`" << endl;
  		cout << endl;
	}

    for (unsigned int ievt = 0; ievt < datasets[datasetId].NofEvtsToRunOver(); ievt++)
    {

	  // Display some information

      if (verbosity > 3)
      {
		        INFO1_MSG <<  "run: " << event->general.runNb;
		        std::cout << " lumi: " << event->general.lumiblock;
      		    std::cout << " event: " << event->general.eventNb;
		  		std::cout << std::endl;
      }

      if (ievt % 100000 == 0) printProgressBar(ievt,datasets[datasetId].NofEvtsToRunOver());

	  // Load the event
	   
	  datasets[datasetId].eventTree()->GetEntry(ievt);
	  IPHCTree::NTTransient::InitializeAfterReading(event);
	  int eventId = event->general.eventNb;
	  sel.LoadEvent(event);
	  
	  // Apply full selection
	  int triggerME = 0;
      int selLastStep = sel.doFullSelection (&(datasets[datasetId]), string("all"),&triggerME);

	  // Get trigger info from selection
      bool trigger_e = false;
      bool trigger_mu = false;
	  if (triggerME % 1 == 1) trigger_e  = true;
	  if (triggerME    >= 10) trigger_mu = true; 

	  // Also get the lepton type
      int lep = sel.GetLeptonType();

      

	  } // end of loop over evts
  }		// end of loop over datasets 

  cout << endl;
  cout << "   ,-----------------------," << endl;
  cout << "   |   Program completed   |" << endl;
  cout << "   `-----------------------`" << endl;
  cout << endl;

  return (0);
}



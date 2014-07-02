	#include "../common.h"
	#include "common.h"

	// Sonic screwdriver headers

	#include "interface/Table.h" 
	#include "interface/SonicScrewdriver.h" 
	#include "interface/tables/TableBackgroundSignal.h" 
	#include "interface/tables/TableDataMC.h" 
	using namespace theDoctor;

	// BabyTuple format and location


#define FOLDER_BABYTUPLES "/opt/sbg/data/safe1/cms/echabert/StopAlex/combinedOneLeptonStopAnalysis/babySkimmer/babyTuple_BDT_0618_skimmed_Caro/"
#define FOLDER_BABYTUPLES2 "/opt/sbg/data/safe1/cms/echabert/StopAlex/combinedOneLeptonStopAnalysis/babySkimmer/babyTuple_BDT_CR45_merged/"
#include "Reader_newFinal0603_BDT_skimmed_caro.h"


	#include "analysisDefinitions.h"
	#include "cutAndCountDefinitions.h"

//required for BDT signal region def
//#include "../AN-14-067/signalRegionDefinitions.h"
#include "BDTDefinition.h"

	#ifndef SIGNAL_REGION_CUTS
	    #error SIGNAL_REGION_CUTS need to be defined.
	#endif
	#ifndef SIGNAL_REGION_TAG
	    #error SIGNAL_REGION_TAG need to be defined.
	#endif



	bool LM150(bool applyMTCut)
	{
	    // Apply MET and MT cuts
	    if (myEvent.MET < 150)            return false;

	    if (myEvent.deltaPhiMETJets < 0.8) return false;
	    if (myEvent.hadronicChi2    > 5)   return false;

	    return true; 
	}

	bool enableMTCut = true;
	bool disableMTCut = false;

	bool goesInPreVetoSelectionMTpeak_withSRCuts()   { return (goesInPreVetoSelectionMTpeak()   && SIGNAL_REGION_CUTS(disableMTCut)); }
	bool goesInPreVetoSelectionMTtail_withSRCuts()   { return (goesInPreVetoSelectionMTtail()   && SIGNAL_REGION_CUTS(enableMTCut) ); }
									      
	bool goesInPreselectionMTpeak_withSRCuts()       { return (goesInPreselectionMTpeak()       && SIGNAL_REGION_CUTS(disableMTCut)); }
	bool goesInPreselectionMTtail_withSRCuts()       { return (goesInPreselectionMTtail()       && SIGNAL_REGION_CUTS(enableMTCut) ); }
	bool goesInPreselectionNoMT_withSRCuts()       { 
	    bool iok = goesInPreselection()       && SIGNAL_REGION_CUTS(disableMTCut);
	    if (myEvent.MT>MT_CUT && (sampleType == "data") )  iok=0;
	    return iok;
	}
									      
	bool goesIn0BtagControlRegionMTpeak_withSRCuts() { return (goesIn0BtagControlRegionMTpeak() && SIGNAL_REGION_CUTS(disableMTCut)); }
	bool goesIn0BtagControlRegionMTtail_withSRCuts() { return (goesIn0BtagControlRegionMTtail() && SIGNAL_REGION_CUTS(enableMTCut) ); }
	bool goesIn0BtagControlRegionNoMT_withSRCuts() { return (goesIn0BtagControlRegion() && SIGNAL_REGION_CUTS(disableMTCut) ); }

	bool goesIn2LControlRegionMTpeak_withSRCuts() { return (goesIn2LControlRegionMTpeak() && SIGNAL_REGION_CUTS(disableMTCut)); }
	bool goesIn2LControlRegionMTtail_withSRCuts() { return (goesIn2LControlRegionMTtail() && SIGNAL_REGION_CUTS(enableMTCut) ); }
	bool goesIn2LControlRegionNoMT_withSRCuts() { return (goesInDileptonControlRegion()  && SIGNAL_REGION_CUTS(disableMTCut)); }

	bool goesInVetoControlRegionMTpeak_withSRCuts() { return (goesInVetoControlRegionMTpeak() && SIGNAL_REGION_CUTS(disableMTCut)); }
	bool goesInVetoControlRegionMTtail_withSRCuts() { return (goesInVetoControlRegionMTtail() && SIGNAL_REGION_CUTS(enableMTCut) ); }
	bool goesInVetoControlRegionNoMT_withSRCuts() { return (goesInVetosControlRegion()  && SIGNAL_REGION_CUTS(disableMTCut)); }


//--- Access to quantiles of the plots   ---//

float QuantileVariabelCut(SonicScrewdriver& sonic, float quantile, string var, vector<string> processClass, string region, string channel){
	//retrieve plots for the all processClasses
	TH1F* hclone = 0;
	TH1F* hMCSum = 0; // sum of all processClasses
	for(unsigned int i=0;i<processClass.size();i++){
		hclone = sonic.get1DHistoClone(var, processClass[i], region, channel);
		if(i==0) hMCSum = hclone;
		else hMCSum->Add(hclone);
	}
	//Search for the quantile
	float integral = hMCSum->Integral();
	float frac  = 0;
	int qbin = 0; // bin where quantile is found
	for(int i=0;i<hMCSum->GetNbinsX();i++){
		if(integral!=0) frac = hMCSum->Integral(0,i)/integral;
		if(frac>=quantile){
			qbin = i;
			break;
		}
	}
	if(integral!=0 && qbin!=hMCSum->GetNbinsX()){
		return hMCSum->GetBinCenter(qbin);
	}
	return -1;
}
  //
float MinNofEvtsVariabelCut(SonicScrewdriver& sonic, int nof, string var, vector<string> processClass, string region, string channel){
	//retrieve plots for the all processClasses
	TH1F* hclone = 0;
	TH1F* hMCSum = 0; // sum of all processClasses
	for(unsigned int i=0;i<processClass.size();i++){
		hclone = sonic.get1DHistoClone(var, processClass[i], region, channel);
		if(i==0) hMCSum = hclone;
		else hMCSum->Add(hclone);
	}
	//Search for the quantile
	float integral = 0;
	int qbin = 0; // bin where quantile is found
	for(int i=hMCSum->GetNbinsX();i>0;i--){
		integral = hMCSum->Integral(i,hMCSum->GetNbinsX());
		if(integral>=nof){
			qbin = i;
			break;
		}
	}
	if(integral!=0 && qbin!=hMCSum->GetNbinsX()){
		return hMCSum->GetBinCenter(qbin);
	}
	return -1;
}


	// #########################################################################
	//                              Main function
	// #########################################################################

	int main (int argc, char *argv[])
	{

	  if (signalRegionLabel(SIGNAL_REGION_TAG) == "") { DEBUG_MSG << "Please define the signal region label associated to tag '" << SIGNAL_REGION_TAG << "'" << endl; return -1; }

	  printBoxedMessage("Starting plot generation");

	  // ####################
	  // ##   Init tools   ##
	  // ####################
	  
	     // Create a sonic Screwdriver
	      SonicScrewdriver screwdriver;

	     // ##########################
	     // ##   Create Variables   ##
	     // ##########################
	    /*

	     screwdriver.AddVariable("MET",            "MET",                     "GeV",    16,50,530,      &(myEvent.MET),                  "logY");
	     screwdriver.AddVariable("MT",             "M_{T}",                   "GeV",    40,0,400,       &(myEvent.MT),                   "logY");
	     screwdriver.AddVariable("MT_2",             "M_{T}",                   "GeV",    16,0,400,       &(myEvent.MT),                   "logY");
	     screwdriver.AddVariable("MT_3",             "M_{T}",                   "GeV",    8,0,400,       &(myEvent.MT),                   "logY");
	     screwdriver.AddVariable("MT_4",             "M_{T}",                   "GeV",    4,0,400,       &(myEvent.MT),                   "logY");

	     screwdriver.AddVariable("MTpeak",         "M_{T}",                   "GeV",    20,0,100,       &(myEvent.MT),                   "noOverflowInLastBin");
	     screwdriver.AddVariable("MTtail",         "M_{T}",                   "GeV",    30,100,400,     &(myEvent.MT),                   "logY,noUnderflowInFirstBin");
	     screwdriver.AddVariable("deltaPhiMETJets","#Delta#Phi(MET,j_{1,2})", "rad",    16,0,3.2,       &(myEvent.deltaPhiMETJets),      "");
	     screwdriver.AddVariable("MT2W",           "M_{T2}^{W}",              "GeV",    20,0,500,       &(myEvent.MT2W),                 "");
	     screwdriver.AddVariable("HTratio",        "H_{T}^{ratio}",           "",       20,0,1.2,       &(myEvent.HTRatio),              "");
	     screwdriver.AddVariable("HadronicChi2",   "Hadronic #chi^{2}",       "",       40,0,10,        &(myEvent.hadronicChi2),         "");
	     screwdriver.AddVariable("HT",             "H_{T}",                   "",       46,120,1500,    &(myEvent.HT),                   "");
	     screwdriver.AddVariable("leadingBPt",     "p_{T}(leading b-jet)",    "GeV",    20,0,400,       &(myEvent.leadingBPt),           "");
	     screwdriver.AddVariable("leadingJetPt",   "p_{T}(leading jet)",      "GeV",    20,0,600,       &(myEvent.leadingJetPt),         "");
	     screwdriver.AddVariable("leptonPt",       "p_{T}(lepton)",           "GeV",    28,20,300,      &(myEvent.leadingLeptonPt),      "");
	     screwdriver.AddVariable("Mlb",            "M_{lb}",                  "GeV",    26,0,520,       &(myEvent.Mlb),                  "");
	     screwdriver.AddVariable("Mlb_hemi",       "M_{lb}_hemi",             "GeV",    26,0,520,       &(myEvent.Mlb_hemi),             "");
	     screwdriver.AddVariable("M3b",            "M3b",                     "GeV",    20,50,750,      &(myEvent.M3b),                  "");
	     screwdriver.AddVariable("deltaRLeptonB",  "#DeltaR(l,leading b)",    "",       20,0,5,         &(myEvent.deltaRLeptonLeadingB), "");
	     screwdriver.AddVariable("HTLeptonPtMET",  "HT + MET + p_{T}(lepton)","GeV",    20,100,2100,    &(myEvent.HTPlusLeptonPtPlusMET),"");
	     screwdriver.AddVariable("METoverSqrtHT",  "MET / #sqrt{H_{T}}",      "",       32,0,32,        &(myEvent.METoverSqrtHT),        "");


	     screwdriver.AddVariable("leadingNonBPt",     "p_{T}(leading nonb-jet)",    "GeV",    10,0,500,       &(myEvent.leadingNonBPt),           "");
	     screwdriver.AddVariable("leadingNonBPt_1",     "p_{T}(leading nonb-jet)",    "GeV",    20,0,500,       &(myEvent.leadingNonBPt),           "");
*/
	     float leadingNonBPtN5; 
/*	     
	     screwdriver.AddVariable("leadingNonBPtN5",     "p_{T}(leading nonb-jet) (Njet>4)",    "GeV",    10,0,500,       &(leadingNonBPtN5),           "noUnderflowInFirstBin");
	     screwdriver.AddVariable("leadingNonBPtN5_1",     "p_{T}(leading nonb-jet) (Njet>4)",    "GeV",    20,0,500,       &(leadingNonBPtN5),           "noUnderflowInFirstBin");
*/
	     float nnn;
//	     screwdriver.AddVariable("Njets",           "#Jets",                  "",       11,-0.5,10.5,        &(nnn),                "");

	     float leadingLeptonEta;
	     float secondLeptonEta;
	     float secondLeptonPt;
/*	     screwdriver.AddVariable("2ndleptonPt",    "p_{T}(2nd lepton)",       "GeV",    28,20,300,      &(secondLeptonPt),      "noUnderflowInFirstBin");
	     screwdriver.AddVariable("leptonEta",       "#eta(lepton)",       "",       25,-2.5,2.5,        &(leadingLeptonEta),      "");
	     screwdriver.AddVariable("2ndleptonEta",    "#eta(2nd lepton)",   "",       25,-2.5,2.5,        &(secondLeptonEta),      "noUnderflowInFirstBin");
*/	     float pdgchannel;
//	     screwdriver.AddVariable("channelID",       "channelID",         "",       5,0.5,5.5,      &(pdgchannel),      "");
	     float dilepmass;
//	     screwdriver.AddVariable("dilepM",          "M_{ll}",             "GeV",    28,1.,211.,         &(dilepmass),      "noUnderflowInFirstBin");

	     int nn2;
	     // screwdriver.AddVariable("Nbjets",           "#b-Jets",                  "",       5,-0.5,4.5,        &(nn2),                "");


             float bdt1;
             float bdt2;
             float bdt3;
             float bdt4;
             float bdt5;
             float bdt6;
             float bdt7;
             float bdt8;
             float bdt9;
             float bdt10;
             float bdt11;
             float bdt12;
             float bdt13;
             float bdt14;
             float bdt15;
             float bdt16;

             screwdriver.AddVariable("BDT1",   "T2tt_1",     "",   100,-0.5,0.5, &(bdt1), "");
             screwdriver.AddVariable("BDT2",   "T2tt_2",     "",   100,-0.5,0.5, &(bdt2), "");
             screwdriver.AddVariable("BDT3",   "T2tt_5",     "",   100,-0.5,0.5, &(bdt3), "");

             screwdriver.AddVariable("BDT4",   "T2bw075_1",     "",   100,-0.5,0.5, &(bdt4), "");
             screwdriver.AddVariable("BDT5",   "T2bw075_2",     "",   100,-0.5,0.5, &(bdt5), "");
             screwdriver.AddVariable("BDT6",   "T2bw075_3",     "",   100,-0.5,0.5, &(bdt6), "");
             screwdriver.AddVariable("BDT7",   "T2bw075_5",     "",   100,-0.5,0.5, &(bdt7), "");

             screwdriver.AddVariable("BDT8",   "T2bw050_1",     "",   100,-0.5,0.5, &(bdt8), "");
             screwdriver.AddVariable("BDT9",   "T2bw050_3",     "",   100,-0.5,0.5, &(bdt9), "");
             screwdriver.AddVariable("BDT10",   "T2bw050_4",     "",   100,-0.5,0.5, &(bdt10), "");
             screwdriver.AddVariable("BDT11",   "T2bw050_5",     "",   100,-0.5,0.5, &(bdt11), "");
             screwdriver.AddVariable("BDT12",   "T2bw050_6",     "",   100,-0.5,0.5, &(bdt12), "");

             screwdriver.AddVariable("BDT13",   "T2bw025_1",     "",   100,-0.5,0.5, &(bdt13), "");
             screwdriver.AddVariable("BDT14",   "T2bw025_3",     "",   100,-0.5,0.5, &(bdt14), "");
             screwdriver.AddVariable("BDT15",   "T2bw025_4",     "",   100,-0.5,0.5, &(bdt15), "");
             screwdriver.AddVariable("BDT16",   "T2bw025_6",     "",   100,-0.5,0.5, &(bdt16), "");



	     // #########################################################
	     // ##   Create ProcessClasses (and associated datasets)   ##
	     // #########################################################

	     screwdriver.AddProcessClass("1ltop", "1l top",                             "background",kRed-7);
                    if (madgraph) {
                    screwdriver.AddDataset("ttbar_madgraph_1l",   "1ltop",  0, 0);
                    }
                    else {
                    screwdriver.AddDataset("ttbar_powheg",   "1ltop",  0, 0);
                    }
                    screwdriver.AddDataset("singleTop_st",   "1ltop",  0, 0);

	     
	     screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",kCyan-3);
                    if (madgraph) {
                    screwdriver.AddDataset("ttbar_madgraph_2l",   "ttbar_2l",  0, 0);
                    }
  
	     
	     screwdriver.AddProcessClass("W+jets",   "W+jets",                          "background",kOrange-2);
		     screwdriver.AddDataset("W+jets",    "W+jets", 0, 0);

	     screwdriver.AddProcessClass("rare",   "rare",                              "background",kMagenta-5);
		     screwdriver.AddDataset("rare",   "rare", 0, 0);
	     
	     screwdriver.AddProcessClass("data",   "data",                              "data",COLORPLOT_BLACK);
		     screwdriver.AddDataset("SingleElec",   "data", 0, 0);
		     screwdriver.AddDataset("SingleMuon",   "data", 0, 0);

	     if (CR45) {
		     screwdriver.AddDataset("DoubleElec",   "data", 0, 0);
		     screwdriver.AddDataset("DoubleMuon",   "data", 0, 0);
		     screwdriver.AddDataset("MuEl",   "data", 0, 0);
	     }

	     // TODO : add signal here and check contamination in control regions

	     // ##########################
	     // ##    Create Regions    ##
	     // ##########################

	     screwdriver.AddRegion("MTpeak",     signalRegionLabel(SIGNAL_REGION_TAG)+";MT peak Control Region", &goesInPreselectionMTpeak_withSRCuts);
	     screwdriver.AddRegion("0btag",      signalRegionLabel(SIGNAL_REGION_TAG)+";0 b-tag Control Region", &goesIn0BtagControlRegionMTtail_withSRCuts, "rebin=2");
	     if (CR45) {
	     std::string optionsig;
             if (signalRegionLabel(SIGNAL_REGION_TAG) == "cut_nocut") optionsig="rebin=1";
	     else optionsig="rebin=2";
	     screwdriver.AddRegion("2leptons",      signalRegionLabel(SIGNAL_REGION_TAG)+";2 leptons  (No MT cut)", &goesIn2LControlRegionNoMT_withSRCuts, optionsig);
	     screwdriver.AddRegion("antiveto",      signalRegionLabel(SIGNAL_REGION_TAG)+";anti-veto on 2nd lepton (No MT cut)", &goesInVetoControlRegionNoMT_withSRCuts, optionsig);
	     screwdriver.AddRegion("2lepMTtail",    signalRegionLabel(SIGNAL_REGION_TAG)+";2 leptons Control Region", &goesIn2LControlRegionMTtail_withSRCuts, optionsig);
	     screwdriver.AddRegion("antivMTtail",   signalRegionLabel(SIGNAL_REGION_TAG)+";anti-veto on 2nd lepton Control Region", &goesInVetoControlRegionMTtail_withSRCuts, optionsig);
	     }
	     else {
	     screwdriver.AddRegion("sr_fullMT",     signalRegionLabel(SIGNAL_REGION_TAG)+"; (Not MT cut)", &goesInPreselectionNoMT_withSRCuts);
	     screwdriver.AddRegion("0btag_fullMT",      signalRegionLabel(SIGNAL_REGION_TAG)+";0 b-tag (Not MT cut)", &goesIn0BtagControlRegionNoMT_withSRCuts, "rebin=2");
	     }

	     // ##########################
	     // ##   Create Channels    ##
	     // ##########################
	   
	     screwdriver.AddChannel("singleLepton", "1l-channels",         &goesInSingleLeptonChannel);
	     screwdriver.AddChannel("singleElec",   "e-channel",              &goesInSingleElecChannel  );
	     screwdriver.AddChannel("singleMuon",   "#mu-channel",            &goesInSingleMuonChannel  );
	     if (CR45) {
	     screwdriver.AddChannel("doubleLepton",   "2l-channel",              &goesInDoubleLeptonChannel  );
	     screwdriver.AddChannel("doubleElec",   "ee-channel",              &goesInDoubleElecChannel  );
	     screwdriver.AddChannel("doubleMuon",   "#mu#mu-channel",          &goesInDoubleMuonChannel  );
	     screwdriver.AddChannel("emu",   "e#mu-channel",            &goesInMuonElecChannel  );
	     }
	     
	  // ########################################
	  // ##       Create histograms and        ##
	  // ##  schedule type of plots to produce ##
	  // ########################################

	     // Create histograms
	     screwdriver.Create1DHistos();

	     // Schedule plots
	     screwdriver.SchedulePlots("1DDataMCComparison");

	     // Config plots

	     screwdriver.SetGlobalStringOption("Plot", "infoTopRight", "CMS Internal");
	     screwdriver.SetGlobalStringOption("Plot", "infoTopLeft",  "#sqrt{s} = 8 TeV, L = 19.5 fb^{-1}");

	     screwdriver.SetGlobalBoolOption("Plot", "exportPdf", true);
	     screwdriver.SetGlobalBoolOption("Plot", "exportEps", false);
	     screwdriver.SetGlobalBoolOption("Plot", "exportPng", false);

	  // ########################################
	  // ##       Run over the datasets        ##
	  // ########################################

	  vector<string> datasetsList;
	  screwdriver.GetDatasetList(&datasetsList);

	  cout << "   > Reading datasets... " << endl;
	  cout << endl;
	  string signalRegionLabel(SIGNAL_REGION_TAG);

	  Table scaleFactors = Table("scaleFactors/"+signalRegionLabel+".tab");
	  Figure SFR_Wjets  = scaleFactors.Get("value","SFR_W+jets");
          cout << "SFR_Wjets " << SFR_Wjets.value() << endl;

	  for (unsigned int d = 0 ; d < datasetsList.size() ; d++)
	  {
	     string currentDataset = datasetsList[d];
	     string currentProcessClass = screwdriver.GetProcessClass(currentDataset);

	     sampleName = currentDataset;
	     sampleType = screwdriver.GetProcessClassType(currentProcessClass);
	    
	     // Open the tree
	     //      TFile f((string(FOLDER_BABYTUPLES)+currentDataset+".root").c_str());
	     //      TTree* theTree = (TTree*) f.Get("babyTuple");
	     string namefile;
	     if (CR45) {
	       namefile=string(FOLDER_BABYTUPLES2)+currentDataset+".root";
	     }
	     else {
	       namefile=string(FOLDER_BABYTUPLES)+currentDataset+".root";
	     }
	     TFile f(namefile.c_str());
	     TTree* theTree = (TTree*) f.Get("babyTuple"); 
	     
	     intermediatePointers pointers;
	     InitializeBranchesForReading(theTree,&myEvent,&pointers);

	  // ########################################
	  // ##        Run over the events         ##
	  // ########################################
	  //
	
     bool ttbarDatasetToBeSplitted = false;
     if (currentDataset.find("ttbar")!=std::string::npos && currentDataset.find("ttbar_madgraph_1l")==std::string::npos && currentDataset.find("ttbar_madgraph_2l")==std::string::npos)
        ttbarDatasetToBeSplitted = true;


	      int nEntries = theTree->GetEntries();
	      for (int i = 0 ; i < nEntries ; i++)
	      {
		  if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

		  // Get the i-th entry
		  ReadEvent(theTree,i,&pointers,&myEvent);

		  float weight = getWeight();

		  // Split 1-lepton ttbar and 2-lepton ttbar
		  string currentProcessClass_ = currentProcessClass;
//		  if ((currentDataset == "ttbar_powheg") && (myEvent.numberOfGenLepton == 2)) 
//		      currentProcessClass_ = "ttbar_2l";
          if (ttbarDatasetToBeSplitted && (myEvent.numberOfGenLepton == 2))
              currentProcessClass_ = "ttbar_2l";


	  	if(CR4_2j) if(myEvent.nJets<2) continue; 
	  	if(CR4_3j) if(myEvent.nJets<3) continue; 
	  	if(CR4_4j) if(myEvent.nJets<4) continue; 

		  nnn=myEvent.nJets;
		  nn2= myEvent.nBTag;

		  leadingNonBPtN5=-1;
                 if (nnn>4) leadingNonBPtN5= myEvent.leadingNonBPt;

          leadingLeptonEta = (myEvent.leadingLepton).Eta();
          secondLeptonEta = -1000.;
          secondLeptonPt = -1000.;
          dilepmass = -1.;
          if (myEvent.numberOfLepton==2) {
           secondLeptonEta = (myEvent.secondLepton).Eta();
           secondLeptonPt = (myEvent.secondLepton).Pt();
           dilepmass = (myEvent.leadingLepton + myEvent.secondLepton).M();
          }

          pdgchannel=0;
          if (myEvent.numberOfLepton==1) {
            if (abs(myEvent.leadingLeptonPDGId) == 11) pdgchannel=1;
            else if (abs(myEvent.leadingLeptonPDGId) == 13) pdgchannel=2;
          }
          else if (myEvent.numberOfLepton==2) {
            if (abs(myEvent.leadingLeptonPDGId) == 11 && abs(myEvent.secondLeptonPDGId) == 11) pdgchannel=3;
            else if (abs(myEvent.leadingLeptonPDGId) == 13 && abs(myEvent.secondLeptonPDGId) == 13) pdgchannel=4;
            else pdgchannel=5;
          }

             bdt1=myEvent.BDT_output_t2tt_R1;
             bdt2=myEvent.BDT_output_t2tt_R2;
             bdt3=myEvent.BDT_output_t2tt_R5;
             bdt4=myEvent.BDT_output_t2bw075_R1;
             bdt5=myEvent.BDT_output_t2bw075_R2;
             bdt6=myEvent.BDT_output_t2bw075_R3;
             bdt7=myEvent.BDT_output_t2bw075_R5;
             bdt8=myEvent.BDT_output_t2bw050_R1;
             bdt9=myEvent.BDT_output_t2bw050_R3;
             bdt10=myEvent.BDT_output_t2bw050_R4;
             bdt11=myEvent.BDT_output_t2bw050_R5;
             bdt12=myEvent.BDT_output_t2bw050_R6;
             bdt13=myEvent.BDT_output_t2bw025_R1;
             bdt14=myEvent.BDT_output_t2bw025_R3;
             bdt15=myEvent.BDT_output_t2bw025_R4;
             bdt16=myEvent.BDT_output_t2bw025_R6;

           if ((myEvent.numberOfGenLepton == 1) && myEvent.MT>MT_CUT && (currentProcessClass_ == "1ltop" || currentProcessClass_ == "W+jets")) weight*=SFR_Wjets.value();

          screwdriver.AutoFillProcessClass(currentProcessClass_,weight);

      } 
      printProgressBar(nEntries,nEntries,currentDataset);
      cout << endl;
      f.Close();

  }

  // #############################
  // ##   Apply scale factors   ##
  // #############################


  Figure SF_pre     = scaleFactors.Get("value","SF_pre");
  Figure SF_post    = scaleFactors.Get("value","SF_post");
  Figure SF_0btag   = scaleFactors.Get("value","SF_0btag");

  //TableDataMC(&screwdriver,{"MTpeak" },"singleLepton").Print();

  screwdriver.ApplyScaleFactor("ttbar_2l","MTpeak","singleLepton",SF_pre);
  screwdriver.ApplyScaleFactor("1ltop",   "MTpeak","singleLepton",SF_post);
  screwdriver.ApplyScaleFactor("W+jets",  "MTpeak","singleLepton",SF_post);

  screwdriver.ApplyScaleFactor("W+jets","0btag","singleLepton",SF_0btag);
  screwdriver.ApplyScaleFactor("1ltop", "0btag","singleLepton",SF_0btag);

// to be commented if de-commented above  if (myEvent.MT>MT_CUT && (currentProcessClass_ == "ttbar_powheg" || ....
//  screwdriver.ApplyScaleFactor("W+jets","0btag","singleLepton",SFR_Wjets); 
//  screwdriver.ApplyScaleFactor("1ltop", "0btag","singleLepton",SFR_Wjets);  /// Caro le rajoute :)
 

  if (CR45) {
  Table scaleFactors_2leptons = Table("scaleFactors/"+signalRegionLabel+"_2leptons.tab");
  Figure SF_2l = scaleFactors_2leptons.Get("value","SF2lpeak");

  screwdriver.ApplyScaleFactor("ttbar_2l","2leptons","doubleLepton",SF_2l);
  screwdriver.ApplyScaleFactor("1ltop","2leptons","doubleLepton",SF_2l);
  screwdriver.ApplyScaleFactor("W+jets","2leptons","doubleLepton",SF_2l);

  Figure SF_2lrem = scaleFactors_2leptons.Get("value","SF2ltail");
  //  screwdriver.ApplyScaleFactor("ttbar_2l","2lepMTtail","doubleLepton",SF_2l*SF_2lrem);
  screwdriver.ApplyScaleFactor("ttbar_2l","2lepMTtail","doubleLepton",SF_2l);  // sans normalization suppl sur ttbar2l
  screwdriver.ApplyScaleFactor("1ltop","2lepMTtail","doubleLepton",SF_2l);
  screwdriver.ApplyScaleFactor("W+jets","2lepMTtail","doubleLepton",SF_2l);

  Figure SF_veto = scaleFactors_2leptons.Get("value","SFvetopeak");
  screwdriver.ApplyScaleFactor("ttbar_2l","antiveto","singleLepton",SF_pre);
  screwdriver.ApplyScaleFactor("1ltop","antiveto","singleLepton",SF_veto);
  screwdriver.ApplyScaleFactor("W+jets","antiveto","singleLepton",SF_veto);
          cout << "SF_pre " << SF_pre.value() << endl;
          cout << "SF_veto " << SF_veto.value() << endl;

  Figure SF_verem = scaleFactors_2leptons.Get("value","SFvetotail");
  //  screwdriver.ApplyScaleFactor("ttbar_2l","antivMTtail","singleLepton",SF_veto*SF_verem);
  screwdriver.ApplyScaleFactor("ttbar_2l","antivMTtail","singleLepton",SF_pre); // sans normalization suppl sur ttbar2l
  screwdriver.ApplyScaleFactor("1ltop","antivMTtail","singleLepton",SF_veto);
  screwdriver.ApplyScaleFactor("W+jets","antivMTtail","singleLepton",SF_veto);
  }
  else {
  screwdriver.ApplyScaleFactor("ttbar_2l","sr_fullMT","singleLepton",SF_pre);
  screwdriver.ApplyScaleFactor("1ltop",   "sr_fullMT","singleLepton",SF_post);
  screwdriver.ApplyScaleFactor("W+jets",  "sr_fullMT","singleLepton",SF_post);

  screwdriver.ApplyScaleFactor("W+jets","0btag_fullMT","singleLepton",SF_0btag);
  screwdriver.ApplyScaleFactor("1ltop", "0btag_fullMT","singleLepton",SF_0btag);
  
  // need to manually applied it above if (myEvent.MT>MT_CUT && (currentProcessClass_ == "ttbar_powheg" || ....
//  screwdriver.ApplyScaleFactor("W+jets","0btag_fullMT","singleLepton",SFR_Wjets);
//  screwdriver.ApplyScaleFactor("1ltop", "0btag_fullMT","singleLepton",SFR_Wjets);  ///  Manually applied above

  }


  //TableDataMC(&screwdriver,{"MTpeak" },"singleLepton").Print();

  // ###################################
  // ##   Make plots and write them   ##
  // ###################################
 
  cout << endl;
  cout << "   > Making plots..." << endl;
  screwdriver.MakePlots();

  //--- Looking quantiles for all the BDTs
  float quantile = 0.90;
  int MinNof = 30;
  vector<string> vars = {"BDT1","BDT2","BDT3","BDT4","BDT5","BDT6","BDT7","BDT8","BDT9","BDT10",
  			 "BDT11","BDT12","BDT13","BDT14","BDT15","BDT16"};
  vector<string> processClasses = {"1ltop","ttbar_2l","W+jets","rare"};
  string channel = "doubleLepton";
  
  string region = "2leptons";
  cout<<"#########################################"<<endl;
  cout<<"## Cut for MinNof = "<<MinNof<<endl;
  cout<<"#########################################"<<endl;
  for(unsigned int i=0;i<vars.size();i++){
  	//float BDTCut = QuantileVariabelCut(screwdriver, quantile, vars[i], processClasses, region, channel);
  	float BDTCut = MinNofEvtsVariabelCut(screwdriver, MinNof, vars[i], processClasses, region, channel);
  	cout<<"# var: "<<vars[i]<<"\t"<<BDTCut<<endl;
  }
  cout<<"#########################################"<<endl;
  region = "antiveto";
  for(unsigned int i=0;i<vars.size();i++){
  	//float BDTCut = QuantileVariabelCut(screwdriver, quantile, vars[i], processClasses, region, channel);
  	float BDTCut = MinNofEvtsVariabelCut(screwdriver, MinNof, vars[i], processClasses, region, channel);
  	cout<<"# var: "<<vars[i]<<"\t"<<BDTCut<<endl;
  }
  cout<<"#########################################"<<endl;

  MinNof = 50;
  region = "2leptons";
  cout<<"#########################################"<<endl;
  cout<<"## Cut for MinNof = "<<MinNof<<endl;
  cout<<"#########################################"<<endl;
  for(unsigned int i=0;i<vars.size();i++){
  	//float BDTCut = QuantileVariabelCut(screwdriver, quantile, vars[i], processClasses, region, channel);
  	float BDTCut = MinNofEvtsVariabelCut(screwdriver, MinNof, vars[i], processClasses, region, channel);
  	cout<<"# var: "<<vars[i]<<"\t"<<BDTCut<<endl;
  }
  cout<<"#########################################"<<endl;
  region = "antiveto";
  for(unsigned int i=0;i<vars.size();i++){
  	//float BDTCut = QuantileVariabelCut(screwdriver, quantile, vars[i], processClasses, region, channel);
  	float BDTCut = MinNofEvtsVariabelCut(screwdriver, MinNof, vars[i], processClasses, region, channel);
  	cout<<"# var: "<<vars[i]<<"\t"<<BDTCut<<endl;
  }
  cout<<"#########################################"<<endl;
  
  MinNof = 100;
  region = "2leptons";
  cout<<"#########################################"<<endl;
  cout<<"## Cut for MinNof = "<<MinNof<<endl;
  cout<<"#########################################"<<endl;
  for(unsigned int i=0;i<vars.size();i++){
  	//float BDTCut = QuantileVariabelCut(screwdriver, quantile, vars[i], processClasses, region, channel);
  	float BDTCut = MinNofEvtsVariabelCut(screwdriver, MinNof, vars[i], processClasses, region, channel);
  	cout<<"# var: "<<vars[i]<<"\t"<<BDTCut<<endl;
  }
  cout<<"#########################################"<<endl;
  region = "antiveto";
  for(unsigned int i=0;i<vars.size();i++){
  	//float BDTCut = QuantileVariabelCut(screwdriver, quantile, vars[i], processClasses, region, channel);
  	float BDTCut = MinNofEvtsVariabelCut(screwdriver, MinNof, vars[i], processClasses, region, channel);
  	cout<<"# var: "<<vars[i]<<"\t"<<BDTCut<<endl;
  }
  cout<<"#########################################"<<endl;
  
  MinNof = 150;
  region = "2leptons";
  cout<<"#########################################"<<endl;
  cout<<"## Cut for MinNof = "<<MinNof<<endl;
  cout<<"#########################################"<<endl;
  for(unsigned int i=0;i<vars.size();i++){
  	//float BDTCut = QuantileVariabelCut(screwdriver, quantile, vars[i], processClasses, region, channel);
  	float BDTCut = MinNofEvtsVariabelCut(screwdriver, MinNof, vars[i], processClasses, region, channel);
  	cout<<"# var: "<<vars[i]<<"\t"<<BDTCut<<endl;
  }
  cout<<"#########################################"<<endl;
  region = "antiveto";
  for(unsigned int i=0;i<vars.size();i++){
  	//float BDTCut = QuantileVariabelCut(screwdriver, quantile, vars[i], processClasses, region, channel);
  	float BDTCut = MinNofEvtsVariabelCut(screwdriver, MinNof, vars[i], processClasses, region, channel);
  	cout<<"# var: "<<vars[i]<<"\t"<<BDTCut<<endl;
  }
  cout<<"#########################################"<<endl;
  
  cout << "   > Saving plots..." << endl;
  //screwdriver.WritePlots(string("./controlPlotsDataMC/")+SIGNAL_REGION_TAG);
  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################
  
  printBoxedMessage("Program done.");
  return (0);
}


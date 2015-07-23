//#define USE_GEN_INFO
//#define USE_GEN_INFO_EXT
#define USE_LEP1
//#define USE_LEP1_EXT
#define USE_LEP2
//#define USE_LEP2_EXT
#define USE_JETS
//#define USE_JETS_EXT
//#define USE_PV
//#define USE_WEIGHTS
#define USE_VAR_BASELINE
//#define USE_GLOBAL_VAR
//#define USE_OLD_VAR
#define USE_SKIMMING_VAR

#include <algorithm>

#include "../common/common.h"
//#include "topness_tt2l_mc_reso.h"
#include "topness/topness2.h"

#define BDISCRI_CUT 0.814
#define topness_computation

//#define FOLDER_BABYTUPLES_SKIM "/opt/sbg/data/data1/cms/echabert/Stop2015/BabyTuples/v1_30_06/skimming2/"
#define FOLDER_BABYTUPLES_SKIM "/opt/sbg/data/data1/cms/echabert/Stop2015/BabyTuples/v1_20_07/skimming/"
//#define MC_INFO_CHECK


bool goesInAnyChannel()                             { return (goesInSingleLeptonChannel() || goesInDoubleLeptonChannel());                  }

bool sort_discri(pair<float,int> i, pair<float,int> j){ return (i.first>j.first);}

float CallTopness(int indice1, int indice2, double& lost_W_pt, double& lost_W_pz, double& nu_pz, double &tWL, double &tTL, double& tTM){
		        //cout<<myEvent.pfmet<<endl;
			float tmptop = topness2Minimization(
	     			myEvent.lep1_pt, myEvent.lep1_eta, myEvent.lep1_phi, myEvent.lep1_mass,
	   			myEvent.jet_pt[indice1], myEvent.jet_eta[indice1], myEvent.jet_phi[indice1], myEvent.jet_mass[indice1],
	   			myEvent.jet_pt[indice2], myEvent.jet_eta[indice2], myEvent.jet_phi[indice2], myEvent.jet_mass[indice2],
	    			myEvent.pfmet, myEvent.pfmet_phi,
  				lost_W_pt, lost_W_pz, nu_pz,
				tWL, tTL, tTM);
			return tmptop;
}

float ComputeTopness(int& index_b1, int& index_b2, double &tWL, double &tTL, double& tTM){
       
        // check that we have at least 2 jets and 1 leading lepton
	if(myEvent.ngoodjets<2 || myEvent.lep1_pt==0) return -999;
	
	// order the jets in b-tagging (1st == most b-tagged)
	vector<pair<float,int> > jet_order(myEvent.ngoodjets);
	//first element = discri - second element = index
	for(unsigned int i=0;i<myEvent.jet_CSV.size();i++){
		jet_order[i] = pair<float,int>(myEvent.jet_CSV[i],(int)i);
	}
	// Sort the vector
	std::sort (jet_order.begin(), jet_order.end(), sort_discri);
	
	vector<int> jet_btag_index;
	vector<int> jet_nonbtag_index;
	int max_comb = 3;
	for(unsigned int i=0;i<jet_order.size();i++){
		// jets b-tagged
		if(jet_order[i].first>BDISCRI_CUT)
			jet_btag_index.push_back(jet_order[i].second);
		// non b-tagged
		else
		  if (jet_btag_index.size()<2 && jet_btag_index.size()+jet_nonbtag_index.size()<max_comb)
			jet_nonbtag_index.push_back(jet_order[i].second);
	}
  
 	//new
	if(jet_btag_index.size()>=2) max_comb = 3;
	else max_comb = 4;
	//

	double lost_W_pt_min;
	double lost_W_pz_min;
	double nu_pz_min;
	double topness_min = 1000;

	double lost_W_pt;
	double lost_W_pz;
	double nu_pz;
	
	//test - we only look for the most max b-tagged jets (even if its passes the threshold or not)
	    for(unsigned int i=0;i<max_comb;i++){
	    //for(unsigned int i=0;i<jet_order.size();i++){
		    //for(unsigned int j=i+1;j<jet_order.size();j++){
		    for(unsigned int j=i+1;j<max_comb;j++){
		    	int indice1 = jet_order[i].second;
		    	int indice2 = jet_order[j].second;
			float tmptop = CallTopness(indice1, indice2, lost_W_pt, lost_W_pz, nu_pz, tWL, tTL, tTM);
			//check if it is the min value
		   	if(tmptop<topness_min){
		   	 	topness_min = tmptop;
				lost_W_pt_min = lost_W_pt;
				lost_W_pz_min = lost_W_pz;
				nu_pz_min = nu_pz;
				index_b1 = indice1 ; index_b2 = indice2;
		    	}
			// Autre permutation
			tmptop = CallTopness(indice2, indice1, lost_W_pt, lost_W_pz, nu_pz, tWL, tTL, tTM);
			//check if it is the min value
		   	if(tmptop<topness_min){
		   	 	topness_min = tmptop;
				lost_W_pt_min = lost_W_pt;
				lost_W_pz_min = lost_W_pz;
				nu_pz_min = nu_pz;
				index_b1 = indice1 ; index_b2 = indice2;
		    	}
		    }
	    }
/*
	if (jet_btag_index.size()>0){
    	    for(unsigned int i=0;i<jet_btag_index.size();i++){
		    for(unsigned int j=i+1;j<jet_btag_index.size();j++){
		    	int indice1 = jet_btag_index[i];
		    	int indice2 = jet_btag_index[j];
			float tmptop = CallTopness(indice1, indice2, lost_W_pt, lost_W_pz, nu_pz, tWL, tTL, tTM);
			//check if it is the min value
		   	if(tmptop<topness_min){
		   	 	topness_min = tmptop;
				lost_W_pt_min = lost_W_pt;
				lost_W_pz_min = lost_W_pz;
				nu_pz_min = nu_pz;
				index_b1 = indice1 ; index_b2 = indice2;
		    	}
			// Autre permutation
			tmptop = CallTopness(indice2, indice1, lost_W_pt, lost_W_pz, nu_pz, tWL, tTL, tTM);
			//check if it is the min value
		   	if(tmptop<topness_min){
		   	 	topness_min = tmptop;
				lost_W_pt_min = lost_W_pt;
				lost_W_pz_min = lost_W_pz;
				nu_pz_min = nu_pz;
				index_b1 = indice1 ; index_b2 = indice2;
		    	}
		    }
		    //new - yrdy
		    if(jet_btag_index.size()==1)
		    for(unsigned int j=i+1;j<jet_nonbtag_index.size();j++){
		    	int indice1 = jet_btag_index[i];
		    	int indice2 = jet_nonbtag_index[j];  
			float tmptop = CallTopness(indice1, indice2, lost_W_pt, lost_W_pz, nu_pz, tWL, tTL, tTM);
			//check if it is the min value
		   	if(tmptop<topness_min){
		   	 	topness_min = tmptop;
				lost_W_pt_min = lost_W_pt;
				lost_W_pz_min = lost_W_pz;
				nu_pz_min = nu_pz;
				index_b1 = indice1 ; index_b2 = indice2;
		    	}
			// Autre permutation
			tmptop = CallTopness(indice1, indice2, lost_W_pt, lost_W_pz, nu_pz, tWL, tTL, tTM);
			//check if it is the min value
		   	if(tmptop<topness_min){
		   	 	topness_min = tmptop;
				lost_W_pt_min = lost_W_pt;
				lost_W_pz_min = lost_W_pz;
				nu_pz_min = nu_pz;
				index_b1 = indice1 ; index_b2 = indice2;
		    	}
		    }
	     }
	}
	else if (jet_nonbtag_index.size()>=2){
	     //new - try to limit combinatorics
	     int max = jet_nonbtag_index.size();
	     //if(max>4) max = 4;
	     //if(max>3) max = 3;
	     //for(unsigned int i=0;i<max;i++){	
	       // new
	       //int indice0 = jet_nonbtag_index[i];
	       int indice0 = 0;
	       for(unsigned int j=1;j<max;j++){	
	       //for(unsigned int j=i+1;j<max;j++){	
	     	    //int indice = jet_nonbtag_index[i];
	     	    int indice = jet_nonbtag_index[j];
		    // take the most leading jet
		    //float tmptop = CallTopness(0, indice, lost_W_pt, lost_W_pz, nu_pz, tWL, tTL, tTM);
		    float tmptop = CallTopness(indice0, indice, lost_W_pt, lost_W_pz, nu_pz, tWL, tTL, tTM);
	      	    //check if it is the min value
		    if(tmptop<topness_min){
		   	 	topness_min = tmptop;
				lost_W_pt_min = lost_W_pt;
				lost_W_pz_min = lost_W_pz;
				nu_pz_min = nu_pz;
				//index_b1 = 0; index_b2 = indice;
				index_b1 = indice0; index_b2 = indice;
		    }
	  	    // Autre permutation
		    //tmptop = CallTopness(indice, 0, lost_W_pt, lost_W_pz, nu_pz, tWL, tTL, tTM);
		    tmptop = CallTopness(indice, indice0, lost_W_pt, lost_W_pz, nu_pz, tWL, tTL, tTM);
		    //check if it is the min value
		    if(tmptop<topness_min){
		   	 	topness_min = tmptop;
				lost_W_pt_min = lost_W_pt;
				lost_W_pz_min = lost_W_pz;
				nu_pz_min = nu_pz;
				//index_b1 = 0; index_b2 = indice;
				index_b1 = indice; index_b2 = indice0;
		    }
		//}//new loop
	      }
	}
	*/
 	// Check the agreement !!!
	// not yet implemented !!
	// Return topness value
	return topness_min;



}

bool TopnessMatched(int index_b1, int index_b2){
	if(myEvent.matched){
	  if(index_b1 == myEvent.index_b && index_b2 == myEvent.index_bbar) return true;
	  if(index_b2 == myEvent.index_b && index_b1 == myEvent.index_bbar) return true;
	}
	return false;
}

// #########################################################################
//                              Main function
// #########################################################################

int main (int argc, char *argv[])
{
    //loadBDTSignalRegions();

    if(argc == 2){
    	cout<<"load"<<endl;
	default_param2(topness2_param_);
	LoadParam2(string(argv[1]));
    }
    else{
    	cout<<"You shoul provide the name of the config file !"<<endl;
    	return -1;
    }
    printBoxedMessage("Starting tables generation");

    // ####################
    // ##   Init tools   ##
    // ####################

        // Create a sonic Screwdriver
        SonicScrewdriver screwdriver;
     
     
     // ##########################
     // ##   Create Variables   ##
     // ##########################
  
   
     vector<string> colTags = {"default","resMass", "aCM", "allTerms","allTermsBias"};
     vector<string> colLabels = {"default","reso mass", "aCM terms", "all terms", "id + bias"};
     vector<string> rowTags = {"purity"};
     vector<string> rowLabels = {"purity"};
     Table table_topness_purity(colTags,rowTags,colLabels,rowLabels);
     int n_tt2l_sel = 0;
     int n_tt2l_sel_matchable = 0;

     int n_topness_var = 5;
     vector< vector< int > > topness_purity;
     topness_purity.resize(n_topness_var);
     for(int i=0;i<n_topness_var;i++) topness_purity[i].resize(2);

     //tt2l_mc_info mcinfo;
     //tt2l_reso res;
    

     float d_met;
     float topness_def;
     float topness_m1; // change the resolution term for Invis.
     float topness_m2; // change the resolution term for aCM
     float topness_m3; // change the resolution term for all
     float topness_m4; // change the resolution term for all
     float topness_m5; // m3 + correct for MET bias
     float topness_m6; // m3 + correct for MET bias
     float topness_m7; // m3 + correct for MET bias
     float topness_m8; // m3 + correct for MET bias
     float topness_m9; // m3 + correct for MET bias
     float topness_m10; // m3 + correct for MET bias
     float topness_m11; // m3 + correct for MET bias
     float topness_m12; // m3 + correct for MET bias
     float topness_m13; // m3 + correct for MET bias

     screwdriver.AddVariable("MT2W",             "MT2W",                   "GeV",    40,0,400,       &(myEvent.MT2W),                   "");

     //screwdriver.AddVariable("lostLeptonEta",             "Eta of lost lepton",            "",    40, -6, 6,     &(res.lost_eta),                   "");

     // topness
     screwdriver.AddVariable("topness",             "topness - reference",            "",    80, -5, 15,     &(topness_def),                   "");
     /*
     screwdriver.AddVariable("topness_m12",          "topness - resolution modified",  "",    40, -20, 20,     &(topness_m12),                   "");
     screwdriver.AddVariable("topness_m13",          "topness - resolution modified",  "",    40, -20, 20,     &(topness_m13),                   "");
*/
//int index_b;	//
	//int index_bbar; //
//	vector<int> index_isr;



    // #########################################################
    // ##   Create ProcessClasses (and associated datasets)   ##
    // #########################################################
	
	screwdriver.AddProcessClass("ttbar_1l", "t#bar{t} #rightarrow l+jets",                             "background",kRed-7);
                screwdriver.AddDataset("ttbar-madgraph",                "ttbar_1l",  0, 0);


        screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",kCyan-3);
///*	
	screwdriver.AddProcessClass("single_top", "single top",                             "background",kRed-5);
                screwdriver.AddDataset("singleTopbar_s",                "single_top",  0, 0);
                screwdriver.AddDataset("singleTopbar_t",                "single_top",  0, 0);
                screwdriver.AddDataset("singleTop_s",                "single_top",  0, 0);
                screwdriver.AddDataset("singleTop_t",                "single_top",  0, 0);
	 
        screwdriver.AddProcessClass("W+jets",   "W+jets",                          "background",kOrange-2);
            screwdriver.AddDataset("Wjets", "W+jets", 0, 0);
        screwdriver.AddProcessClass("VV",   "di-boson",                          "background",kOrange-3);
            //screwdriver.AddDataset("ZZ", "ZZ", 0, 0);
            screwdriver.AddDataset("WZ", "WZ", 0, 0);
       
       screwdriver.AddProcessClass("ttV",   "ttV",                          "background",kOrange-4);
            screwdriver.AddDataset("ttW", "ttV", 0, 0);
            screwdriver.AddDataset("ttZ", "ttV", 0, 0);
//	*/
	screwdriver.AddProcessClass("T2tt_850_100",   "T2tt_850_100",                          "signal",kGreen+2);
            screwdriver.AddDataset("T2tt_850_100", "T2tt_850_100", 0, 0);
	screwdriver.AddProcessClass("T2tt_650_100",   "T2tt_650_100",                          "signal",kGreen+3);
           screwdriver.AddDataset("T2tt_650_100", "T2tt_650_100", 0, 0);
       // screwdriver.AddProcessClass("T2tt_500_325",   "T2tt_500_325",                          "signal",kGreen+4);
         //   screwdriver.AddDataset("T2tt_500_325", "T2tt_500_325", 0, 0);


    // ##########################
    // ##    Create Regions    ##
    // ##########################

        //screwdriver.AddRegion("presel_MTpeak",          "Preselection (MT peak)",      &goesInMTpeak);
        //screwdriver.AddRegion("presel_MTtail",          "Preselection (MT peak)",      &goesInMTtail);
        screwdriver.AddRegion("baseline",          "Baseline selection",      &goesInBaselineSearchSR);
        screwdriver.AddRegion("baslineLargeDM",          "Baseline S.R.  - Large #DeltaM",      &goesInLargeDMSR);
        screwdriver.AddRegion("baselineSmallDM",          "Baseline S.R. - Small #DeltaM",      &goesInSmallDMSR);
        screwdriver.AddRegion("baseline2b",          "Baseline selection",      &goesInBaselineSearchSR2b);
        screwdriver.AddRegion("baselineSmallDM2b",          "Baseline S.R. - Small #DeltaM",      &goesInSmallDMSR2b);


    // ##########################
    // ##   Create Channels    ##
    // ##########################

        //screwdriver.AddChannel("singleLepton", "e/#mu-channels",  &goesInSingleLeptonChannel);
        //screwdriver.AddChannel("singleElec",   "e-channel",       &goesInSingleElecChannel  );
        //screwdriver.AddChannel("singleMuon",   "#mu-channel",     &goesInSingleMuonChannel  );
        screwdriver.AddChannel("allChannels",  "",                &goesInAnyChannel         );

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     // Create histograms
     screwdriver.Create1DHistos();
     
     // Create 2D histos
    
    
    // #########################################################

     // Schedule plots
     screwdriver.SchedulePlots("1DDataMCComparison");
     screwdriver.SchedulePlots("1DStack");
     screwdriver.SchedulePlots("1DSuperimposed");
     //screwdriver.SchedulePlots("2D");


     // Config plots

     screwdriver.SetGlobalStringOption("DataMCComparison",  "includeSignal",                    "stack");
     screwdriver.SetGlobalFloatOption ("DataMCComparison",  "factorSignal",                     1.0    );
     
     screwdriver.SetGlobalStringOption("1DStack",           "includeSignal",    "superimposed");
     screwdriver.SetGlobalFloatOption ("1DStack",           "factorSignal",     1.0    );

     screwdriver.SetGlobalStringOption("Plot", "infoTopRight", "CMS Preliminary");
     screwdriver.SetGlobalStringOption("Plot", "infoTopLeft",  "#sqrt{s} = 8 TeV, L = 10 fb^{-1}");

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

        for (unsigned int d = 0 ; d < datasetsList.size() ; d++)
        {
            string currentDataset = datasetsList[d];
            string currentProcessClass = screwdriver.GetProcessClass(currentDataset);

            sampleName = currentDataset;
            sampleType = screwdriver.GetProcessClassType(currentProcessClass);

            // Open the tree
            string treePath = string(FOLDER_BABYTUPLES_SKIM)+currentDataset+".root";
            TFile f(treePath.c_str());
            TTree* theTree = (TTree*) f.Get("babyTuple");

	    //cout<<"v1"<<endl;
            InitializeBranchesForReading(theTree,&myEvent);
	    //cout<<"v2"<<endl;

        // ########################################
        // ##        Run over the events         ##
        // ########################################

            bool ttbarDatasetToBeSplitted = false;
            if (findSubstring(currentDataset,"ttbar")
            && (currentDataset != "ttbar_madgraph_1l")
            && (currentDataset != "ttbar_madgraph_2l"))
                ttbarDatasetToBeSplitted = true;

            int nEntries = theTree->GetEntries();
            //if(nEntries>10000000) nEntries = 10000000;
            //if(nEntries>5000000) nEntries = 5000000;
            //if(nEntries>500000) nEntries = 500000;
	    double tWL, tTL, tTM;
	    float sf_fracEvent = (float)nEntries/theTree->GetEntries();
	    for (int i = 0 ; i < nEntries ; i++)
            {
                if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

		// Get the i-th entry
		ReadEvent(theTree,i,&myEvent);

                string currentProcessClass_ = currentProcessClass;
		float weight = getWeight(currentDataset,theTree->GetEntries(), sf_fracEvent);

		
		//Compute Delta MET
		d_met = myEvent.pfmet-myEvent.genMET;

		//Topness study
		bool is_tt2l = false;
		if(myEvent.ttbar_decay == 2){
			is_tt2l = true; 
			n_tt2l_sel++;
			if(myEvent.matched == true) n_tt2l_sel_matchable++;
		}
			
			#ifdef topness_computation

		        int index_b1 = -1;
			int index_b2 = -1;
			//cout<<topness_param_.use_cox_box<<endl;
			//cout<<topness_param_.aW<<endl;	
			//default_param(topness_param_);
			//DumpParam();

			topness_def = ComputeTopness(index_b1,index_b2, tWL, tTL, tTM);
			//cout<<"topness = "<<topness_def<<endl;
			if(TopnessMatched(index_b1, index_b2)) topness_purity[0][0]++;
			// changing the resolution terms
			/*
			// default computation
			// -- load default parameter s
			default_param(topness_param_);
			topness_def = ComputeTopness(index_b1,index_b2);
			if(TopnessMatched(index_b1, index_b2)) topness_purity[0][0]++;
			// changing the resolution terms
			default_param(topness_param_);
			topness_param_.aW = 40;
			topness_param_.aT = 50;
			
			topness_m1 = ComputeTopness(index_b1,index_b2);
			if(TopnessMatched(index_b1, index_b2)) topness_purity[0][1]++;			
			//
			default_param(topness_param_);
			topness_param_.aCM = 70;
			topness_param_.aCM_mean = 500;
			
			topness_m2 = ComputeTopness(index_b1,index_b2);
			if(TopnessMatched(index_b1, index_b2)) topness_purity[0][2]++;			
			
			//
			default_param(topness_param_);
			topness_param_.aW = 40;
			topness_param_.aT = 50;
			topness_param_.aCM = 200;
			topness_param_.aCM_mean = 650;
			
			topness_m3 = ComputeTopness(index_b1,index_b2);
			if(TopnessMatched(index_b1, index_b2)) topness_purity[0][3]++;			
			
			//
			default_param(topness_param_);
			topness_param_.aCM = 250;
			topness_param_.aCM_mean = 670;
			
			topness_m4 = ComputeTopness(index_b1,index_b2);
			if(TopnessMatched(index_b1, index_b2)) topness_purity[0][4]++;			
			
			
			//correct for MET bias
			topness_param_.aW = 40;
			topness_param_.aT = 50;
			topness_param_.aCM = 200;
			topness_param_.aCM_mean =670;
			topness_m7 = ComputeTopness(index_b1,index_b2);
			//if(TopnessMatched(index_b1, index_b2)) topness_purity[0][5]++;			
		
			//correct for MET bias
			//if(myEvent.pfmet>80) myEvent.pfmet = myEvent.pfmet - 50;
			default_param(topness_param_);
			topness_param_.aW = 80;
			topness_param_.aT = 100;
			topness_param_.aCM = 200;
			topness_param_.aCM_mean = 670;
			topness_m8 = ComputeTopness(index_b1,index_b2);
			//if(TopnessMatched(index_b1, index_b2)) topness_purity[0][5]++;			
			
			//correct for MET bias
			//if(myEvent.pfmet>80) myEvent.pfmet = myEvent.pfmet - 50;
			default_param(topness_param_);
			//topness_param_.aW = 80;
			//topness_param_.aT = 100;
			topness_param_.aCM = 65;
			topness_param_.aCM_mean = 650;
			topness_param_.use_cox_box = true;
			topness_m9 = ComputeTopness(index_b1,index_b2);
			//if(TopnessMatched(index_b1, index_b2)) topness_purity[0][5]++;		
			
			
			//correct for MET bias
			//if(myEvent.pfmet>80) myEvent.pfmet = myEvent.pfmet - 50;
			default_param(topness_param_);
			topness_param_.aW = 40;
			topness_param_.aT = 50;
			topness_param_.aCM = 65;
			topness_param_.aCM_mean = 650;
			topness_param_.use_cox_box = true;
			topness_m10 = ComputeTopness(index_b1,index_b2);
			
			
			//correct for MET bias
			//if(myEvent.pfmet>80) myEvent.pfmet = myEvent.pfmet - 50;
			default_param(topness_param_);
			topness_param_.aW = 40;
			topness_param_.aT = 50;
			topness_param_.aCM = 80;
			topness_param_.aCM_mean = 800;
			topness_param_.use_cox_box = true;
			topness_m11 = ComputeTopness(index_b1,index_b2);

			//correct for MET bias
			//if(myEvent.pfmet>80) myEvent.pfmet = myEvent.pfmet - 50;
			default_param(topness_param_);
			//topness_param_.aW = 40;
			//topness_param_.aT = 50;
			//topness_param_.aCM = 80;
			//topness_param_.aCM_mean = 800;
			//topness_param_.use_cox_box = true;
			topness_param_.usePtPz = true;
			topness_m12 = ComputeTopness(index_b1,index_b2);
*/
			//correct for MET bias
			//if(myEvent.pfmet>80) myEvent.pfmet = myEvent.pfmet - 50;
			/*
			default_param(topness_param_);
			topness_param_.aW = 40;
			topness_param_.aT = 50;
			//topness_param_.aCM = 80;
			//topness_param_.aCM_mean = 800;
			//topness_param_.use_cox_box = true;
			topness_param_.usePtPz = true;
			topness_m13 = ComputeTopness(index_b1,index_b2);
*/
/*

			//correct for MET bias
			//if(myEvent.pfmet>80) myEvent.pfmet = myEvent.pfmet - 50;
			default_param(topness_param_);
			topness_param_.aW = 40;
			topness_param_.aT = 50;
			topness_param_.aCM = 350;
			topness_param_.aCM_mean = 850;
			topness_m6 = ComputeTopness(index_b1,index_b2);
			//if(TopnessMatched(index_b1, index_b2, res)) topness_purity[0][5]++;			
			
			//correct for MET bias
			//cout<<myEvent.pfmet<<endl;
			//if(myEvent.pfmet>80) myEvent.pfmet = myEvent.pfmet - 50;
			if(myEvent.pfmet>80) myEvent.pfmet = myEvent.pfmet*0.70;// - 50;
			//cout<<myEvent.pfmet<<endl;
			default_param(topness_param_);
			topness_param_.aW = 40;
			topness_param_.aT = 50;
			topness_param_.aCM = 300;
			topness_param_.aCM_mean = 650;
			topness_m5 = ComputeTopness(index_b1,index_b2);
			//if(TopnessMatched(index_b1, index_b2, res)) topness_purity[0][5]++;			
			*/
			#endif
                
		// Split 1-lepton ttbar and 2-lepton ttbar
                //if (ttbarDatasetToBeSplitted && (myEvent.genlepsfromtop == 2))
                if (ttbarDatasetToBeSplitted && is_tt2l)
                    currentProcessClass_ = "ttbar_2l";
                
		screwdriver.AutoFillProcessClass(currentProcessClass_,weight);

            }
            printProgressBar(nEntries,nEntries,currentDataset);
            cout << endl;
            f.Close();

        }
  
  // ###################################
  // ##   Make plots and write them   ##
  // ###################################

  cout << endl;
  cout << "   > Making plots..." << endl;
  screwdriver.MakePlots();
  cout << "   > Saving plots..." << endl;

  //string output_dir = %./plots/"+string(argv[1])+"/pow2/";
  string output_dir = "./plots/"+string(argv[1])+"/comb3_4/";
  //string output_dir = "./plots/"+string(argv[1])+"/pow2/loopnonb_all/";
  //string output_dir = "./plots/topness2/"+string(argv[1])+"/";
  //screwdriver.WritePlots(string("./plots/plotsProducer/"));
  screwdriver.WritePlots(output_dir);

  printBoxedMessage("Plot generation completed");
        
  printBoxedMessage("Writing the table ... ");
 
 vector<string> regions  = { 
 	"presel_MTtail", "baseline", "baslineLargeDM", "baselineSmallDM"
				    //"presel_MTpeak", "presel_MTtail",
				    /*
				    "preveto_MTpeak",      "preveto_MTtail",
                                    "signalRegion_MTpeak", "signalRegion_MTtail",
                                    "0btag_MTpeak",        "0btag_MTtail",
                                    "reversedVeto_MTpeak", "reversedVeto_MTtail",
                                    "2leptons", "2leptons_MTpeak", "2leptons_MTtail",
                                  */};

  //string exportFile = "rawYieldTables/prediction.tab";
  //TableDataMC(&screwdriver,regions,"allChannels").Print(exportFile,4);

  // #############################
  // ##   Post-plotting tests   ##
  // #############################
  

  //Table topness - purity
  for(int i=0;i<n_topness_var;i++){
  	table_topness_purity.Set(0,i, Figure(topness_purity[i][0],sqrt(topness_purity[i][0]))/Figure(n_tt2l_sel,sqrt(n_tt2l_sel)));
  	table_topness_purity.Set(1,i, Figure(topness_purity[i][0],sqrt(topness_purity[i][0]))/Figure(n_tt2l_sel_matchable,sqrt(n_tt2l_sel_matchable)));
  }
  table_topness_purity.Print(output_dir+"purity.dat");

  printBoxedMessage("Program done.");
/*
 string region = "baseline";
 string channel = "allChannels";
 string var;
 var = "topness_m5";
 //loop over the backgroumd
 vector<string> processClassList;
 //GetProcessClassTagList()
 screwdriver.GetProcessClassTagList(&processClassList);
 TH1F* h_Eff_Bkg = 0;
 TH1F* h_Eff_Sig = 0;
 bool first = true;
 for(unsigned int i=0;i<processClassList.size();i++){
 	string proc = processClassList[i];
 	if(scredrive.GetProcessClassType(proc) == "background"){
 		TH1F* h = screewdriver.Get1DHistoClone(var , processClass, region, channel);
 		if(first){
			h_Eff_Bkg = (TH1F*) h->Clone("");
			first = false;
		}
		for(int i=1;i<h->GetNBinsX();i++){
			h_Eff_Bkgh->Integral(i,h->GetNBinsX()+1);
		}
	}
 }
 */
	return (0);
}


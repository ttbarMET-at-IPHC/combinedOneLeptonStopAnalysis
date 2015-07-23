#define USE_GEN_INFO
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


#include "../common/common.h"

#include <algorithm>
#include "TLorentzVector.h"
#include "topness/topness.h"
#include "topness_tt2l_mc_reso.h"

#define BDISCRI_CUT 0.814
#define topness_computation

//#define FOLDER_BABYTUPLES_SKIM "/opt/sbg/data/data1/cms/echabert/Stop2015/BabyTuples/v1_30_06/skimming2/"
#define FOLDER_BABYTUPLES_SKIM "/opt/sbg/data/data1/cms/echabert/Stop2015/BabyTuples/v1_20_07/skimming/"
//#define MC_INFO_CHECK



//#######################################
//#######################################
// The goal is to produce a tree
// which will contain the MC info
// about the masses (gen-nu+reco objects)
// So one should run on ttbar sample only
// It will be usefull to determine the 
// resolution terms
// This code can also be used to determine
// the fraction of the several terms
// entering in the definition of topness
//#######################################
//######################################



bool PassFilter(TTree* theTree, long int i){
    //theTree->GetBranch("genlepsfromtop")->GetEntry(i);
    theTree->GetBranch("mt_met_lep")->GetEntry(i);
    theTree->GetBranch("pfmet")->GetEntry(i);
    theTree->GetBranch("ngoodleps")->GetEntry(i);
    theTree->GetBranch("ngoodjets")->GetEntry(i);
  //  return goesInPreselection(); 
    //cout<<"there"<< goesInBaselineSearchSR()<<endl;
    return goesInBaselineSearchSR();

}



bool goesInAnyChannel()                             { return (goesInSingleLeptonChannel() || goesInDoubleLeptonChannel());                  }

bool sort_discri(pair<float,int> i, pair<float,int> j){ return (i.first>j.first);}

float CallTopness(int indice1, int indice2, double& lost_W_pt, double& lost_W_pz, double& nu_pz, double &tWL, double &tTL, double& tTM){
		        //cout<<myEvent.pfmet<<endl;
			float tmptop = topnessMinimization(
	     			myEvent.lep1_pt, myEvent.lep1_eta, myEvent.lep1_phi, myEvent.lep1_mass,
	   			myEvent.jet_pt[indice1], myEvent.jet_eta[indice1], myEvent.jet_phi[indice1], myEvent.jet_mass[indice1],
	   			myEvent.jet_pt[indice2], myEvent.jet_eta[indice2], myEvent.jet_phi[indice2], myEvent.jet_mass[indice2],
	    			myEvent.pfmet, myEvent.pfmet_phi,
  				lost_W_pt, lost_W_pz, nu_pz,
				tWL, tTL, tTM);
			return tmptop;
}

float ComputeTopness(int& index_b1, int& index_b2, double &tWL, double &tTL, double& tTM){
	//cout<<"here! "<<myEvent.ngoodjets<<" "<< myEvent.lep1_pt<<endl;
	// check that we have at least 2 jets and 1 leading lepton
	if(myEvent.ngoodjets<2 || myEvent.lep1_pt==0) return -999;
	//cout<<"there!"<<endl;
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

	for(unsigned int i=0;i<jet_order.size();i++){
		// jets b-tagged
		if(jet_order[i].first>BDISCRI_CUT)
			jet_btag_index.push_back(jet_order[i].second);
		// non b-tagged
		else
			jet_nonbtag_index.push_back(jet_order[i].second);
	}
  

	double lost_W_pt_min;
	double lost_W_pz_min;
	double nu_pz_min;
	double topness_min = 1000;

	double lost_W_pt;
	double lost_W_pz;
	double nu_pz;

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
	     for(unsigned int i=0;i<jet_nonbtag_index.size();i++){	
	     	    int indice = jet_nonbtag_index[i];
		    //cout<<"indice ComputeTopness "<<indice<<endl;
		    // take the most leading jet
		    float tmptop = CallTopness(0, indice, lost_W_pt, lost_W_pz, nu_pz, tWL, tTL, tTM);
	      	    //check if it is the min value
		    if(tmptop<topness_min){
		   	 	topness_min = tmptop;
				lost_W_pt_min = lost_W_pt;
				lost_W_pz_min = lost_W_pz;
				nu_pz_min = nu_pz;
				index_b1 = 0; index_b2 = indice;
		    }
	  	    // Autre permutation
		    tmptop = CallTopness(indice, 0, lost_W_pt, lost_W_pz, nu_pz, tWL, tTL, tTM);
		    //check if it is the min value
		    if(tmptop<topness_min){
		   	 	topness_min = tmptop;
				lost_W_pt_min = lost_W_pt;
				lost_W_pz_min = lost_W_pz;
				nu_pz_min = nu_pz;
				index_b1 = 0; index_b2 = indice;
		    }
	      }
	}
	//cout<<"bf compute topness "<<index_b1<<" "<<index_b2<<endl;	
 	// Check the agreement !!!
	// not yet implemented !!
	// Return topness value
	return topness_min;



}

bool TopnessMatched(int index_b1, int index_b2, tt2l_reso& res){
	//cout<<index_b1<<" "<<index_b2<<" "<<res.index_b<<" "<<res.index_bbar<<endl;
	if(res.matched){
	  if(index_b1 == res.index_b && index_b2 == res.index_bbar) return true;
	  if(index_b2 == res.index_b && index_b1 == res.index_bbar) return true;
	}
	return false;
}

// #########################################################################
//                              Main function
// #########################################################################

int main (int argc, char *argv[])
{
    //loadBDTSignalRegions();

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

     tt2l_mc_info mcinfo;
     tt2l_reso res;
    

     float d_met;
     float topness_def;

     screwdriver.AddVariable("MET",            "MET",                  "GeV",    26,0,520,       &(myEvent.pfmet),                  "");
     screwdriver.AddVariable("MT",             "M_{T}",                   "GeV",    40,0,400,       &(myEvent.mt_met_lep),                   "");

     //Specific studu
     //DeltaPhi (nu,nu)
     //Pt of radiation
     //screwdriver.AddVariable("METReso",             "MET resolution",                 "GeV",    60,-600,600,       &(d_met),                   "");
     //screwdriver.AddVariable("HTReso",             "HT resolution",                   "GeV",    40,-400,400,       &(res.d_HT),                   "");
     //screwdriver.AddVariable("PzReso",             "Pz resolution",                   "GeV",    40,-400,400,       &(res.d_pz),                   "");
     //screwdriver.AddVariable("lostLeptonPt",             "Pt of lost lepton",            "",    40, 0, 200,     &(res.lost_pt),                   "");
     //screwdriver.AddVariable("lostLeptonEta",             "Eta of lost lepton",            "",    40, -6, 6,     &(res.lost_eta),                   "");
     //screwdriver.AddVariable("HT_visttbar",             "HT of 2b and lepton",            "",    40, 0, 2000,     &(res.HT),                   "");
     //screwdriver.AddVariable("Pz_visttbar",             "Pz of 2b and lepton",            "",    40, 0, 2000,     &(res.pz),                   "");
     //screwdriver.AddVariable("HT_all",             "HT all jets and leading lepton",            "",    40, 0, 2000,     &(res.HT_all),                   "");
     //screwdriver.AddVariable("genMET",             "Generated MET",            "",    40, 0, 1200,     &(myEvent.genMET),                   "");
    
     float metReso = 50.;
     float massWl;
     float massTl;
     float massTm;
     float massTmsm;
     
     
     TFile fout("metResoTerm.root","RECREATE");
     //Create a tree
     TTree* otree = new TTree("T","tree");
     otree->Branch("massWl", &massWl);
     otree->Branch("massTl", &massTl);
     otree->Branch("massTm", &massTm);
     otree->Branch("met", &myEvent.pfmet);
     otree->Branch("MT", &myEvent.mt_met_lep);
     otree->Branch("dphi", &myEvent.minDPhi_jmet);
     otree->Branch("hadchi2", &myEvent.hadronic_top_chi2);
     otree->Branch("topness", &myEvent.topness);
     otree->Branch("PassTauvVeto", &myEvent.PassTauVeto);
     otree->Branch("PassTrackVeto", &myEvent.PassTrackVeto);
     
     TH1F* hmassWl = new TH1F("hmassWl","",50,0,200);
     TH1F* hmassTl = new TH1F("hmassTl","",50,0,500);
     TH1F* hmassTm = new TH1F("hmassTm","",50,0,500);
     TH1F* hmass2Wl = new TH1F("hmass2Wl","",50,0,15000);
     TH1F* hmass2Tl = new TH1F("hmass2Tl","",50,0,150000);
     TH1F* hmass2Tm = new TH1F("hmass2Tm","",50,0,300000);
     TH1F* hmassTmsm = new TH1F("hmassTmsm","",50,0,500);
     double tWL;
     double tTL;
     double tTM;
     TH1F* htWL = new TH1F("htWL","",50,0,500);
     TH1F* htTL = new TH1F("htTL","",50,0,500);
     TH1F* htTM = new TH1F("htTM","",50,0,500);
     TH1F* htWL_frac = new TH1F("htWL_frac","",50,0,1);
     TH1F* htTL_frac = new TH1F("htTL_frac","",50,0,1);
     TH1F* htTM_frac = new TH1F("htTM_frac","",50,0,1);
     TH1F* htCM_frac = new TH1F("htCM_frac","",50,0,1);

     screwdriver.AddVariable("massWl",             "mass Wl",            "",    50, 0, 200,     &(massWl),                   "");
     screwdriver.AddVariable("massTl",             "mass Tl",            "",    50, 0, 200,     &(massTl),                   "");
     screwdriver.AddVariable("massTm",             "mass Tm",            "",    50, 0, 200,     &(massTm),                   "");
     screwdriver.AddVariable("massTmsm",             "mass Tm - smearing of neutrinos",            "",    50, 0, 200,     &(massTmsm),                   "");

     // topness
     screwdriver.AddVariable("topness",             "topness - reference",            "",    60, -15, 15,     &(topness_def),                   "");


    // #########################################################
    // ##   Create ProcessClasses (and associated datasets)   ##
    // #########################################################
	
	screwdriver.AddProcessClass("ttbar_1l", "t#bar{t} #rightarrow l+jets",                             "background",kRed-7);
                screwdriver.AddDataset("ttbar-madgraph",                "ttbar_1l",  0, 0);


        screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",kCyan-3);
/*	
	screwdriver.AddProcessClass("single_top", "single top",                             "background",kRed-5);
                screwdriver.AddDataset("singleTopbar_s",                "single_top",  0, 0);
                screwdriver.AddDataset("singleTopbar_t",                "single_top",  0, 0);
                screwdriver.AddDataset("singleTop_s",                "single_top",  0, 0);
                screwdriver.AddDataset("singleTop_t",                "single_top",  0, 0);
	 
        screwdriver.AddProcessClass("W+jets",   "W+jets",                          "background",kOrange-2);
            screwdriver.AddDataset("Wjets", "W+jets", 0, 0);
        screwdriver.AddProcessClass("VV",   "di-boson",                          "background",kOrange-3);
            screwdriver.AddDataset("ZZ", "ZZ", 0, 0);
            screwdriver.AddDataset("WZ", "WZ", 0, 0);
       
       screwdriver.AddProcessClass("ttV",   "ttV",                          "background",kOrange-4);
            screwdriver.AddDataset("ttW", "ttV", 0, 0);
            screwdriver.AddDataset("ttZ", "ttV", 0, 0);
	*/
	/*
	screwdriver.AddProcessClass("T2tt_850_100",   "T2tt_850_100",                          "signal",kGreen+2);
            screwdriver.AddDataset("T2tt_850_100", "T2tt_850_100", 0, 0);
	screwdriver.AddProcessClass("T2tt_650_325",   "T2tt_650_325",                          "signal",kGreen+3);
           screwdriver.AddDataset("T2tt_650_325", "T2tt_650_325", 0, 0);
       */
       // screwdriver.AddProcessClass("T2tt_500_325",   "T2tt_500_325",                          "signal",kGreen+4);
         //   screwdriver.AddDataset("T2tt_500_325", "T2tt_500_325", 0, 0);


    // ##########################
    // ##    Create Regions    ##
    // ##########################

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
     //screwdriver.Add2DHisto("genMET","METReso");
     //screwdriver.Add2DHisto("nofJets","METReso");
    
    
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
     screwdriver.SetGlobalFloatOption ("1DStack",           "factorSignal",     100.0    );

     screwdriver.SetGlobalStringOption("Plot", "infoTopRight", "CMS Preliminary");
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

        for (unsigned int d = 0 ; d < datasetsList.size() ; d++)
        {
            string currentDataset = datasetsList[d];
            string currentProcessClass = screwdriver.GetProcessClass(currentDataset);

            sampleName = currentDataset;
            sampleType = screwdriver.GetProcessClassType(currentProcessClass);

            // Open the tree
            string treePath = string(FOLDER_BABYTUPLES_SKIM)+currentDataset+".root";
            //string treePath = string(FOLDER_BABYTUPLES)+currentDataset+".root";
            TFile f(treePath.c_str());
            TTree* theTree = (TTree*) f.Get("babyTuple");

	    //cout<<"v1"<<endl;
            InitializeBranchesForReading(theTree,&myEvent);
	    //cout<<"v2"<<endl;
	    //return -1;
        // ########################################
        // ##        Run over the events         ##
        // ########################################

	default_param(topness_param_);
	LoadParam(string("config_11.txt"));
	//topness_param.aTL = 10;
	//topness_param.aTM = 5;
	TRandom rand;

            bool ttbarDatasetToBeSplitted = false;
            if (findSubstring(currentDataset,"ttbar")
            && (currentDataset != "ttbar_madgraph_1l")
            && (currentDataset != "ttbar_madgraph_2l"))
                ttbarDatasetToBeSplitted = true;

            int nEntries = theTree->GetEntries();
            //if(nEntries>10000000) nEntries = 10000000;
            //if(nEntries>5000000) nEntries = 5000000;
            //if(nEntries>1000) nEntries = 1000;
	    float sf_fracEvent = (float)nEntries/theTree->GetEntries();
	    for (int i = 0 ; i < nEntries ; i++)
            {
                if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);
                //if(!PassFilter(theTree,i)) continue;

		// Get the i-th entry
		ReadEvent(theTree,i,&myEvent);
                string currentProcessClass_ = currentProcessClass;
		float weight = getWeight(currentDataset,theTree->GetEntries(), sf_fracEvent);
		//cout<<"toto"<<endl;
		Fill_tt2l_mc_info(mcinfo,myEvent);
		//cout<<"toto"<<endl;
		fill_t2l_reso(res, mcinfo,myEvent);
		
		//Compute Delta MET
		//d_met = myEvent.pfmet-myEvent.genMET;

		//Topness study
		bool is_tt2l = false;
		int index_b1 = 0;
		int index_b2 = 0;
		//cerr<<mcinfo.ttbar_decay<<endl;
		if(mcinfo.ttbar_decay == 2){
			is_tt2l = true; 
			n_tt2l_sel++;
			if(res.matched == true && myEvent.jet_pt.size()>0){// && myEvent.neutrinos.size()==2){
			//	cerr<<"here!"<<endl;
				
				
				//to speed time: comment it - sufficient for resolution computation
				//
				//topness_def = ComputeTopness(index_b1,index_b2, tWL, tTL, tTM);
			 	//cout<<"indexes: "<<index_b1<<" "<<index_b2<<endl;	
				n_tt2l_sel_matchable++;
                		//lead lepton
				TLorentzVector lep;
				lep.SetPtEtaPhiM(myEvent.lep1_pt, myEvent.lep1_eta, myEvent.lep1_phi, 0);
				//cerr<<"here!"<<endl;
				//w-lep
				float dr1 = mcinfo.top_l.DeltaR(lep);
				float dr2 = mcinfo.atop_l.DeltaR(lep);
				//cerr<<"here!"<<endl;
				TLorentzVector jetlb, jetlbbar;
				//cout<<dr1<<" "<<dr2<<endl;
				if(dr1<0.1){
					massWl = (mcinfo.top_nu+lep).M();
					//cout<<res.index_b<<" "<<res.index_bbar<<endl;
					//cout<<myEvent.jet_pt.size()<<endl;
					jetlb.SetPtEtaPhiM(myEvent.jet_pt[res.index_b], myEvent.jet_eta[res.index_b], myEvent.jet_phi[res.index_b], myEvent.jet_mass[res.index_b]);
					massTl = (mcinfo.top_nu+lep+jetlb).M();
					jetlbbar.SetPtEtaPhiM(myEvent.jet_pt[res.index_bbar], myEvent.jet_eta[res.index_b], myEvent.jet_phi[res.index_b], myEvent.jet_mass[res.index_b]);
					//massTm = (mcinfo.atop_W+jetlbbar).M();
					massTm = (mcinfo.atop_l+mcinfo.atop_nu+jetlbbar).M();
					float new_pt = rand.Gaus(mcinfo.atop_W.Pt(),metReso);
					TLorentzVector misWrand;
					misWrand.SetPtEtaPhiE(new_pt, mcinfo.atop_W.Eta(),mcinfo.atop_W.Phi(),sqrt(new_pt*new_pt+pow(mcinfo.atop_W.Pz(),2)+pow(mcinfo.atop_W.M(),2)));
					massTmsm = (misWrand+jetlbbar).M();
					
					//if( TopnessMatched(index_b1, index_b2, res)){
					

					//This constraint is useless !
					//if( index_b1 == res.index_b && index_b2 == res.index_bbar){
						hmassWl->Fill(massWl);
						hmassTl->Fill(massTl);
						hmassTm->Fill(massTm);
						hmassTmsm->Fill(massTmsm);
						hmass2Wl->Fill(massWl*massWl);
						hmass2Tl->Fill(massTl*massTl);
						hmass2Tm->Fill(massTm*massTm);
						htWL->Fill(sqrt(topness_terms.tWL));
						htTL->Fill(sqrt(topness_terms.tTL));
						htTM->Fill(sqrt(topness_terms.tTM));
						htWL_frac->Fill(topness_terms.tWL/topness_terms.topness);
						htTL_frac->Fill(topness_terms.tTL/topness_terms.topness);
						htTM_frac->Fill(topness_terms.tTM/topness_terms.topness);
						htCM_frac->Fill(topness_terms.tCM/topness_terms.topness);
						//cout<<"there "<<sqrt(tWL)<<endl;
						
						otree->Fill();
					//}
				}
				else if(dr2<0.1){
					massWl = (mcinfo.atop_nu+lep).M();
					jetlbbar.SetPtEtaPhiM(myEvent.jet_pt[res.index_bbar], myEvent.jet_eta[res.index_b], myEvent.jet_phi[res.index_b], myEvent.jet_mass[res.index_b]);
					jetlb.SetPtEtaPhiM(myEvent.jet_pt[res.index_b], myEvent.jet_eta[res.index_b], myEvent.jet_phi[res.index_b], myEvent.jet_mass[res.index_b]);
					massTm = (mcinfo.top_W+jetlb).M();
					float new_pt = rand.Gaus(mcinfo.top_W.Pt(),metReso);
					TLorentzVector misWrand;
					misWrand.SetPtEtaPhiE(new_pt, mcinfo.top_W.Eta(),mcinfo.top_W.Phi(),sqrt(new_pt*new_pt+pow(mcinfo.top_W.Pz(),2)+pow(mcinfo.top_W.M(),2)));
					massTmsm = (misWrand+jetlb).M();
					
					//if( TopnessMatched(index_b1, index_b2, res)){
					
					
					//This constraint is useless !
					//if( index_b1 == res.index_bbar && index_b2 == res.index_b){
						hmassWl->Fill(massWl);
						hmassTl->Fill(massTl);
						hmassTm->Fill(massTm);
						hmass2Wl->Fill(massWl*massWl);
						hmass2Tl->Fill(massTl*massTl);
						hmass2Tm->Fill(massTm*massTm);
						hmassTmsm->Fill(massTmsm);
						htWL->Fill(sqrt(topness_terms.tWL));
						htTL->Fill(sqrt(topness_terms.tTL));
						htTM->Fill(sqrt(topness_terms.tTM));
						//cout<<tWL<<" "<<tTL<<" "<<tTM<<" "<<topness_def<<" "<<exp(topness_def)<<endl;
						htWL_frac->Fill(topness_terms.tWL/topness_terms.topness);
						htTL_frac->Fill(topness_terms.tTL/topness_terms.topness);
						htTM_frac->Fill(topness_terms.tTM/topness_terms.topness);
						htCM_frac->Fill(topness_terms.tCM/topness_terms.topness);
					//}
						otree->Fill();
					
				}
			}
		}
			
		

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
   fout.cd();
   hmassWl->Write();
   hmassTl->Write();
   hmassTm->Write();
   hmass2Wl->Write();
   hmass2Tl->Write();
   hmass2Tm->Write();
   hmassTmsm->Write();
   htWL->Write();
   htTL->Write();
   htTM->Write();
   htWL_frac->Write();
   htTL_frac->Write();
   htTM_frac->Write();
   htCM_frac->Write();

   //Write the tree
   otree->Write();
   fout.Write();
   fout.Close();

  cout << endl;
  cout << "   > Making plots..." << endl;
  screwdriver.MakePlots();
  cout << "   > Saving plots..." << endl;

  string output_dir = "./plots/"+string(argv[1])+"/";
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
  	table_topness_purity.Set(i,0, Figure(topness_purity[i][0],sqrt(topness_purity[i][0]))/Figure(n_tt2l_sel,sqrt(n_tt2l_sel)));
  	table_topness_purity.Set(i,1, Figure(topness_purity[i][0],sqrt(topness_purity[i][0]))/Figure(n_tt2l_sel_matchable,sqrt(n_tt2l_sel_matchable)));
  }
  table_topness_purity.Print();

  printBoxedMessage("Program done.");
	
  return (0);
}


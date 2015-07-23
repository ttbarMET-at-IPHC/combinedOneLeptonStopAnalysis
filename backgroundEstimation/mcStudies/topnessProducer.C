#include "../common/common.h"
#include "topness_tt2l_mc_reso.h"

bool PassFilter(TTree* theTree, long int i){
    //theTree->GetBranch("genlepsfromtop")->GetEntry(i);
    theTree->GetBranch("pfmet")->GetEntry(i);
    theTree->GetBranch("ngoodleps")->GetEntry(i);
    theTree->GetBranch("ngoodjets")->GetEntry(i);
    return goesInPreselection();

}


bool goesInAnyChannel()                             { return (goesInSingleLeptonChannel() || goesInDoubleLeptonChannel());                  }

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

     tt2l_mc_info mcinfo;
     tt2l_reso res;
     
     screwdriver.AddVariable("MET",            "MET",                  "GeV",    26,0,520,       &(myEvent.pfmet),                  "");
     screwdriver.AddVariable("MT",             "M_{T}",                   "GeV",    40,0,400,       &(myEvent.MT),                   "");
     
     //Specific studu
     //DeltaPhi (nu,nu)
     //Pt of radiation
     screwdriver.AddVariable("METReso",             "MET resolution",                 "GeV",    40,-400,400,       &(res.d_met),                   "");
     screwdriver.AddVariable("HTReso",             "HT resolution",                   "GeV",    40,-400,400,       &(res.d_HT),                   "");
     screwdriver.AddVariable("PzReso",             "Pz resolution",                   "GeV",    40,-400,400,       &(res.d_pz),                   "");
     screwdriver.AddVariable("Pz_ttbar",             "Pz of ttbar system",            "GeV",    40,-2500,2500,     &(res.Pz_ttbar),                   "");
     screwdriver.AddVariable("Pt_ttbar",             "Pt of ttbar system",            "GeV",    40,0,1500,     &(res.Pt_ttbar),                   "");
     screwdriver.AddVariable("nofISR",             "Number of ISR",            "",    10, 0, 10,     &(res.nofISR),                   "");
     screwdriver.AddVariable("nofJets",             "Number of jets",            "",    10, 0, 10,     &(res.njets),                   "");
     screwdriver.AddVariable("lostLeptonPt",             "Pt of lost lepton",            "",    40, 0, 200,     &(res.lost_pt),                   "");
     screwdriver.AddVariable("lostLeptonEta",             "Eta of lost lepton",            "",    40, -6, 6,     &(res.lost_eta),                   "");
     screwdriver.AddVariable("HT_visttbar",             "HT of 2b and lepton",            "",    40, 0, 2000,     &(res.HT),                   "");
     screwdriver.AddVariable("Pz_visttbar",             "Pz of 2b and lepton",            "",    40, 0, 2000,     &(res.pz),                   "");
     screwdriver.AddVariable("HT_all",             "HT all jets and leading lepton",            "",    40, 0, 2000,     &(res.HT_all),                   "");
     screwdriver.AddVariable("genMET",             "Generated MET",            "",    40, 0, 1200,     &(res.genMET),                   "");
	//int index_b;	//
	//int index_bbar; //
//	vector<int> index_isr;



    // #########################################################
    // ##   Create ProcessClasses (and associated datasets)   ##
    // #########################################################
	///*
        screwdriver.AddProcessClass("ttbar_1l", "t#bar{t} #rightarrow l+jets",                             "background",kRed-7);
                screwdriver.AddDataset("ttbar-madgraph",                "ttbar_1l",  0, 0);
            //screwdriver.AddDataset("singleTop_st",                  "1ltop",  0, 0);


        screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",kCyan-3);
	    /*
        screwdriver.AddProcessClass("W+jets",   "W+jets",                          "background",kOrange-2);
            screwdriver.AddDataset("Wjets", "W+jets", 0, 0);
        screwdriver.AddProcessClass("VV",   "di-boson",                          "background",kOrange-3);
        screwdriver.AddProcessClass("ttV",   "ttV",                          "background",kOrange-4);
            screwdriver.AddDataset("ttW", "ttW", 0, 0);
            screwdriver.AddDataset("ttZ", "ttZ", 0, 0);
	*/


    // ##########################
    // ##    Create Regions    ##
    // ##########################

        //screwdriver.AddRegion("presel_MTpeak",          "Preselection (MT peak)",      &goesInMTpeak);
        screwdriver.AddRegion("presel_MTtail",          "Preselection (MT peak)",      &goesInMTtail);
        screwdriver.AddRegion("baseline",          "Baseline selection",      &goesInBaselineSearchSR);
        screwdriver.AddRegion("baslineLargeDM",          "Baseline S.R.  - Large #DeltaM",      &goesInLargeDMSR);
        screwdriver.AddRegion("baselineSmallDM",          "Baseline S.R. - Small #DeltaM",      &goesInSmallDMSR);


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
     screwdriver.Add2DHisto("genMET","METReso");
     screwdriver.Add2DHisto("HT_all","METReso");
     screwdriver.Add2DHisto("HT_visttbar","METReso");
     screwdriver.Add2DHisto("nofJets","METReso");
     screwdriver.Add2DHisto("lostLeptonPt","METReso");
     screwdriver.Add2DHisto("lostLeptonEta","METReso");
     screwdriver.Add2DHisto("Pt_ttbar","METReso");
/*     
     screwdriver.Add2DHisto("Pt_ttbar","HTReso");
     screwdriver.Add2DHisto("nofISR","HTReso");
     screwdriver.Add2DHisto("HT_all","HTReso");
     screwdriver.Add2DHisto("HT_visttbar","HTReso");
     screwdriver.Add2DHisto("nofJets","HTReso");
     screwdriver.Add2DHisto("lostLeptonPt","HTReso");
     screwdriver.Add2DHisto("lostLeptonEta","HTReso");
     screwdriver.Add2DHisto("Pt_ttbar","HTReso");
     
     screwdriver.Add2DHisto("Pz_ttbar","PzReso");
     screwdriver.Add2DHisto("nofISR","PzReso");
     screwdriver.Add2DHisto("HT_all","PzReso");
     screwdriver.Add2DHisto("Pz_visttbar","PzReso");
     screwdriver.Add2DHisto("nofJets","PzReso");
     screwdriver.Add2DHisto("lostLeptonPt","PzReso");
     screwdriver.Add2DHisto("lostLeptonEta","PzReso");
     screwdriver.Add2DHisto("Pz_ttbar","PzReso");

*/


    // #########################################################

     // Schedule plots
     screwdriver.SchedulePlots("1DDataMCComparison");
     screwdriver.SchedulePlots("1DStack");
     screwdriver.SchedulePlots("1DSuperimposed");
     screwdriver.SchedulePlots("2D");

     // Config plots

     screwdriver.SetGlobalStringOption("DataMCComparison",  "includeSignal",                    "stack");
     screwdriver.SetGlobalFloatOption ("DataMCComparison",  "factorSignal",                     1.0    );
     
     screwdriver.SetGlobalStringOption("1DStack",           "includeSignal",    "superimposed");
     screwdriver.SetGlobalFloatOption ("1DStack",           "factorSignal",     1.0    );

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
            string treePath = string(FOLDER_BABYTUPLES)+currentDataset+".root";
            TFile f(treePath.c_str());
            TTree* theTree = (TTree*) f.Get("babyTuple");

            InitializeBranchesForReading(theTree,&myEvent);

        // ########################################
        // ##        Run over the events         ##
        // ########################################

            bool ttbarDatasetToBeSplitted = false;
            if (findSubstring(currentDataset,"ttbar")
            && (currentDataset != "ttbar_madgraph_1l")
            && (currentDataset != "ttbar_madgraph_2l"))
                ttbarDatasetToBeSplitted = true;

            int nEntries = theTree->GetEntries();
            for (int i = 0 ; i < nEntries ; i++)
            {
                if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

                if(!PassFilter(theTree,i)) continue;
		
		// Get the i-th entry
		ReadEvent(theTree,i,&myEvent);

         	if(goesInPreselection() && goesInAnyChannel()){
		//cout<<myEvent.MT<<" "<<myEvent.pfmet<<" "<<myEvent.ngoodleps<<" "<<myEvent.ngoodjets<<endl;
		//cout<<"preselection: "<<goesInPreselection()<<endl;
		//cout<<"all channel"<< goesInAnyChannel() <<  endl;
		}
		//cout<<""<< <<  endl;
		//cout<<""<< <<  endl;
		//cout<<""<< <<  endl;
	 	float weight = getWeight();

                // Split 1-lepton ttbar and 2-lepton ttbar
                string currentProcessClass_ = currentProcessClass;
                if (ttbarDatasetToBeSplitted && (myEvent.genlepsfromtop == 2))
                    currentProcessClass_ = "ttbar_2l";



		//Topness study
		if(goesInPreselection()){
			//cout<<"##############################"<<endl;
			//cout<<"event "<<i<<endl;
			Fill_tt2l_mc_info(mcinfo,myEvent);
			//if(mcinfo.isr_id.size()>=2) DumpMCinfo(mcinfo);
			if(mcinfo.top_lepdecay && mcinfo.atop_lepdecay){
				fill_t2l_reso(res, mcinfo,myEvent);
			//	cout<<"Event "<<i<<" is dileptonic"<<endl;
			}
			//if (myEvent.genlepsfromtop == 2)  cout<<" Event "<<i<<" 2 leptons "<<endl;
		}
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

  screwdriver.WritePlots(string("./plots/plotsProducer/"));

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################

  printBoxedMessage("Program done.");

	return (0);
}


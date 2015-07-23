//#define USE_GEN_INFO
//#define USE_GEN_INFO_EXT
#define USE_LEP1
//#define USE_LEP1_EXT
//#define USE_LEP2
//#define USE_LEP2_EXT
//#define USE_JETS
//#define USE_JETS_EXT
//#define USE_PV
//#define USE_WEIGHTS
#define USE_VAR_BASELINE
//#define USE_GLOBAL_VAR
//#define USE_OLD_VAR
#define USE_SKIMMING_VAR

#include "../common/common.h"

#ifndef SIGNAL_REGION_CUTS
    #error SIGNAL_REGION_CUTS need to be defined.
#endif
#ifndef SIGNAL_REGION_TAG
    #error SIGNAL_REGION_TAG need to be defined.
#endif

//#define SIGNAL_CONTAMINATION_INPUT "T2tt_475_175"

#define FOLDER_BABYTUPLES_SKIM "/opt/sbg/data/data1/cms/echabert/Stop2015/BabyTuples/v1_20_07/skimming/"

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
          //  screwdriver.AddDataset("ZZ", "ZZ", 0, 0);
            screwdriver.AddDataset("WZ", "WZ", 0, 0);
       
       screwdriver.AddProcessClass("ttV",   "ttV",                          "background",kOrange-4);
            screwdriver.AddDataset("ttW", "ttV", 0, 0);
            screwdriver.AddDataset("ttZ", "ttV", 0, 0);

	screwdriver.AddProcessClass("T2tt_850_100",   "T2tt_850_100",                          "signal",kGreen+2);
            screwdriver.AddDataset("T2tt_850_100", "T2tt_850_100", 0, 0);
	screwdriver.AddProcessClass("T2tt_650_100",   "T2tt_650_100",                          "signal",kGreen+3);
           screwdriver.AddDataset("T2tt_650_100", "T2tt_650_100", 0, 0);
        screwdriver.AddProcessClass("T2tt_500_325",   "T2tt_500_325",                          "signal",kGreen+4);
            screwdriver.AddDataset("T2tt_500_325", "T2tt_500_325", 0, 0);


    // ##########################
    // ##    Create Regions    ##
    // ##########################

        //screwdriver.AddRegion("presel_MTpeak",          "Preselection (MT peak)",      &goesInMTpeak);
        //screwdriver.AddRegion("presel_MTtail",          "Preselection (MT peak)",      &goesInMTtail);
        screwdriver.AddRegion("preselection",          "Preselection ",      &goesInPreselection);
        screwdriver.AddRegion("preselection_noveto",          "Preselection no veto",      &goesInPreselectionNoVeto);
        screwdriver.AddRegion("baseline",          "Baseline selection",      &goesInBaselineSearchSR);
        screwdriver.AddRegion("baselineLargeDM",          "Baseline S.R.  - Large #DeltaM",      &goesInLargeDMSR);
        screwdriver.AddRegion("baselineSmallDM",          "Baseline S.R. - Small #DeltaM",      &goesInSmallDMSR);
        screwdriver.AddRegion("baseline2b",          "Baseline selection",      &goesInBaselineSearchSR2b);
        screwdriver.AddRegion("baselineSmallDM2b",          "Baseline S.R. - Small #DeltaM",      &goesInSmallDMSR2b);


    // ##########################
    // ##   Create Channels    ##
    // ##########################

        //screwdriver.AddChannel("singleLepton", "e/#mu-channels",  &goesInSingleLeptonChannel);
        //screwdriver.AddChannel("singleElec",   "e-channel",       &goesInSingleElecChannel  );
        //screwdriver.AddChannel("singleMuon",   "#mu-channel",     &goesInSingleMuonChannel  );

        /*
        screwdriver.AddChannel("doubleLepton", "2l-channel",      &goesInDoubleLeptonChannel);
        screwdriver.AddChannel("doubleElec",   "ee-channel",      &goesInDoubleElecChannel  );
        screwdriver.AddChannel("doubleMuon",   "#mu#mu-channel",  &goesInDoubleMuonChannel  );
        screwdriver.AddChannel("emu",          "e#mu-channel",    &goesInMuonElecChannel    );
        */

        screwdriver.AddChannel("allChannels",  "",                &goesInAnyChannel         );

    // ########################################
    // ##       Create histograms and        ##
    // ##  schedule type of plots to produce ##
    // ########################################

        // Create histograms
        screwdriver.Create1DHistos();

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
	    float sf_fracEvent = (float)nEntries/theTree->GetEntries();
            for (int i = 0 ; i < nEntries ; i++)
            {
                if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

                // Get the i-th entry
                //ReadEvent(theTree,i,&pointers,&myEvent);
                //cout<<"can I read the event ?"<<endl;
		ReadEvent(theTree,i,&myEvent);
		//cout<<"> Yes I can !"<<endl;

         	//if(goesInPreselection() && goesInAnyChannel()){
		//cout<<myEvent.MT<<" "<<myEvent.pfmet<<" "<<myEvent.ngoodleps<<" "<<myEvent.ngoodjets<<endl;
		//cout<<"preselection: "<<goesInPreselection()<<endl;
		//cout<<"all channel"<< goesInAnyChannel() <<  endl;
		//}
		//cout<<""<< <<  endl;
		//cout<<""<< <<  endl;
		//cout<<""<< <<  endl;
	 	//float weight = getWeight();
		float weight = getWeight(currentDataset,theTree->GetEntries(), sf_fracEvent);

                // Split 1-lepton ttbar and 2-lepton ttbar
                string currentProcessClass_ = currentProcessClass;
                //if (ttbarDatasetToBeSplitted && (myEvent.genlepsfromtop == 2))
                //    currentProcessClass_ = "ttbar_2l";
		
		//cout<<goesInBaselineSearchSR()<<endl;
		//cout<<goesInAnyChannel()<<endl;
		//cout<<weight<<endl;
                screwdriver.AutoFillProcessClass(currentProcessClass_,weight);

            }
            printProgressBar(nEntries,nEntries,currentDataset);
            cout << endl;
            f.Close();

        }

        // ####################################
        // ##        Write the table         ##
        // ####################################

        printBoxedMessage("Writing the table ... ");

        vector<string> regions  = { 
				    "preselection", "preselection_noveto", "baseline", "baselineLargeDM", "baselineSmallDM" 
				    //"presel_MTpeak", "presel_MTtail",
				    /*
				    "preveto_MTpeak",      "preveto_MTtail",
                                    "signalRegion_MTpeak", "signalRegion_MTtail",
                                    "0btag_MTpeak",        "0btag_MTtail",
                                    "reversedVeto_MTpeak", "reversedVeto_MTtail",
                                    "2leptons", "2leptons_MTpeak", "2leptons_MTtail",
                                  */};

        string exportFile = "rawYieldTables/prediction.tab";
        string exportFile2 = "rawYieldTables/predictionSig.tab";
        TableDataMC(&screwdriver,regions,"allChannels").Print(exportFile,4);
        TableBackgroundSignal(&screwdriver,regions,"allChannels").Print(exportFile2,4);

        vector<string> secondLeptonInAcceptanceRegions  = {
                                                              "preveto_MTtail",
                                                              "secondLeptonInAcceptance",
                                                              "singleTrack",
                                                              "hadronicTau"
                                                          };

        //string exportFile2 = "secondLeptonInAcceptance/"+string(SIGNAL_REGION_TAG)+".tab";
        //TableDataMC(&screwdriver,secondLeptonInAcceptanceRegions,"allChannels").Print(exportFile2,4);

        printBoxedMessage("Table generation completed");

	return (0);
}


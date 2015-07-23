#include "../common/common.h"

#ifndef SIGNAL_REGION_CUTS
    #error SIGNAL_REGION_CUTS need to be defined.
#endif
#ifndef SIGNAL_REGION_TAG
    #error SIGNAL_REGION_TAG need to be defined.
#endif

//#define SIGNAL_CONTAMINATION_INPUT "T2tt_475_175"



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
	/*
        screwdriver.AddProcessClass("1ltop", "1l top",                             "background",kRed-7);
            #ifdef USING_TTBAR_POWHEG
                screwdriver.AddDataset("ttbar_powheg",                "1ltop",  0, 0);
            #endif
            #ifdef USING_TTBAR_MADGRAPH
                screwdriver.AddDataset("ttbar_madgraph_1l",             "1ltop",  0, 0);
            #endif
            screwdriver.AddDataset("singleTop_st",                  "1ltop",  0, 0);


        screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",kCyan-3);
            #ifdef USING_TTBAR_MADGRAPH
                screwdriver.AddDataset("ttbar_madgraph_2l",   "ttbar_2l",  0, 0);
            #endif

        screwdriver.AddProcessClass("W+jets",   "W+jets",                          "background",kOrange-2);
            screwdriver.AddDataset("W+jets",    "W+jets", 0, 0);

        screwdriver.AddProcessClass("rare",   "rare",                              "background",kMagenta-5);
            screwdriver.AddDataset("rare",   "rare", 0, 0);

        screwdriver.AddProcessClass("data",   "data",                              "data",COLORPLOT_BLACK);
            screwdriver.AddDataset("SingleElec",   "data", 0, 0);
            screwdriver.AddDataset("SingleMuon",   "data", 0, 0);
            screwdriver.AddDataset("DoubleElec",   "data", 0, 0);
            screwdriver.AddDataset("DoubleMuon",   "data", 0, 0);
            screwdriver.AddDataset("MuEl",         "data", 0, 0);
	*/
        //screwdriver.AddProcessClass("signal",            "signal",                "background",kMagenta-5);
           // screwdriver.AddDataset("T2tt_650_325", "signal", 0, 0);
        screwdriver.AddProcessClass("W+jets",   "W+jets",                          "background",kOrange-2);
            screwdriver.AddDataset("T2tt_650_325", "W+jets", 0, 0);


    // ##########################
    // ##    Create Regions    ##
    // ##########################

        screwdriver.AddRegion("presel_MTpeak",          "Preselection (MT peak)",      &goesInMTpeak);
        screwdriver.AddRegion("presel_MTtail",          "Preselection (MT peak)",      &goesInMTtail);


    // ##########################
    // ##   Create Channels    ##
    // ##########################

        //screwdriver.AddChannel("singleLepton", "e/#mu-channels",  &goesInSingleLeptonChannel);
        screwdriver.AddChannel("singleElec",   "e-channel",       &goesInSingleElecChannel  );
        screwdriver.AddChannel("singleMuon",   "#mu-channel",     &goesInSingleMuonChannel  );

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

                // Get the i-th entry
                //ReadEvent(theTree,i,&pointers,&myEvent);
                //cout<<"can I read the event ?"<<endl;
		ReadEvent(theTree,i,&myEvent);
		//cout<<"> Yes I can !"<<endl;

         	if(goesInPreselection() && goesInAnyChannel()){
		cout<<myEvent.MT<<" "<<myEvent.pfmet<<" "<<myEvent.ngoodleps<<" "<<myEvent.ngoodjets<<endl;
		cout<<"preselection: "<<goesInPreselection()<<endl;
		cout<<"all channel"<< goesInAnyChannel() <<  endl;
		}
		//cout<<""<< <<  endl;
		//cout<<""<< <<  endl;
		//cout<<""<< <<  endl;
	 	float weight = getWeight();

                // Split 1-lepton ttbar and 2-lepton ttbar
                string currentProcessClass_ = currentProcessClass;
                if (ttbarDatasetToBeSplitted && (myEvent.genlepsfromtop == 2))
                    currentProcessClass_ = "ttbar_2l";

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
				    "presel_MTpeak", "presel_MTtail",
				    /*
				    "preveto_MTpeak",      "preveto_MTtail",
                                    "signalRegion_MTpeak", "signalRegion_MTtail",
                                    "0btag_MTpeak",        "0btag_MTtail",
                                    "reversedVeto_MTpeak", "reversedVeto_MTtail",
                                    "2leptons", "2leptons_MTpeak", "2leptons_MTtail",
                                  */};

        string exportFile = "rawYieldTables/"+string(SIGNAL_REGION_TAG)+".tab";
        TableDataMC(&screwdriver,regions,"allChannels").Print(exportFile,4);

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


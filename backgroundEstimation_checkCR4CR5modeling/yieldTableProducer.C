#include "common.h"

#ifndef SIGNAL_REGION_CUTS
    #error SIGNAL_REGION_CUTS need to be defined.
#endif
#ifndef SIGNAL_REGION_TAG
    #error SIGNAL_REGION_TAG need to be defined.
#endif

bool goesInPreVetoSelectionMTpeak_withSRCuts()      { return (goesInPreVetoSelectionMTpeak()          && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInPreVetoSelectionMTtail_withSRCuts()      { return (goesInPreVetoSelectionMTtail()          && SIGNAL_REGION_CUTS(enableMTCut) ); }
                                                                      
bool goesInPreselectionMTpeak_withSRCuts()          { return (goesInPreselectionMTpeak()              && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInPreselectionMTtail_withSRCuts()          { return (goesInPreselectionMTtail()              && SIGNAL_REGION_CUTS(enableMTCut) ); }

bool goesIn0BtagControlRegionMTpeak_withSRCuts()    { return (goesIn0BtagControlRegionMTpeak()        && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesIn0BtagControlRegionMTtail_withSRCuts()    { return (goesIn0BtagControlRegionMTtail()        && SIGNAL_REGION_CUTS(enableMTCut) ); }

bool goesInDileptonControlRegion_withSRCuts()       { return (goesInDileptonControlRegion()           && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInDileptonControlRegionMTpeak_withSRCuts() { return (goesInDileptonControlRegionMTpeak()     && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInDileptonControlRegionMTtail_withSRCuts() { return (goesInDileptonControlRegionMTtail()     && SIGNAL_REGION_CUTS(enableMTCut) ); }

bool goesInVetoControlRegionMTpeak_withSRCuts()     { return (goesInVetoControlRegionMTpeak()         && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInVetoControlRegionMTtail_withSRCuts()     { return (goesInVetoControlRegionMTtail()         && SIGNAL_REGION_CUTS(enableMTCut) ); }

bool goesInDileptonControlRegion1or2jets_withSRCuts()     { return (goesInDileptonControlRegion(2)    && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInDileptonControlRegion3jets_withSRCuts()        { return (goesInDileptonControlRegion(3)    && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInDileptonControlRegionAtLeast4jets_withSRCuts() { return (goesInDileptonControlRegion(4)    && SIGNAL_REGION_CUTS(disableMTCut)); }

bool goesInAnyChannel()                             { return (goesInSingleLeptonChannel() || goesInDoubleLeptonChannel());                  }

// #########################################################################
//                              Main function
// #########################################################################

int main (int argc, char *argv[])
{
  
    // Special region for 2, 3 or 4 jets with 50, 100 or 150 minimum events
    string CustomJetRequirement; 
    if (argc == 2) 
    { 
        CustomJetRequirement = argv[1];
        NOMINAL_BDT_CUT = false;
        printBoxedMessage("Running with custom jet requirement : "+CustomJetRequirement);
        LoadBDTCut(CustomJetRequirement);
    }
    
    printBoxedMessage("Starting tables generation");

    // ####################
    // ##   Init tools   ##
    // ####################

        // Create a sonic Screwdriver
        SonicScrewdriver screwdriver;

    // #########################################################
    // ##   Create ProcessClasses (and associated datasets)   ##
    // #########################################################

        screwdriver.AddProcessClass("1ltop", "1l top",                             "background",kRed-7);
            #ifdef USING_TTBAR_POWHEG
                screwdriver.AddDataset("ttbar_powheg",                "1ltop",  0, 0);
            #endif
            #ifdef USING_TTBAR_MADGRAPH
                screwdriver.AddDataset("ttbar_madgraph_1l",             "1ltop",  0, 0);
            #endif
            //screwdriver.AddDataset("ttbar_madgraph_scaledown",    "1ltop",  0, 0);
            //screwdriver.AddDataset("ttbar_madgraph_scaleup",      "1ltop",  0, 0);
            //screwdriver.AddDataset("ttbar_madgraph_matchingdown", "1ltop",  0, 0);
            //screwdriver.AddDataset("ttbar_madgraph_matchingup",   "1ltop",  0, 0);
            //screwdriver.AddDataset("ttbar_madgraph_mass166-5",    "1ltop",  0, 0);
            //screwdriver.AddDataset("ttbar_madgraph_mass178-5",    "1ltop",  0, 0);

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

    // ##########################
    // ##    Create Regions    ##
    // ##########################

        screwdriver.AddRegion("preveto_MTpeak",          "Preselection (MT peak)",      &goesInPreVetoSelectionMTpeak_withSRCuts);
        screwdriver.AddRegion("preveto_MTtail",          "Preselection (MT tail)",      &goesInPreVetoSelectionMTtail_withSRCuts);

        screwdriver.AddRegion("signalRegion_MTpeak",     "Preselection (MT peak)",      &goesInPreselectionMTpeak_withSRCuts);
        screwdriver.AddRegion("signalRegion_MTtail",     "Preselection (MT tail)",      &goesInPreselectionMTtail_withSRCuts, "blinded");

        screwdriver.AddRegion("0btag_MTpeak",            "0 b-tag (MT peak)",           &goesIn0BtagControlRegionMTpeak_withSRCuts);
        screwdriver.AddRegion("0btag_MTtail",            "0 b-tag (MT tail)",           &goesIn0BtagControlRegionMTtail_withSRCuts);

        screwdriver.AddRegion("2leptons",                "2 leptons",                   &goesInDileptonControlRegion_withSRCuts);
        screwdriver.AddRegion("2leptons_MTpeak",         "2 leptons (MT peak)",         &goesInDileptonControlRegionMTpeak_withSRCuts);
        screwdriver.AddRegion("2leptons_MTtail",         "2 leptons (MT tail)",         &goesInDileptonControlRegionMTtail_withSRCuts);
        screwdriver.AddRegion("2leptons_1or2jets",       "2 leptons (1 or 2 jets)",     &goesInDileptonControlRegion1or2jets_withSRCuts);
        screwdriver.AddRegion("2leptons_3jets",          "2 leptons (3 jets)",          &goesInDileptonControlRegion3jets_withSRCuts);
        screwdriver.AddRegion("2leptons_atLeast4jets",   "2 leptons (at least 4 jets)", &goesInDileptonControlRegionAtLeast4jets_withSRCuts);
        
        screwdriver.AddRegion("reversedVeto_MTpeak",     "Reversed veto (MT peak)",     &goesInVetoControlRegionMTpeak_withSRCuts);
        screwdriver.AddRegion("reversedVeto_MTtail",     "Reversed veto (MT tail)",     &goesInVetoControlRegionMTtail_withSRCuts);
        
    // ##########################
    // ##   Create Channels    ##
    // ##########################

        /*
        screwdriver.AddChannel("singleLepton", "e/#mu-channels",  &goesInSingleLeptonChannel);
        screwdriver.AddChannel("singleElec",   "e-channel",       &goesInSingleElecChannel  );
        screwdriver.AddChannel("singleMuon",   "#mu-channel",     &goesInSingleMuonChannel  );
        
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

            intermediatePointers pointers;
            InitializeBranchesForReading(theTree,&myEvent,&pointers);

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
                ReadEvent(theTree,i,&pointers,&myEvent);
    
                if (CustomJetRequirement != "")
                {
                    if (CustomJetRequirement == "CR4_2j")         if (myEvent.nJets < 2) continue;
                    if (CustomJetRequirement == "CR4_3j")         if (myEvent.nJets < 3) continue;
                    if (CustomJetRequirement == "CR4_4j"
                     || CustomJetRequirement == "CR4_4j_50evts" 
                     || CustomJetRequirement == "CR4_4j_100evts" 
                     || CustomJetRequirement == "CR4_4j_150evts") if (myEvent.nJets < 4) continue; 
                }
                
                float weight = getWeight();

                // Split 1-lepton ttbar and 2-lepton ttbar
                string currentProcessClass_ = currentProcessClass;
                if (ttbarDatasetToBeSplitted && (myEvent.numberOfGenLepton == 2))
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

        vector<string> regions  = { "preveto_MTpeak",      "preveto_MTtail",      
                                    "signalRegion_MTpeak", "signalRegion_MTtail", 
                                    "0btag_MTpeak",        "0btag_MTtail",  
                                    "reversedVeto_MTpeak", "reversedVeto_MTtail",
                                    "2leptons",            "2leptons_MTtail",     "2leptons_MTpeak",
                                    "2leptons_1or2jets",   "2leptons_3jets",      "2leptons_atLeast4jets",
                                  };

        string exportFile;
        if (CustomJetRequirement != "")
        {
            system(("mkdir -p rawYieldTables/"+CustomJetRequirement).c_str());
            exportFile = "rawYieldTables/"+CustomJetRequirement+"/"+string(SIGNAL_REGION_TAG)+".tab";
        }
        else
            exportFile = "rawYieldTables/"+string(SIGNAL_REGION_TAG)+".tab";

        TableDataMC(&screwdriver,regions,"allChannels").Print(exportFile,4);

        printBoxedMessage("Table generation completed");

        return (0);
}


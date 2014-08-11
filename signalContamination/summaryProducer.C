#include "../backgroundEstimation_common/common.h"

// #########################################################################
//                              Main function
// #########################################################################

int main (int argc, char *argv[])
{
    loadBDTSignalRegions();
    loadCnCSignalRegions();

    printBoxedMessage("Starting plot generation");

    // ####################
    // ##   Init tools   ##
    // ####################

    // Create a sonic Screwdriver
    SonicScrewdriver screwdriver;

    // ##########################
    // ##    Create Regions    ##
    // ##########################

    string globalTag = argv[1];
    string globalLabel = argv[2];

    vector<string> signalRegionsTagList;
    vector<string> signalRegionsLabelList;
    for (int i = 3 ; i < argc ; i++)
    {
        signalRegionsTagList.push_back(argv[i]);

        string label = signalRegionLabel(argv[i],"root");
        label = label.substr(label.find(", ")+2);
        signalRegionsLabelList.push_back(label);
    }

    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        screwdriver.AddRegion(signalRegionsTagList[i],
                              signalRegionsLabelList[i],
                              0);
    }

    // ##########################
    // ##   Create Channels    ##
    // ##########################

    screwdriver.AddChannel("contamination", "Signal contamination level;"+globalLabel,  0);

    // ########################
    // ##   Create Figures   ##
    // ########################

    screwdriver.AddFigure("MTpeak",               "M_{T} peak",            "");
    screwdriver.AddFigure("0btag_MTpeak",         "0 b-tag, M_{T} peak",   "");
    screwdriver.AddFigure("0btag_MTtail",         "0 b-tag, M_{T} tail",   "");
    screwdriver.AddFigure("2leptons",             "2 leptons",             "");
    screwdriver.AddFigure("reversedVeto_MTpeak",  "1l+veto, M_{T} peak",   "");
    screwdriver.AddFigure("reversedVeto_MTtail",  "1l+veto, M_{T} tail",   "");

    // ########################################
    // ##       Create histograms and        ##
    // ##  schedule type of plots to produce ##
    // ########################################

    // Schedule plots
    screwdriver.SchedulePlots("1DFigure","name=contamination,figures=MTpeak:0btag_MTpeak:0btag_MTtail:2leptons:reversedVeto_MTpeak:reversedVeto_MTtail,channel=contamination,min=0,max=1.2");

    // Config plots
    screwdriver.SetGlobalStringOption("1DStackFigurePerProcess",  "includeSignal",   "stack");
    screwdriver.SetGlobalFloatOption ("1DStackFigurePerProcess",  "factorSignal",      1.0  );

    screwdriver.SetGlobalStringOption("Plot", "infoTopRight", "CMS Internal");
    screwdriver.SetGlobalStringOption("Plot", "infoTopLeft",  "#sqrt{s} = 8 TeV, L = 19.5 fb^{-1}");

    screwdriver.SetGlobalBoolOption("Plot", "exportPdf", true);
    screwdriver.SetGlobalBoolOption("Plot", "exportEps", false);
    screwdriver.SetGlobalBoolOption("Plot", "exportPng", false);

    // #########################
    // ##   Set the figures   ##
    // #########################

    // Loop on the signal regions and set the figure values...
    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        // Read input tables
        Table contaminationTable = Table(string("./rawTables/") +signalRegionsTagList[i]+".tab");

        string signalRegionTag_ = signalRegionsTagList[i];

        Figure contamination_MTpeak                 = contaminationTable.Get("MTpeak",              signalRegionTag_);
        Figure contamination_0btag_MTpeak           = contaminationTable.Get("0btag_MTpeak",        signalRegionTag_);
        Figure contamination_0btag_MTtail           = contaminationTable.Get("0btag_MTtail",        signalRegionTag_);
        Figure contamination_2leptons               = contaminationTable.Get("2leptons",            signalRegionTag_);
        Figure contamination_reversedVeto_MTpeak    = contaminationTable.Get("reversedVeto_MTpeak", signalRegionTag_);
        Figure contamination_reversedVeto_MTtail    = contaminationTable.Get("reversedVeto_MTtail", signalRegionTag_);

        screwdriver.SetFigure("MTpeak",              signalRegionsTagList[i], "contamination",  contamination_MTpeak             );
        screwdriver.SetFigure("0btag_MTpeak",        signalRegionsTagList[i], "contamination",  contamination_0btag_MTpeak       );
        screwdriver.SetFigure("0btag_MTtail",        signalRegionsTagList[i], "contamination",  contamination_0btag_MTtail       );
        screwdriver.SetFigure("2leptons",            signalRegionsTagList[i], "contamination",  contamination_2leptons           );
        screwdriver.SetFigure("reversedVeto_MTpeak", signalRegionsTagList[i], "contamination",  contamination_reversedVeto_MTpeak);
        screwdriver.SetFigure("reversedVeto_MTtail", signalRegionsTagList[i], "contamination",  contamination_reversedVeto_MTtail);
    }

    // ##############################
    // ##   Make and write plots   ##
    // ##############################

    cout << endl;
    cout << "   > Making plots..." << endl;
    screwdriver.MakePlots();
    cout << "   > Saving plots..." << endl;
    screwdriver.WritePlots("./summary/"+globalTag+"/");

    printBoxedMessage("Plot generation completed");

    // #############################
    // ##   Post-plotting tests   ##
    // #############################

    printBoxedMessage("Program done.");
    return (0);
}


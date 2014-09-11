#include "common.h"

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

    // #########################################################
    // ##   Create ProcessClasses (and associated datasets)   ##
    // #########################################################

    // ##########################
    // ##    Create Regions    ##
    // ##########################

    string globalTag = argv[1];
    string globalLabel = argv[2];

    vector<string> signalRegionsTagList;
    for (int i = 3 ; i < argc ; i++)
    {
        signalRegionsTagList.push_back(argv[i]);
    }

    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        screwdriver.AddRegion(signalRegionsTagList[i],
                              signalRegionsTagList[i],
                              0);
    }

    // ##########################
    // ##   Create Channels    ##
    // ##########################

    screwdriver.AddChannel("MTtail",    globalLabel+";Raw MT tail correction scale factors",       0);

    // ########################
    // ##   Create Figures   ##
    // ########################

    screwdriver.AddFigure("SFR_1ltop",   "SFR_{1l top}",  "");
    screwdriver.AddFigure("SFR_Wjets",   "SFR_{W+jets}",  "");

    // ########################################
    // ##       Create histograms and        ##
    // ##  schedule type of plots to produce ##
    // ########################################

    // Read input tables
    Table tableMTtailCorrection = Table("./results/latest/MTtailCorrection/rawSFR.tab");

    if (globalTag == "BDT")
    {
        Figure average1ltop = tableMTtailCorrection.Get("SFR_1ltop","BDT_average");
        Figure averageWjets = tableMTtailCorrection.Get("SFR_Wjets","BDT_average");

        string additionalAverageLine1ltop = string(",lineValue=")+floatToString(average1ltop.value())
                                                  +",lineUncert="+floatToString(average1ltop.error());
        string additionalAverageLineWjets = string(",lineValue=")+floatToString(averageWjets.value())
                                                  +",lineUncert="+floatToString(averageWjets.error());

        screwdriver.SchedulePlots("1DFigure","name=1ltop,figures=SFR_1ltop,channel=MTtail,min=0,max=4"+additionalAverageLine1ltop);
        screwdriver.SchedulePlots("1DFigure","name=Wjets,figures=SFR_Wjets,channel=MTtail,min=0,max=4"+additionalAverageLineWjets);
    }
    else
    {
        screwdriver.SchedulePlots("1DFigure","name=1ltopAndWjets,figures=SFR_Wjets:SFR_1ltop,channel=MTtail,min=0,max=4");
    }

    // Schedule plots

    // Config plots
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

        Figure SFR_1ltop = tableMTtailCorrection.Get("SFR_1ltop",signalRegionsTagList[i]);
        Figure SFR_Wjets = tableMTtailCorrection.Get("SFR_Wjets",signalRegionsTagList[i]);

        screwdriver.SetFigure("SFR_1ltop", signalRegionsTagList[i], "MTtail",   SFR_1ltop);
        screwdriver.SetFigure("SFR_Wjets", signalRegionsTagList[i], "MTtail",   SFR_Wjets);
    }

    // ##############################
    // ##   Make and write plots   ##
    // ##############################

    cout << endl;
    cout << "   > Making plots..." << endl;
    screwdriver.MakePlots();
    cout << "   > Saving plots..." << endl;
    screwdriver.WritePlots("./results/latest/summaryPlots/MTtailCorrection/"+globalTag+"/");

    printBoxedMessage("Plot generation completed");

    // #############################
    // ##   Post-plotting tests   ##
    // #############################

    printBoxedMessage("Program done.");
    return (0);
}


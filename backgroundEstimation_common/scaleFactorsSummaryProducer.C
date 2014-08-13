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
    vector<string> signalRegionsLabelList;
    for (int i = 3 ; i < argc ; i++)
    {
        signalRegionsTagList.push_back(argv[i]);
        string label = signalRegionLabel(argv[i],"root");
        replace(label, "BDT T2tt-",     "BDT ");
        replace(label, "BDT T2bw025-",  "BDT ");
        replace(label, "BDT T2bw050-",  "BDT ");
        replace(label, "BDT T2bw075-",  "BDT ");
        replace(label, "T2tt, ",        ""    );
        replace(label, "T2bw x=0.25, ", ""    );
        replace(label, "T2bw x=0.50, ", ""    );
        replace(label, "T2bw x=0.75, ", ""    );
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

    screwdriver.AddChannel("MTpeak",    globalLabel+";MT peak normalization scale factors",    0);
    screwdriver.AddChannel("MTtail",    globalLabel+";MT tail correction scale factors",       0);

    // ########################
    // ##   Create Figures   ##
    // ########################

    screwdriver.AddFigure("SF_peak_pre",    "pre-veto",     "");
    screwdriver.AddFigure("SF_peak_post",   "post-veto",    "");
    screwdriver.AddFigure("SF_peak_0btag",  "0 btag",       "");
    screwdriver.AddFigure("SF_peak_veto",   "reversed-veto","");
    screwdriver.AddFigure("SF_tail_1ltop",  "1l top",       "");
    screwdriver.AddFigure("SF_tail_Wjets",  "W+jets",       "");

    // ########################################
    // ##       Create histograms and        ##
    // ##  schedule type of plots to produce ##
    // ########################################

    // Schedule plots
    screwdriver.SchedulePlots("1DFigure","name=peakNormalization,figures=SF_peak_pre:SF_peak_post:SF_peak_0btag:SF_peak_veto,channel=MTpeak,min=0,max=4");
    screwdriver.SchedulePlots("1DFigure","name=tailCorrection,figures=SF_tail_1ltop:SF_tail_Wjets,channel=MTtail,min=0,max=5");

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
        Table scaleFactors = Table(string("./results/latest/prediction/scaleFactors/") +signalRegionsTagList[i]+".tab");
        Table tableMTtailCorrection = Table("./results/latest/MTtailCorrection/SF_MTtail.tab");

        string signalRegionTag_ = signalRegionsTagList[i];
        if (findSubstring(signalRegionTag_,"BDT"))
        {
            size_t pos;
            pos = signalRegionTag_.find("_low");
            if (pos != string::npos) signalRegionTag_ = signalRegionTag_.substr(0,pos);
            pos = signalRegionTag_.find("_medium");
            if (pos != string::npos) signalRegionTag_ = signalRegionTag_.substr(0,pos);
            pos = signalRegionTag_.find("_high");
            if (pos != string::npos) signalRegionTag_ = signalRegionTag_.substr(0,pos);
        }
        Figure SF_MTtail_Wjets = tableMTtailCorrection.Get("SFR_Wjets",signalRegionTag_);
        Figure SF_MTtail_1ltop = tableMTtailCorrection.Get("SFR_1ltop",signalRegionTag_);

        Figure SF_pre           = scaleFactors.Get("value","SF_pre");
        Figure SF_post          = scaleFactors.Get("value","SF_post");
        Figure SF_0btag         = scaleFactors.Get("value","SF_0btag");
        Figure SF_vetopeak      = scaleFactors.Get("value","SF_vetopeak");

        screwdriver.SetFigure("SF_peak_pre",   signalRegionsTagList[i], "MTpeak",   SF_pre);
        screwdriver.SetFigure("SF_peak_post",  signalRegionsTagList[i], "MTpeak",   SF_post);
        screwdriver.SetFigure("SF_peak_0btag", signalRegionsTagList[i], "MTpeak",   SF_0btag);
        screwdriver.SetFigure("SF_peak_veto",  signalRegionsTagList[i], "MTpeak",   SF_vetopeak);
        screwdriver.SetFigure("SF_tail_1ltop", signalRegionsTagList[i], "MTtail",   SF_MTtail_1ltop);
        screwdriver.SetFigure("SF_tail_Wjets", signalRegionsTagList[i], "MTtail",   SF_MTtail_Wjets);
    }

    // ##############################
    // ##   Make and write plots   ##
    // ##############################

    cout << endl;
    cout << "   > Making plots..." << endl;
    screwdriver.MakePlots();
    cout << "   > Saving plots..." << endl;
    screwdriver.WritePlots("./results/latest/summaryPlots/scaleFactors/"+globalTag+"/");

    printBoxedMessage("Plot generation completed");

    // #############################
    // ##   Post-plotting tests   ##
    // #############################

    printBoxedMessage("Program done.");
    return (0);
}


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

    screwdriver.AddProcessClass("1ltop",    "1l top",                            "background",  kRed-7);
    screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}",   "background",  kCyan-3);
    screwdriver.AddProcessClass("W+jets",   "W+jets",                            "background",  kOrange-2);
    screwdriver.AddProcessClass("rare",     "rare",                              "background",  kMagenta-5);
    screwdriver.AddProcessClass("data",     "data",                              "data",        COLORPLOT_BLACK);

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

    screwdriver.AddChannel("signalRegion_MTpeak",   globalLabel+";Preselection;M_{T} peak",   0);
    screwdriver.AddChannel("signalRegion_MTtail",   globalLabel+";Preselection;M_{T} tail",   0);

    screwdriver.AddChannel("0btag_MTpeak",          globalLabel+";0 b-tag Control Region;M_{T} peak",        0);
    screwdriver.AddChannel("0btag_MTtail",          globalLabel+";0 b-tag Control Region;M_{T} tail",        0);

    screwdriver.AddChannel("reversedVeto_MTpeak",   globalLabel+";Reversed veto Control Region;M_{T} peak",  0);
    screwdriver.AddChannel("reversedVeto_MTtail",   globalLabel+";Reversed veto Control Region;M_{T} tail",  0);

    screwdriver.AddChannel("2leptons",              globalLabel+";2 leptons Control Region;No M_{T} cut",                0);
    screwdriver.AddChannel("2leptons_MTpeak",       globalLabel+";2 leptons Control Region;M_{T} peak",      0);
    screwdriver.AddChannel("2leptons_MTtail",       globalLabel+";2 leptons Control Region;M_{T} tail",      0);

    // ########################
    // ##   Create Figures   ##
    // ########################

    screwdriver.AddFigurePerProcess("yield","Yield", "logY");

    // ########################################
    // ##       Create histograms and        ##
    // ##  schedule type of plots to produce ##
    // ########################################

    // Schedule plots
    screwdriver.SchedulePlots("1DDataMCComparisonFigure");

    // Config plots

    screwdriver.SetGlobalStringOption("1DStackFigurePerProcess",  "includeSignal",                    "stack");
    screwdriver.SetGlobalFloatOption ("1DStackFigurePerProcess",  "factorSignal",                     1.0    );

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
        Table rawYields    = Table(string("./results/latest/prediction/rawYieldTables/")+signalRegionsTagList[i]+".tab");
        Table scaleFactors = Table(string("./results/latest/prediction/scaleFactors/")  +signalRegionsTagList[i]+".tab");
        Table tableMTtailCorrection = Table("./results/latest/MTtailCorrection/SF_MTtail.tab");

        Figure SF_pre           = scaleFactors.Get("value","SF_pre");
        Figure SF_post          = scaleFactors.Get("value","SF_post");
        Figure SF_0btag         = scaleFactors.Get("value","SF_0btag");
        Figure SF_vetopeak      = scaleFactors.Get("value","SF_vetopeak");

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

        screwdriver.SetFigure("yield", "1ltop",    signalRegionsTagList[i], "signalRegion_MTpeak", rawYields.Get("signalRegion_MTpeak","1ltop"   ) * SF_post);
        screwdriver.SetFigure("yield", "ttbar_2l", signalRegionsTagList[i], "signalRegion_MTpeak", rawYields.Get("signalRegion_MTpeak","ttbar_2l") * SF_pre );
        screwdriver.SetFigure("yield", "W+jets",   signalRegionsTagList[i], "signalRegion_MTpeak", rawYields.Get("signalRegion_MTpeak","W+jets"  ) * SF_post);
        screwdriver.SetFigure("yield", "rare",     signalRegionsTagList[i], "signalRegion_MTpeak", rawYields.Get("signalRegion_MTpeak","rare"    ));
        screwdriver.SetFigure("yield", "data",     signalRegionsTagList[i], "signalRegion_MTpeak", rawYields.Get("signalRegion_MTpeak","data"    ));

        screwdriver.SetFigure("yield", "1ltop",    signalRegionsTagList[i], "signalRegion_MTtail", rawYields.Get("signalRegion_MTtail","1ltop"   ) * SF_post * SF_MTtail_1ltop);
        screwdriver.SetFigure("yield", "ttbar_2l", signalRegionsTagList[i], "signalRegion_MTtail", rawYields.Get("signalRegion_MTtail","ttbar_2l") * SF_pre                   );
        screwdriver.SetFigure("yield", "W+jets",   signalRegionsTagList[i], "signalRegion_MTtail", rawYields.Get("signalRegion_MTtail","W+jets"  ) * SF_post * SF_MTtail_Wjets);
        screwdriver.SetFigure("yield", "rare",     signalRegionsTagList[i], "signalRegion_MTtail", rawYields.Get("signalRegion_MTtail","rare"    ));
        screwdriver.SetFigure("yield", "data",     signalRegionsTagList[i], "signalRegion_MTtail", rawYields.Get("signalRegion_MTtail","data"    ));

        screwdriver.SetFigure("yield", "1ltop",    signalRegionsTagList[i], "0btag_MTpeak",        rawYields.Get("0btag_MTpeak","1ltop"   ) * SF_0btag);
        screwdriver.SetFigure("yield", "ttbar_2l", signalRegionsTagList[i], "0btag_MTpeak",        rawYields.Get("0btag_MTpeak","ttbar_2l"));
        screwdriver.SetFigure("yield", "W+jets",   signalRegionsTagList[i], "0btag_MTpeak",        rawYields.Get("0btag_MTpeak","W+jets"  ) * SF_0btag);
        screwdriver.SetFigure("yield", "rare",     signalRegionsTagList[i], "0btag_MTpeak",        rawYields.Get("0btag_MTpeak","rare"    ));
        screwdriver.SetFigure("yield", "data",     signalRegionsTagList[i], "0btag_MTpeak",        rawYields.Get("0btag_MTpeak","data"    ));

        screwdriver.SetFigure("yield", "1ltop",    signalRegionsTagList[i], "0btag_MTtail",        rawYields.Get("0btag_MTtail","1ltop"   ) * SF_0btag * SF_MTtail_1ltop);
        screwdriver.SetFigure("yield", "ttbar_2l", signalRegionsTagList[i], "0btag_MTtail",        rawYields.Get("0btag_MTtail","ttbar_2l"));
        screwdriver.SetFigure("yield", "W+jets",   signalRegionsTagList[i], "0btag_MTtail",        rawYields.Get("0btag_MTtail","W+jets"  ) * SF_0btag * SF_MTtail_Wjets);
        screwdriver.SetFigure("yield", "rare",     signalRegionsTagList[i], "0btag_MTtail",        rawYields.Get("0btag_MTtail","rare"    ));
        screwdriver.SetFigure("yield", "data",     signalRegionsTagList[i], "0btag_MTtail",        rawYields.Get("0btag_MTtail","data"    ));

        screwdriver.SetFigure("yield", "1ltop",    signalRegionsTagList[i], "reversedVeto_MTpeak", rawYields.Get("reversedVeto_MTpeak","1ltop"   ) * SF_vetopeak);
        screwdriver.SetFigure("yield", "ttbar_2l", signalRegionsTagList[i], "reversedVeto_MTpeak", rawYields.Get("reversedVeto_MTpeak","ttbar_2l") * SF_pre );
        screwdriver.SetFigure("yield", "W+jets",   signalRegionsTagList[i], "reversedVeto_MTpeak", rawYields.Get("reversedVeto_MTpeak","W+jets"  ) * SF_vetopeak);
        screwdriver.SetFigure("yield", "rare",     signalRegionsTagList[i], "reversedVeto_MTpeak", rawYields.Get("reversedVeto_MTpeak","rare"    ));
        screwdriver.SetFigure("yield", "data",     signalRegionsTagList[i], "reversedVeto_MTpeak", rawYields.Get("reversedVeto_MTpeak","data"    ));

        screwdriver.SetFigure("yield", "1ltop",    signalRegionsTagList[i], "reversedVeto_MTtail", rawYields.Get("reversedVeto_MTtail","1ltop"   ) * SF_vetopeak * SF_MTtail_1ltop);
        screwdriver.SetFigure("yield", "ttbar_2l", signalRegionsTagList[i], "reversedVeto_MTtail", rawYields.Get("reversedVeto_MTtail","ttbar_2l") * SF_pre );
        screwdriver.SetFigure("yield", "W+jets",   signalRegionsTagList[i], "reversedVeto_MTtail", rawYields.Get("reversedVeto_MTtail","W+jets"  ) * SF_vetopeak * SF_MTtail_Wjets);
        screwdriver.SetFigure("yield", "rare",     signalRegionsTagList[i], "reversedVeto_MTtail", rawYields.Get("reversedVeto_MTtail","rare"    ));
        screwdriver.SetFigure("yield", "data",     signalRegionsTagList[i], "reversedVeto_MTtail", rawYields.Get("reversedVeto_MTtail","data"    ));

        screwdriver.SetFigure("yield", "1ltop",    signalRegionsTagList[i], "2leptons_MTpeak",     rawYields.Get("2leptons_MTpeak","1ltop"   ));
        screwdriver.SetFigure("yield", "ttbar_2l", signalRegionsTagList[i], "2leptons_MTpeak",     rawYields.Get("2leptons_MTpeak","ttbar_2l"));
        screwdriver.SetFigure("yield", "W+jets",   signalRegionsTagList[i], "2leptons_MTpeak",     rawYields.Get("2leptons_MTpeak","W+jets"  ));
        screwdriver.SetFigure("yield", "rare",     signalRegionsTagList[i], "2leptons_MTpeak",     rawYields.Get("2leptons_MTpeak","rare"    ));
        screwdriver.SetFigure("yield", "data",     signalRegionsTagList[i], "2leptons_MTpeak",     rawYields.Get("2leptons_MTpeak","data"    ));

        screwdriver.SetFigure("yield", "1ltop",    signalRegionsTagList[i], "2leptons_MTtail",     rawYields.Get("2leptons_MTtail","1ltop"   ));
        screwdriver.SetFigure("yield", "ttbar_2l", signalRegionsTagList[i], "2leptons_MTtail",     rawYields.Get("2leptons_MTtail","ttbar_2l"));
        screwdriver.SetFigure("yield", "W+jets",   signalRegionsTagList[i], "2leptons_MTtail",     rawYields.Get("2leptons_MTtail","W+jets"  ));
        screwdriver.SetFigure("yield", "rare",     signalRegionsTagList[i], "2leptons_MTtail",     rawYields.Get("2leptons_MTtail","rare"    ));
        screwdriver.SetFigure("yield", "data",     signalRegionsTagList[i], "2leptons_MTtail",     rawYields.Get("2leptons_MTtail","data"    ));

        screwdriver.SetFigure("yield", "1ltop",    signalRegionsTagList[i], "2leptons",            rawYields.Get("2leptons","1ltop"   ));
        screwdriver.SetFigure("yield", "ttbar_2l", signalRegionsTagList[i], "2leptons",            rawYields.Get("2leptons","ttbar_2l"));
        screwdriver.SetFigure("yield", "W+jets",   signalRegionsTagList[i], "2leptons",            rawYields.Get("2leptons","W+jets"  ));
        screwdriver.SetFigure("yield", "rare",     signalRegionsTagList[i], "2leptons",            rawYields.Get("2leptons","rare"    ));
        screwdriver.SetFigure("yield", "data",     signalRegionsTagList[i], "2leptons",            rawYields.Get("2leptons","data"    ));
    }

    // ##############################
    // ##   Make and write plots   ##
    // ##############################

    cout << endl;
    cout << "   > Making plots..." << endl;
    screwdriver.MakePlots();
    cout << "   > Saving plots..." << endl;
    screwdriver.WritePlots("./results/latest/summaryPlots/yields/"+globalTag+"/");

    printBoxedMessage("Plot generation completed");

    // #############################
    // ##   Post-plotting tests   ##
    // #############################

    printBoxedMessage("Program done.");
    return (0);
}


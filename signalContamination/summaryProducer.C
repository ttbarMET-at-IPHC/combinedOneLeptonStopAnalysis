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

    bool runningOnCnC = findSubstring(globalTag,"CnC");

    vector<string> signalRegionsTagList;
    vector<string> signalRegionsLabelList;
    vector<string> signalRegionsLatexLabelList;

    for (int i = 3 ; i < argc ; i++)
    {
        signalRegionsTagList.push_back(argv[i]);

        string label = signalRegionLabel(argv[i],"root");
        if (runningOnCnC) label = label.substr(label.find(", ")+2);
        else           {  label = label.substr(4); label = "BDT"+label.substr(label.find("-")); }

        string latexLabel = signalRegionLabel(argv[i],"latex");
        signalRegionsLabelList.push_back(label);
        signalRegionsLatexLabelList.push_back(latexLabel);
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

    screwdriver.AddFigure("MTpeak",        "M_{T} peak",    "");
    screwdriver.AddFigure("signalRegion",  "Signal region", "");
    screwdriver.AddFigure("ratioMTpeak",   "MTpeak / SR",   "");
    if (runningOnCnC)
    {
        screwdriver.AddFigure("0btag",        "0 b-tag",       "");
        screwdriver.AddFigure("ratio0btag",   "0 b-tag / SR",  "");
    }

    // ########################################
    // ##       Create histograms and        ##
    // ##  schedule type of plots to produce ##
    // ########################################

    // Schedule plots
    screwdriver.SchedulePlots("1DFigure","name=MTpeak,figures=MTpeak,channel=contamination,min=0,max=0.4");
    screwdriver.SchedulePlots("1DFigure","name=signalRegion,figures=signalRegion,channel=contamination,min=0,max=5");
    screwdriver.SchedulePlots("1DFigure","name=ratioMTpeak,figures=ratioMTpeak,channel=contamination,min=0,max=1.2");
    if (runningOnCnC)
    {
        screwdriver.SchedulePlots("1DFigure","name=0btag,figures=0btag,channel=contamination,min=0,max=0.4");
        screwdriver.SchedulePlots("1DFigure","name=ratio0btag,figures=ratio0btag,channel=contamination,min=0,max=1.2");
    }

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

    Table contamination0btagTable = Table("./rawTables/0btag.tab");

    vector<string> columns = { "MT peak" };
    if (runningOnCnC) columns.push_back("0 b-tag");

    Table summaryTable(columns, signalRegionsTagList, columns, signalRegionsLatexLabelList );

    // Loop on the signal regions and set the figure values...
    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        // Read input tables
        Table contaminationTable = Table(string("./rawTables/") +signalRegionsTagList[i]+".tab");

        string signalRegionTag_ = signalRegionsTagList[i];

        Figure contamination_MTpeak                = contaminationTable.Get("MTpeak",              signalRegionTag_);
        Figure contamination_signalRegion          = contaminationTable.Get("signalRegion",        signalRegionTag_);

        screwdriver.SetFigure("MTpeak",        signalRegionsTagList[i], "contamination",  contamination_MTpeak);
        screwdriver.SetFigure("signalRegion",  signalRegionsTagList[i], "contamination",  contamination_signalRegion);
        screwdriver.SetFigure("ratioMTpeak",   signalRegionsTagList[i], "contamination",  contamination_MTpeak/contamination_signalRegion);

        summaryTable.Set("MT peak", signalRegionsTagList[i], contamination_MTpeak/contamination_signalRegion);

        if (runningOnCnC)
        {
            Figure contamination_0btag             = contamination0btagTable.Get("MTcut",          signalRegionTag_);
            screwdriver.SetFigure("0btag",         signalRegionsTagList[i], "contamination",  contamination_0btag);
            screwdriver.SetFigure("ratio0btag",    signalRegionsTagList[i], "contamination",  contamination_0btag/contamination_signalRegion);

            summaryTable.Set("0 b-tag", signalRegionsTagList[i], contamination_0btag /contamination_signalRegion);
        }

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

    summaryTable.PrintLatex("summary/"+globalTag+"/summary.tex");

    printBoxedMessage("Program done.");
    return (0);
}


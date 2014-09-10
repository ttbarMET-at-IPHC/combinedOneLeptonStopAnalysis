#include "common.h"

int main (int argc, char *argv[])
{
    loadBDTSignalRegions();
    loadCnCSignalRegions();

    if (argc <= 3)
    {
        WARNING_MSG << "Arguments : [report_tag] [report_label] [list of regions]" << endl;
        return -1;
    }

    // Read tag and label

    string reportTag   = argv[1];
    string reportLabel = argv[2];

    // Read list of signal regions to consider

    vector<string> signalRegionsTagList;
    for (int i = 3 ; i < argc ; i++)
        signalRegionsTagList.push_back(argv[i]);

    // Create list of label for each region

    vector<string> signalRegionsLabelList;
    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        string label = signalRegionLabel(signalRegionsTagList[i],"latex");
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

    // Read tables for each signal regions

    vector<Table> predictions;
    vector<Table> scaleFactors;
    vector<Table> systematics;
    vector<Table> dataInput;

    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        predictions .push_back(Table("results/latest/prediction/"               +signalRegionsTagList[i]+".tab"));
        scaleFactors.push_back(Table("results/latest/prediction/scaleFactors/"  +signalRegionsTagList[i]+".tab"));
        systematics .push_back(Table("results/latest/prediction/systematics/"   +signalRegionsTagList[i]+".tab"));
        dataInput   .push_back(Table("results/latest/prediction/rawYieldTables/"+signalRegionsTagList[i]+".tab"));
    }

    // Fill summary tables

    vector<string> processesWithDataTagList   = processesTagList;
    vector<string> processesWithDataLabelList = processesLabelList;

    processesWithDataTagList  .push_back("data");
    processesWithDataLabelList.push_back("data");

    Table rawYieldSummary           (signalRegionsTagList, processesTagList,         signalRegionsLabelList,  processesLabelList         );
    Table predictionSummary         (signalRegionsTagList, processesWithDataTagList, signalRegionsLabelList,  processesWithDataLabelList );
    Table scaleFactorsSummary       (signalRegionsTagList, scaleFactorsTagList,      signalRegionsLabelList,  scaleFactorsLabelList      );
    Table absoluteSystematicsSummary(signalRegionsTagList, systematicsTagList,       signalRegionsLabelList,  systematicsLabelList       );
    Table relativeSystematicsSummary(signalRegionsTagList, systematicsTagList,       signalRegionsLabelList,  systematicsLabelList       );

    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        string signalRegion = signalRegionsTagList[i];

        // Fill the raw vs prediction table
        for (unsigned int j = 0 ; j < processesTagList.size() ; j++)
        {
            string processTag = processesTagList[j];

            rawYieldSummary.Set(signalRegion,processTag,predictions[i].Get("raw_mc",processTag));

            Figure pred = predictions[i].Get("prediction",processTag);

            predictionSummary.Set(signalRegion,processTag,pred);
        }
        Figure data = dataInput[i].Get("signalRegion_MTtail","data");
        predictionSummary.Set(signalRegion,"data",data);

        // Fill the scale factors table
        for (unsigned int j = 0 ; j < scaleFactorsTagList.size() ; j++)
        {
            string scaleFactorTag = scaleFactorsTagList[j];
            scaleFactorsSummary.Set(signalRegion,scaleFactorTag,scaleFactors[i].Get("value",scaleFactorTag));
        }

        // Fill the systematics table
        for (unsigned int j = 0 ; j < systematicsTagList.size() ; j++)
        {
            string systematicTag = systematicsTagList[j];
            absoluteSystematicsSummary.Set(signalRegion,systematicTag,systematics[i].Get("absolute",systematicTag));
            relativeSystematicsSummary.Set(signalRegion,systematicTag,systematics[i].Get("relative",systematicTag) * 100);
        }
    }

    cout << "\\begin{table}[!ht]" << endl;
    cout << "\\begin{center}" << endl;
    cout << "{ \\footnotesize" << endl;
    rawYieldSummary.PrintLatex(2);
    cout << "}" << endl;
    cout << "\\caption{Raw expected MC yields for " << reportLabel << " signal regions. \\label{tab:report_raw_" << reportTag << "}}" << endl;
    cout << "\\end{center}" << endl;
    cout << "\\end{table}" << endl;

    cout << endl << endl;

    cout << "\\begin{table}[!ht]" << endl;
    cout << "\\begin{center}" << endl;
    cout << "{ \\footnotesize" << endl;
    scaleFactorsSummary.PrintLatex(2);
    cout << "}" << endl;
    cout << "\\caption{Scale factors derived for the " << reportLabel << " signal regions. \\label{tab:report_sf_" << reportTag << "}}" << endl;
    cout << "\\end{center}" << endl;
    cout << "\\end{table}" << endl;

    cout << endl << endl;

    cout << "\\begin{table}[!ht]" << endl;
    cout << "\\begin{center}" << endl;
    cout << "{ \\footnotesize" << endl;
    predictionSummary.PrintLatex(2);
    cout << "}" << endl;
    cout << "\\caption{Background prediction for the " << reportLabel << " signal regions. \\label{tab:report_yield_" << reportTag << "}}" << endl;
    cout << "\\end{center}" << endl;
    cout << "\\end{table}" << endl;

    cout << endl << endl;

    cout << "\\begin{table}[!ht]" << endl;
    cout << "\\begin{center}" << endl;
    cout << "{ \\footnotesize" << endl;
    absoluteSystematicsSummary.PrintLatex(1,"noError");
    cout << "}" << endl;
    cout << "\\caption{Absolute uncertainties for the " << reportLabel << " signal regions. \\label{tab:report_abs_" << reportTag << "}}" << endl;
    cout << "\\end{center}" << endl;
    cout << "\\end{table}" << endl;

    cout << endl << endl;

    cout << "\\begin{table}[!ht]" << endl;
    cout << "\\begin{center}" << endl;
    cout << "{ \\footnotesize" << endl;
    relativeSystematicsSummary.PrintLatex(1,"noError");
    cout << "}" << endl;
    cout << "\\caption{Relative uncertainties for the " << reportLabel << " signal regions. \\label{tab:report_rel_" << reportTag << "}}" << endl;
    cout << "\\end{center}" << endl;
    cout << "\\end{table}" << endl;

    rawYieldSummary           .Print("results/latest/report/rawYieldSummary_"+reportTag+".tab",2);
    scaleFactorsSummary       .Print("results/latest/report/scaleFactors_"+reportTag+".tab",2);
    predictionSummary         .Print("results/latest/report/prediction_"+reportTag+".tab",2);
    absoluteSystematicsSummary.Print("results/latest/report/absoluteUncertainties_"+reportTag+".tab",1,"noError");
    relativeSystematicsSummary.Print("results/latest/report/relativeUncertainties_"+reportTag+".tab",1,"noError");

    return (0);
}


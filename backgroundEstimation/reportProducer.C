#include "../common.h"

// Sonic screwdriver headers

#include "interface/Table.h" 
#include "interface/SonicScrewdriver.h" 
using namespace theDoctor;

#include "common.h"

int main (int argc, char *argv[])
{

    if (argc <= 1) { WARNING_MSG << "No signal region specified" << endl; return -1; }

    // Read list of signal regions to consider
    
    vector<string> signalRegionsTagList;
    for (int i = 1 ; i < argc ; i++) 
        signalRegionsTagList.push_back(argv[i]);

    // Create list of label for each region

    vector<string> signalRegionsLabelList;
    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        signalRegionsLabelList.push_back(signalRegionLabel(signalRegionsTagList[i],"latex"));
    }

    // Read tables for each signal regions
    
    vector<Table> predictions;
    vector<Table> scaleFactors;
    vector<Table> systematics;

    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        predictions .push_back(Table("prediction/" +signalRegionsTagList[i]+".tab"));
        scaleFactors.push_back(Table("scaleFactors/"+signalRegionsTagList[i]+".tab"));
        systematics .push_back(Table("systematics/" +signalRegionsTagList[i]+".tab"));
    }

    // Fill summary tables

    Table rawYieldSummary(signalRegionsTagList,processesTagList,signalRegionsLabelList,processesLabelList);
    Table predictionSummary(signalRegionsTagList,processesTagList,signalRegionsLabelList,processesLabelList);
    Table scaleFactorsSummary(signalRegionsTagList,scaleFactorsTagList,signalRegionsLabelList,scaleFactorsLabelList);
    Table absoluteSystematicsSummary(signalRegionsTagList,systematicsTagList,signalRegionsLabelList,systematicsLabelList);
    Table relativeSystematicsSummary(signalRegionsTagList,systematicsTagList,signalRegionsLabelList,systematicsLabelList);

    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        for (unsigned int j = 0 ; j < processesTagList.size() ; j++)
        {
            rawYieldSummary  .Set(signalRegionsTagList[i],processesTagList[j],
                                                                              predictions[i].Get("raw_mc",processesTagList[j])); 
            predictionSummary.Set(signalRegionsTagList[i],processesTagList[j],
                                                                              predictions[i].Get("prediction",processesTagList[j])); 
        }
        for (unsigned int j = 0 ; j < scaleFactorsTagList.size() ; j++)
        {
            scaleFactorsSummary.Set(signalRegionsTagList[i],scaleFactorsTagList[j],
                                                                                   scaleFactors[i].Get("value",scaleFactorsTagList[j])); 
        }
        for (unsigned int j = 0 ; j < systematicsTagList.size() ; j++)
        {
            absoluteSystematicsSummary.Set(signalRegionsTagList[i],systematicsTagList[j],
                                                                                         systematics[i].Get("absolute",systematicsTagList[j])); 
            relativeSystematicsSummary.Set(signalRegionsTagList[i],systematicsTagList[j],
                                                                                         systematics[i].Get("relative",systematicsTagList[j])); 
        }
    }
  
    cout << "Raw yields\\\\" << endl;
    rawYieldSummary.PrintLatex(2);

    cout << "Scale factors\\\\" << endl;
    scaleFactorsSummary.PrintLatex(2);

    cout << "Absolute uncertainties\\\\" << endl;
    absoluteSystematicsSummary.PrintLatex(1,"noError");

    cout << "Relative uncertainties\\\\" << endl;
    relativeSystematicsSummary.PrintLatex(1,"noError");
    
    cout << "Prediction\\\\" << endl;
    predictionSummary.PrintLatex(2);

    return (0);
}


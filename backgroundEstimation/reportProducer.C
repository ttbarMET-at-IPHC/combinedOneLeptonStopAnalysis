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
    vector<Table> scaleFactors_2leptons;
    vector<Table> systematics;
    vector<Table> raw1;
    vector<Table> raw2;

    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        raw1.push_back(Table("rawYieldTables/" +signalRegionsTagList[i]+".tab"));
        raw2.push_back(Table("rawYieldTables/" +signalRegionsTagList[i]+"_2leptons.tab"));
        predictions .push_back(Table("prediction/" +signalRegionsTagList[i]+".tab"));
        scaleFactors.push_back(Table("scaleFactors/"+signalRegionsTagList[i]+".tab"));
        systematics .push_back(Table("systematics/" +signalRegionsTagList[i]+".tab"));
        if (CR45) scaleFactors_2leptons.push_back(Table("scaleFactors/"+signalRegionsTagList[i]+"_2leptons.tab"));
    }

    // Fill summary tables

    Table rawYieldSummary(signalRegionsTagList,processesTagList,signalRegionsLabelList,processesLabelList);
    Table predictionSummary(signalRegionsTagList,processesTagList,signalRegionsLabelList,processesLabelList);
    Table scaleFactorsSummary(signalRegionsTagList,scaleFactorsTagList,signalRegionsLabelList,scaleFactorsLabelList);
    Table absoluteSystematicsSummary(signalRegionsTagList,systematicsTagList,signalRegionsLabelList,systematicsLabelList);
    Table relativeSystematicsSummary(signalRegionsTagList,systematicsTagList,signalRegionsLabelList,systematicsLabelList);

    Table scaleFactors2lSummary(signalRegionsTagList,scaleFactorsTagList_2leptons,signalRegionsLabelList,scaleFactorsLabelList_2leptons);

    vector<string> info_n;
       info_n.push_back("CR4 $MT<100$");
       info_n.push_back("CR4 $50<MT<80$");
       info_n.push_back("CR4 $MT>100$");
       info_n.push_back("CR5 $MT<100$");
       info_n.push_back("CR5 $50<MT<80$");
       info_n.push_back("CR5 $MT>100$");
    Table rawCR45(info_n,signalRegionsTagList,info_n,signalRegionsLabelList);

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
        if (CR45) {
        for (unsigned int j = 0 ; j < scaleFactorsTagList_2leptons.size() ; j++)
        {
            scaleFactors2lSummary.Set(signalRegionsTagList[i],scaleFactorsTagList_2leptons[j],
                                                                                   scaleFactors_2leptons[i].Get("value",scaleFactorsTagList_2leptons[j])); 
        }
        }
        for (unsigned int j = 0 ; j < systematicsTagList.size() ; j++)
        {
            absoluteSystematicsSummary.Set(signalRegionsTagList[i],systematicsTagList[j],
                                                                                         systematics[i].Get("absolute",systematicsTagList[j])); 
            relativeSystematicsSummary.Set(signalRegionsTagList[i],systematicsTagList[j],
                                                                                         systematics[i].Get("relative",systematicsTagList[j]) * 100); 
        }
    }
    for (unsigned int j = 0 ; j < signalRegionsTagList.size() ; j++)
    {
      rawCR45.Set(info_n[0],signalRegionsTagList[j],raw2[j].Get("2leptons_MTinv", "data"));
      rawCR45.Set(info_n[1],signalRegionsTagList[j],raw2[j].Get("2leptons_MTpeak","data"));
      rawCR45.Set(info_n[2],signalRegionsTagList[j],raw2[j].Get("2leptons_MTtail","data"));
      rawCR45.Set(info_n[3],signalRegionsTagList[j],raw1[j].Get("antiveto_MTinv", "data"));
      rawCR45.Set(info_n[4],signalRegionsTagList[j],raw1[j].Get("antiveto_MTpeak","data"));
      rawCR45.Set(info_n[5],signalRegionsTagList[j],raw1[j].Get("antiveto_MTtail","data"));
            
    }

   
    cout << "\\documentclass[a4paper, 12pt]{article}" << endl;
    cout << "\\usepackage{amsmath}" << endl;
    cout << "\\usepackage[landscape]{geometry}" << endl;
    //cout << "\\usepackage{geometry}" << endl;
    cout << "\\geometry{a4paper, top=0.5cm, bottom=0.5cm, left=0.3cm, right=0.3cm}" << endl;

    cout << "\\begin{document}" << endl;
    cout << "\\begin{center}" << endl;

    cout << "{\\Large Raw yields}\\\\" << endl;
    rawYieldSummary.PrintLatex(2);
    cout << "\\\\ \\bigskip" << endl;

    cout << "{\\Large Scale factors}\\\\" << endl;
    scaleFactorsSummary.PrintLatex(2);
    cout << "\\\\ \\bigskip" << endl;

    cout << "{\\Large Absolute uncertainties}\\\\" << endl;
    absoluteSystematicsSummary.PrintLatex(1,"noError");
    cout << "\\\\ \\bigskip" << endl;

    cout << "{\\Large Relative uncertainties (in percents)}\\\\" << endl;
    relativeSystematicsSummary.PrintLatex(1,"noError");
    cout << "\\\\ \\bigskip" << endl;
    
    cout << "{\\Large Prediction}\\\\" << endl;
    predictionSummary.PrintLatex(2);
    cout << "\\\\ \\bigskip" << endl;
 
    if (CR45) {   
    cout << "\\newpage" << endl;
    cout << "{\\Large Scale Factors for CR4-CR5}\\\\" << endl;
    scaleFactors2lSummary.PrintLatex(2);
    cout << "\\\\ \\bigskip" << endl;

    cout << "\\newpage" << endl;
    cout << "{\\Large Data numbers in  CR4-CR5 }\\\\" << endl;
    rawCR45.PrintLatex(1,"noError");
    cout << "\\\\ \\bigskip" << endl;
    }
 

    cout << "\\end{center}" << endl;
    cout << "\\end{document}" << endl;

    return (0);
}


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
        signalRegionsLabelList.push_back(signalRegionLabel(signalRegionsTagList[i],"latex"));
    }

    // Read tables for each signal regions
    
    vector<Table> tables;

    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        tables.push_back(Table("results/latest/prediction/secondLeptonInAcceptance/"+signalRegionsTagList[i]+".tab"));
    }

    vector<string> categoriesTagList   = { "singleTrack", "hadronicTau", "outOfAcc" };
    vector<string> categoriesLabelList = { "{\\footnotesize{$t\\bar{t} \\rightarrow \\ell\\ell$ with single track}}", 
                                           "{\\footnotesize{$t\\bar{t} \\rightarrow \\ell\\ell$ with hadr. $\\tau$}}", 
                                           "{\\footnotesize{$t\\bar{t} \\rightarrow \\ell\\ell$ out of acc.}}" };

    // Fill summary tables
    Table summary  (signalRegionsTagList,  categoriesTagList,    signalRegionsLabelList,  categoriesLabelList   );

    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        string signalRegion = signalRegionsTagList[i];

        summary.Set(signalRegion,"singleTrack", tables[i].Get("singleTrack",   "ttbar_2l")
                                              / tables[i].Get("preveto_MTtail","ttbar_2l"));
        summary.Set(signalRegion,"hadronicTau", tables[i].Get("hadronicTau",   "ttbar_2l")
                                              / tables[i].Get("preveto_MTtail","ttbar_2l"));
        summary.Set(signalRegion,"outOfAcc", Figure(1.0,0) - (summary.Get(signalRegion,"singleTrack") + summary.Get(signalRegion,"hadronicTau")));

    }
    
    cout << "\\begin{table}[!ht]" << endl;
    cout << "\\begin{center}" << endl;
    summary.PrintLatex(2);
    cout << "\\caption{Fraction of $t\\bar{t} \\rightarrow \\ell\\ell$ events with a track or an hadronic $\\tau$ in the acceptance and the out-of-acceptance fraction, for " << reportLabel << " signal regions. \\label{tab:report_secondLepInAcceptance_" << reportTag << "}}" << endl;
    cout << "\\end{center}" << endl;
    cout << "\\end{table}" << endl;

    return (0);
}


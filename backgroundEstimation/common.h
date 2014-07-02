#ifndef BACKGROUNDESTIMATION_COMMON
#define BACKGROUNDESTIMATION_COMMON

//bool CR45 = false ; 
bool CR45 = true ; // make also plots for 2-leptons and anti-veto on 2nd lepton CR
bool madgraph = true;

vector<string> systematicsTagList = 
{
    "tt->ll_(CR4,CR5)",
    "tt->ll_(nJets)",
    "tt->ll_(veto)",
    "SFR_Wjets",
    "1ltop_tailToPeak",
    "MTpeak" ,
    "W+jets_(cross_section)",
    "Rare_(cross_section)",
    "tt->ll_(MCstat)",
    "1l_top_(MCstat)",
    "W+jets_(MCstat)",
    "rare_(MCstat)",
    "total"
};

vector<string> systematicsLabelList =
{
    "$t\\bar{t} \\rightarrow \\ell \\ell$ (CR4 \\& CR5 tests)",
    "$t\\bar{t} \\rightarrow \\ell \\ell$ (jets modeling)",
    "$t\\bar{t} \\rightarrow \\ell \\ell$ (2nd lepton veto)",
    "$SFR_{\\text{W+jets}}$ uncertainty",
    "1$\\ell$ top tail-to-peak ratio",
    "$M_T$ peak (data and MC stat)" ,
    "$W$+jets (cross section)",
    "rare (cross section)",
    "$t\\bar{t} \\rightarrow \\ell \\ell$ (MC stat)",
    "1$\\ell$ top (MC stat)",
    "$W$+jets (MC stat)",
    "rare (MC stat)",
    "total"
};

vector<string> processesTagList = 
{
    "1ltop",
    "ttbar_2l",
    "W+jets",
    "rare",
    "totalSM"
};

vector<string> processesLabelList = 
{
    "1$\\ell$ top",
    "$t\\bar{t} \\rightarrow \\ell \\ell$",
    "$W$+jets",
    "rare",
    "total SM"
};

vector<string> scaleFactorsTagList = 
{
    "SF_pre",
    "SF_post",
    "SF_0btag",
    "SFR_W+jets",
    "R_W+jets",
    "R_1ltop"
};

vector<string> scaleFactorsTagList_2leptons = 
{
    "SF2lpeak",
    "SF2ltail",
    "K3",
    "K4",
    "SF_pre",
    "SFR_W+jets",
    "SFvetopeak",
    "SFvetotail",
};

vector<string> scaleFactorsLabelList = 
{
    "$SF_{pre}$",
    "$SF_{post}$",
    "$SF_{\\text{0 $b$-tag}}$",
    "$SFR_{\\text{$W$+jets}}$",
    "$R_{\\text{$W$+jets}}$",
    "$R_{\\text{1$\\ell$top}}$"
};

vector<string> scaleFactorsLabelList_2leptons =
{
    "$SF_{2lpeak}$",
    "$SF_{2ltail}$",
    "K3",
    "K4",
    "$SF_{pre}$",
    "$SFR_{\\text{$W$+jets}}$",
    "$SF_{vetopeak}$",
    "$SF_{vetotail}$",
};

string signalRegionLabel(string signalRegionTag, string format = "root")
{
    if (format == "root")
    {
             if (signalRegionTag == "preselection"                          ) return "Preselection";
        else if (signalRegionTag == "crossCheck_LM150"                      ) return "Previous AN, LM150";
        else if (signalRegionTag == "cut_nocut"                             ) return "Preselection";
        else if (signalRegionTag == "cutAndCount_T2tt_offShellLoose"        ) return "C&C T2tt, Off-shell loose";
        else if (signalRegionTag == "cutAndCount_T2tt_offShellTight"        ) return "C&C T2tt, Off-shell tight";
        else if (signalRegionTag == "cutAndCount_T2tt_lowDeltaM"            ) return "C&C T2tt, Low #Delta m";
        else if (signalRegionTag == "cutAndCount_T2tt_mediumDeltaM"         ) return "C&C T2tt, Medium #Delta m";
        else if (signalRegionTag == "cutAndCount_T2tt_highDeltaM"           ) return "C&C T2tt, High #Delta m";
        else if (signalRegionTag == "cutAndCount_T2bw025_veryOffShell_loose") return "C&C T2bw x=0.25, Very off-shell loose";
        else if (signalRegionTag == "cutAndCount_T2bw025_offShell_loose"    ) return "C&C T2bw x=0.25, Off-shell loose";
        else if (signalRegionTag == "cutAndCount_T2bw025_lowDeltaM_tight"   ) return "C&C T2bw x=0.25, Low #Delta m tight";
        else if (signalRegionTag == "cutAndCount_T2bw025_highDeltaM"        ) return "C&C T2bw x=0.25, High #Delta m";
        else if (signalRegionTag == "cutAndCount_T2bw050_offShell_loose"    ) return "C&C T2bw x=0.50, Off-shell loose";
        else if (signalRegionTag == "cutAndCount_T2bw050_lowMass"           ) return "C&C T2bw x=0.50, Low mass";
        else if (signalRegionTag == "cutAndCount_T2bw050_mediumDeltaM_loose") return "C&C T2bw x=0.50, Medium #Delta m loose"; 
        else if (signalRegionTag == "cutAndCount_T2bw050_highDeltaM"        ) return "C&C T2bw x=0.50, High #Delta m";
        else if (signalRegionTag == "cutAndCount_T2bw075_lowDeltaM_tight"   ) return "C&C T2bw x=0.75, Low #Delta m tight";
        else if (signalRegionTag == "cutAndCount_T2bw075_mediumDeltaM"      ) return "C&C T2bw x=0.75, Medium #Delta m"; 
        else if (signalRegionTag == "cutAndCount_T2bw075_highDeltaM"        ) return "C&C T2bw x=0.75, High #Delta m";
        else if (signalRegionTag == "BDT_T2tt_1"                            ) return "T2tt, BDT 1";
        else if (signalRegionTag == "BDT_T2tt_2"                            ) return "T2tt, BDT 2";
        else if (signalRegionTag == "BDT_T2tt_5_loose"                      ) return "T2tt, BDT 5 loose";
        else if (signalRegionTag == "BDT_T2tt_5_tight"                      ) return "T2tt, BDT 5 tight";
        else if (signalRegionTag == "BDT_T2bw075_1"                         ) return "T2bw x=0.75, BDT 1";
        else if (signalRegionTag == "BDT_T2bw075_2"                         ) return "T2bw x=0.75, BDT 2";
        else if (signalRegionTag == "BDT_T2bw075_3"                         ) return "T2bw x=0.75, BDT 3";
        else if (signalRegionTag == "BDT_T2bw075_5"                         ) return "T2bw x=0.75, BDT 5";
        else if (signalRegionTag == "BDT_T2bw050_1_loose"                   ) return "T2bw x=0.50, BDT 1 loose";
        else if (signalRegionTag == "BDT_T2bw050_1_tight"                   ) return "T2bw x=0.50, BDT 1 tight";
        else if (signalRegionTag == "BDT_T2bw050_3"                         ) return "T2bw x=0.50, BDT 3";
        else if (signalRegionTag == "BDT_T2bw050_4"                         ) return "T2bw x=0.50, BDT 4";
        else if (signalRegionTag == "BDT_T2bw050_5"                         ) return "T2bw x=0.50, BDT 5";
        else if (signalRegionTag == "BDT_T2bw050_6"                         ) return "T2bw x=0.50, BDT 6";
        else if (signalRegionTag == "BDT_T2bw025_1"                         ) return "T2bw x=0.25, BDT 1";
        else if (signalRegionTag == "BDT_T2bw025_3"                         ) return "T2bw x=0.25, BDT 3";
        else if (signalRegionTag == "BDT_T2bw025_4"                         ) return "T2bw x=0.25, BDT 4";
        else if (signalRegionTag == "BDT_T2bw025_6"                         ) return "T2bw x=0.25, BDT 6";
        else return "";                                                                           
    }                                                                                             
    else
    {
             if (signalRegionTag == "preselection"                          ) return                           "Preselection";
        else if (signalRegionTag == "crossCheck_LM150"                      ) return                           "LM150";
        else if (signalRegionTag == "cut_nocut"                             ) return                           "Preselection";
        else if (signalRegionTag == "cutAndCount_T2tt_offShellLoose"        ) return /*"C\\&C T2tt, "+*/       "Off-shell loose";
        else if (signalRegionTag == "cutAndCount_T2tt_offShellTight"        ) return /*"C\\&C T2tt, "+*/       "Off-shell tight";
        else if (signalRegionTag == "cutAndCount_T2tt_lowDeltaM"            ) return /*"C\\&C T2tt, "+*/       "Low $\\Delta m$";
        else if (signalRegionTag == "cutAndCount_T2tt_mediumDeltaM"         ) return /*"C\\&C T2tt, "+*/       "Medium $\\Delta m$";
        else if (signalRegionTag == "cutAndCount_T2tt_highDeltaM"           ) return /*"C\\&C T2tt, "+*/       "High $\\Delta m$";
        else if (signalRegionTag == "cutAndCount_T2bw025_veryOffShell_loose") return /*"C\\&C T2bw x=0.25, "+*/"Very off-shell loose";
        else if (signalRegionTag == "cutAndCount_T2bw025_offShell_loose"    ) return /*"C\\&C T2bw x=0.25, "+*/"Off-shell loose";
        else if (signalRegionTag == "cutAndCount_T2bw025_lowDeltaM_tight"   ) return /*"C\\&C T2bw x=0.25, "+*/"Low $\\Delta m$ tight";
        else if (signalRegionTag == "cutAndCount_T2bw025_highDeltaM"        ) return /*"C\\&C T2bw x=0.25, "+*/"High $\\Delta m$";
        else if (signalRegionTag == "cutAndCount_T2bw050_offShell_loose"    ) return /*"C\\&C T2bw x=0.50, "+*/"Off-shell loose";
        else if (signalRegionTag == "cutAndCount_T2bw050_lowMass"           ) return /*"C\\&C T2bw x=0.50, "+*/"Low mass";
        else if (signalRegionTag == "cutAndCount_T2bw050_mediumDeltaM_loose") return /*"C\\&C T2bw x=0.50, "+*/"Medium $\\Delta m$ loose"; 
        else if (signalRegionTag == "cutAndCount_T2bw050_highDeltaM"        ) return /*"C\\&C T2bw x=0.50, "+*/"High $\\Delta m$";
        else if (signalRegionTag == "cutAndCount_T2bw075_lowDeltaM_tight"   ) return /*"C\\&C T2bw x=0.75, "+*/"Low $\\Delta m$ tight";
        else if (signalRegionTag == "cutAndCount_T2bw075_mediumDeltaM"      ) return /*"C\\&C T2bw x=0.75, "+*/"Medium $\\Delta m$"; 
        else if (signalRegionTag == "cutAndCount_T2bw075_highDeltaM"        ) return /*"C\\&C T2bw x=0.75, "+*/"High $\\Delta m$";
        else if (signalRegionTag == "BDT_T2tt_1"                            ) return /*"T2tt, "+*/"BDT 1";
        else if (signalRegionTag == "BDT_T2tt_2"                            ) return /*"T2tt, "+*/"BDT 2";
        else if (signalRegionTag == "BDT_T2tt_5_loose"                      ) return /*"T2tt, "+*/"BDT 5 loose";
        else if (signalRegionTag == "BDT_T2tt_5_tight"                      ) return /*"T2tt, "+*/"BDT 5 tight";
        else if (signalRegionTag == "BDT_T2bw075_1"                         ) return /*"T2bw x=0.75, "+*/"BDT 1";
        else if (signalRegionTag == "BDT_T2bw075_2"                         ) return /*"T2bw x=0.75, "+*/"BDT 2";
        else if (signalRegionTag == "BDT_T2bw075_3"                         ) return /*"T2bw x=0.75, "+*/"BDT 3";
        else if (signalRegionTag == "BDT_T2bw075_5"                         ) return /*"T2bw x=0.75, "+*/"BDT 5";
        else if (signalRegionTag == "BDT_T2bw050_1_loose"                   ) return /*"T2bw x=0.50, "+*/"BDT 1 loose";
        else if (signalRegionTag == "BDT_T2bw050_1_tight"                   ) return /*"T2bw x=0.50, "+*/"BDT 1 tight";
        else if (signalRegionTag == "BDT_T2bw050_3"                         ) return /*"T2bw x=0.50, "+*/"BDT 3";
        else if (signalRegionTag == "BDT_T2bw050_4"                         ) return /*"T2bw x=0.50, "+*/"BDT 4";
        else if (signalRegionTag == "BDT_T2bw050_5"                         ) return /*"T2bw x=0.50, "+*/"BDT 5";
        else if (signalRegionTag == "BDT_T2bw050_6"                         ) return /*"T2bw x=0.50, "+*/"BDT 6";
        else if (signalRegionTag == "BDT_T2bw025_1"                         ) return /*"T2bw x=0.25, "+*/"BDT 1";
        else if (signalRegionTag == "BDT_T2bw025_3"                         ) return /*"T2bw x=0.25, "+*/"BDT 3";
        else if (signalRegionTag == "BDT_T2bw025_4"                         ) return /*"T2bw x=0.25, "+*/"BDT 4";
        else if (signalRegionTag == "BDT_T2bw025_6"                         ) return /*"T2bw x=0.25, "+*/"BDT 6";
        else return "";                                                                               
        
    }
}

#endif

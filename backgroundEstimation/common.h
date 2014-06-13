#ifndef BACKGROUNDESTIMATION_COMMON
#define BACKGROUNDESTIMATION_COMMON

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
    "$t\\bar{t} \\rightarrow \ell \\ell$ (CR4 \\& CR5 tests)",
    "$t\\bar{t} \\rightarrow \ell \\ell$ (jets modeling)",
    "$t\\bar{t} \\rightarrow \ell \\ell$ (2nd lepton veto)",
    "$SFR_{\\text{W+jets}}$ uncertainty",
    "$1\\ell t$ tail-to-peak ratio",
    "$M_T$ peak (data and MC stat)" ,
    "$W$+jets (cross_section)",
    "rare (cross_section)",
    "$t\\bar{t} \\rightarrow \\ell \\ell$ (MC_stat)",
    "$1\\ell t$ (MC stat)",
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

vector<string> scaleFactorsLabelList = 
{
    "$SF_{pre}$",
    "$SF_{post}$",
    "$SF_{\text{0 $b$-tag}}$",
    "$SFR_{\text{$W$+jets}}$",
    "$R_{\text{$W$+jets}}$",
    "$R_{\text{1$\ell$top}}$"
};

string signalRegionLabel(string signalRegionTag, string format = "root")
{
    if (format == "root")
    {
             if (signalRegionTag == "cutAndCount_T2tt_offShellLoose") return "C&C T2tt, Off-shell loose";
        else if (signalRegionTag == "cutAndCount_T2tt_offShellTight") return "C&C T2tt, Off-shell tight";
        else if (signalRegionTag == "cutAndCount_T2tt_lowDeltaM"    ) return "C&C T2tt, Low #Delta m";
        else if (signalRegionTag == "cutAndCount_T2tt_mediumDeltaM" ) return "C&C T2tt, Medium #Delta m";
        else if (signalRegionTag == "cutAndCount_T2tt_highDeltaM"   ) return "C&C T2tt, High #Delta m";
        else return "";
    }
    else
    {
             if (signalRegionTag == "cutAndCount_T2tt_offShellLoose") return "C\\&C T2tt, Off-shell loose";
        else if (signalRegionTag == "cutAndCount_T2tt_offShellTight") return "C\\&C T2tt, Off-shell tight";
        else if (signalRegionTag == "cutAndCount_T2tt_lowDeltaM"    ) return "C\\&C T2tt, Low \\Delta m";
        else if (signalRegionTag == "cutAndCount_T2tt_mediumDeltaM" ) return "C\\&C T2tt, Medium \\Delta m";
        else if (signalRegionTag == "cutAndCount_T2tt_highDeltaM"   ) return "C\\&C T2tt, High \\Delta m";
        else return "";
    }
}

#endif

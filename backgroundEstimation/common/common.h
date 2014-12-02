#ifndef BACKGROUNDESTIMATION_COMMON
#define BACKGROUNDESTIMATION_COMMON

#include "../../common.h"
bool enableMTCut = true;
bool disableMTCut = false;

// ###############################
// #  Sonic screwdriver headers  #
// ###############################

#include "interface/Table.h"
#include "interface/SonicScrewdriver.h"
#include "interface/tables/TableBackgroundSignal.h"
#include "interface/tables/TableDataMC.h"
using namespace theDoctor;

#include <TCanvas.h>
#include <TH1F.h>

// ###################################
// #  BabyTuple format and location  #
// ###################################

#define FOLDER_BABYTUPLES "../../store/babyTuples_0603_withBDT_skim/merged_1lepton4jetsMET80_or_2leptons1jetMET50/"
//#define FOLDER_BABYTUPLES "../../store/babyTuples_1030_withBDT_skim/1lepton4jetsMET80/"
#include "Reader.h"

#define BDT_OUTPUT_AVAILABLE
#define SECOND_LEPTON_IN_ACCEPTANCE_ALREADY_COMPUTED
#define ISR_JET_ALREADY_COMPUTED
#define LEADING_NON_B_PT_ALREADY_COMPUTED

// #########################################
// #  Background estimation configuration  #
// #########################################

#define USING_TTBAR_POWHEG
#define USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
#define USING_CR4_AND_CR5_CHECKS_FROM_LOOSENED_SIGNAL_REGIONS

// #################################################
// #  Analysis, cut-and-count and BDT definitions  #
// #################################################

#include "../../common/selectionDefinitions.h"
#include "../../common/cutAndCountDefinitions.h"
#include "../../common/signalRegionDefinitions.h"
#include "../../common/secondGeneratedLeptonType.h"

// ####################
// #  Processes list  #
// ####################

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

// ######################
// #  Systematics list  #
// ######################

vector<string> systematicsTagList =
{
    "tt->ll_(CR4,CR5)",
    "tt->ll_(nJets)",
    "tt->ll_(veto)",
    #ifdef USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
    "SF_MTtail_Wjets",
    "SF_MTtail_1ltop",
    #else
    "SFR_Wjets",
    "1ltop_tailToPeak",
    #endif
    "MTpeak" ,
    #ifdef USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
    "1ltop_(cross_section)",
    #endif
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
    #ifdef USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
    "$SFR_{\\text{W+jets}}$ uncertainty",
    "$SFR_{\\text{1$\\ell$ top}}$ uncertainty",
    #else
    "$SFR_{\\text{W+jets}}$ uncertainty",
    "1$\\ell$ top tail-to-peak ratio",
    #endif
    "$M_T$ peak (data and MC stat)" ,
    #ifdef USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
    "1$\\ell$ top (cross section)",
    #endif
    "$W$+jets (cross section)",
    "rare (cross section)",
    "$t\\bar{t} \\rightarrow \\ell \\ell$ (MC stat)",
    "1$\\ell$ top (MC stat)",
    "$W$+jets (MC stat)",
    "rare (MC stat)",
    "total"
};

// ########################
// #  Scale factors list  #
// ########################

vector<string> scaleFactorsTagList =
{
    "SF_pre",
    "SF_post",
    "SF_0btag",
    "SF_vetopeak",
    #ifdef USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
    "SF_MTtail_Wjets",
    "SF_MTtail_1ltop",
    #else
        "SFR_W+jets",
        "R_W+jets",
        "R_1ltop",
    #endif
};

vector<string> scaleFactorsLabelList =
{
    "$SF_{\\text{pre}}$",
    "$SF_{\\text{post}}$",
    "$SF_{\\text{0 $b$-tag}}$",
    "$SF_{\\text{veto}}$",
    #ifdef USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
    "$SFR_{\\text{W+jets}}$",
    "$SFR_{\\text{1$\\ell$ top}}$",
    #else
        "$SFR_{\\text{$W$+jets}}$",
        "$R_{\\text{$W$+jets}}$",
        "$R_{\\text{1$\\ell$top}}$",
    #endif
};

vector<string> scaleFactorsTagList_CR4CR5modelingChecks =
{
    "SF_pre",
    "SF_2l",
    "SF_2ltail",
    "SF_vetopeak",
    "SF_vetotail"
};

vector<string> scaleFactorsLabelList_CR4CR5modelingChecks =
{
    "$SF^{M_T \\text{peak}}_{\\text{presel, pre-veto}}$",
    "$SF^{M_T \\text{peak+tail}}_{\\text{2 leptons}}$",
    "$SF^{M_T \\text{tail}}_{\\text{2 leptons}}$",
    "$SF^{M_T \\text{peak}}_{\\text{reversed veto}}$",
    "$SF^{M_T \\text{tail}}_{\\text{reversed veto}}$",
};

string signalRegionLabel(string region, string mode)
{
         if (findSubstring(region,"BDT_"))                                        return BDTlabel(region,mode);
    else if ((findSubstring(region,"cutAndCount_")) || region == "preselection")  return CnClabel(region,mode);
    else                                                                          return "unknown";
}

#endif

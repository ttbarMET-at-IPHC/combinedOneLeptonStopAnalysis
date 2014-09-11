//
// ########################################################################
// #  Thanks to Wouter Verkerke, awesome stat teacher, for original code  #
// #                                                                      #
// # 'ORGANIZATION AND SIMULTANEOUS FITS' RooFit tutorial macro #501      #
// # Using simultaneous p.d.f.s to describe simultaneous fits to multiple #
// # datasets                                                             #
// #                                                                      #
// # 07/2008                                                              #
// ########################################################################

// ROOT headers

#include "RooGlobalFunc.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooChebychev.h"
#include "RooAddPdf.h"
#include "RooSimultaneous.h"
#include "RooCategory.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TFile.h"
#include "TH1F.h"
#include "THStack.h"
#include "TRandom.h"
#include "TMath.h"

using namespace RooFit;

// STL headers

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Sonic screwdriver headers

#include "interface/Table.h"
#include "interface/Figure.h"

using namespace theDoctor;

// ####################################
// # MT tail correction configuration #
// ####################################

#define INPUT_FOLDER  "./inputPlots/"
#define OUTPUT_FOLDER "./results/"

#define OBSERVABLE_FOR_FIT "Mlb"
TRandom* randomnessGenerator;

// Uncertainty of the template fit method itself,
// coming from test with MCstat, JES, algorithms, ...

#define TEMPLATE_FIT_METHOD_UNCERTAINTY 0.2


// ##############################
// # Regions for BDT correction #
// ##############################

vector<string> listBDTSignalRegions =
{
    "BDT_T2tt_1",
    "BDT_T2tt_2",
    "BDT_T2tt_5",
    "BDT_T2bw075_1",
    "BDT_T2bw075_2",
    "BDT_T2bw075_3",
    "BDT_T2bw075_5",
    "BDT_T2bw050_1",
    "BDT_T2bw050_3",
    "BDT_T2bw050_4",
    "BDT_T2bw050_5",
    "BDT_T2bw050_6",
    "BDT_T2bw025_1",
    "BDT_T2bw025_3",
    "BDT_T2bw025_4",
    "BDT_T2bw025_6"
};

vector<string> listBDTSignalRegions_MTtail =
{
    "BDT_MTtail_T2tt_1",
    "BDT_MTtail_T2tt_2",
    "BDT_MTtail_T2tt_5",
    "BDT_MTtail_T2bw075_1",
    "BDT_MTtail_T2bw075_2",
    "BDT_MTtail_T2bw075_3",
    "BDT_MTtail_T2bw075_5",
    "BDT_MTtail_T2bw050_1",
    "BDT_MTtail_T2bw050_3",
    "BDT_MTtail_T2bw050_4",
    "BDT_MTtail_T2bw050_5",
    "BDT_MTtail_T2bw050_6",
    "BDT_MTtail_T2bw025_1",
    "BDT_MTtail_T2bw025_3",
    "BDT_MTtail_T2bw025_4",
    "BDT_MTtail_T2bw025_6"
};

vector<string> listBDTSignalRegions_MTpeak =
{
    "BDT_MTpeak_T2tt_1",
    "BDT_MTpeak_T2tt_2",
    "BDT_MTpeak_T2tt_5",
    "BDT_MTpeak_T2bw075_1",
    "BDT_MTpeak_T2bw075_2",
    "BDT_MTpeak_T2bw075_3",
    "BDT_MTpeak_T2bw075_5",
    "BDT_MTpeak_T2bw050_1",
    "BDT_MTpeak_T2bw050_3",
    "BDT_MTpeak_T2bw050_4",
    "BDT_MTpeak_T2bw050_5",
    "BDT_MTpeak_T2bw050_6",
    "BDT_MTpeak_T2bw025_1",
    "BDT_MTpeak_T2bw025_3",
    "BDT_MTpeak_T2bw025_4",
    "BDT_MTpeak_T2bw025_6"
};

vector<string> listBDTSignalRegions_MTpeak_NoBtag =
{
    "BDT_MTPeakNoBtag_T2tt_1",
    "BDT_MTPeakNoBtag_T2tt_2",
    "BDT_MTPeakNoBtag_T2tt_5",
    "BDT_MTPeakNoBtag_T2bw075_1",
    "BDT_MTPeakNoBtag_T2bw075_2",
    "BDT_MTPeakNoBtag_T2bw075_3",
    "BDT_MTPeakNoBtag_T2bw075_5",
    "BDT_MTPeakNoBtag_T2bw050_1",
    "BDT_MTPeakNoBtag_T2bw050_3",
    "BDT_MTPeakNoBtag_T2bw050_4",
    "BDT_MTPeakNoBtag_T2bw050_5",
    "BDT_MTPeakNoBtag_T2bw050_6",
    "BDT_MTPeakNoBtag_T2bw025_1",
    "BDT_MTPeakNoBtag_T2bw025_3",
    "BDT_MTPeakNoBtag_T2bw025_4",
    "BDT_MTPeakNoBtag_T2bw025_6"
};

vector<string> listBDTSignalRegions_MTpeak_OneBtag =
{
    "BDT_MTPeakOneBtag_T2tt_1",
    "BDT_MTPeakOneBtag_T2tt_2",
    "BDT_MTPeakOneBtag_T2tt_5",
    "BDT_MTPeakOneBtag_T2bw075_1",
    "BDT_MTPeakOneBtag_T2bw075_2",
    "BDT_MTPeakOneBtag_T2bw075_3",
    "BDT_MTPeakOneBtag_T2bw075_5",
    "BDT_MTPeakOneBtag_T2bw050_1",
    "BDT_MTPeakOneBtag_T2bw050_3",
    "BDT_MTPeakOneBtag_T2bw050_4",
    "BDT_MTPeakOneBtag_T2bw050_5",
    "BDT_MTPeakOneBtag_T2bw050_6",
    "BDT_MTPeakOneBtag_T2bw025_1",
    "BDT_MTPeakOneBtag_T2bw025_3",
    "BDT_MTPeakOneBtag_T2bw025_4",
    "BDT_MTPeakOneBtag_T2bw025_6"
};

// ########################################
// # Regions for cut-and-count correction #
// ########################################

vector<string> listIndividualCuts =
{
    "MT_100",
    "MT_120",
    "MT_125",
    "MT_130",
    "MT_135",
    "MT_140",
    "MET_200",
    "MET_250",
    "MET_300",
    "MET_350",
    "METoverSqrtHT_6",
    "METoverSqrtHT_7",
    "METoverSqrtHT_8",
    "METoverSqrtHT_9",
    "METoverSqrtHT_10",
    "METoverSqrtHT_12",
    "BPt_100",
    "BPt_180",
    "DPhi_02",
    "DPhi_08",
    "ISRJet",
    "MT2W_180",
    "MT2W_190",
    "MT2W_200"
};

vector<string> listIndividualCuts_MTtail =
{
    "CR0btag_MTtail_MT_100",
    "CR0btag_MTtail_MT_120",
    "CR0btag_MTtail_MT_125",
    "CR0btag_MTtail_MT_130",
    "CR0btag_MTtail_MT_135",
    "CR0btag_MTtail_MT_140",

    "CR0btag_MTtail_MET_200",
    "CR0btag_MTtail_MET_250",
    "CR0btag_MTtail_MET_300",
    "CR0btag_MTtail_MET_350",

    "CR0btag_MTtail_METoverSqrtHT_6",
    "CR0btag_MTtail_METoverSqrtHT_7",
    "CR0btag_MTtail_METoverSqrtHT_8",
    "CR0btag_MTtail_METoverSqrtHT_9",
    "CR0btag_MTtail_METoverSqrtHT_10",
    "CR0btag_MTtail_METoverSqrtHT_12",

    "CR0btag_MTtail_BPt_100",
    "CR0btag_MTtail_BPt_180",

    "CR0btag_MTtail_DPhi_02",
    "CR0btag_MTtail_DPhi_08",

    "CR0btag_MTtail_ISRJet",

    "CR0btag_MTtail_MT2W_180",
    "CR0btag_MTtail_MT2W_190",
    "CR0btag_MTtail_MT2W_200"
};

vector<string>  listIndividualCuts_MTpeak =
{
    "0btag_MTpeak",
    "0btag_MTpeak",
    "0btag_MTpeak",
    "0btag_MTpeak",
    "0btag_MTpeak",
    "0btag_MTpeak",

    "CR0btag_MTpeak_MET_200",
    "CR0btag_MTpeak_MET_250",
    "CR0btag_MTpeak_MET_300",
    "CR0btag_MTpeak_MET_350",

    "CR0btag_MTpeak_METoverSqrtHT_6",
    "CR0btag_MTpeak_METoverSqrtHT_7",
    "CR0btag_MTpeak_METoverSqrtHT_8",
    "CR0btag_MTpeak_METoverSqrtHT_9",
    "CR0btag_MTpeak_METoverSqrtHT_10",
    "CR0btag_MTpeak_METoverSqrtHT_12",

    "CR0btag_MTpeak_BPt_100",
    "CR0btag_MTpeak_BPt_180",

    "CR0btag_MTpeak_DPhi_02",
    "CR0btag_MTpeak_DPhi_08",

    "CR0btag_MTpeak_ISRJet",

    "CR0btag_MTpeak_MT2W_180",
    "CR0btag_MTpeak_MT2W_190",
    "CR0btag_MTpeak_MT2W_200"
};

vector<string> listCutAndCounts =
{
    "preselection",
    "cutAndCount_T2tt_offShellLoose",
    "cutAndCount_T2tt_offShellTight",
    "cutAndCount_T2tt_lowDeltaM",
    "cutAndCount_T2tt_mediumDeltaM",
    "cutAndCount_T2tt_highDeltaM",

    "cutAndCount_T2bw025_offShell",
    "cutAndCount_T2bw025_lowMasses",
    "cutAndCount_T2bw025_highMasses",

    "cutAndCount_T2bw050_offShell",
    "cutAndCount_T2bw050_lowMasses",
    "cutAndCount_T2bw050_mediumDeltaM",
    "cutAndCount_T2bw050_highDeltaM",

    "cutAndCount_T2bw075_lowDeltaM",
    "cutAndCount_T2bw075_mediumDeltaM",
    "cutAndCount_T2bw075_highDeltaM"
};

map<string,vector<string> > listCutAndCounts_cuts;

void initCutAndCountCuts()
{
    listCutAndCounts_cuts["preselection"]                     = { "MT_100"                     };
    listCutAndCounts_cuts["cutAndCount_T2tt_offShellLoose"]   = { "MT_125"                     };
    listCutAndCounts_cuts["cutAndCount_T2tt_offShellTight"]   = { "MT_130", "MET_300"          };
    listCutAndCounts_cuts["cutAndCount_T2tt_lowDeltaM"]       = { "MT_140", "METoverSqrtHT_8"  };
    listCutAndCounts_cuts["cutAndCount_T2tt_mediumDeltaM"]    = { "MT_140", "MET_200"          };
    listCutAndCounts_cuts["cutAndCount_T2tt_highDeltaM"]      = { "MT_130", "MET_300"          }; // (FIXME) NB : real MET cut here is 350 ...

    listCutAndCounts_cuts["cutAndCount_T2bw025_offShell"]     = { "MT_120", "METoverSqrtHT_9"  };
    listCutAndCounts_cuts["cutAndCount_T2bw025_lowMasses"]    = { "MT_120", "METoverSqrtHT_6"  };
    listCutAndCounts_cuts["cutAndCount_T2bw025_highMasses"]   = { "MT_120", "MET_300"          };

    listCutAndCounts_cuts["cutAndCount_T2bw050_offShell"]     = { "MT_120", "METoverSqrtHT_9"  };
    listCutAndCounts_cuts["cutAndCount_T2bw050_lowMasses"]    = { "MT_135", "METoverSqrtHT_6"  };
    listCutAndCounts_cuts["cutAndCount_T2bw050_mediumDeltaM"] = { "MT_140", "METoverSqrtHT_7"  };
    listCutAndCounts_cuts["cutAndCount_T2bw050_highDeltaM"]   = { "MT_120", "MET_300"          };

    listCutAndCounts_cuts["cutAndCount_T2bw075_lowDeltaM"]    = { "MT_120", "METoverSqrtHT_12" };
    listCutAndCounts_cuts["cutAndCount_T2bw075_mediumDeltaM"] = { "MT_130", "METoverSqrtHT_10" };
    listCutAndCounts_cuts["cutAndCount_T2bw075_highDeltaM"]   = { "MT_140", "MET_300"          };
}

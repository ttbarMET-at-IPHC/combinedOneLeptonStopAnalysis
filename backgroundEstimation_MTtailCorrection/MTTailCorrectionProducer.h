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

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
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

// ##############################
// # Regions for BDT correction #
// ##############################

vector<string> listBDTSignalRegions_MTtail = 
{
    "LowBDT_T2tt_1",  
    "LowBDT_T2tt_2",  
    "LowBDT_T2tt_5",  
    "LowBDT_T2bw075_1",  
    "LowBDT_T2bw075_2",  
    "LowBDT_T2bw075_3",  
    "LowBDT_T2bw075_5",  
    "LowBDT_T2bw050_1",  
    "LowBDT_T2bw050_3",  
    "LowBDT_T2bw050_4",  
    "LowBDT_T2bw050_5",  
    "LowBDT_T2bw050_6",  
    "LowBDT_T2bw025_1",  
    "LowBDT_T2bw025_3",  
    "LowBDT_T2bw025_4",  
    "LowBDT_T2bw025_6"
};

vector<string> listBDTSignalRegions_MTpeak = 
{
    "LowBDT_MTpeak_T2tt_1",  
    "LowBDT_MTpeak_T2tt_2",  
    "LowBDT_MTpeak_T2tt_5",  
    "LowBDT_MTpeak_T2bw075_1",  
    "LowBDT_MTpeak_T2bw075_2",  
    "LowBDT_MTpeak_T2bw075_3",  
    "LowBDT_MTpeak_T2bw075_5",  
    "LowBDT_MTpeak_T2bw050_1",  
    "LowBDT_MTpeak_T2bw050_3",  
    "LowBDT_MTpeak_T2bw050_4",  
    "LowBDT_MTpeak_T2bw050_5",  
    "LowBDT_MTpeak_T2bw050_6",  
    "LowBDT_MTpeak_T2bw025_1",  
    "LowBDT_MTpeak_T2bw025_3",  
    "LowBDT_MTpeak_T2bw025_4",  
    "LowBDT_MTpeak_T2bw025_6"
};

vector<string> listBDTSignalRegions_MTpeak_NoBtag = 
{
    "LowBDT_MTPeakNoBtag_T2tt_1",  
    "LowBDT_MTPeakNoBtag_T2tt_2",  
    "LowBDT_MTPeakNoBtag_T2tt_5",  
    "LowBDT_MTPeakNoBtag_T2bw075_1",  
    "LowBDT_MTPeakNoBtag_T2bw075_2",  
    "LowBDT_MTPeakNoBtag_T2bw075_3",  
    "LowBDT_MTPeakNoBtag_T2bw075_5",  
    "LowBDT_MTPeakNoBtag_T2bw050_1",  
    "LowBDT_MTPeakNoBtag_T2bw050_3",  
    "LowBDT_MTPeakNoBtag_T2bw050_4",  
    "LowBDT_MTPeakNoBtag_T2bw050_5",  
    "LowBDT_MTPeakNoBtag_T2bw050_6",  
    "LowBDT_MTPeakNoBtag_T2bw025_1",  
    "LowBDT_MTPeakNoBtag_T2bw025_3",  
    "LowBDT_MTPeakNoBtag_T2bw025_4",  
    "LowBDT_MTPeakNoBtag_T2bw025_6"
};

vector<string> listBDTSignalRegions_MTpeak_OneBtag = 
{
    "LowBDT_MTPeakOneBtag_T2tt_1",  
    "LowBDT_MTPeakOneBtag_T2tt_2",  
    "LowBDT_MTPeakOneBtag_T2tt_5",  
    "LowBDT_MTPeakOneBtag_T2bw075_1",  
    "LowBDT_MTPeakOneBtag_T2bw075_2",  
    "LowBDT_MTPeakOneBtag_T2bw075_3",  
    "LowBDT_MTPeakOneBtag_T2bw075_5",  
    "LowBDT_MTPeakOneBtag_T2bw050_1",  
    "LowBDT_MTPeakOneBtag_T2bw050_3",  
    "LowBDT_MTPeakOneBtag_T2bw050_4",  
    "LowBDT_MTPeakOneBtag_T2bw050_5",  
    "LowBDT_MTPeakOneBtag_T2bw050_6",  
    "LowBDT_MTPeakOneBtag_T2bw025_1",  
    "LowBDT_MTPeakOneBtag_T2bw025_3",  
    "LowBDT_MTPeakOneBtag_T2bw025_4",  
    "LowBDT_MTPeakOneBtag_T2bw025_6"
};

// ########################################
// # Regions for cut-and-count correction #
// ########################################

vector<string> listIndividualCuts_MTtail = 
{
    "CR0btag_MTtail_MT_120",
    "CR0btag_MTtail_MT_125", 
    "CR0btag_MTtail_MT_130",
    "CR0btag_MTtail_MT_140", 
    "CR0btag_MTtail_MT_150", 
    "CR0btag_MTtail_MT_160",
    "CR0btag_MTtail_MET_200",  
    "CR0btag_MTtail_MET_250",  
    "CR0btag_MTtail_MET_300",  
    "CR0btag_MTtail_MET_320",  
    "CR0btag_MTtail_METoverSqrtHT_6",  
    "CR0btag_MTtail_METoverSqrtHT_7",  
    "CR0btag_MTtail_METoverSqrtHT_8",  
    "CR0btag_MTtail_METoverSqrtHT_9",  
    "CR0btag_MTtail_METoverSqrtHT_10",  
    "CR0btag_MTtail_METoverSqrtHT_12",  
    "CR0btag_MTtail_BPt_100", 
    "CR0btag_MTtail_BPt_150",  
    "CR0btag_MTtail_BPt_180",  
    "CR0btag_MTtail_DPhi_02",  
    "CR0btag_MTtail_DPhi_08",  
    "CR0btag_MTtail_ISRJet",  
    "CR0btag_MTtail_MT2W_180",  
    "CR0btag_MTtail_MT2W_190"         
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
    "CR0btag_MTpeak_MET_320",  
    "CR0btag_MTpeak_METoverSqrtHT_6",  
    "CR0btag_MTpeak_METoverSqrtHT_7",  
    "CR0btag_MTpeak_METoverSqrtHT_8",  
    "CR0btag_MTpeak_METoverSqrtHT_9",  
    "CR0btag_MTpeak_METoverSqrtHT_10",  
    "CR0btag_MTpeak_METoverSqrtHT_12",  
    "CR0btag_MTpeak_BPt_100",  
    "CR0btag_MTtail_BPt_150", 
    "CR0btag_MTpeak_BPt_180",  
    "CR0btag_MTpeak_DPhi_02",  
    "CR0btag_MTpeak_DPhi_08",  
    "CR0btag_MTpeak_ISRJet",  
    "CR0btag_MTpeak_MT2W_180",  
    "CR0btag_MTpeak_MT2W_190"         
};

vector<string> listCutAndCounts = 
{
    "cutAndCount_T2tt_offShellLoose",                
    "cutAndCount_T2tt_offShellTight",           
    "cutAndCount_T2tt_lowDeltaM",                
    "cutAndCount_T2tt_mediumDeltaM",             
    "cutAndCount_T2tt_highDeltaM",             
    "cutAndCount_T2bw025_veryOffShell_loose",     
    "cutAndCount_T2bw025_offShell_looSS",         
    "cutAndCount_T2bw025_lowDeltaM_tight",         
    "cutAndCount_T2bw025_highDeltaM",         
    "cutAndCount_T2bw050_offShell_loose",         
    "cutAndCount_T2bw050_lowMass",             
    "cutAndCount_T2bw050_mediumDeltaM_loose",     
    "cutAndCount_T2bw050_highDeltaM",     
    "cutAndCount_T2bw075_lowDeltaM_tight",         
    "cutAndCount_T2bw075_mediumDeltaM"         
};

map<string,vector<string> > listCutAndCounts_cuts;
void initCutAndCountCuts()
{
    listCutAndCounts_cuts["cutAndCount_T2tt_offShellLoose"]         = {"MT_125" };
    listCutAndCounts_cuts["cutAndCount_T2tt_offShellTight"]         = {"MT_130", "MET_300"};
    listCutAndCounts_cuts["cutAndCount_T2tt_lowDeltaM"]             = {"MT_140", "METoverSqrtHT_8"};
    listCutAndCounts_cuts["cutAndCount_T2tt_mediumDeltaM"]          = {"MT_140", "MET_200"};
    listCutAndCounts_cuts["cutAndCount_T2tt_highDeltaM"]            = {"MT_130", "MET_350"};
    listCutAndCounts_cuts["cutAndCount_T2bw025_veryOffShell_loose"] = {"MT_120", "METoverSqrtHT_9"};
    listCutAndCounts_cuts["cutAndCount_T2bw025_offShell_looSS"]     = {"MT_120", "METoverSqrtHT_7"};
    listCutAndCounts_cuts["cutAndCount_T2bw025_lowDeltaM_tight"]    = {"MT_120", "METoverSqrtHT_6"};
    listCutAndCounts_cuts["cutAndCount_T2bw025_highDeltaM"]         = {"MT_140", "METoverSqrtHT_10"};
    listCutAndCounts_cuts["cutAndCount_T2bw050_offShell_loose"]     = {"MT_120", "METoverSqrtHT_9"};
    listCutAndCounts_cuts["cutAndCount_T2bw050_lowMass"]            = {"MT_120", "METoverSqrtHT_6"};
    listCutAndCounts_cuts["cutAndCount_T2bw050_mediumDeltaM_loose"] = {"MT_150", "METoverSqrtHT_7"};
    listCutAndCounts_cuts["cutAndCount_T2bw050_highDeltaM"]         = {"MT_160", "METoverSqrtHT_10"};
    listCutAndCounts_cuts["cutAndCount_T2bw075_lowDeltaM_tight"]    = {"MT_120", "METoverSqrtHT_12"};
    listCutAndCounts_cuts["cutAndCount_T2bw075_mediumDeltaM"]       = {"MT_120", "METoverSqrtHT_10"};
    listCutAndCounts_cuts["cutAndCount_T2bw075_highDeltaM"]         = {"MT_140", "MET_320"};
}

vector<string> listCutAndCountsRegions_MTtail = 
{
    "CR0bag_MTtail_T2tt_offShellLoose",         
    "CR0bag_MTtail_T2tt_offShellTight",         
    "CR0bag_MTtail_T2tt_lowDeltaM",             
    "CR0bag_MTtail_T2tt_mediumDeltaM",          
    "CR0bag_MTtail_T2tt_highDeltaM",            
    "CR0bag_MTtail_T2bw025_veryOffShell_loose", 
    "CR0bag_MTtail_T2bw025_offShell_loose",     
    "CR0bag_MTtail_T2bw025_lowDeltaM_tight",    
    "CR0bag_MTtail_T2bw025_highDeltaM",         
    "CR0bag_MTtail_T2bw050_offShell_loose",     
    "CR0bag_MTtail_T2bw050_lowMass",            
    "CR0bag_MTtail_T2bw050_mediumDeltaM_loose", 
    "CR0bag_MTtail_T2bw050_highDeltaM",         
    "CR0bag_MTtail_T2bw075_lowDeltaM_tight",    
    "CR0bag_MTtail_T2bw075_mediumDeltaM",       
    "CR0bag_MTtail_T2bw075_highDeltaM"
};

vector<string> listCutAndCountsRegions_MTpeak = 
{
    "CR0bag_MTpeak_T2tt_offShellLoose",         
    "CR0bag_MTpeak_T2tt_offShellTight",         
    "CR0bag_MTpeak_T2tt_lowDeltaM",             
    "CR0bag_MTpeak_T2tt_mediumDeltaM",          
    "CR0bag_MTpeak_T2tt_highDeltaM",            
    "CR0bag_MTpeak_T2bw025_veryOffShell_loose", 
    "CR0bag_MTpeak_T2bw025_offShell_loose",     
    "CR0bag_MTpeak_T2bw025_lowDeltaM_tight",    
    "CR0bag_MTpeak_T2bw025_highDeltaM",         
    "CR0bag_MTpeak_T2bw050_offShell_loose",     
    "CR0bag_MTpeak_T2bw050_lowMass",            
    "CR0bag_MTpeak_T2bw050_mediumDeltaM_loose", 
    "CR0bag_MTpeak_T2bw050_highDeltaM",         
    "CR0bag_MTpeak_T2bw075_lowDeltaM_tight",    
    "CR0bag_MTpeak_T2bw075_mediumDeltaM",       
    "CR0bag_MTpeak_T2bw075_highDeltaM"
};

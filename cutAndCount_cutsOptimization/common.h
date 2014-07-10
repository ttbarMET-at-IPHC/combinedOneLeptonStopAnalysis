#ifndef BACKGROUNDESTIMATION_COMMON
#define BACKGROUNDESTIMATION_COMMON

#include "../common.h"
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

#define FOLDER_BABYTUPLES "../store/babyTuples_0603_withBDT_skim/1lepton4jetsMET80/"
#include "Reader.h"

#define SECOND_LEPTON_IN_ACCEPTANCE_ALREADY_COMPUTED
#define BDT_OUTPUT_AVAILABLE
#define ISR_JET_ALREADY_COMPUTED

// #################################################
// #  Analysis, cut-and-count and BDT definitions  #
// #################################################

#ifndef SECOND_LEPTON_IN_ACCEPTANCE_ALREADY_COMPUTED
    #include "../AN-14-067/SecondLeptonInAcceptance.h"
#endif
#include "../AN-14-067/selectionDefinitions.h"
#include "../AN-14-067/cutAndCountDefinitions.h"
#include "../AN-14-067/signalRegionDefinitions.h"

float figureOfMerit(float S, float B, string mode, bool minimumEventRequirement = true, float systUncertainty = SYST_UNCERTAINTY)
{
    if (minimumEventRequirement)
    {
        if (B < 1.0) B = 1.0;
        if (S < 3) return 0;
    }

    float f_B = systUncertainty;
    B += f_B*f_B*B*B;

    if (mode == "discovery") return S / sqrt(B);
    if (mode == "exclusion") return S / sqrt(S + B);
    if (mode == "azimov")    return sqrt(2 * ((S+B) * log(1 + S/B) - S));
    
    return -1.0;
}

#endif

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
#define USING_TTBAR_POWHEG

// #################################################
// #  Analysis, cut-and-count and BDT definitions  #
// #################################################

#ifndef SECOND_LEPTON_IN_ACCEPTANCE_ALREADY_COMPUTED
    #include "../AN-14-067/SecondLeptonInAcceptance.h"
#endif
#include "../AN-14-067/selectionDefinitions.h"
#include "../AN-14-067/cutAndCountDefinitions.h"
#include "../AN-14-067/signalRegionDefinitions.h"

float figureOfMerit(float S, float B, string mode, bool minimumEventRequirement = true, float systUncertainty = 0.15)
{
    if (minimumEventRequirement)
    {
        if (B < 1.0) B = 1.0;
        if (S < 2) return 0;
    }

    float f_B = systUncertainty;
    B += f_B*f_B*B*B;

    if (mode == "discovery") return S / sqrt(B);
    if (mode == "exclusion") return S / sqrt(S + B);
    if (mode == "azimov")    return sqrt(2 * ((S+B) * log(1 + S/B) - S));
    
    return -1.0;
}

void formatAndWriteMapPlot(SonicScrewdriver* screwdriver, TH2F* theHisto, string name, string comment, string pathExport)
{
    Plot thePlot(name,"custom",screwdriver->GetGlobalOptions());
    thePlot.SetParameter("name",name);
    thePlot.AddToInPlotInfo(comment);

    thePlot.getCanvas()->SetRightMargin(0.1);
	theHisto->GetXaxis()->SetTitle("m_{#tilde{t}} [GeV]");
	theHisto->GetXaxis()->SetTitleFont(PLOTDEFAULTSTYLES_FONT+2);
	theHisto->GetXaxis()->SetTitleSize(0.05);
	theHisto->GetXaxis()->SetTitleOffset(0.9);
	theHisto->GetXaxis()->SetLabelFont(PLOTDEFAULTSTYLES_FONT+3);
	theHisto->GetXaxis()->SetLabelSize(22);
	theHisto->GetYaxis()->SetTitle("m_{#chi^{0}} [GeV]");
	theHisto->GetYaxis()->SetTitleFont(PLOTDEFAULTSTYLES_FONT+2);
	theHisto->GetYaxis()->SetTitleSize(0.05);
	theHisto->GetYaxis()->SetTitleOffset(0.9);
	theHisto->GetYaxis()->SetLabelFont(PLOTDEFAULTSTYLES_FONT+3);
	theHisto->GetYaxis()->SetLabelSize(22);
    theHisto->SetTitle("");
    theHisto->SetStats(0);
    theHisto->Draw("COLZ TEXT");
    thePlot.Update();
    TPaletteAxis *pal = (TPaletteAxis*) theHisto->GetListOfFunctions()->FindObject("palette");
    if (pal != 0) 
    {
        pal->SetX1NDC(0.901);
        pal->SetY1NDC(0.1);
        pal->SetX2NDC(0.93);
        pal->SetY2NDC(1.0-thePlot.getCanvas()->GetTopMargin());
    }

    TLine* line1 = new TLine(160,-12.5,560,387.5);
    line1->SetLineWidth(2);
    line1->SetLineStyle(2);
    line1->Draw();
    thePlot.Write(pathExport,"custom",screwdriver->GetGlobalOptions());
}
#endif

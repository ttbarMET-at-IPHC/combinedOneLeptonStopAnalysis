#include "interface/Table.h"
#include "interface/SonicScrewdriver.h"
#include "interface/tables/TableBackgroundSignal.h"
#include "interface/tables/TableDataMC.h"
using namespace theDoctor;

#include "../../../common.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"

string signalRegionName(string decaymode, int stopmass, int lspmass);
float readSF(string SR, string signalType, int mStop, int mLSP, string option = "");

int main()
{
    TH2F histoRMTpeak ("R_MTpeak", "R = (S/B)_MTpeak / (S/B)_signalRegion",28,112.5,812.5,16,-12.5,387.5);
    TH2F histoR0btag  ("R_0btag",  "R = (S/B)_0btag  / (S/B)_signalRegion",28,112.5,812.5,16,-12.5,387.5);
    TH2F histoCMTpeak ("C_MTpeak", "C = (S/B)_MTpeak",28,112.5,812.5,16,-12.5,387.5);
    TH2F histoC0btag  ("C_0btag",  "C = (S/B)_0btag",28,112.5,812.5,16,-12.5,387.5);
    TH2F histoCSR     ("C_SR",     "P = (S/B)_signalRegion",28,112.5,812.5,16,-12.5,387.5);

    string signalType = "T2bw-025";

    float mStopLimit = 0;
    if (signalType == "T2tt"    ) mStopLimit = 475;
    if (signalType == "T2bw-075") mStopLimit = 475;
    if (signalType == "T2bw-050") mStopLimit = 575;
    if (signalType == "T2bw-025") mStopLimit = 525;

    for (int mStop = 125 ; mStop <= 800 ; mStop += 25)
    for (int mLSP  =  0  ; mLSP  < 400 ; mLSP  += 25)
    {
        float R_MTpeak  = -1.0;
        float R_0btag   = -1.0;
        float C_MTpeak  = -1.0;
        float C_0btag   = -1.0;
        float C_SR      = -1.0;

        string SRname;

        if (mStop - mLSP <  100) continue;

        SRname = signalRegionName(signalType,mStop,mLSP);

        if (((mStop - 125) % 50 == 0) && ((mLSP  -  25) % 50 == 0))
        {
            R_MTpeak  = readSF(SRname,signalType,mStop,mLSP,"R_MTpeak");
            R_0btag   = readSF(SRname,signalType,mStop,mLSP,"R_0btag" );
            C_MTpeak  = readSF(SRname,signalType,mStop,mLSP,"C_MTpeak");
            C_0btag   = readSF(SRname,signalType,mStop,mLSP,"C_0btag" );
            C_SR      = readSF(SRname,signalType,mStop,mLSP,"C_SR"    );
        }
        else if (((mStop - 125) % 50 != 0) && ((mLSP  -  25) % 50 == 0))
        {
            float R_MTpeak_left   = readSF(SRname,signalType,mStop-25,mLSP,"R_MTpeak");
            float R_MTpeak_right  = readSF(SRname,signalType,mStop+25,mLSP,"R_MTpeak");
            float R_0btag_left    = readSF(SRname,signalType,mStop-25,mLSP,"R_0btag" );
            float R_0btag_right   = readSF(SRname,signalType,mStop+25,mLSP,"R_0btag" );
            float C_MTpeak_left   = readSF(SRname,signalType,mStop-25,mLSP,"C_MTpeak");
            float C_MTpeak_right  = readSF(SRname,signalType,mStop+25,mLSP,"C_MTpeak");
            float C_0btag_left    = readSF(SRname,signalType,mStop-25,mLSP,"C_0btag" );
            float C_0btag_right   = readSF(SRname,signalType,mStop+25,mLSP,"C_0btag" );
            float C_SR_left       = readSF(SRname,signalType,mStop-25,mLSP,"C_SR"    );
            float C_SR_right      = readSF(SRname,signalType,mStop+25,mLSP,"C_SR"    );

            R_MTpeak = 0.0;
            R_0btag  = 0.0;
            C_MTpeak = 0.0;
            C_0btag  = 0.0;
            C_SR     = 0.0;

            int counter = 0;

     if (R_MTpeak_left  != 0) { R_MTpeak += R_MTpeak_left;  R_0btag += R_0btag_left;  C_MTpeak += C_MTpeak_left;  C_0btag += C_0btag_left;  C_SR += C_SR_left; counter++;  }
     if (R_MTpeak_right != 0) { R_MTpeak += R_MTpeak_right; R_0btag += R_0btag_right; C_MTpeak += C_MTpeak_right; C_0btag += C_0btag_right; C_SR += C_SR_right;counter++;  }
     if (counter        != 0) { R_MTpeak /= counter;        R_0btag /= counter;       C_MTpeak /= counter;        C_0btag /= counter;       C_SR /= counter;               }
        }
        else if (((mStop - 125) % 50 == 0) && ((mLSP  -  25) % 50 != 0))
        {
            float R_MTpeak_top     = readSF(SRname,signalType,mStop,mLSP+25,"R_MTpeak");
            float R_MTpeak_bottom  = readSF(SRname,signalType,mStop,mLSP-25,"R_MTpeak");
            float R_0btag_top      = readSF(SRname,signalType,mStop,mLSP+25,"R_0btag");
            float R_0btag_bottom   = readSF(SRname,signalType,mStop,mLSP-25,"R_0btag");

            float C_MTpeak_top     = readSF(SRname,signalType,mStop,mLSP+25,"C_MTpeak");
            float C_MTpeak_bottom  = readSF(SRname,signalType,mStop,mLSP-25,"C_MTpeak");
            float C_0btag_top      = readSF(SRname,signalType,mStop,mLSP+25,"C_0btag");
            float C_0btag_bottom   = readSF(SRname,signalType,mStop,mLSP-25,"C_0btag");
            float C_SR_top         = readSF(SRname,signalType,mStop,mLSP+25,"C_SR");
            float C_SR_bottom      = readSF(SRname,signalType,mStop,mLSP-25,"C_SR");

            R_MTpeak = 0.0;
            R_0btag  = 0.0;
            C_MTpeak = 0.0;
            C_0btag  = 0.0;
            C_SR     = 0.0;

            int counter = 0;

     if (R_MTpeak_top    != 0) { R_MTpeak += R_MTpeak_top;    R_0btag += R_0btag_top;    C_MTpeak += C_MTpeak_top;    C_0btag += C_0btag_top;    C_SR += C_SR_top; counter++;    }
     if (R_MTpeak_bottom != 0) { R_MTpeak += R_MTpeak_bottom; R_0btag += R_0btag_bottom; C_MTpeak += C_MTpeak_bottom; C_0btag += C_0btag_bottom; C_SR += C_SR_bottom;counter++;  }
     if (counter         != 0) { R_MTpeak /= counter;         R_0btag /= counter;        C_MTpeak /= counter;         C_0btag /= counter;        C_SR /= counter;                }
        }
        else
        {
            float R_MTpeak_topleft      = readSF(SRname,signalType,mStop-25,mLSP+25,"R_MTpeak");
            float R_MTpeak_bottomleft   = readSF(SRname,signalType,mStop-25,mLSP-25,"R_MTpeak");
            float R_MTpeak_topright     = readSF(SRname,signalType,mStop+25,mLSP+25,"R_MTpeak");
            float R_MTpeak_bottomright  = readSF(SRname,signalType,mStop+25,mLSP-25,"R_MTpeak");
            float R_0btag_topleft       = readSF(SRname,signalType,mStop-25,mLSP+25,"R_0btag");
            float R_0btag_bottomleft    = readSF(SRname,signalType,mStop-25,mLSP-25,"R_0btag");
            float R_0btag_topright      = readSF(SRname,signalType,mStop+25,mLSP+25,"R_0btag");
            float R_0btag_bottomright   = readSF(SRname,signalType,mStop+25,mLSP-25,"R_0btag");
            float C_MTpeak_topleft      = readSF(SRname,signalType,mStop-25,mLSP+25,"C_MTpeak");
            float C_MTpeak_bottomleft   = readSF(SRname,signalType,mStop-25,mLSP-25,"C_MTpeak");
            float C_MTpeak_topright     = readSF(SRname,signalType,mStop+25,mLSP+25,"C_MTpeak");
            float C_MTpeak_bottomright  = readSF(SRname,signalType,mStop+25,mLSP-25,"C_MTpeak");
            float C_0btag_topleft       = readSF(SRname,signalType,mStop-25,mLSP+25,"C_0btag");
            float C_0btag_bottomleft    = readSF(SRname,signalType,mStop-25,mLSP-25,"C_0btag");
            float C_0btag_topright      = readSF(SRname,signalType,mStop+25,mLSP+25,"C_0btag");
            float C_0btag_bottomright   = readSF(SRname,signalType,mStop+25,mLSP-25,"C_0btag");
            float C_SR_topleft          = readSF(SRname,signalType,mStop-25,mLSP+25,"C_SR");
            float C_SR_bottomleft       = readSF(SRname,signalType,mStop-25,mLSP-25,"C_SR");
            float C_SR_topright         = readSF(SRname,signalType,mStop+25,mLSP+25,"C_SR");
            float C_SR_bottomright      = readSF(SRname,signalType,mStop+25,mLSP-25,"C_SR");

            R_MTpeak = 0.0;
            R_0btag  = 0.0;
            C_MTpeak = 0.0;
            C_0btag  = 0.0;
            C_SR     = 0.0;

            int counter = 0;

            if (R_MTpeak_topleft     != 0) { R_MTpeak += R_MTpeak_topleft;     R_0btag += R_0btag_topleft;     C_MTpeak += C_MTpeak_topleft;     C_0btag += C_0btag_topleft;     C_SR += C_SR_topleft;    counter++; }
            if (R_MTpeak_topright    != 0) { R_MTpeak += R_MTpeak_topright;    R_0btag += R_0btag_topright;    C_MTpeak += C_MTpeak_topright;    C_0btag += C_0btag_topright;    C_SR += C_SR_topright;   counter++; }
            if (R_MTpeak_bottomleft  != 0) { R_MTpeak += R_MTpeak_bottomleft;  R_0btag += R_0btag_bottomleft;  C_MTpeak += C_MTpeak_bottomleft;  C_0btag += C_0btag_bottomleft;  C_SR += C_SR_bottomleft; counter++; }
            if (R_MTpeak_bottomright != 0) { R_MTpeak += R_MTpeak_bottomright; R_0btag += R_0btag_bottomright; C_MTpeak += C_MTpeak_bottomright; C_0btag += C_0btag_bottomright; C_SR += C_SR_bottomright;counter++; }
            if (counter              != 0) { R_MTpeak /= counter;              R_0btag /= counter;             C_MTpeak /= counter;              C_0btag /= counter;             C_SR /= counter;                       }
        }

        histoRMTpeak.Fill((float) mStop, (float) mLSP,R_MTpeak);
        histoR0btag .Fill((float) mStop, (float) mLSP,R_0btag );
        histoCMTpeak.Fill((float) mStop, (float) mLSP,C_MTpeak);
        histoC0btag .Fill((float) mStop, (float) mLSP,C_0btag );
        histoCSR    .Fill((float) mStop, (float) mLSP,C_SR    );
    }

    gStyle->SetPaintTextFormat("4.2f");
    TCanvas c("","",800,600);
    PlotDefaultStyles::ApplyDefaultAxisStyle(histoRMTpeak.GetXaxis(),"m_{#tilde{t}} [GeV]");
    PlotDefaultStyles::ApplyDefaultAxisStyle(histoRMTpeak.GetYaxis(),"m_{#chi^{0} [GeV]}" );
    histoRMTpeak.SetMinimum(0);
    histoRMTpeak.SetMaximum(0.5);
    histoRMTpeak.SetStats(0);
    histoRMTpeak.Draw("colz text");
    c.SaveAs(("outputs/R_MTpeak_"+signalType+".pdf").c_str());

    TCanvas c2("","",800,600);
    PlotDefaultStyles::ApplyDefaultAxisStyle(histoR0btag.GetXaxis(),"m_{#tilde{t}} [GeV]");
    PlotDefaultStyles::ApplyDefaultAxisStyle(histoR0btag.GetYaxis(),"m_{#chi^{0} [GeV]}" );
    histoR0btag.SetMinimum(0);
    histoR0btag.SetMaximum(0.5);
    histoR0btag.SetStats(0);
    histoR0btag.Draw("colz text");
    c2.SaveAs(("outputs/R_0btag_"+signalType+".pdf").c_str());

    TCanvas c3("","",800,600);
    PlotDefaultStyles::ApplyDefaultAxisStyle(histoC0btag.GetXaxis(),"m_{#tilde{t}} [GeV]");
    PlotDefaultStyles::ApplyDefaultAxisStyle(histoC0btag.GetYaxis(),"m_{#chi^{0} [GeV]}" );
    histoC0btag.SetMinimum(0);
    histoC0btag.SetMaximum(0.5);
    histoC0btag.SetStats(0);
    histoC0btag.Draw("colz text");
    c3.SaveAs(("outputs/C_0btag_"+signalType+".pdf").c_str());

    TCanvas c4("","",800,600);
    PlotDefaultStyles::ApplyDefaultAxisStyle(histoCMTpeak.GetXaxis(),"m_{#tilde{t}} [GeV]");
    PlotDefaultStyles::ApplyDefaultAxisStyle(histoCMTpeak.GetYaxis(),"m_{#chi^{0} [GeV]}" );
    histoCMTpeak.SetMinimum(0);
    histoCMTpeak.SetMaximum(0.5);
    histoCMTpeak.SetStats(0);
    histoCMTpeak.Draw("colz text");
    c4.SaveAs(("outputs/C_MTpeak_"+signalType+".pdf").c_str());

    TCanvas c5("","",800,600);
    PlotDefaultStyles::ApplyDefaultAxisStyle(histoCSR.GetXaxis(),"m_{#tilde{t}} [GeV]");
    PlotDefaultStyles::ApplyDefaultAxisStyle(histoCSR.GetYaxis(),"m_{#chi^{0} [GeV]}" );
    histoCSR.SetMinimum(0);
    histoCSR.SetMaximum(0.5);
    histoCSR.SetStats(0);
    histoCSR.Draw("colz text");
    c5.SaveAs(("outputs/C_SR_"+signalType+".pdf").c_str());
}

float readSF(string SR, string signalType_, int mStop, int mLSP, string option)
{
    ifstream f("BDT_"+SR+".dump");

    int counter = 0;
    float R_MTpeak = 0;
    float R_0btag  = 0;
    float C_MTpeak = 0;
    float C_0btag  = 0;
    float C_SR     = 0;
    string signalType;
    int inputmStop;
    int inputmLSP;

    while (1)
    {
        counter++;

        f >> signalType >> inputmStop >> inputmLSP >> C_MTpeak >> C_0btag >> C_SR >> R_MTpeak >> R_0btag;

        if (counter >= 1000) { return 0; }
        if (signalType != signalType_) continue;
        if ((inputmStop != mStop) || (inputmLSP != mLSP)) continue;
        else { break; }
    }

    //cout << signalType << " " << inputmStop << " " << inputmLSP << " " << C_MTpeak << " " << C_0btag << " " << C_SR << " " << R_MTpeak << " " << R_0btag << endl;

         if (option == "R_MTpeak") return R_MTpeak;
    else if (option == "R_0btag")  return R_0btag;
    else if (option == "C_MTpeak") return C_MTpeak;
    else if (option == "C_0btag")  return C_0btag;
    else if (option == "C_SR")     return C_SR;
    else                           return 0;
}

string signalRegionName(string decaymode, int stopmass, int lspmass)
{
    string SR = "";
    if (decaymode == "T2bw-075")
    {
        if (lspmass > stopmass - 200)
            SR = "T2bw075_1";
        if (lspmass <= stopmass - 200 && lspmass > stopmass - 325)
            SR = "T2bw075_2";
        if (lspmass <= stopmass - 325 && lspmass > stopmass - 475)
            SR = "T2bw075_3";
        if (lspmass <= stopmass - 475)
            SR = "T2bw075_5_lowDM";
        if (lspmass <= stopmass - 575)
            SR = "T2bw075_5_highDM";
    }
    if (decaymode == "T2bw-050")
    {
        if (lspmass > stopmass - 150){
            SR = "T2bw050_1_lowDM_lowLSP";
            if (lspmass >= 125) SR = "T2bw050_1_lowDM_highLSP";
        }
        if (lspmass <= stopmass - 150 && lspmass > stopmass - 250) {
            SR = "T2bw050_1_highDM";
        }
        if (lspmass <= stopmass - 250 && lspmass > stopmass - 400)
            SR = "T2bw050_3";
        if (lspmass <= stopmass - 400 && lspmass > stopmass - 550)
            SR = "T2bw050_4";
        if (lspmass <= stopmass - 550 && lspmass > stopmass - 625)
            SR = "T2bw050_5";
        if (lspmass <= stopmass - 625)
            SR = "T2bw050_6";
    }
    if (decaymode == "T2bw-025")
    {
        if (lspmass > stopmass - 225)
            SR = "T2bw025_1";
        if (lspmass <= stopmass - 225 && lspmass > stopmass - 275)
            SR = "T2bw025_3";
        if (lspmass <= stopmass - 275 && lspmass > stopmass - 375)
            SR = "T2bw025_3";
        if (lspmass <= stopmass - 375 && lspmass > stopmass - 600){
            SR = "T2bw025_4_highLSP";
            if (lspmass <= 100) SR = "T2bw025_4_lowLSP";
        }
        if (lspmass <= stopmass - 600)
            SR = "T2bw025_6";
    }
    if (decaymode == "T2tt")
    {
        if (lspmass > stopmass - 225 ) {
            SR = "T2tt_1_highLSP";
            if (lspmass <= 100 && lspmass > 50)
                SR = "T2tt_1_mediumLSP";
            if (lspmass <= 50)
                SR = "T2tt_1_lowLSP";
        }
        if (lspmass <= stopmass - 225 && lspmass > stopmass - 325)
            SR = "T2tt_2";
        if (lspmass <= stopmass - 325 && lspmass > stopmass - 400)
            SR = "T2tt_5_lowDM";
        if (lspmass <= stopmass - 400 )
            SR = "T2tt_5_highDM";
    }
    return SR;
}




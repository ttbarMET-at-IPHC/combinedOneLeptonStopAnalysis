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
    TH2F histoSFpre   ("SF_pre",   "",28,112.5,812.5,16,-12.5,387.5);
    TH2F histoSFpost  ("SF_post",  "",28,112.5,812.5,16,-12.5,387.5);
    TH2F histoSFR1ltop("SFR_1ltop","",28,112.5,812.5,16,-12.5,387.5);
    TH2F histoSFRWjets("SFR_Wjets","",28,112.5,812.5,16,-12.5,387.5);

    string signalType = "T2tt";

    float mStopLimit = 0;
    if (signalType == "T2tt"    ) mStopLimit = 475;
    if (signalType == "T2bw-075") mStopLimit = 475;
    if (signalType == "T2bw-050") mStopLimit = 575;
    if (signalType == "T2bw-025") mStopLimit = 525;

    for (int mStop = 125 ; mStop <= 800 ; mStop += 25)
    for (int mLSP  =  0  ; mLSP  < 400 ; mLSP  += 25)
    {
        float SF_pre    = -1.0;
        float SF_post   = -1.0;
        float SFR_1ltop = -1.0;
        float SFR_Wjets = -1.0;

        string SRname;

        if (mStop - mLSP <  100) continue;
        if ((mStop - mLSP > 300) && (mStop > mStopLimit)) continue;

        SRname = signalRegionName(signalType,mStop,mLSP);

        if (((mStop - 125) % 50 == 0) && ((mLSP  -  25) % 50 == 0))
        {
            SF_pre    = readSF(SRname,signalType,mStop,mLSP,"SF_pre" );
            SF_post   = readSF(SRname,signalType,mStop,mLSP,"SF_post");
            SFR_1ltop = readSF(SRname,signalType,mStop,mLSP,"SFR_1ltop");
            SFR_Wjets = readSF(SRname,signalType,mStop,mLSP,"SFR_Wjets"  );
        }
        else if (((mStop - 125) % 50 != 0) && ((mLSP  -  25) % 50 == 0))
        {
            float SF_pre_left     = readSF(SRname,signalType,mStop-25,mLSP,"SF_pre");
            float SF_pre_right    = readSF(SRname,signalType,mStop+25,mLSP,"SF_pre");
            float SF_post_left    = readSF(SRname,signalType,mStop-25,mLSP,"SF_post");
            float SF_post_right   = readSF(SRname,signalType,mStop+25,mLSP,"SF_post");
            float SFR_1ltop_left  = readSF(SRname,signalType,mStop-25,mLSP,"SFR_1ltop");
            float SFR_1ltop_right = readSF(SRname,signalType,mStop+25,mLSP,"SFR_1ltop");
            float SFR_Wjets_left  = readSF(SRname,signalType,mStop-25,mLSP,"SFR_Wjets");
            float SFR_Wjets_right = readSF(SRname,signalType,mStop+25,mLSP,"SFR_Wjets");

            SF_pre    = 0.0;
            SF_post   = 0.0;
            SFR_1ltop = 0.0;
            SFR_Wjets = 0.0;

            int counter = 0;

            if (SF_pre_left   != 0) { SF_pre += SF_pre_left;     SF_post += SF_post_left;    SFR_1ltop += SFR_1ltop_left;    SFR_Wjets += SFR_Wjets_left;    counter++; }
            if (SF_pre_right  != 0) { SF_pre += SF_pre_right;    SF_post += SF_post_right;   SFR_1ltop += SFR_1ltop_right;   SFR_Wjets += SFR_Wjets_right;   counter++; }
            if (counter       != 0) { SF_pre /= counter;         SF_post /= counter;         SFR_1ltop /= counter;           SFR_Wjets /= counter;  }
        }
        else if (((mStop - 125) % 50 == 0) && ((mLSP  -  25) % 50 != 0))
        {
            float SF_pre_top       = readSF(SRname,signalType,mStop,mLSP+25,"SF_pre");
            float SF_pre_bottom    = readSF(SRname,signalType,mStop,mLSP-25,"SF_pre");
            float SF_post_top      = readSF(SRname,signalType,mStop,mLSP+25,"SF_post");
            float SF_post_bottom   = readSF(SRname,signalType,mStop,mLSP-25,"SF_post");
            float SFR_1ltop_top    = readSF(SRname,signalType,mStop,mLSP+25,"SFR_1ltop");
            float SFR_1ltop_bottom = readSF(SRname,signalType,mStop,mLSP-25,"SFR_1ltop");
            float SFR_Wjets_top    = readSF(SRname,signalType,mStop,mLSP+25,"SFR_Wjets");
            float SFR_Wjets_bottom = readSF(SRname,signalType,mStop,mLSP-25,"SFR_Wjets");

            SF_pre    = 0.0;
            SF_post   = 0.0;
            SFR_1ltop = 0.0;
            SFR_Wjets = 0.0;

            int counter = 0;

            if (SF_pre_top     != 0) { SF_pre += SF_pre_top;      SF_post += SF_post_top;     SFR_1ltop += SFR_1ltop_top;     SFR_Wjets += SFR_Wjets_top;     counter++; }
            if (SF_pre_bottom  != 0) { SF_pre += SF_pre_bottom;   SF_post += SF_post_bottom;  SFR_1ltop += SFR_1ltop_bottom;  SFR_Wjets += SFR_Wjets_bottom;  counter++; }
            if (counter        != 0) { SF_pre /= counter;         SF_post /= counter;         SFR_1ltop /= counter;           SFR_Wjets /= counter;  }
        }
        else
        {
            float SF_pre_topleft        = readSF(SRname,signalType,mStop-25,mLSP+25,"SF_pre");
            float SF_pre_bottomleft     = readSF(SRname,signalType,mStop-25,mLSP-25,"SF_pre");
            float SF_pre_topright       = readSF(SRname,signalType,mStop+25,mLSP+25,"SF_pre");
            float SF_pre_bottomright    = readSF(SRname,signalType,mStop+25,mLSP-25,"SF_pre");
            float SF_post_topleft       = readSF(SRname,signalType,mStop-25,mLSP+25,"SF_post");
            float SF_post_bottomleft    = readSF(SRname,signalType,mStop-25,mLSP-25,"SF_post");
            float SF_post_topright      = readSF(SRname,signalType,mStop+25,mLSP+25,"SF_post");
            float SF_post_bottomright   = readSF(SRname,signalType,mStop+25,mLSP-25,"SF_post");
            float SFR_1ltop_topleft     = readSF(SRname,signalType,mStop-25,mLSP+25,"SFR_1ltop");
            float SFR_1ltop_bottomleft  = readSF(SRname,signalType,mStop-25,mLSP-25,"SFR_1ltop");
            float SFR_1ltop_topright    = readSF(SRname,signalType,mStop+25,mLSP+25,"SFR_1ltop");
            float SFR_1ltop_bottomright = readSF(SRname,signalType,mStop+25,mLSP-25,"SFR_1ltop");
            float SFR_Wjets_topleft     = readSF(SRname,signalType,mStop-25,mLSP+25,"SFR_Wjets");
            float SFR_Wjets_bottomleft  = readSF(SRname,signalType,mStop-25,mLSP-25,"SFR_Wjets");
            float SFR_Wjets_topright    = readSF(SRname,signalType,mStop+25,mLSP+25,"SFR_Wjets");
            float SFR_Wjets_bottomright = readSF(SRname,signalType,mStop+25,mLSP-25,"SFR_Wjets");

            SF_pre    = 0.0;
            SF_post   = 0.0;
            SFR_1ltop = 0.0;
            SFR_Wjets = 0.0;

            int counter = 0;

            if (SF_pre_topleft     != 0) { SF_pre += SF_pre_topleft;     SF_post += SF_post_topleft;     SFR_1ltop += SFR_1ltop_topleft;        SFR_Wjets += SFR_Wjets_topleft;     counter++; }
            if (SF_pre_topright    != 0) { SF_pre += SF_pre_topright;    SF_post += SF_post_topright;    SFR_1ltop += SFR_1ltop_topright;       SFR_Wjets += SFR_Wjets_topright;    counter++; }
            if (SF_pre_bottomleft  != 0) { SF_pre += SF_pre_bottomleft;  SF_post += SF_post_bottomleft;  SFR_1ltop += SFR_1ltop_bottomleft;     SFR_Wjets += SFR_Wjets_bottomleft;  counter++; }
            if (SF_pre_bottomright != 0) { SF_pre += SF_pre_bottomright; SF_post += SF_post_bottomright; SFR_1ltop += SFR_1ltop_bottomright;    SFR_Wjets += SFR_Wjets_bottomright; counter++; }
            if (counter            != 0) { SF_pre /= counter;            SF_post /= counter;             SFR_1ltop /= counter;                  SFR_Wjets /= counter;  }
        }

        histoSFpre   .Fill((float) mStop, (float) mLSP,SF_pre);
        histoSFpost  .Fill((float) mStop, (float) mLSP,SF_post);
        histoSFR1ltop.Fill((float) mStop, (float) mLSP,SFR_1ltop);
        histoSFRWjets.Fill((float) mStop, (float) mLSP,SFR_Wjets);
    }

    gStyle->SetPaintTextFormat("4.2f");
    TCanvas c("","",800,600);
    PlotDefaultStyles::ApplyDefaultAxisStyle(histoSFpre.GetXaxis(),"m_{#tilde{t}} [GeV]");
    PlotDefaultStyles::ApplyDefaultAxisStyle(histoSFpre.GetYaxis(),"m_{#chi^{0} [GeV]}" );
    histoSFpre.SetMinimum(0.1);
    histoSFpre.SetStats(0);
    histoSFpre.Draw("colz text");
    c.SaveAs(("outputs/SF_pre_"+signalType+".pdf").c_str());

    TCanvas c2("","",800,600);
    PlotDefaultStyles::ApplyDefaultAxisStyle(histoSFpost.GetXaxis(),"m_{#tilde{t}} [GeV]");
    PlotDefaultStyles::ApplyDefaultAxisStyle(histoSFpost.GetYaxis(),"m_{#chi^{0} [GeV]}" );
    histoSFpost.SetMinimum(0.1);
    histoSFpost.SetStats(0);
    histoSFpost.Draw("colz text");
    c2.SaveAs(("outputs/SF_post_"+signalType+".pdf").c_str());

    TCanvas c3("","",800,600);
    PlotDefaultStyles::ApplyDefaultAxisStyle(histoSFR1ltop.GetXaxis(),"m_{#tilde{t}} [GeV]");
    PlotDefaultStyles::ApplyDefaultAxisStyle(histoSFR1ltop.GetYaxis(),"m_{#chi^{0} [GeV]}" );
    histoSFR1ltop.SetMinimum(0.8);
    histoSFR1ltop.SetStats(0);
    histoSFR1ltop.Draw("colz text");
    c3.SaveAs(("outputs/SFR_1ltop_"+signalType+".pdf").c_str());

    TCanvas c4("","",800,600);
    PlotDefaultStyles::ApplyDefaultAxisStyle(histoSFRWjets.GetXaxis(),"m_{#tilde{t}} [GeV]");
    PlotDefaultStyles::ApplyDefaultAxisStyle(histoSFRWjets.GetYaxis(),"m_{#chi^{0} [GeV]}" );
    histoSFRWjets.SetMinimum(0.8);
    histoSFRWjets.SetStats(0);
    histoSFRWjets.Draw("colz text");
    c4.SaveAs(("outputs/SFR_Wjets_"+signalType+".pdf").c_str());

}

float readSF(string SR, string signalType_, int mStop, int mLSP, string option)
{
    ifstream f("BDT_"+SR+".dump");

    int counter = 0;
    float SF_pre = 0;
    float SF_post = 0;
    float SFR_1ltop = 0;
    float SFR_Wjets = 0;
    string signalType;
    int inputmStop;
    int inputmLSP;

    while (1)
    {
        counter++;

        f >> signalType >> inputmStop >> inputmLSP >> SF_pre >> SF_post >> SFR_1ltop >> SFR_Wjets;

        if (counter >= 1000) { return 0; }
        if (signalType != signalType_) continue;
        if ((inputmStop != mStop) || (inputmLSP != mLSP)) continue;
        else { break; }
    }

         if (option == "SF_pre")    return SF_pre;
    else if (option == "SF_post")   return SF_post;
    else if (option == "SFR_1ltop") return SFR_1ltop;
    else if (option == "SFR_Wjets") return SFR_Wjets;
    else                            return 0;
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




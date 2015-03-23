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
float readGlobalSF(string SR, string signalType, int mStop, int mLSP, string option = "");

string polarizationScenario;

int main(int argc, char *argv[])
{

    if (argc < 3) { cerr << "Missing argument" << endl; return -1; }

    PlotDefaultStyles::SetSmoothPalette("soft");

    polarizationScenario = argv[1];
    string signalType = argv[2];

    TH2F globalSFmap("globalSFmap",("B_{SC-corrected}/B_{no SC} for "+signalType+" signal type").c_str(),28,112.5,812.5,16,-12.5,387.5);
    TH2F globalSFmap_uncert("globalSFmap_uncert","",28,112.5,812.5,16,-12.5,387.5);
    float mStopLimit = 0;
    if (signalType == "T2tt"    ) mStopLimit = 475;
    if (signalType == "T2bw-075") mStopLimit = 475;
    if (signalType == "T2bw-050") mStopLimit = 575;
    if (signalType == "T2bw-025") mStopLimit = 525;

    ofstream output("outputs/backgroundPrediction_BDT_"+signalType+".C");

    output << "pair<float,float> backgroundPrediction_BDT(int mStop, int mLSP)" << endl;
    output << "{" << endl;

    for (int mStop = 125 ; mStop <= 800 ; mStop += 25)
    for (int mLSP  =  0  ; mLSP  < 400 ; mLSP  += 25)
    {
        float globalSF                      = 1.0;
        float globalSF_uncert               = 1.0;
        float uncorrected_background_yield  = 1.0;
        float uncorrected_background_uncert = 1.0;
        string SRname;
        if (mStop - mLSP <  100) continue;

        output << "     if ((mStop == " << mStop << ") && (mLSP == "<< mLSP << ")) return pair<float,float>";

/*        if ((mStop - mLSP >= 300) && (mStop > 600))
        {
            if (
            //    ((mStop == 450) && (mLSP == 50))
            // || ((mStop == 550) && (mLSP == 100))
             //||
             ((mStop == 650) && (mLSP == 50)))
            {
                SRname = signalRegionName(signalType,mStop,mLSP);
                globalSF = readGlobalSF(SRname,signalType,mStop,mLSP);
                globalSFmap.Fill((float) mStop, (float) mLSP,globalSF);
            }
            continue;
        }
        */

        SRname = signalRegionName(signalType,mStop,mLSP);

        if ((((mStop - mLSP > 300) && (mStop > mStopLimit)) || (mLSP > 250))
                // Very dirty fix, if someome see this, you're allowed to wipe me hard
         || ((polarizationScenario != "nominal") && (signalType == "T2bw-050") && (mStop == 575) ))
        {
            uncorrected_background_yield  = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_yield" );
            uncorrected_background_uncert = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_uncert");

            globalSFmap.Fill((float) mStop, (float) mLSP,1.0);

            output << "(" << uncorrected_background_yield << "," << uncorrected_background_uncert << ");" << endl;
            continue;
        }

             if (((mStop - 125) % 50 == 0) && ((mLSP  -  25) % 50 == 0))
        {
            globalSF                      = readGlobalSF(SRname,signalType,mStop,mLSP);
            globalSF_uncert               = readGlobalSF(SRname,signalType,mStop,mLSP,"globalSF_uncert"  );
            uncorrected_background_yield  = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_yield" );
            uncorrected_background_uncert = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_uncert");
        }
        else if (((mStop - 125) % 50 != 0) && ((mLSP  -  25) % 50 == 0))
        {
            float SFleft  = readGlobalSF(SRname,signalType,mStop-25,mLSP);
            float SFright = readGlobalSF(SRname,signalType,mStop+25,mLSP);
            float SFleft_uncert  = readGlobalSF(SRname,signalType,mStop-25,mLSP,"globalSF_uncert");
            float SFright_uncert = readGlobalSF(SRname,signalType,mStop+25,mLSP,"globalSF_uncert");

            globalSF = 0.0;
            globalSF_uncert = 0.0;
            int counter = 0;

            if (SFleft  != 0) { globalSF += SFleft;  globalSF_uncert += SFleft_uncert; counter++;
                                uncorrected_background_yield  = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_yield" );
                                uncorrected_background_uncert = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_uncert"); }
            if (SFright != 0) { globalSF += SFright; globalSF_uncert += SFright_uncert; counter++;
                                uncorrected_background_yield  = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_yield" );
                                uncorrected_background_uncert = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_uncert"); }

            if (counter != 0) { globalSF /= counter; globalSF_uncert /= counter; }
        }
        else if (((mStop - 125) % 50 == 0) && ((mLSP  -  25) % 50 != 0))
        {
            float SFbottom = readGlobalSF(SRname,signalType,mStop,mLSP-25);
            float SFtop    = readGlobalSF(SRname,signalType,mStop,mLSP+25);
            float SFbottom_uncert = readGlobalSF(SRname,signalType,mStop,mLSP-25,"globalSF_uncert");
            float SFtop_uncert    = readGlobalSF(SRname,signalType,mStop,mLSP+25,"globalSF_uncert");
            globalSF = 0.0;
            globalSF_uncert = 0.0;
            int counter = 0;
            if (SFbottom != 0) { globalSF += SFbottom; globalSF_uncert += SFbottom_uncert; counter++;
                                 uncorrected_background_yield  = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_yield" );
                                 uncorrected_background_uncert = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_uncert"); }
            if (SFtop != 0)    { globalSF += SFtop;    globalSF_uncert += SFtop_uncert; counter++;
                                 uncorrected_background_yield  = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_yield" );
                                 uncorrected_background_uncert = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_uncert"); }

            if (counter != 0)  { globalSF /= counter; globalSF_uncert /= counter; }
        }
        else
        {
            float SFbottomleft  = readGlobalSF(SRname,signalType,mStop-25,mLSP-25);
            float SFbottomright = readGlobalSF(SRname,signalType,mStop+25,mLSP-25);
            float SFtopleft     = readGlobalSF(SRname,signalType,mStop-25,mLSP+25);
            float SFtopright    = readGlobalSF(SRname,signalType,mStop+25,mLSP+25);
            float SFbottomleft_uncert  = readGlobalSF(SRname,signalType,mStop-25,mLSP-25,"globalSF_uncert");
            float SFbottomright_uncert = readGlobalSF(SRname,signalType,mStop+25,mLSP-25,"globalSF_uncert");
            float SFtopleft_uncert     = readGlobalSF(SRname,signalType,mStop-25,mLSP+25,"globalSF_uncert");
            float SFtopright_uncert    = readGlobalSF(SRname,signalType,mStop+25,mLSP+25,"globalSF_uncert");

            globalSF = 0.0;
            globalSF_uncert = 0.0;
            int counter = 0;

            if (SFbottomleft  != 0) { globalSF += SFbottomleft;  globalSF_uncert += SFbottomleft_uncert; counter++;
                                      uncorrected_background_yield  = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_yield" );
                                      uncorrected_background_uncert = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_uncert"); }
            if (SFbottomright != 0) { globalSF += SFbottomright; globalSF_uncert += SFbottomright_uncert; counter++;
                                      uncorrected_background_yield  = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_yield" );
                                      uncorrected_background_uncert = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_uncert"); }
            if (SFtopleft     != 0) { globalSF += SFtopleft;     globalSF_uncert += SFtopleft_uncert; counter++;
                                      uncorrected_background_yield  = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_yield" );
                                      uncorrected_background_uncert = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_uncert"); }
            if (SFtopright    != 0) { globalSF += SFtopright;    globalSF_uncert += SFtopright_uncert; counter++;
                                      uncorrected_background_yield  = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_yield" );
                                      uncorrected_background_uncert = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_uncert"); }

            if (counter != 0)  { globalSF /= counter; globalSF_uncert /= counter; }
        }

        output << "(" << uncorrected_background_yield*globalSF << "," << uncorrected_background_uncert*globalSF_uncert << ");" << endl;
        globalSFmap.Fill((float) mStop, (float) mLSP,globalSF);
        globalSFmap_uncert.Fill((float) mStop, (float) mLSP,globalSF_uncert);
    }


    output << "     cout << \"Warning : looking for prediction for mStop,mLSP = \" << mStop << \",\" << mLSP << \" but no prediction available...\"<< endl; " << endl;
    output << "     return pair<float,float>(-1.0,-1.0);" << endl;
    output << "}" << endl;

    gStyle->SetPaintTextFormat("4.2f");
    TCanvas c("","",800,600);
    PlotDefaultStyles::ApplyDefaultAxisStyle(globalSFmap.GetXaxis(),"m_{#tilde{t}} [GeV]");
    PlotDefaultStyles::ApplyDefaultAxisStyle(globalSFmap.GetYaxis(),"m_{#chi^{0} [GeV]}" );
    globalSFmap.SetMinimum(0.7);
    globalSFmap.SetStats(0);
    globalSFmap.Draw("colz text");
    c.Update();
    TPaletteAxis *pal = (TPaletteAxis*) globalSFmap.GetListOfFunctions()->FindObject("palette");
    if (pal != 0)
    {
        pal->SetX1NDC(0.901);
        pal->SetY1NDC(0.1);
        pal->SetX2NDC(0.93);
        pal->SetY2NDC(0.9);
    }

    c.SaveAs(("outputs/globalSFmap_"+signalType+".pdf").c_str());
    c.SaveAs(("outputs/globalSFmap_"+signalType+".root").c_str());

    TCanvas c2("","",800,600);
    PlotDefaultStyles::ApplyDefaultAxisStyle(globalSFmap_uncert.GetXaxis(),"m_{#tilde{t}} [GeV]");
    PlotDefaultStyles::ApplyDefaultAxisStyle(globalSFmap_uncert.GetYaxis(),"m_{#chi^{0}} [GeV]" );
    globalSFmap_uncert.SetMinimum(0.7);
    globalSFmap_uncert.SetStats(0);
    globalSFmap_uncert.Draw("colz text");
    c2.SaveAs(("outputs/globalSFmap_uncert_"+signalType+".pdf").c_str());
    c2.SaveAs(("outputs/globalSFmap_uncert_"+signalType+".root").c_str());


}

float readGlobalSF(string SR, string signalType_, int mStop, int mLSP, string option)
{
    ifstream f(polarizationScenario+"/BDT_"+SR+".dump");

    int counter = 0;
    float globalSF_yield = 0;
    float globalSF_uncert = 0;
    string signalType;
    int inputmStop;
    int inputmLSP;
    float uncorrected_bkg_yield;
    float uncorrected_bkg_uncert;
    float corrected_bkg_yield;
    float corrected_bkg_uncert;

    while (1)
    {
        counter++;

        f >> signalType >> inputmStop >> inputmLSP >> uncorrected_bkg_yield >> uncorrected_bkg_uncert >> corrected_bkg_yield >> corrected_bkg_uncert >> globalSF_yield >> globalSF_uncert;

        if (counter >= 1000) { return 0; }
        if (signalType != signalType_) continue;
        if ((inputmStop != mStop) || (inputmLSP != mLSP)) continue;
        else { break; }

    }

    if (globalSF_yield == -1) globalSF_yield = 0;

    if (globalSF_yield  < 0) globalSF_yield = 0;
    if (globalSF_uncert < 0) globalSF_uncert = 0;
    if (uncorrected_bkg_yield < 0) { cout << "WARNING - uncorrected bkg yield < 0, that should not happen at all !" << endl; exit(-1); }

         if (option == "")                  return globalSF_yield;
    else if (option == "globalSF_uncert")   return globalSF_uncert;
    else if (option == "uncorr_bkg_yield")  return uncorrected_bkg_yield;
    else if (option == "uncorr_bkg_uncert") return uncorrected_bkg_uncert;
    else                                    return 0;
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




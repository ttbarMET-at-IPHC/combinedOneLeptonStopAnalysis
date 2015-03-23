
#include "../../../common.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"

float readGlobalSF(string SR, string signalType, int mStop, int mLSP, string option = "");

int main()
{
    vector<string> signalTypes = { "T2tt", "T2bw075", "T2bw050", "T2bw025" };

    vector<string> SRnames_T2bw025 = { "highMasses", "lowMasses", "offShell" };
    vector<string> SRnames_T2bw050 = { "highDeltaM", "lowMasses", "mediumDeltaM", "offShell" };
    vector<string> SRnames_T2bw075 = { "highDeltaM", "lowDeltaM", "mediumDeltaM" };
    vector<string> SRnames_T2tt    = { "highDeltaM", "lowDeltaM", "mediumDeltaM", "offShellLoose", "offShellTight" };

    for (unsigned int st = 0 ; st < signalTypes.size() ; st++)
    {
        string signalType = signalTypes[st];

        float mStopLimit = 0;
        vector<string> SRnames;

        if (signalType == "T2tt"    ) { mStopLimit = 475; SRnames = SRnames_T2tt;    }
        if (signalType == "T2bw075") { mStopLimit = 475; SRnames  = SRnames_T2bw075; }
        if (signalType == "T2bw050") { mStopLimit = 575; SRnames  = SRnames_T2bw050; }
        if (signalType == "T2bw025") { mStopLimit = 525; SRnames  = SRnames_T2bw025; }

        for (unsigned int SRid = 0 ; SRid < SRnames.size() ; SRid++)
        {
            string SRname = SRnames[SRid];
            SRname = signalType + "_" + SRname;

            TH2F globalSFmap("globalSFmap","",28,112.5,812.5,16,-12.5,387.5);
            TH2F globalSFmap_uncert("globalSFmap_uncert","",28,112.5,812.5,16,-12.5,387.5);

            ofstream output("outputs/backgroundPrediction_CnC_"+SRname+".C");

            output << "pair<float,float> backgroundPrediction_CnC(int mStop, int mLSP)" << endl;
            output << "{" << endl;

            for (int mStop = 125 ; mStop <= 800 ; mStop += 25)
            for (int mLSP  =  0  ; mLSP  < 400 ; mLSP  += 25)
            {
                float globalSF                      = 1.0;
                float globalSF_uncert               = 1.0;
                float uncorrected_background_yield  = 1.0;
                float uncorrected_background_uncert = 1.0;
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

                if ((mStop - mLSP > 300) && (mStop > mStopLimit))
                {
                    uncorrected_background_yield  = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_yield" );
                    uncorrected_background_uncert = readGlobalSF(SRname,signalType,mStop,mLSP,"uncorr_bkg_uncert");

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
            globalSFmap.SetMinimum(0.7);
            globalSFmap.SetStats(0);
            globalSFmap.Draw("colz text");
            c.SaveAs(("outputs/globalSFmap_"+SRname+".pdf").c_str());

            TCanvas c2("","",800,600);
            globalSFmap_uncert.SetMinimum(0.7);
            globalSFmap_uncert.SetStats(0);
            globalSFmap_uncert.Draw("colz text");
            c2.SaveAs(("outputs/globalSFmap_uncert_"+SRname+".pdf").c_str());

        }
    }
}

float readGlobalSF(string SR, string signalType_, int mStop, int mLSP, string option)
{
    if (signalType_ == "T2bw075") signalType_ = "T2bw-075";
    if (signalType_ == "T2bw050") signalType_ = "T2bw-050";
    if (signalType_ == "T2bw025") signalType_ = "T2bw-025";

    ifstream f("cutAndCount_"+SR+".dump");

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

         if (option == "")                  return globalSF_yield;
    else if (option == "globalSF_uncert")   return globalSF_uncert;
    else if (option == "uncorr_bkg_yield")  return uncorrected_bkg_yield;
    else if (option == "uncorr_bkg_uncert") return uncorrected_bkg_uncert;
    else                                    return 0;
}


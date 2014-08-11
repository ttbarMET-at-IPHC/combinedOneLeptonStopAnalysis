//#include "../common.h"
#include <fstream>

// Sonic screwdriver headers

#include "interface/Table.h"
#include "interface/SonicScrewdriver.h"
using namespace theDoctor;

#include "../backgroundEstimation_common/common.h"

#include <string>

//-- Root dependancies
#include <TCanvas.h>
#include <TH1F.h>
#include <TLegend.h>
#include <TStyle.h>

#define CR4MTtailUncert 0.25
#define ExtrapUncert 0.2

int main (int argc, char *argv[])
{
    loadBDTSignalRegions();

    gStyle->SetOptStat(0);
    gStyle->SetLegendFillColor();

    if (argc <= 1) { WARNING_MSG << "No signal region specified" << endl; return -1; }

    // Read list of signal regions to consider

    vector<string> signalRegionsTagList;
    for (int i = 1 ; i < argc ; i++)
        signalRegionsTagList.push_back(argv[i]);

    // List of CR
    vector<string> customRequirementList =
    {
        "30evts_in_CR4_2jets_MTTail",
        "30evts_in_CR4_3jets_MTTail",
        "30evts_in_CR4_4jets_MTTail"
    };
    vector<string> CRTagLabel =
    {
        "CR 2leptons, Njets#geq2 (using loosened cut 2 jets)",
        "CR 2leptons, Njets#geq3 (using loosened cut 3 jets)",
        "CR 2leptons, Njets#geq4 (using loosened cut 4 jets)",
        "CR antiveto (using loosened cut 2 jets)",
        "CR antiveto (using loosened cut 3 jets)",
        "CR antiveto (using loosened cut 4 jets)",
    };

    vector<Color_t> CRColorList = {kRed,kOrange-1,kBlue-1,kGreen-1,kOrange+1,kBlue+1,kGreen+1};

    vector<int> CRMarkerStyle = {28,22,20,25,23,24,26};

    // Read tables for each CR
    vector<vector<Table> > SF_est;
    for (unsigned int i = 0 ; i < customRequirementList.size() ; i++)
    {
        vector<Table> tables;
        for (unsigned int j = 0 ; j < signalRegionsTagList.size() ; j++)
        {
            tables.push_back(Table("scaleFactors/"+customRequirementList[i]+"/"+signalRegionsTagList[j]+".tab"));
        }
        SF_est.push_back(tables);
    }
    vector<vector<Table> > SF_est_CR4;

    // Create a plot for each CR
    vector<TH1F*> CRplots(customRequirementList.size()*2);
    for (unsigned int i = 0 ; i < customRequirementList.size() ; i++)
    {
        string hname = "h_"+customRequirementList[i];
        CRplots[i]                              = new TH1F(("CR4_with_"+hname).c_str(),"",signalRegionsTagList.size(),0,signalRegionsTagList.size());
        CRplots[i+customRequirementList.size()] = new TH1F(("CR5_with_"+hname).c_str(),"",signalRegionsTagList.size(),0,signalRegionsTagList.size());
    }

    for (unsigned int i = 0 ; i < customRequirementList.size() * 2 ; i++)
    {
        for(unsigned int j=0;j<signalRegionsTagList.size();j++)
        {
            CRplots[i]->GetXaxis()->SetBinLabel(j+1, signalRegionLabel(signalRegionsTagList[j], "root").c_str());
        }
        CRplots[i]->SetLineColor(CRColorList[i]);
        CRplots[i]->SetLineWidth(2);
        CRplots[i]->SetMarkerStyle(CRMarkerStyle[i]);
        CRplots[i]->SetMarkerColor(CRColorList[i]);
        CRplots[i]->SetMarkerSize(1.5);
    }

    TH1F* h_cr4_4j_noncorr = new TH1F("h_cr4_4j_noncorr","",signalRegionsTagList.size(),0,signalRegionsTagList.size());
    for(unsigned int j=0;j<signalRegionsTagList.size();j++)
    {
        h_cr4_4j_noncorr->GetXaxis()->SetBinLabel(j+1, signalRegionLabel(signalRegionsTagList[j], "root").c_str());
    }


    //Fill the histo
    for (unsigned int i = 0 ; i < customRequirementList.size() ; i++)
    {
        for (unsigned int j = 0 ; j < signalRegionsTagList.size() ; j++)
        {
            Figure SF_CR4      = SF_est[i][j].Get("value","SF_2l");
            Figure SF_CR4_tail = SF_est[i][j].Get("value","SF_2ltail");
            Figure SF_CR5_peak = SF_est[i][j].Get("value","SF_vetopeak");
            Figure SF_CR5_tail = SF_est[i][j].Get("value","SF_vetotail");

            // "Old" version
            //CRplots[i]->SetBinContent(j+1,SF_CR4.value());
            //CRplots[i]->SetBinError(j+1,sqrt(pow(SF_CR4.error(),2.)+pow(SF_CR4.value()*CR4MTtailUncert,2)));

            // New version
            CRplots[i]->SetBinContent(j+1,SF_CR4_tail.value());
            CRplots[i]->SetBinError  (j+1,SF_CR4_tail.error());

            if (customRequirementList[i].find("4jets") != string::npos)
            {
                h_cr4_4j_noncorr->SetBinContent(j+1,SF_CR4_tail.value());
                h_cr4_4j_noncorr->SetBinError  (j+1,SF_CR4_tail.error());
            }

            CRplots[i+customRequirementList.size()]->SetBinContent(j+1,SF_CR5_tail.value());
            CRplots[i+customRequirementList.size()]->SetBinError  (j+1,SF_CR5_tail.error());
        }
    }

    TCanvas c1;
    TLegend legCR(0.55,0.78,0.99,0.99);
    for (unsigned int i = 0 ; i < customRequirementList.size() * 2; i++)
    {
        if (i == 0)
        {
            CRplots[i]->SetMinimum(-0.5);
            CRplots[i]->SetMaximum(2.5);
            CRplots[i]->Draw();
        }
        else
            CRplots[i]->Draw("same");

        legCR.AddEntry(CRplots[i],CRTagLabel[i].c_str(),"lp");
    }

    //determine the enveloppe
    TH1F* hMin = (TH1F*) CRplots[0]->Clone();
    TH1F* hMax = (TH1F*) CRplots[0]->Clone();
    hMin->SetLineColor(kBlack);
    hMax->SetLineColor(kBlack);
    hMin->SetLineWidth(3);
    hMax->SetLineWidth(3);
    for (unsigned int j = 0 ; j < signalRegionsTagList.size() ; j++)
    {
        float min = 10.;
        float max = 0.;
        float minError = 10;
        //First: find the value compatible with one with the lowest uncertainty
        for (unsigned int i = 0 ; i < customRequirementList.size() * 2; i++)
        {
            float value = CRplots[i]->GetBinContent(j+1);
            float error = CRplots[i]->GetBinError(j+1);

            if((value-error)<=1 && (value+error)>=1)
            {
                if(error<minError)
                {
                    minError = error;
                    max = value+error;
                    min = value-error;
                }
            }
        }
        for (unsigned int i = 0 ; i < customRequirementList.size() * 2 ; i++)
        {
            float value = CRplots[i]->GetBinContent(j+1);

            //Use central value
            if(value<min) min = value;
            if(value>max) max = value;
        }
        hMin->SetBinContent(j+1,min);
        hMin->SetBinError(j+1,0);
        hMax->SetBinContent(j+1,max);
        hMax->SetBinError(j+1,0);
    }
    hMin->Draw("histsame");
    hMax->Draw("histsame");
    legCR.SetFillColor(kWhite);
    legCR.Draw("same");

    TFile fout("results/plots_BDT.root","RECREATE");
    c1.Write();
    //fout.Close();

    //Plots CR4
    TCanvas cR4;
    h_cr4_4j_noncorr->SetMinimum(0.);
    h_cr4_4j_noncorr->SetMaximum(2.);
    h_cr4_4j_noncorr->Draw();
    cR4.Write();


    //-------------------------------------------------------------------------------
    // Create a table with the uncertainy for each SR
    //-------------------------------------------------------------------------------
    vector<string> columns = {"value"};
    //Table SF_tt2l(signalRegionsTagList,columns,signalRegionsTagList,columns);
    Table SF_tt2l(columns,signalRegionsTagList,columns,signalRegionsTagList);
    for (unsigned int j = 0 ; j < signalRegionsTagList.size() ; j++)
    {
        float min = hMin->GetBinContent(j+1);
        float max = hMax->GetBinContent(j+1);

        float uncert = 1-min;
        if ((max-1)>uncert) uncert = max-1;
        uncert*=(1+ExtrapUncert);
        Figure funcert(1.0,uncert);

        SF_tt2l.Set("value",signalRegionsTagList[j],funcert);
    }

    SF_tt2l.Print("results/uncertainties_BDT.tab",4);


    //-------------------------------------------------------------------------------
    // Perform plots as function of the stat (30-50-100 evts in the tail of the BDTs)
    //-------------------------------------------------------------------------------

    // List of CRevts
    customRequirementList.clear();
    customRequirementList.push_back("150evts_in_CR4_4jets_MTTail");
    customRequirementList.push_back("100evts_in_CR4_4jets_MTTail");
    customRequirementList.push_back("50evts_in_CR4_4jets_MTTail");
    customRequirementList.push_back("30evts_in_CR4_4jets_MTTail");

    vector<string> labels = { "150 evts", "100 evts","50 evts","30 evts"};

    CRColorList.clear();
    CRColorList = {2,42,43,44,45,46,4,36,38,40,9,20,22,24,26,27,28,8,29, 30,33,34};

    // Read tables for each CR
    SF_est.clear();
    for (unsigned int i = 0 ; i < customRequirementList.size() ; i++)
    {
        vector<Table> tables;
        for (unsigned int j = 0 ; j < signalRegionsTagList.size() ; j++)
        {
            tables.push_back(Table("scaleFactors/"+customRequirementList[i]+"/"+signalRegionsTagList[j]+".tab"));
        }
        SF_est.push_back(tables);
    }

    // Create a plot for each Signal Region
    vector<TH1F*> SRplots(signalRegionsTagList.size());
    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        string hname = "h_"+signalRegionsTagList[i];
        SRplots[i] = new TH1F(hname.c_str(),"",customRequirementList.size(),0,customRequirementList.size());
        for(unsigned int j=0;j<customRequirementList.size();j++)
        {
            SRplots[i]->GetXaxis()->SetBinLabel(j+1,labels[j].c_str());
        }
        SRplots[i]->SetLineColor(CRColorList[i]);
        SRplots[i]->SetLineWidth(2);
    }

    //Fill the histo

    for (unsigned int i = 0 ; i < customRequirementList.size() ; i++)
    {
        for (unsigned int j = 0 ; j < signalRegionsTagList.size() ; j++)
        {

            Figure SF_CR4      = SF_est[i][j].Get("value","SF_2l");
            Figure SF_CR4_tail = SF_est[i][j].Get("value","SF_2ltail");
            Figure SF_CR5_peak = SF_est[i][j].Get("value","SF_vetopeak");
            Figure SF_CR5_tail = SF_est[i][j].Get("value","SF_vetotail");

            Figure CR4_ref = SF_est[0][j].Get("value","SF_2ltail");
            Figure CR4 = SF_CR4 / CR4_ref;

            SRplots[j]->SetBinContent(i+1,CR4.value());
            SRplots[j]->SetBinError(i+1,CR4.error());
        }
    }

    TCanvas c2;
    TLegend legSR(0.73,0.1,0.97,0.9);
    c2.SetRightMargin(0.26);
    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        if (i==0)
        {
            SRplots[i]->SetMinimum(0.);
            SRplots[i]->SetMaximum(2.5);
            SRplots[i]->Draw();
        }
        else
            SRplots[i]->Draw("same");

        legSR.AddEntry(SRplots[i],signalRegionLabel(signalRegionsTagList[i], "root").c_str(),"lp");
    }
    legSR.Draw("same");
    c2.Write();


    fout.Close();




    return (0);
}


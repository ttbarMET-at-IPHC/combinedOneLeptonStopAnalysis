#include "common.h"
#include "cutAndCountDefinitions_cutsLoosenedForCR4CR5.h"

// FIXME these are not used, is that a bug or a feature ?
#define CR4MTtailUncert 0.25
#define ExtrapUncert 0.2

void GetEnveloppe(float * SFval,float * SFerr, float tabSize, float & min, float & max)
{

    min = 10.;
    max = 0.;
    float minError = 10;

    float value=0;
    float error=0;
    // First: find the value compatible with one with the lowest uncertainty
    for (int i=0; i<tabSize; i++) 
    {
        value = SFval[i];
        error = SFerr[i];
        if((value-error)<=1 && (value+error)>=1)
        {
            if(error<minError){

                minError = error;
                max = value+error;
                min = value-error;
            }
        }  
    }
    // Second : enlarge for value non-compatible with 1.
    for (int i=0; i<tabSize; i++) 
    {
        value = SFval[i];
        if(value<min) min = value;
        if(value>max) max = value;
    }
}


int main (int argc, char *argv[])
{

    //TStyle* gStyle;
    gStyle->SetOptStat(0);
    gStyle->SetLegendFillColor();//kRed);

    if (argc <= 1) { WARNING_MSG << "No signal region specified" << endl; return -1; }

    //
    // Read list of signal regions to consider
    // 
    vector<string> signalRegionsTagList;
    for (int i = 1 ; i < argc ; i++) 
    {
        signalRegionsTagList.push_back(argv[i]);
    }
    
    init_cutsLoosenedForCR4CR5_numbersOfRegions();
    int maxNumberOfAlternativeLoosenedRegions = 1;
    for (unsigned int r = 0 ; r < signalRegionsTagList.size() ; r++)
    {
        string regionTag = signalRegionsTagList[r];
        if (maxNumberOfAlternativeLoosenedRegions < cutAndCount_numbersOfAlternativeLoosenedRegions[regionTag])
            maxNumberOfAlternativeLoosenedRegions = cutAndCount_numbersOfAlternativeLoosenedRegions[regionTag];
    }

    //
    //  Initializing histograms
    //

    vector<TH1F*> SF_tt2l_CR4;
    vector<TH1F*> SF_tt2l_CR5;
    int markerStyles_CR4[5] = {21, 22, 23, 27, 28};
    int markerStyles_CR5[5] = {24, 25, 26, 29, 30};
    for (int i = 0 ; i < maxNumberOfAlternativeLoosenedRegions ; i++)
    {
        SF_tt2l_CR4.push_back(new TH1F(("SF_tt2l_CR4_"+intToString(i)).c_str(), "",  signalRegionsTagList.size(),  0,  signalRegionsTagList.size()));
        SF_tt2l_CR5.push_back(new TH1F(("SF_tt2l_CR5_"+intToString(i)).c_str(), "",  signalRegionsTagList.size(),  0,  signalRegionsTagList.size()));

        SF_tt2l_CR4[i]->SetLineColor(2);
        SF_tt2l_CR4[i]->SetMarkerColor(2);
        SF_tt2l_CR4[i]->SetMarkerStyle(markerStyles_CR4[i]);
        SF_tt2l_CR5[i]->SetLineColor(4);
        SF_tt2l_CR5[i]->SetMarkerColor(4);
        SF_tt2l_CR5[i]->SetMarkerStyle(markerStyles_CR5[i]);
    }

    TH1F* hMin          = new TH1F("hMin","hMin",signalRegionsTagList.size(),0,signalRegionsTagList.size());
    TH1F* hMax          = new TH1F("hMax","hMax",signalRegionsTagList.size(),0,signalRegionsTagList.size());

    hMin->SetLineColor(kBlack);
    hMax->SetLineColor(kBlack);
    hMin->SetLineWidth(3);
    hMax->SetLineWidth(3);

    // Set custom labels in histogram
    for(unsigned int j=0;j<signalRegionsTagList.size();j++)
    {
        string sr = signalRegionLabel(signalRegionsTagList[j],"root","long");
        if (signalRegionsTagList[j] == "preselection") 
            SF_tt2l_CR4[0]->GetXaxis()->SetBinLabel(j+1, sr.c_str());
        else 
            SF_tt2l_CR4[0]->GetXaxis()->SetBinLabel(j+1, sr.c_str());
    }


    vector<string> columns = {"value"};
    Table enveloppe(columns,signalRegionsTagList);

    for (unsigned int j = 0 ; j < signalRegionsTagList.size() ; j++)
    {
        string signalRegionTag = signalRegionsTagList[j];
        int numberOfAlternativeLoosenedRegions = cutAndCount_numbersOfAlternativeLoosenedRegions[signalRegionTag];

        float SFval[numberOfAlternativeLoosenedRegions*2]; 
        float SFerr[numberOfAlternativeLoosenedRegions*2];
        for (int i=0 ; i< numberOfAlternativeLoosenedRegions*2 ; i++) 
        {
            SFval[i] = 1.;
            SFerr[i] = -0.01; // negative to not be part of the enveloppe, if initial value
        }

        for (int altR = 0 ; altR < numberOfAlternativeLoosenedRegions ; altR++)
        {
            Table sigR;
            if (signalRegionTag == "preselection") 
                sigR  = Table("scaleFactors/"+signalRegionTag+".tab");
            else 
                sigR  = Table("scaleFactors/"+signalRegionTag+"_cutsLoosenedForCR4CR5_"+intToString(altR+1)+".tab");

            Figure SF_2ltail   = sigR.Get("value","SF_2ltail");
            Figure SF_vetotail = sigR.Get("value","SF_vetotail");

            SF_tt2l_CR4[altR]->SetBinContent(j+1,SF_2ltail.value());
            SF_tt2l_CR4[altR]->SetBinError  (j+1,SF_2ltail.error());
            SF_tt2l_CR5[altR]->SetBinContent(j+1,SF_vetotail.value());
            SF_tt2l_CR5[altR]->SetBinError  (j+1,SF_vetotail.error());

            SFval[2*altR  ] = SF_2ltail.value();
            SFerr[2*altR  ] = SF_2ltail.error();
            SFval[2*altR+1] = SF_vetotail.value();
            SFerr[2*altR+1] = SF_vetotail.error();
        }
        
        float min =0;
        float max =10;
        GetEnveloppe(SFval, SFerr, numberOfAlternativeLoosenedRegions*2, min, max);

        hMin->SetBinContent(j+1,min);
        hMin->SetBinError(j+1,0);
        hMax->SetBinContent(j+1,max);
        hMax->SetBinError(j+1,0);
        float uncert = 1-min;
        if ((max-1)>uncert) uncert = max-1;
        Figure funcert(1.0,uncert);
        enveloppe.Set("value",signalRegionsTagList[j],funcert);

    }

    enveloppe.Print("results/uncertainties_CnC.tab",4);

    TFile fout("results/plots_CC.root","RECREATE");
    TCanvas c1;
    c1.cd();
    float miny= SF_tt2l_CR4[0]->GetMinimum();
    if (SF_tt2l_CR5[0]->GetMinimum()<miny) miny = SF_tt2l_CR5[0]->GetMinimum();

    SF_tt2l_CR4[0]->Draw("E1");
    SF_tt2l_CR5[0]->Draw("same,E1");
    for (int i=1 ; i< maxNumberOfAlternativeLoosenedRegions ; i++)
    {
        SF_tt2l_CR4[i]->Draw("same,E1");
        SF_tt2l_CR5[i]->Draw("same,E1");
    }
    hMin->Draw("histsame");
    hMax->Draw("histsame");

    c1.Write();
    fout.Close();

    return (0);
}


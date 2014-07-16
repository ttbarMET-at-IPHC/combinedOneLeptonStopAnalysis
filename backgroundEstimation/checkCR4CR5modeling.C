//#include "../common.h"
#include <fstream>

// Sonic screwdriver headers

#include "interface/Table.h" 
#include "interface/SonicScrewdriver.h" 
using namespace theDoctor;

#include "common.h"

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

    //TStyle* gStyle;
    gStyle->SetOptStat(0);
    gStyle->SetLegendFillColor();//kRed);

    if (argc <= 1) { WARNING_MSG << "No signal region specified" << endl; return -1; }

    // Read list of signal regions to consider
    
    vector<string> signalRegionsTagList;
    for (int i = 1 ; i < argc ; i++) 
        signalRegionsTagList.push_back(argv[i]);

    // List of CR
    vector<string> CRTagList;
    //CRTagList.push_back("CR4_1j");
    CRTagList.push_back("CR4_2j");
    CRTagList.push_back("CR4_3j");
    CRTagList.push_back("CR4_4j");
    CRTagList.push_back("CR5_2j");
    CRTagList.push_back("CR5_3j");
    CRTagList.push_back("CR5_4j");

    vector<Color_t> CRColorList = {kRed,kOrange-1,kBlue-1,kGreen-1,kOrange+1,kBlue+1,kGreen+1};

    vector<int> CRMarkerStyle = {28,22,20,25,23,24,26}; 

    // Read tables for each CR
    vector<vector<Table> > SF_est;
    for (unsigned int i = 0 ; i < CRTagList.size() ; i++)
    {
        vector<Table> tables;
        for (unsigned int j = 0 ; j < signalRegionsTagList.size() ; j++)
        {
            tables.push_back(Table("scaleFactors/"+CRTagList[i]+"/"+signalRegionsTagList[j]+".tab"));
        }
        SF_est.push_back(tables);
    }
    vector<vector<Table> > SF_est_CR4;

    // Create a plot for each CR
    vector<TH1F*> CRplots(CRTagList.size());
    for (unsigned int i = 0 ; i < CRTagList.size() ; i++)
    {
        string hname = "h_"+CRTagList[i];
        CRplots[i] = new TH1F(hname.c_str(),"",signalRegionsTagList.size(),0,signalRegionsTagList.size());
        for(unsigned int j=0;j<signalRegionsTagList.size();j++){
            CRplots[i]->GetXaxis()->SetBinLabel(j+1,signalRegionsTagList[j].c_str());
        }
        CRplots[i]->SetLineColor(CRColorList[i]);
        CRplots[i]->SetLineWidth(2);
        CRplots[i]->SetMarkerStyle(CRMarkerStyle[i]);
        CRplots[i]->SetMarkerColor(CRColorList[i]);
        CRplots[i]->SetMarkerSize(1.5);
    }

    //Fill the histo
    for (unsigned int i = 0 ; i < CRTagList.size() ; i++)
    {
        for (unsigned int j = 0 ; j < signalRegionsTagList.size() ; j++)
        {
            Figure SF_CR4      = SF_est[i][j].Get("value","SF_2l");
            Figure SF_CR4_tail = SF_est[i][j].Get("value","SF_2ltail");
            Figure SF_CR5_peak = SF_est[i][j].Get("value","SF_vetopeak");
            Figure SF_CR5_tail = SF_est[i][j].Get("value","SF_vetotail");

            Figure CR4 = SF_CR4;//*SF_CR4_tail;
            Figure CR5 = SF_CR5_tail;
            if(CRTagList[i].find("CR4")!=std::string::npos){
                CRplots[i]->SetBinContent(j+1,CR4.value());
                CRplots[i]->SetBinError(j+1,sqrt(pow(CR4.error(),2.)+pow(CR4.value()*CR4MTtailUncert,2)));
            }
            else{
                CRplots[i]->SetBinContent(j+1,CR5.value());
                CRplots[i]->SetBinError(j+1,CR5.error());
            }
        }
    }

    TCanvas c1;
    TLegend legCR(0.65,0.65,0.88,0.88);
    for (unsigned int i = 0 ; i < CRTagList.size() ; i++)
    {
        if(i==0) CRplots[i]->Draw();
        else     CRplots[i]->Draw("same");
        legCR.AddEntry(CRplots[i],CRTagList[i].c_str(),"lp");
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
        for (unsigned int i = 0 ; i < CRTagList.size() ; i++)
        {
            float value = CRplots[i]->GetBinContent(j+1);
            float error = CRplots[i]->GetBinError(j+1);
            if((value-error)<=1 && (value+error)>=1){ 
                if(error<minError){
                    minError = error;
                    max = value+error;
                    min = value-error;
                }
            }
        }
        for (unsigned int i = 0 ; i < CRTagList.size() ; i++)
        {
            float value = CRplots[i]->GetBinContent(j+1);
            //float error = CRplots[i]->GetBinError(j);
            //Use central value
            if(value<min) min = value;
            if(value>max) max = value;
        }
        hMin->SetBinContent(j+1,min);
        hMin->SetBinError(j+1,0);
        hMax->SetBinContent(j+1,max);
        hMax->SetBinError(j+1,0);
        //cout<<"min: "<<min<<" max: "<<max<<endl;
    }
    hMin->Draw("histsame");
    hMax->Draw("histsame");
    legCR.Draw("same");


    TFile fout("SF.root","RECREATE");
    c1.Write();
    //fout.Close();


    //Plots CR4
    TCanvas cR4;
    TLegend legCR4(0.65,0.65,0.88,0.88);
    legCR4.Draw("same");
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
        float uncert = min;
        if(max>min) uncert = max;
        uncert*=(1+ExtrapUncert);
        //Figure funcert(uncert,0);
        Figure funcert(1.0,uncert-1);
        SF_tt2l.Set("value",signalRegionsTagList[j],funcert);
    }

    SF_tt2l.Print("scaleFactors/checkCR4CR5modeling.tab",4);


    //-------------------------------------------------------------------------------
    // Perform plots as function of the stat (30-50-100 evts in the tail of the BDTs)
    //-------------------------------------------------------------------------------

    // List of CRevts
    CRTagList.clear();
    CRTagList.push_back("CR4_4j_150evts");
    CRTagList.push_back("CR4_4j_100evts");
    CRTagList.push_back("CR4_4j_50evts");
    CRTagList.push_back("CR4_4j");

    vector<string> labels = {"100 evts","50 evts","30 evts"};

    CRColorList.clear();
    CRColorList = {kRed-1,kRed,kRed+1,kBlue-2,kBlue-1,kBlue,kBlue+1,kPink-2,kPink-1,kPink,kPink+1,kPink+2,kBlack-3,kBlack-2,kBlack-1,kBlack};
    //vector<int> CRMarkerStyle = {28,22,20,23,24}; 

    // Read tables for each CR
    SF_est.clear();
    for (unsigned int i = 0 ; i < CRTagList.size() ; i++)
    {
        vector<Table> tables;
        for (unsigned int j = 0 ; j < signalRegionsTagList.size() ; j++)
        {
            tables.push_back(Table("scaleFactors/"+CRTagList[i]+"/"+signalRegionsTagList[j]+".tab"));
        }
        SF_est.push_back(tables);
    }

    // Create a plot for each Signal Region
    vector<TH1F*> SRplots(signalRegionsTagList.size());
    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        string hname = "h_"+signalRegionsTagList[i];
        SRplots[i] = new TH1F(hname.c_str(),"",CRTagList.size()-1,0,CRTagList.size()-1);
        for(unsigned int j=0;j<CRTagList.size()-1;j++){
            SRplots[i]->GetXaxis()->SetBinLabel(j+1,labels[j].c_str());
        }
        //cout<<i<<endl;
        SRplots[i]->SetLineColor(CRColorList[i]);
        SRplots[i]->SetLineWidth(2);
        /*
           CRplots[i]->SetMarkerStyle(CRMarkerStyle[i]);
           CRplots[i]->SetMarkerSize(1.5);
           */
    }

    //Fill the histo

    for (unsigned int i = 1 ; i < CRTagList.size() ; i++)
    {
        for (unsigned int j = 0 ; j < signalRegionsTagList.size() ; j++)
        {

            Figure SF_CR4_peak = SF_est[i][j].Get("value","SF_2l");
            Figure SF_CR4_tail = SF_est[i][j].Get("value","SF_2ltail");
            Figure SF_CR5_peak = SF_est[i][j].Get("value","SF_vetopeak");
            Figure SF_CR5_tail = SF_est[i][j].Get("value","SF_vetotail");

            Figure CR4_ref = SF_est[0][j].Get("value","SF_2l");
            Figure CR4 = SF_CR4_peak/CR4_ref;
            Figure CR5_ref = SF_est[0][j].Get("value","SF_vetotail");
            Figure CR5 = SF_CR5_tail/CR5_ref;
            if(CRTagList[i].find("CR4")!=std::string::npos){
                SRplots[j]->SetBinContent(i,CR4.value());
                SRplots[j]->SetBinError(i,CR4.error());
            }
            else{
                SRplots[j]->SetBinContent(i,CR5.value());
                SRplots[j]->SetBinError(i,CR5.error());
            }
        }
    }

    TCanvas c2;
    TLegend legSR(0.65,0.65,0.88,0.88);
    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        if(i==0) SRplots[i]->Draw();
        else     SRplots[i]->Draw("same");
        legSR.AddEntry(SRplots[i],signalRegionsTagList[i].c_str(),"lp");
    }
    legSR.Draw("same");
    c2.Write();


    fout.Close();



    return (0);
}


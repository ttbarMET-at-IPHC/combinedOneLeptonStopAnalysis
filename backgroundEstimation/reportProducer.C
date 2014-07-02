#include "../common.h"

// Sonic screwdriver headers

#include "interface/Table.h" 
#include "interface/SonicScrewdriver.h" 
using namespace theDoctor;

#include "common.h"

#include <string>

//-- Root dependancies
#include <TCanvas.h>
#include <TH1F.h>

int main (int argc, char *argv[])
{

    if (argc <= 1) { WARNING_MSG << "No signal region specified" << endl; return -1; }

    // Read list of signal regions to consider
    
    vector<string> signalRegionsTagList;
    for (int i = 1 ; i < argc ; i++) 
        signalRegionsTagList.push_back(argv[i]);

    // Create list of label for each region

    vector<string> signalRegionsLabelList;
    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        signalRegionsLabelList.push_back(signalRegionLabel(signalRegionsTagList[i],"latex"));
    }

    // Read tables for each signal regions
    
    vector<Table> predictions;
    vector<Table> scaleFactors;
    vector<Table> systematics;

    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        predictions .push_back(Table("prediction/" +signalRegionsTagList[i]+".tab"));
        scaleFactors.push_back(Table("scaleFactors/"+signalRegionsTagList[i]+".tab"));
        systematics .push_back(Table("systematics/" +signalRegionsTagList[i]+".tab"));
    }



    // Read tables for ttbar dilepton SF
    Table SF_tt2l("/opt/sbg/data/safe1/cms/echabert/StopCaro/combinedOneLeptonStopAnalysis/backgroundEstimationBDT/SF_tt2l.tab");
    

    // Fill summary tables

    Table rawYieldSummary(signalRegionsTagList,processesTagList,signalRegionsLabelList,processesLabelList);
    Table predictionSummary(signalRegionsTagList,processesTagList,signalRegionsLabelList,processesLabelList);
    Table scaleFactorsSummary(signalRegionsTagList,scaleFactorsTagList,signalRegionsLabelList,scaleFactorsLabelList);
    Table absoluteSystematicsSummary(signalRegionsTagList,systematicsTagList,signalRegionsLabelList,systematicsLabelList);
    Table relativeSystematicsSummary(signalRegionsTagList,systematicsTagList,signalRegionsLabelList,systematicsLabelList);

    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        for (unsigned int j = 0 ; j < processesTagList.size() ; j++)
        {
            rawYieldSummary  .Set(signalRegionsTagList[i],processesTagList[j],
                                                                              predictions[i].Get("raw_mc",processesTagList[j])); 
            
	    //extract additionnal info for ttbar dilepton
	    if(processesTagList[j]=="ttbar_2l"){
	    	string SR = signalRegionsTagList[i];
		if(SR.find("tight")!=std::string::npos) SR=SR.substr(0,SR.find("tight"))+"loose";
	    	float SF = SF_tt2l.Get(SR,"value").value();
		Figure pred (predictions[i].Get("prediction",processesTagList[j]).value(),sqrt(pow(predictions[i].Get("prediction",processesTagList[j]).error(),2)+pow(predictions[i].Get("prediction",processesTagList[j]).value()*SF,2)));

	    	predictionSummary.Set(signalRegionsTagList[i],processesTagList[j],pred);
	    }
	    else predictionSummary.Set(signalRegionsTagList[i],processesTagList[j],
                                                                              predictions[i].Get("prediction",processesTagList[j])); 
        }
        for (unsigned int j = 0 ; j < scaleFactorsTagList.size() ; j++)
        {
            scaleFactorsSummary.Set(signalRegionsTagList[i],scaleFactorsTagList[j],
                                                                                   scaleFactors[i].Get("value",scaleFactorsTagList[j])); 
        }
        for (unsigned int j = 0 ; j < systematicsTagList.size() ; j++)
        {
            absoluteSystematicsSummary.Set(signalRegionsTagList[i],systematicsTagList[j],
                                                                                         systematics[i].Get("absolute",systematicsTagList[j])); 
            relativeSystematicsSummary.Set(signalRegionsTagList[i],systematicsTagList[j],
                                                                                         systematics[i].Get("relative",systematicsTagList[j]) * 100); 
        }
    }
    
    string dir="reports/";
    
    //-------------------------------------------//
    // Saving plots in ROOT
    //-------------------------------------------//

     //establish the vector of label
     vector<string> labels(signalRegionsTagList.size());
     for(unsigned int i=0;i<labels.size();i++){
     	string label = signalRegionsTagList[i];
	size_t pos = label.find("cutAndCount_");
	if(pos!=std::string::npos) label = label.substr(pos+12);
	pos = label.find("BDT_");
	if(pos!=std::string::npos) label = label.substr(pos+4);
	pos = label.find("T2tt_");
	if(pos!=std::string::npos) label = label.substr(pos+5);
	pos = label.find("T2bw025_");
	if(pos!=std::string::npos) label = label.substr(pos+8);
	pos = label.find("T2bw050_");
	if(pos!=std::string::npos) label = label.substr(pos+8);
	pos = label.find("T2bw075_");
	if(pos!=std::string::npos) label = label.substr(pos+8);
     	labels[i] = label;
     }

     TFile* froot = new TFile("reports/report.root","RECREATE");
     froot->cd();

    //-------------------------------------------//
    //-------------------------------------------//
    // Plot SFs in graph
    //-------------------------------------------//
    TCanvas c;
    for (unsigned int j = 0 ; j < scaleFactorsTagList.size() ; j++)
    {
    	string hname = "h_"+scaleFactorsTagList[j];
    	TH1F hSF(hname.c_str(),scaleFactorsTagList[j].c_str(),signalRegionsTagList.size(),0,signalRegionsTagList.size());
    	for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
        {
            hSF.SetBinContent(i+1,scaleFactors[i].Get("value",scaleFactorsTagList[j]).value());
            hSF.SetBinError(i+1,scaleFactors[i].Get("value",scaleFactorsTagList[j]).error());
	    //hSF.GetXaxis()->SetBinLabel(i+1,signalRegionsTagList[j].c_str());
        }
	string fname = dir+scaleFactorsTagList[j]+".png";
	hSF.Draw();
	c.Print(fname.c_str());
    
    }
    //-------------------------------------------//
     //compute (Rw-R1l)/(average)
    //-------------------------------------------//
     TH1F hR_RelDiff("hR_relDiff","relative difference btw R_W+jets and R_1ltop",signalRegionsTagList.size(),0,signalRegionsTagList.size());
     for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
     {
 	    float R_1ltop = scaleFactors[i].Get("value","R_1ltop").value();
	    float R_Wjets = scaleFactors[i].Get("value","R_W+jets").value();
    	    float relDiff = (R_Wjets-R_1ltop)/(R_Wjets+R_1ltop)*2;
            hR_RelDiff.SetBinContent(i+1,relDiff);
	    //hR_RelDiff.GetXaxis()->SetBinLabel(i+1,signalRegionsTagList[i].c_str());
	    hR_RelDiff.GetXaxis()->SetBinLabel(i+1,labels[i].c_str());
    }
    hR_RelDiff.Draw();
    c.Print("reports/R_RelDiff.png");
    //-------------------------------------------//
    
    //-------------------------------------------//
    // Compute SF for the bkg
    //-------------------------------------------//
    for (unsigned int j = 0 ; j < processesTagList.size() ; j++)
    {
    	string hname = "h_SF_"+processesTagList[j];
    	TH1F hSF(hname.c_str(),hname.c_str(),signalRegionsTagList.size(),0,signalRegionsTagList.size());
    	for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    	{
    		float pred = predictions[i].Get("prediction",processesTagList[j]).value();
		float raw = predictions[i].Get("raw_mc",processesTagList[j]).value();
    		float SF = 0;
		if(raw!=0) SF = pred/raw;
		float SFerror = sqrt(pow(SF/pred*predictions[i].Get("prediction",processesTagList[j]).error(),2.)+pow(SF/raw*predictions[i].Get("raw_mc",processesTagList[j]).error(),2.));
		hSF.SetBinContent(i+1,SF);
		hSF.SetBinError(i+1,SFerror);
	        //hSF.GetXaxis()->SetBinLabel(i+1,signalRegionsTagList[j].c_str());
	        hSF.GetXaxis()->SetBinLabel(i+1,labels[j].c_str());
        	cout<<signalRegionsTagList[i]<<" "<<processesTagList[j]<<" "<<SF<<endl;
	}
	string fname = dir+string("SF_")+processesTagList[j]+".png";
	hSF.Draw();
	c.Print(fname.c_str());
    }
    //-------------------------------------------//

    //-------------------------------------------//
    // Compute the fraction of each bkg 
    //-------------------------------------------//
    vector<TH1F*> BkgFraction(processesTagList.size()-1);
    int indice = 0;
    for (unsigned int j = 0 ; j < processesTagList.size() ; j++)
    {
    	 	if(processesTagList[j]=="totalSM") continue;
		string hname = "h_Fraction_"+processesTagList[j];
    		BkgFraction[indice] = new TH1F(hname.c_str(),scaleFactorsTagList[j].c_str(),signalRegionsTagList.size(),0,signalRegionsTagList.size());
		indice++;
    }
    TLegend legBkg(0.9,0.9,0.99,0.99);
    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
    	 float totalSM = predictions[i].Get("prediction","totalSM").value();
	 indice = 0;
   	 for (unsigned int j = 0 ; j < processesTagList.size() ; j++)
   	 {
    	 	if(processesTagList[j]=="totalSM") continue;
    		float pred = predictions[i].Get("prediction",processesTagList[j]).value();
    		float fraction = 0;
		if(pred!=0) fraction = pred/totalSM;
		BkgFraction[indice]->SetBinContent(i+1,fraction);
		if(processesTagList[j]=="1ltop"){
			BkgFraction[indice]->SetFillColor(kRed-7);
			legBkg.AddEntry(BkgFraction[indice],"1ltop","F");
		}
		if(processesTagList[j]=="ttbar_2l"){
			BkgFraction[indice]->SetFillColor(kCyan-3);
			legBkg.AddEntry(BkgFraction[indice],"ttbar_2l","F");
		}
		if(processesTagList[j]=="W+jets"){
			BkgFraction[indice]->SetFillColor(kOrange-2);
			legBkg.AddEntry(BkgFraction[indice],"W+jets","F");
		}
		if(processesTagList[j]=="rare"){
			BkgFraction[indice]->SetFillColor(kMagenta-5);
			legBkg.AddEntry(BkgFraction[indice],"rare","F");
		}
		indice++;
	 }
    }
    //create THStack 
    THStack* hStack = 0;
    if(BkgFraction.size()>0) hStack = new THStack(BkgFraction[0]);
    for(unsigned int i = 0; i< BkgFraction.size();i++)
    {
    	hStack->Add(BkgFraction[i]);
    }
    hStack->SetName("stack_FractionBkg");
    string fname("stackFractionBkg.png");
    c.cd();
    hStack->Draw();
    legBkg.Draw("same");
    //hSF.Draw();
    c.Print(fname.c_str());
    //-------------------------------------------//
    
    
    vector<TH1F*> SystFraction(systematicsTagList.size()-1);
    indice = 0;
    for (unsigned int j = 0 ; j < systematicsTagList.size() ; j++)
    {
    	 	cout<<systematicsTagList[j]<<endl;
		if(systematicsTagList[j]=="total") continue;
		string hname = "h_Fraction_"+systematicsTagList[j];
    		cout<<systematicsTagList.size()<<" "<<SystFraction.size()<<" "<<indice<<" "<<j<<" "<<systematicsTagList[j]<<endl;
		SystFraction[indice] = new TH1F(hname.c_str(),hname.c_str(),signalRegionsTagList.size(),0,signalRegionsTagList.size());
		indice++;
		cout<<"here"<<endl;
    }
    cout<<"THE END"<<endl;
    TLegend legSyst(0.9,0.9,0.99,0.99);
    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
    	 float total = pow(systematics[i].Get("absolute","total").value(),2);
	 indice = 0;
   	 for (unsigned int j = 0 ; j < systematicsTagList.size() ; j++)
   	 {
    	 	if(systematicsTagList[j]=="total"){
			cout<<"total: "<<systematics[i].Get("absolute",systematicsTagList[j]).value()<<endl;
			continue;
    			
		}
		float uncert = pow(systematics[i].Get("absolute",systematicsTagList[j]).value(),2);
    		float fraction = 0;
		if(total!=0) fraction = uncert/total;
		cout<<"total: "<<total<<"uncert: "<<systematics[i].Get("absolute",systematicsTagList[j]).value()<<" "<<uncert<<" fraction:"<<fraction<<endl;
		SystFraction[indice]->SetBinContent(i+1,fraction);
		if(systematicsTagList[j]=="1ltop_tailToPeak"){
			SystFraction[indice]->SetFillColor(kRed-7);
			legSyst.AddEntry(SystFraction[indice],"1ltop_tailToPeak","F");
		}
		if(systematicsTagList[j]=="1l_top_(MCstat)"){
			SystFraction[indice]->SetFillColor(kRed-7);
			legSyst.AddEntry(SystFraction[indice],"1l_top_(MCstat)","F");
		}
		if(systematicsTagList[j]=="MTpeak"){
			SystFraction[indice]->SetFillColor(kRed-7);
			legSyst.AddEntry(SystFraction[indice],"MTpeak","F");
		}
		if(systematicsTagList[j]=="tt->ll_(CR4,CR5)"){
			SystFraction[indice]->SetFillColor(kCyan-3);
			legSyst.AddEntry(SystFraction[indice],"tt->ll_(CR4,CR5)","F");
		}
		if(systematicsTagList[j]=="tt->ll_(nJets)"){
			SystFraction[indice]->SetFillColor(kCyan-3);
			legSyst.AddEntry(SystFraction[indice],"tt->ll_(nJets)","F");
		}
		if(systematicsTagList[j]=="tt->ll_(veto)"){
			SystFraction[indice]->SetFillColor(kCyan-3);
			legSyst.AddEntry(SystFraction[indice],"tt->ll_(veto)","F");
		}
		if(systematicsTagList[j]=="tt->ll_(MCstat)"){
			SystFraction[indice]->SetFillColor(kCyan-3);
			legSyst.AddEntry(SystFraction[indice],"tt->ll_(MCstat)","F");
		}
		if(systematicsTagList[j]=="SFR_Wjets"){
			SystFraction[indice]->SetFillColor(kOrange-2);
			legSyst.AddEntry(SystFraction[indice],"SFR_Wjets","F");
		}
		if(systematicsTagList[j]=="W+jets_(cross_section)"){
			SystFraction[indice]->SetFillColor(kOrange-2);
			legSyst.AddEntry(SystFraction[indice],"W+jets_(cross_section)","F");
		}
		if(systematicsTagList[j]=="W+jets_(MCstat)"){
			SystFraction[indice]->SetFillColor(kOrange-2);
			legSyst.AddEntry(SystFraction[indice],"W+jets_(MCstat)","F");
		}
		if(systematicsTagList[j]=="Rare_(cross_section)"){
			SystFraction[indice]->SetFillColor(kMagenta-5);
			legSyst.AddEntry(SystFraction[indice],"Rare_(cross_section)","F");
		}
		if(systematicsTagList[j]=="rare_(MCstat)"){
			SystFraction[indice]->SetFillColor(kMagenta-5);
			legSyst.AddEntry(SystFraction[indice],"rare_(MCstat)","F");
		}
	 }
    }
    //create THStack 
    THStack* hStackSyst = 0;
    if(SystFraction.size()>0){
    	//SystFraction[0]->GetYaxis()->SetRangeUser(0.,1.0);
	hStackSyst = new THStack(SystFraction[0]);
    }
    for(unsigned int i = 0; i< SystFraction.size();i++)
    {
    	hStackSyst->Add(SystFraction[i]);
    }
    hStackSyst->SetName("stack_FractionSyst");
    fname = string("stackFractionSyst.png");
    c.cd();
    //hStackSyst->GetYaxis()->SetRangeUser(0.,1.0);
    hStackSyst->Draw();
    legSyst.Draw("same");
    //hSF.Draw();
    c.Print(fname.c_str());
    //-------------------------------------------//

    froot->Write();
    froot->Close();

   
    cout << "\\documentclass[a4paper, 12pt]{article}" << endl;
    cout << "\\usepackage{amsmath}" << endl;
    cout << "\\usepackage[landscape]{geometry}" << endl;
    //cout << "\\usepackage{geometry}" << endl;
    cout << "\\geometry{a4paper, top=0.5cm, bottom=0.5cm, left=0.3cm, right=0.3cm}" << endl;

    cout << "\\begin{document}" << endl;
    cout << "\\begin{center}" << endl;

    cout << "{\\Large Raw yields}\\\\" << endl;
    rawYieldSummary.PrintLatex(2);
    cout << "\\\\ \\bigskip" << endl;

    cout << "{\\Large Scale factors}\\\\" << endl;
    scaleFactorsSummary.PrintLatex(2);
    cout << "\\\\ \\bigskip" << endl;

    cout << "{\\Large Absolute uncertainties}\\\\" << endl;
    absoluteSystematicsSummary.PrintLatex(1,"noError");
    cout << "\\\\ \\bigskip" << endl;

    cout << "{\\Large Relative uncertainties (in percents)}\\\\" << endl;
    relativeSystematicsSummary.PrintLatex(1,"noError");
    cout << "\\\\ \\bigskip" << endl;
    
    cout << "{\\Large Prediction}\\\\" << endl;
    predictionSummary.PrintLatex(2);
    cout << "\\\\ \\bigskip" << endl;
    
    cout << "\\end{center}" << endl;
    cout << "\\end{document}" << endl;

    return (0);
}


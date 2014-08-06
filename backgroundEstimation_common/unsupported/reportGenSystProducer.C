#include "../common.h"
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

int main (int argc, char *argv[])
{

    if (argc <= 1) { WARNING_MSG << "No signal region specified" << endl; return -1; }

    // Read list of signal regions to consider
    
    vector<string> signalRegionsTagList;
    for (int i = 1 ; i < argc ; i++) 
        signalRegionsTagList.push_back(argv[i]);

    // List of systematics
    vector<string> systematicsTagList;
    systematicsTagList.push_back("madgraph_matchingdown");
    systematicsTagList.push_back("madgraph_matchingup");
    systematicsTagList.push_back("madgraph_scaledown");
    systematicsTagList.push_back("madgraph_scaleup");
    systematicsTagList.push_back("madgraph_mass166-5");
    systematicsTagList.push_back("madgraph_mass178-5");


    // Read tables for each signal regions
    
    vector<Table> predictions;

    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        predictions .push_back(Table("prediction/" +signalRegionsTagList[i]+".tab"));
    }
   

    // Read tables for systematics
    vector<vector<Table> > sys_pred;
    for (unsigned int i = 0 ; i < systematicsTagList.size() ; i++)
    {
    	vector<Table> tables;
    	for (unsigned int j = 0 ; j < signalRegionsTagList.size() ; j++)
    	{
     	   tables.push_back(Table("results/"+systematicsTagList[i]+"/prediction/" +signalRegionsTagList[j]+".tab"));
	}
	sys_pred.push_back(tables);
    }

    // Create list of label for each region

    vector<string> signalRegionsLabelList;
    for (unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
        signalRegionsLabelList.push_back(signalRegionLabel(signalRegionsTagList[i],"latex"));
    }

    vector<string> columnsTagList;
    columnsTagList.push_back("nominal");
    columnsTagList.push_back("syst");
    vector<string> columnsLabelList;
    columnsLabelList.push_back("total SM");
    columnsLabelList.push_back("total SM (with gen syst.");

    Table predictionSummary(columnsTagList,signalRegionsTagList,columnsLabelList,signalRegionsLabelList);
   
    for (unsigned int i = 0 ; i < processesTagList.size() ; i++)
    {
        //  Create the histos
    	vector<TH1F*> hPred(systematicsTagList.size());
   	for (unsigned int j = 0 ; j < systematicsTagList.size() ; j++)
    	{
    		string hname = "h_"+processesTagList[i]+"_"+systematicsTagList[j];
    		hPred[j] = new TH1F(hname.c_str(),hname.c_str(),signalRegionsTagList.size(),0,signalRegionsTagList.size());
    	}

	//  Compute the generator systematics
	vector<float> systematics(signalRegionsTagList.size());

    	for (unsigned int j = 0 ; j < signalRegionsTagList.size() ; j++)
    	{
    		//retrieve nominal value
		Figure nominal_we = predictions[j].Get("prediction",processesTagList[i]);
		Figure nominal(predictions[j].Get("prediction",processesTagList[i]).value(),0); //set error == 0 to no double count syst. in the ratio (approx. )
               
	        //Fill Table
		if(processesTagList[i]=="totalSM") predictionSummary.Set("nominal",signalRegionsTagList[j],nominal_we);
		
		for (unsigned int k = 0 ; k < systematicsTagList.size() ; k++)
		{
			//retrieve value prediction for systematic sample
			Figure sysPred = sys_pred[k][j].Get("prediction",processesTagList[i]);
			Figure ratio = sysPred/nominal;
			hPred[k]->SetBinContent(j+1,ratio.value());
			hPred[k]->SetBinError(j+1,ratio.error());
    			
			//Add the systematics
			// add the difference only if the mean is not compatible @ 1 sigma
			//if(fabs(ratio.value()-1)>ratio.error()) systematics[j]+=pow(ratio.value()-1,2);
			if(fabs(ratio.value()-1)>ratio.error()) systematics[j]+=pow(fabs(ratio.value()-1)-ratio.error(),2);
		}
		// Finish computation of the systematics
		systematics[j] = sqrt(systematics[j]);
		
		//Fill table with systematics
		Figure predWithSys(nominal.value(),sqrt(pow(nominal_we.error(),2)+pow(systematics[j]*nominal.value(),2)));
		if(processesTagList[i]=="totalSM") predictionSummary.Set("syst",signalRegionsTagList[j],predWithSys);
    	}
	// Create canvas - draw plots and them 
    	TCanvas c;
   	TLegend leg(0.6,0.7,0.8,0.9);
	for (unsigned int j = 0 ; j < systematicsTagList.size() ; j++)
    	{
		//change color
    		if(j==0) hPred[j]->Draw(""); 
    		else	 hPred[j]->Draw("esame"); 
		//Add them to the legend
		leg.AddEntry(hPred[j],systematicsTagList[j].c_str(),"l");
	}
	leg.Draw("same");
	string cname = "c_"+processesTagList[i]+".png";
	c.Print(cname.c_str());
    }
    
    
    cout << "\\documentclass[a4paper, 12pt]{article}" << endl;
    cout << "\\usepackage{amsmath}" << endl;
    cout << "\\usepackage[landscape]{geometry}" << endl;
    //cout << "\\usepackage{geometry}" << endl;
    cout << "\\geometry{a4paper, top=0.5cm, bottom=0.5cm, left=0.3cm, right=0.3cm}" << endl;

    cout << "\\begin{document}" << endl;
    cout << "\\begin{center}" << endl;

    cout << "{\\Large Prediction}\\\\" << endl;
    predictionSummary.PrintLatex(2);
    cout << "\\\\ \\bigskip" << endl;
    
    cout << "\\end{center}" << endl;
    cout << "\\end{document}" << endl;

    // Create functions for Michael
    
    ofstream ofile("prediction.C");
    ofile<<"//prediction total yield for the different SR"<<endl;
    ofile<<"//first value = yield"<<endl;
    ofile<<"//second vlaue = uncertainty"<<endl;
    for(unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
    	 Figure res = predictionSummary.Get("nominal",signalRegionsTagList[i]);
    	 ofile<<"std::pair<double,double> Pred_"<<signalRegionsTagList[i]<<"(){"<<"return std::pair<double,double>("<<res.value()<<","<<res.error()<<");}"<<endl;
    }
    
    // Idem with gen systematic added
    
    ofstream ofile2("prediction_with_genSyst.C");
    ofile2<<"//prediction total yield for the different SR"<<endl;
    ofile2<<"//first value = yield"<<endl;
    ofile2<<"//second vlaue = uncertainty"<<endl;
    for(unsigned int i = 0 ; i < signalRegionsTagList.size() ; i++)
    {
    	 Figure res = predictionSummary.Get("syst",signalRegionsTagList[i]);
    	 ofile2<<"std::pair<double,double> Pred_"<<signalRegionsTagList[i]<<"(){"<<"return std::pair<double,double>("<<res.value()<<","<<res.error()<<");}"<<endl;
    }
    
    ofile.close();
    return (0);
}


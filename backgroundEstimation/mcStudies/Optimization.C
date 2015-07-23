#include <vector>
#include <algorithm>
#include "TGraph.h"


//----------------------------------------------------------
// Macro Used to optimize cut on variables
// Should be launch in ROOT interative
// It is based on plots produced by topnessProducerX
// It will produce eff_s, eff_b, ROC curve
// It will determine the optimial cut wrt to the Zbi metric
//-----------------------------------------------------------


//---------------
// Comments
// Code could be generalize
// Could be apply to any selection/variaable

gStyle->SetOptStat(0);

struct OptRes{
	TH1F* h_eff_sig;
	TH1F* h_eff_bkg;
	TH1F* h_Zbi;
	TGraph* gROC;
	TCanvas* cplots;
};

double Zbi(double n_sig, double n_b, double rel_uncert_b = 0.3 ){
	double n_on = n_sig+n_b;
	double mu_b_hat = n_b;
	double sigma_b = rel_uncert_b*n_b;
	double tau = mu_b_hat/(sigma_b*sigma_b);
	double n_off = tau*mu_b_hat;
	double P_Bi = TMath::BetaIncomplete(1./(1.+tau),n_on,n_off+1);
	double Z_Bi = sqrt(2)*TMath::ErfInverse(1 - 2*P_Bi);
	return Z_Bi;
}


void Shape(string var = "topness",  string region = "baseline", string channel = "allChannels", string filename = "plots/plotsProducer/1DStack.root"){
	TFile* fplots = new TFile(filename.c_str(),"READ");
	//retrieve canvas
	string cname = channel+"/"+region+"/"+var;
	TCanvas* c = fplots->Get(cname.c_str());
	//c->Draw();
	
	//signal
	string splot_name = "v:"+var+"|p:ttbar_2l|r:"+region+"|c:"+channel+"|t:1DEntries";
	TH1F* histo= (TH1F*) c->GetPrimitive(splot_name.c_str());
	cout<<"mean = "<<histo->GetMean()<<" rms = "<<histo->GetRMS()<<" skewness = "<<histo->GetSkewness()<<" kurtosis = "<<histo->GetKurtosis()<<endl;
	
}


OptRes Optimization(string var = "topness", string signal = "T2tt_850_100", string region = "baseline", string channel = "allChannels", string filename = "plots/plotsProducer/1DStack.root"){

	OptRes res;
	
	//TFile* fplots = new TFile("plots/plotsProducer/1DStack.root","READ");
	TFile* fplots = new TFile(filename.c_str(),"READ");
	//string channel = "allChannels";
	//string region = "baseline";
	//string var = "topness";
	//string var = "topness_m5";
	//string signal = "T2tt_850_100";
	
	
	//retrieve canvas
	string cname = channel+"/"+region+"/"+var;
	TCanvas* c = fplots->Get(cname.c_str());
	//c->Draw();
	
	//signal
	string splot_name = "v:"+var+"|p:"+signal+"|r:"+region+"|c:"+channel+"|t:1DEntries";
	TH1F* h_sig = (TH1F*) c->GetPrimitive(splot_name.c_str());
	//sig->Draw();
	
	//////////////////////////////////////////////
	// Search the Stack that contains all bkg
	//////////////////////////////////////////////
	vector<TH1F*> h_bkgs;
	//c->GetListOfPrimitives()->Print();
	TIter next(c->GetListOfPrimitives());
	TObject* h;
	while ((h = (TH1F*) next())){
	//obj->Draw(next.GetOption());
		if(h->InheritsFrom("THStack")){
			THStack* stack = (THStack*) h;
			for(int i=0;i<stack->GetStack()->GetEntries();i++){
				TH1F* hist = (TH1F*) stack->GetStack()->At(i);
				//cout<<hist->IsA()->GetName()<<endl;
				//cout<<hist->GetName()<<" "<<hist->Integral()<<endl;
				//search tt_2l
				//if(string(hist->GetName()).find("W+jets")!=string::npos) continue;
				if(string(hist->GetName()).find("ttbar_2l")!=string::npos){
					//cout<<"mean = "<<hist->GetMean()<<" rms = "<<hist->GetRMS()<<" kurtosis = "<<hist->GetKurtosis()<<" skewness = "<<hist->GetSkewness()<<endl;
				}
				//else continue;
				//conly save the last one because plots are cumulative !!!
				// that's cheat ....
				if(i==stack->GetStack()->GetEntries()-1) h_bkgs.push_back(hist);
			}
			//cout<<((THStack*)h)->GetStack()->GetEntries()<<endl;
			//TH1* h_bkg = ((TH1*)(h->GetStack()->Last()));
		}
	}
	
	
	////////////////////////////////////////////////
	//     Loop over all bins and compute eff/sig
	////////////////////////////////////////////////
	
	res.h_eff_sig = (TH1F*) h_sig->Clone();	
	res.h_eff_bkg = (TH1F*) h_sig->Clone();	
	res.h_Zbi = (TH1F*) h_sig->Clone();	
	TH1F* h_ROC = new TH1F("h_ROC","ROC curve",10,0,1);

	//Compute the integral
	float integ_sig = res.h_eff_sig->Integral(0,res.h_eff_sig->GetNbinsX()+1);
	float integ_bkg = 0;
	for(unsigned ih=0;ih<h_bkgs.size();ih++){
		integ_bkg+=h_bkgs[ih]->Integral(0,h_bkgs[ih]->GetNbinsX()+1);
	}
	///////////////////////

	//-- efficiencoy for signal
	for(int i=1;i<res.h_eff_sig->GetNbinsX()+1;i++){
		float eff = 0;
		if(integ_sig!=0)  eff = res.h_eff_sig->Integral(i,res.h_eff_sig->GetNbinsX()+1)/integ_sig;
		res.h_eff_sig->SetBinContent(i,eff);
		res.h_eff_sig->SetBinError(i,0);
	}
	//-- efficiency for bkg
	for(int i=1;i<res.h_eff_sig->GetNbinsX()+1;i++){
		float eff = 0;
		for(unsigned ih=0;ih<h_bkgs.size();ih++){
			eff += h_bkgs[ih]->Integral(i,h_bkgs[ih]->GetNbinsX()+1);
		}
		if(integ_bkg!=0)  eff /= integ_bkg;
		res.h_eff_bkg->SetBinContent(i,eff);
		res.h_eff_bkg->SetBinError(i,0);
	}
	//-- compute significance
	//vector<pair<float,float> > roc_v;
	float* roc_sig = new float[res.h_eff_sig->GetNbinsX()];
	float* roc_bkg = new float[res.h_eff_sig->GetNbinsX()];
	int imax = 0;
	for(int i=1;i<res.h_eff_sig->GetNbinsX()+1;i++){
		float Ns = res.h_eff_sig->GetBinContent(i)*integ_sig;
		//float Ns = res.h_eff_sig->GetBinContent(i);
		//float Nb = res.h_eff_bkg->GetBinContent(i);
		float Nb = res.h_eff_bkg->GetBinContent(i)*integ_bkg;
		float signif = 0;
		if(Nb!=0) signif = Ns/sqrt(Nb);
		double zbi = Zbi(Ns, Nb);
		//res.h_Zbi->SetBinContent(i,signif);
		res.h_Zbi->SetBinContent(i,zbi);
		res.h_Zbi->SetBinError(i,0);
		

		//ROC curve
		//h_ROC->SetBinContent(h_ROC->FindBin(res.h_eff_sig->GetBinContent(i)),res.h_eff_bkg->GetBinContent(i));
		//h_ROC->SetBinContent(h_ROC->FindBin(res.h_eff_bkg->GetBinContent(i)),res.h_eff_sig->GetBinContent(i));
		//roc_v.push_back(pair<float,float>(res.h_eff_sig->GetBinContent(i),res.h_eff_bkg->GetBinContent(i));
		roc_sig[imax] = res.h_eff_sig->GetBinContent(i);
		roc_bkg[imax] = 1-res.h_eff_bkg->GetBinContent(i);
		imax++;
	}
	res.gROC = new TGraph(imax,roc_sig,roc_bkg);

	res.h_Zbi->GetXaxis()->SetTitle(var.c_str());
	res.h_Zbi->GetYaxis()->SetTitle("Zbi");

	res.h_eff_bkg->SetTitle("Efficiency on background");
	res.h_eff_bkg->GetXaxis()->SetTitle(var.c_str());
	res.h_eff_bkg->GetYaxis()->SetTitle("#epsilon_{bkg}");
	
	res.h_eff_sig->SetTitle("Efficiency on signal");
	res.h_eff_sig->GetXaxis()->SetTitle(var.c_str());
	res.h_eff_sig->GetYaxis()->SetTitle("#epsilon_{sig}");

	res.gROC->SetTitle("ROC");
	res.gROC->GetXaxis()->SetTitle("#epsilon_{signal}");
	res.gROC->GetYaxis()->SetTitle("1-#epsilon_{bkg}");
	res.gROC->SetLineWidth(2);

 	res.cplots = new TCanvas();
	res.cplots->Divide(2,2);
	res.h_eff_sig->GetYaxis()->SetRangeUser(0,1);
	res.cplots->cd(1);
	res.h_eff_sig->Draw();
	res.h_eff_bkg->SetLineColor(kRed);
	res.cplots->cd(2);
	res.h_eff_bkg->Draw("");
	res.cplots->cd(3);
	res.h_Zbi->Draw();
	res.cplots->cd(4);
   	//h_ROC->Draw();	
	res.gROC->Draw("ACP");
	
	int mbin = res.h_Zbi->GetMaximumBin();
	float maxZbi = -9999;
	for(int i=1;i<res.h_Zbi->GetNbinsX();i++){
		if(res.h_Zbi->GetBinContent(i)>maxZbi && res.h_eff_sig->GetBinContent(i)>0.1){
			maxZbi = res.h_Zbi->GetBinContent(i);
			mbin = i;
		}
	}
	cout<<"@max - Zbi = "<<res.h_Zbi->GetMaximum()<<" cut = "<<res.h_Zbi->GetBinLowEdge(mbin)<<" effs = "<<res.h_eff_sig->GetBinContent(mbin)<<" effb = "<<res.h_eff_bkg->GetBinContent(mbin)<<endl;
	cout<<"Ns = "<<res.h_eff_sig->GetBinContent(mbin)*integ_sig<<" Nb = "<<res.h_eff_bkg->GetBinContent(mbin)*integ_bkg<<endl;
	cout<<"ROC-integral: "<<res.gROC->Integral()<<endl;
	//looking for a bin with eff = XXX
	int mbin2 = 0;
	int mbin7 = 0;
	float eff_ref = 0.75;
	float d_eff_closest = 100;
	for(int i=1;i<res.h_eff_sig->GetNbinsX();i++){
		if(res.h_eff_sig->GetBinLowEdge(i) == 7) mbin7 = i;
		float eff = res.h_eff_sig->GetBinContent(i);
		if(fabs(eff-eff_ref)<d_eff_closest){
			d_eff_closest = fabs(eff-eff_ref);
			//cout<<eff<<" "<<d_eff_closest<<endl;
			mbin2 = i;
		}
	}	
	cout<<"ref - Zbi = "<<res.h_Zbi->GetBinContent(mbin2)<<" cut = "<<res.h_Zbi->GetBinLowEdge(mbin2)<<" effs = "<<res.h_eff_sig->GetBinContent(mbin2)<<" effb = "<<res.h_eff_bkg->GetBinContent(mbin2)<<endl;
	cout<<"Ns = "<<res.h_eff_sig->GetBinContent(mbin2)*integ_sig<<" Nb = "<<res.h_eff_bkg->GetBinContent(mbin2)*integ_bkg<<endl;
	
	//cout<<"cut@7 - Zbi = "<<res.h_Zbi->GetBinContent(mbin7)<<" cut = "<<res.h_Zbi->GetBinLowEdge(mbin7)<<" effs = "<<res.h_eff_sig->GetBinContent(mbin7)<<" effb = "<<res.h_eff_bkg->GetBinContent(mbin7)<<endl;
	//cout<<"Ns = "<<res.h_eff_sig->GetBinContent(mbin7)*integ_sig<<" Nb = "<<res.h_eff_bkg->GetBinContent(mbin7)*integ_bkg<<endl;
	
	//cout<<"Ns_tot = "<<integ_sig<<" Nb_tot = "<<integ_bkg<<endl;
	/*
	THStack* stack = c->GetPrimitive("");
	stack->Draw();
	cout<<stack<<endl;
	//TH1* h_bkg = stack->GetHistogram();
	//cout<<h_bkg<<endl;
	G//h_bkg->Draw();
	TH1* h_bkg = ((TH1*)(stack->GetStack()->Last()));
	h_bkg->Draw();
	cout<<h_bkg->GetNbinsX()<<endl;
*/
	
	/*
	//Compute eff
	TH1* h_eff_bkg = (TH1*) h_bkg->Clone("");
	cout<<h_eff_bkg->GetNbinsX()<<endl;
	for(int i=1;i<h_eff_bkg->GetNbinsX();i++){
		float num = h_bkg->Integral(i,h_bkg->GetNbinsX()+1);
		float den = h_bkg->Integral(0,h_bkg->GetNbinsX()+1);
		float eff = 0;
		if(den!=0) eff== num/den;
		h_eff_bkg->SetBinContent(i,eff);
	}
	h_eff_bkg->Draw();
	cout<<"max = "<<stack->GetHistogram()->GetMaximum()<<endl;
	cout<<c->GetPrimitive("::THStack")<<endl;
	cout<<h_bkg<<endl;
	*/

	return res;
	
}

void All(){
	string varname = "topness";
	//string signal = "T2tt_850_100";
	string signal = "T2tt_650_100";
	//string region = "baseline";
	//string region = "baselineSmallDM";
	string region = "baselineSmallDM300";
	//string region = "baselineSmallDM2b";
	string channel = "allChannels";
	/*
	Optimization("topness", signal, region, channel);
	Optimization("topness_m1", signal, region, channel);
	Optimization("topness_m2", signal, region, channel);
	Optimization("topness_m3", signal, region, channel);
	Optimization("topness_m4", signal, region, channel);
	Optimization("topness_m5", signal, region, channel);
	Optimization("topness_m6", signal, region, channel);
	Optimization("topness_m7", signal, region, channel);
	Optimization("topness_m8", signal, region, channel);
	Optimization("topness_m9", signal, region, channel);
	Optimization("topness_m10", signal, region, channel);
	Optimization("topness_m11", signal, region, channel);
	Optimization("topness_m12", signal, region, channel);
	Optimization("topness_m13", signal, region, channel);
*/
	OptRes res_650_ref = Optimization(varname, "T2tt_650_100", region, channel,"plots/config_def.txt/1DStack.root");
	OptRes res_850_ref = Optimization(varname, "T2tt_850_100", region, channel,"plots/config_def.txt/1DStack.root");
	Shape(varname, region, channel,"plots/config_def.txt/1DSuperimposed.root");
	//Optimization("topness", signal, region, channel,"plots/config_def.txt/pow2/1DStack.root");
	//Optimization("topness", signal, region, channel,"plots/config_def.txt/pow2/b1nonb/1DStack.root");
	/*
	OptRes res_650_comb3 = Optimization(varname, "T2tt_650_100", region, channel,"plots/config_def.txt/comb3_all/1DStack.root");
	OptRes res_850_comb3 = Optimization(varname, "T2tt_850_100", region, channel,"plots/config_def.txt/comb3_all/1DStack.root");
	OptRes res_650_comb4 = Optimization(varname, "T2tt_650_100", region, channel,"plots/config_def.txt/comb4_all/1DStack.root");
	OptRes res_850_comb4 = Optimization(varname, "T2tt_850_100", region, channel,"plots/config_def.txt/comb4_all/1DStack.root");
	OptRes res_650_comb5 = Optimization(varname, "T2tt_650_100", region, channel,"plots/config_def.txt/comb5_all/1DStack.root");
	OptRes res_850_comb5 = Optimization(varname, "T2tt_850_100", region, channel,"plots/config_def.txt/comb5_all/1DStack.root");
	OptRes res_650_comb4_b1 = Optimization(varname, "T2tt_650_100", region, channel,"plots/config_def.txt/comb4/1DStack.root");
	OptRes res_850_comb4_b1 = Optimization(varname, "T2tt_850_100", region, channel,"plots/config_def.txt/comb4/1DStack.root");
	OptRes res_650_comb3_4 = Optimization(varname, "T2tt_650_100", region, channel,"plots/config_def.txt/comb3_4/1DStack.root");
	OptRes res_850_comb3_4 = Optimization(varname, "T2tt_850_100", region, channel,"plots/config_def.txt/comb3_4/1DStack.root");
		
		TCanvas* cROC = new TCanvas();
		TLegend* leg = new TLegend(0.6,0.7,0.8,0.9);
		cROC->Divide(2);
		cROC->cd(1);
		res_650_ref.gROC->SetTitle("ROC - T2tt_650-100");
		res_650_ref.gROC->Draw("ACP");
		res_650_comb3.gROC->SetLineColor(kGreen);
		res_650_comb3.gROC->Draw("same");
		res_650_comb4.gROC->SetLineColor(kRed);
		res_650_comb4.gROC->Draw("same");
		res_650_comb5.gROC->SetLineColor(kBlue);
		res_650_comb5.gROC->Draw("same");
		res_650_comb4_b1.gROC->SetLineColor(kOrange);
		res_650_comb4_b1.gROC->Draw("same");
		res_650_comb3_4.gROC->SetLineColor(kViolet);
		res_650_comb3_4.gROC->Draw("same");
		leg->AddEntry(res_650_ref.gROC,"default","l");
		leg->AddEntry(res_650_comb3.gROC,"3 jets","l");
		leg->AddEntry(res_650_comb4.gROC,"4 jets","l");
		leg->AddEntry(res_650_comb5.gROC,"5 jets","l");
		leg->AddEntry(res_650_comb4_b1.gROC,"4 jets (nb<=1)","l");
		leg->AddEntry(res_650_comb3_4.gROC,"4 jets (nb<=1) && 3 jets (nb>=2)","l");
		leg->Draw("same");
		cROC->cd(2);
		res_850_ref.gROC->SetTitle("ROC - T2tt_850-100");
		res_850_ref.gROC->Draw("ACP");
		res_850_comb3.gROC->SetLineColor(kGreen);
		res_850_comb3.gROC->Draw("same");
		res_850_comb4.gROC->SetLineColor(kRed);
		res_850_comb4.gROC->Draw("same");
		res_850_comb5.gROC->SetLineColor(kBlue);
		res_850_comb5.gROC->Draw("same");
		res_850_comb4_b1.gROC->SetLineColor(kOrange);
		res_850_comb4_b1.gROC->Draw("same");
		res_850_comb3_4.gROC->SetLineColor(kViolet);
		res_850_comb3_4.gROC->Draw("same");
		leg->Draw("same");
		cROC->Draw();
		cROC->Print("combjets.png");
 	return 0;	
*/	
	//for(int i=12;i<13;i++){
	//for(int i=45;i<60;i++){
	for(int i=20;i<22;i++){
	//for(int i=1;i<45;i++){
	//for(int i=1;i<9;i++){
		char dir[120]; 
		sprintf(dir,"plots/config2_%d.txt/",i);
		//sprintf(dir,"plots/config2_%d.txt/comb3_4/",i);
		//sprintf(dir,"plots/config_def.txt/comb3_4/");

		
		//sprintf(dir,"plots/tp2_config_%d.txt/pow2/1b1nonb/",i);
		//sprintf(dir,"plots/tp2_config_%d.txt/pow2/loopnonb_all/",i);
		//sprintf(dir,"plots/tp2_config_%d.txt/pow2/1DStack.root",i);
		//sprintf(dir,"plots/tp2_config_%d.txt/pow2/loopnonb/1DStack.root",i);
		//sprintf(dir,"plots/tp2_config_%d.txt/pow2/loopnonb_all/1DStack.root",i);
		//sprintf(dir,"plots/tp2_config_%d.txt/pow2/comb4/1DStack.root",i);
		//sprintf(dir,"plots/config_%d.txt/pow2/1DStack.root",i);
		//sprintf(dir,"plots/config_%d.txt/pow2/1DStack.root",i);
		//sprintf(dir,"plots_before_20July/plots/config_%d.txt/",i);
		//sprintf(dir,"plots/config_%d.txt/1DStack.root",i);
		//sprintf(dir,"plots/topness2/tp2_config_%d.txt/1DStack.root",i);
		cout<<"config_"<<i<<endl;
 		string file = string(dir)+"purity.dat";
		string command;
		command = string("cat ") + file + string(" | grep purity | awk '{print $8}'");
		system(command.c_str());
		string rfile = string(dir)+"1DStack.root";
		//Optimization(varname, signal, region, channel, rfile);
		OptRes res_650_cur = Optimization(varname, "T2tt_650_100", region, channel, rfile);
		OptRes res_850_cur = Optimization(varname, "T2tt_850_100", region, channel, rfile);
		rfile = string(dir)+"1DSuperimposed.root";
		Shape(varname, region, channel, rfile);
		
		TCanvas* cROC = new TCanvas();
		TLegend* leg = new TLegend(0.6,0.9,0.6,0.9);
		cROC->Divide(2);
		cROC->cd(1);
		res_650_ref.gROC->SetTitle("ROC - T2tt_650-100");
		res_650_ref.gROC->Draw("ACP");
		res_650_cur.gROC->SetLineColor(kRed);
		res_650_cur.gROC->Draw("same");
		leg->AddEntry(res_650_ref.gROC,"default","l");
		leg->AddEntry(res_650_cur.gROC,"current","l");
		leg->Draw("same");
		cROC->cd(2);
		res_850_ref.gROC->SetTitle("ROC - T2tt_850-100");
		res_850_ref.gROC->Draw("ACP");
		res_850_cur.gROC->SetLineColor(kRed);
		res_850_cur.gROC->Draw("same");
		leg->Draw("same");
		cROC->Draw();
		string cname_out(dir);
		std::replace( cname_out.begin(), cname_out.end(), '/', '_');
		cname_out="results/"+cname_out+".png";
		cROC->Print(cname_out.c_str());
	}
}

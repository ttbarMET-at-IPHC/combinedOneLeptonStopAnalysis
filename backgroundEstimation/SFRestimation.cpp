//////////////////////////////////////////////////////////////////////////
//
// 'ORGANIZATION AND SIMULTANEOUS FITS' RooFit tutorial macro #501
// 
// Using simultaneous p.d.f.s to describe simultaneous fits to multiple
// datasets
//
//
//
// 07/2008 - Wouter Verkerke 
// 
/////////////////////////////////////////////////////////////////////////

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooConstVar.h"
#include "RooChebychev.h"
#include "RooAddPdf.h"
#include "RooSimultaneous.h"
#include "RooCategory.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TFile.h"
#include "TH1F.h"
#include "THStack.h"
#include "TRandom.h"
#include "TMath.h"

#include "interface/Table.h" 

#define PlotDir plots4CR1 

using namespace RooFit ;
//using namespace theDoctor ;


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


#include "../sonicScrewdriver/interface/Figure.h"

using namespace std;


//global variable !
TRandom* rand_; 

std::pair<double,double>  GetSF(RooFitResult* res, string param){

  RooRealVar* par_init = (RooRealVar*) res->floatParsInit().find(param.c_str());
  RooRealVar* par_final = (RooRealVar*) res->floatParsFinal().find(param.c_str());
  double vinit = par_init->getVal();
  double vfinal = par_final->getVal();
  double SF = 1.;
  if(par_init!=0) SF = vfinal/vinit;
  double SFerror = 0.;
  if(vfinal!=0) SFerror = SF*par_final->getError()/vfinal;
  cout<<"SF("<<param<<"):\t"<<SF<<" +/- "<<SFerror<<" ("<<SFerror/SF*100<<"%)"<<endl;
  std::pair<double,double> pSF(SF,SFerror);
  return pSF;
}


/*
RooHistPdf* GetRooHistPdf(TFile* fin, string region, string process, string varname, RooRealVar* var, float& norm, bool do_mcstat){
  TH1F* h  =  GetHisto(fin,region,process,varname, norm);
}
*/
//----------  Retrieve pdf (norm distrib) -----------------//

TH1F* GetHisto(TFile* fin, string region, string process, string varname, float& norm){
	bool normDistrib = false;
	string cname = "singleLepton/"+region+"/"+varname;
	//TCanvas* c = fin->Get("singleLepton/0btag/R");
	TCanvas* c = (TCanvas*) fin->Get(cname.c_str());
	string hname = "v:"+varname+"|p:"+process+"|r:"+region+"|c:singleLepton|t:1DEntries";
	TH1F* h = 0;
	if(normDistrib){
		h = (TH1F*) c->GetPrimitive(hname.c_str());
		norm = h->Integral();
	}
	else{
		TPad* pad = (TPad*) c->GetPrimitive("");
		THStack* stack = (THStack*) pad->GetPrimitive("");
		h = (TH1F*) stack->GetHists()->FindObject(hname.c_str());
		norm = h->Integral();
	}
	return (TH1F*) h->Clone();
}

RooHistPdf* GetRooHistPdf(TFile* fin, string region, string process, string varname, RooRealVar* var, float& norm, bool do_mcstat){
  TH1F* h  =  GetHisto(fin,region,process,varname, norm);
  if(do_mcstat){
  	//randomisation of the histo ...
	for(int i=1;i<=h->GetNbinsX();i++){
		h->SetBinContent(i,rand_->Gaus(h->GetBinContent(i),h->GetBinError(i)));
	}
  }
  string rdhname = "rdh_"+region+"_"+process;
  RooDataHist *rdh  = new RooDataHist(rdhname.c_str(),rdhname.c_str(),RooArgList(*var),Import(*h));
  string pdfname = "pdf_"+region+"_"+process;
  RooHistPdf *pdf  = new  RooHistPdf(pdfname.c_str(),pdfname.c_str(),RooArgSet(*var),*rdh);
  return pdf;
}
//---------------------------------------------------------//

//----------  Retrieve pdf combining several regions -----------------//

TH1F* GetMergedHisto(TFile* fin, vector<string> regions, string process, string varname, float& norm){
	vector<float> norms(regions.size());
	vector<TH1F*> histos(regions.size());
	norm = 0;
	for(unsigned int i=0;i<regions.size();i++){
		histos[i] = GetHisto(fin, regions[i], process, varname, norms[i]);
		//sum the norms
		norm+=norms[i];
	}
	//Create the new merged histo
	string hname = varname+"_"+process+"_merged";
	int nbins = 0;
	float min = 0;
	float max = 0;
	if(regions.size()!=0){ 
		nbins=histos[0]->GetNbinsX()*regions.size();
		min = histos[0]->GetXaxis()->GetXmin();
		max = min+(histos[0]->GetXaxis()->GetXmax()-histos[0]->GetXaxis()->GetXmin())*regions.size();
	}
	TH1F* mergedHisto = new TH1F(hname.c_str(),hname.c_str(),nbins,min,max);
	int indice = 1;
	for(unsigned int i=0;i<histos.size();i++){
		for(int ibin=1;ibin<=histos[i]->GetNbinsX();i++){
			mergedHisto->SetBinContent(indice,histos[i]->GetBinContent(ibin));
			mergedHisto->SetBinError(indice,histos[i]->GetBinError(ibin));
			indice++;	
		}
	}
	return mergedHisto;
}

RooHistPdf* GetMergedRooHistPdf(TFile* fin, vector<string> region, string process, string varname, RooRealVar* var, float& norm){
  TH1F* h  =  GetMergedHisto(fin,region,process,varname, norm);
  string rdhname = "rdh_merged_"+process;
  RooDataHist *rdh  = new RooDataHist(rdhname.c_str(),rdhname.c_str(),RooArgList(*var),Import(*h));
  string pdfname = "pdf_merged_"+process;
  RooHistPdf *pdf  = new  RooHistPdf(pdfname.c_str(),pdfname.c_str(),RooArgSet(*var),*rdh);
  return pdf;
}

//----------  Retrieve data histo  -----------------//
TH1F* GetData(TFile* fin, string region, string varname){
	//-- protect againt using JES uncertainty --//
	size_t pos = varname.find("_JESup");
	if(pos!=string::npos){
	        varname = varname.substr(0,pos);
		cout<<"Modify variable name for data "<<varname<<endl;
	}
	pos = varname.find("_JESdown");
	if(pos!=string::npos){
	        varname = varname.substr(0,pos);
		cout<<"Modify variable name for data "<<varname<<endl;
	}
	//-----------------------------------------//

	string cname = "singleLepton/"+region+"/"+varname;
	//TCanvas* c = fin->Get("singleLepton/0btag/R");
	cout<<"name = "<<cname<<endl;
	TCanvas* c = (TCanvas*) fin->Get(cname.c_str());
	cout<<c<<endl;
	TPad* pad = (TPad*) c->GetPrimitive("");
	string hname = "v:"+varname+"|r:"+region+"|c:singleLepton|t:1DSumData";
	TH1F* h = (TH1F*) pad->GetPrimitive(hname.c_str());
	return (TH1F*) h->Clone();
}

RooDataHist* GetRooData(TFile* fin, string region, string varname, RooRealVar* var){
	TH1F* h = GetData(fin, region, varname);
	cout<<"data = "<<h->GetEntries()<<endl;
	string dname = "data_"+region;
  	RooDataHist *datah = new RooDataHist(dname.c_str(),dname.c_str(), RooArgList(*var), Import(*h));
	return datah;
}

//---------- Perform Chi2 test to control variable shape in CR --/
void Chi2Test(TFile* fin, string region, string varname){
	//retrieve data histo
	TH1F* hData = GetData(fin, region, varname);
	cout<<hData->GetEntries()<<endl;
	//retrieve MC sum (THStack);
	string cname = "singleLepton/"+region+"/"+varname;
	TCanvas* c = (TCanvas*) fin->Get(cname.c_str());
	TPad* pad = (TPad*) c->GetPrimitive("");
	THStack* stack = (THStack*) pad->GetPrimitive("");
	//TH1F* hMC = (TH1F*) stack->GetHistogram();
	TH1F* hMC = 0;
	TIter next(stack->GetHists());
	TObject* obj = 0;
	bool first = true;
	while ((obj = next())){
		if(first){
			hMC = (TH1F*) obj->Clone();
			first = false;
		}
		hMC->Add((TH1F*)obj);
	}
	cout<<hMC->GetEntries()<<endl;
	//--- perform tests --/
	cout<<"##################################################"<<endl;
	cout<<"#   Test - region:"<<region<<" var:"<<varname<<endl;
	cout<<"##################################################"<<endl;
	float pvalue = hData->Chi2Test(hMC,"UWp"); //"p CHI2/NDF
	cout<<"# Chi2 test\t"<<pvalue<<endl;
	pvalue = hData->KolmogorovTest(hMC);
	cout<<"# Kolmogorov test\t"<<pvalue<<endl;
	cout<<"##################################################"<<endl;
}

struct FitSetup{
  //--name of the root file --//
  string filename;
  //--- variable used to the fit --//
  string varname;
  float varMin;
  float varMax;
  //--- region used to the fit ---//
  string region;
  //--- Xsection uncertainties --//
  float xs_sysfactor;
  bool do_xs_tt2l_sys;
  bool do_xs_rare_sys;
  //--- MC stat uncertainties --//
  bool do_mcstat;
  //-- algorithm used to the fit --//
  string type;
  string algo;
  //-- Init value uncert --//
  bool do_init_uncert;
  float init_tt1l;
  float init_wjets;

  void Reset(){
	 //filename="plots4CR1/ttbar_madgraph/1DDataMCComparison.root";
	 filename="plots4CR1/ttbar_powheg/1DDataMCComparison.root";
	 //varname="Mlb";
	 varname="Mlb";
	 //varname="M3b";
	 //varname="M3b_extended";
	 //varname="Mlb";
	 varMin = 0;
	 varMax = 1000;
 	 region = "0btag_MTtail";
 	 //--- Xsection uncertainties --//
  	 xs_sysfactor=1.;
         do_xs_tt2l_sys = false;
         do_xs_rare_sys = false;
  	 //--- MC stat uncertainties --//
         do_mcstat = false;
  	 //-- algorithm used to the fit --//
  	 type = "Minuit2";
  	 algo = "MIGRAD";
   	 //-- Init value uncert --//
  	 do_init_uncert = false;
  	 init_tt1l=1.;
  	 init_wjets=1.;
  }
};

struct FitResult{
	string conditions;
	pair<float,float> SF_tt1l;
	pair<float,float> SF_wjets;
	float tt1l_norm;
	float correlation;
	float edm;

	void Reset(){
		tt1l_norm = 0;
		edm = 0;
		SF_tt1l = pair<float,float>(0,0);
		SF_wjets = pair<float,float>(0,0);
		correlation = 0;
	}
	void Print(){
		cout<<"#################"<<endl;
		cout<<"# "<<conditions<<"\t SF_tt1l = "<<SF_tt1l.first<<"+/-"<<SF_tt1l.second<<"\t SF_wjets = "<<SF_wjets.first<<"+/-"<<SF_wjets.second<<endl;
		cout<<"#\t edm = "<<edm<<" correlation = "<<correlation<<endl;
		cout<<"#\t init tt1l: "<<tt1l_norm<<" \t fitted tt1l: "<<tt1l_norm*SF_tt1l.first<<endl;
		cout<<"#################"<<endl;
	}
};


TH1F* TailDescription(string region, string varname, string process, string file1, string file2, float vmin, float vmax){
	TFile* f1 = TFile::Open(file1.c_str());
	float norm1 = 0;
	TH1F* h1 = GetHisto(f1, region, process, varname, norm1);
	TFile* f2 = TFile::Open(file2.c_str());
	float norm2 = 0;
	TH1F* h2 = GetHisto(f2, region, process, varname, norm2);
	int imin = h1->FindBin(vmin);
	int imax = h1->FindBin(vmin);
	Double_t err1, err2 = 0;
	float v1, v2 = 0;
	float r1, r2 = 0;
	float r1_uncert, r2_uncert = 0;
	float ratio =1;
	float ratio_uncert =0;
	int ibin = 1;
	string hname = region+"_"+varname+"_"+process+"_ratio";
	TH1F* hratio = new TH1F(hname.c_str(),hname.c_str(),imax-imin,vmin,vmax);
	for(int i=imin;i<imax;i++){
		v1 = h1->IntegralAndError(i,h1->GetNbinsX()+1,err1);
		r1=v1/h1->Integral();
		r1_uncert = err1/r1*v1;
		v2 = h2->IntegralAndError(i,h2->GetNbinsX()+1,err2);
		r2=v2/h2->Integral();
		r2_uncert = err2/r2*v2;
		ratio = r2/r1;
		ratio_uncert = sqrt(1./(r1*r1)*r1_uncert*r1_uncert+r2*r2*r2_uncert*r2_uncert);
		hratio->SetBinContent(ibin,ratio);
		hratio->SetBinError(ibin,ratio_uncert);
		ibin++;
	}
	return hratio;
}

FitResult  doFit(const FitSetup& setup, string conditions, string fname=string(""))
{

  string varname = setup.varname;
  RooRealVar var(varname.c_str(),varname.c_str(),setup.varMin, setup.varMax);

  //string region="0btag_MTtail"; 
  string region= setup.region;

  //should it be an argument ?
  TFile* fin = 0;
  if(fname=="") fin = TFile::Open(setup.filename.c_str());
  //else fin = TFile::Open(fname.c_str());
  else fin = TFile::Open(fname.c_str());

  //-- normalisation in the MC --//
  float mc_norm_tt1l = 0;
  float mc_norm_tt2l = 0;
  float mc_norm_wjets = 0;
  float mc_norm_rare = 0;

  // C r e a t e   m o d e l   f o r  CR1_peak_lowM3b
  // -------------------------------------------------------------
  // Construct pdf for tt1l
  RooHistPdf *pdf_tt1l  = GetRooHistPdf(fin,region,"1ltop",varname,&var,mc_norm_tt1l, setup.do_mcstat);
  // Construct pdf for tt2l
  RooHistPdf *pdf_tt2l  = GetRooHistPdf(fin,region,"ttbar_2l",varname,&var,mc_norm_tt2l, setup.do_mcstat);
  // Construct pdf for Wjets
  RooHistPdf *pdf_Wjets  = GetRooHistPdf(fin,region,"W+jets",varname,&var,mc_norm_wjets, setup.do_mcstat);
  // Construct pdf for rare
  RooHistPdf *pdf_rare  = GetRooHistPdf(fin,region,"rare",varname,&var,mc_norm_rare, setup.do_mcstat);

  // normalization factors (RooRealVar)
  float val_tt1l = mc_norm_tt1l;
  float val_wjets = mc_norm_wjets;
  if(setup.do_init_uncert){
  	val_tt1l = setup.init_tt1l*mc_norm_tt1l;
  	val_wjets = setup.init_wjets*mc_norm_wjets;
  }
  RooRealVar norm_tt1l("norm_tt1l","norm_tt1l",val_tt1l,0.25*mc_norm_tt1l,10.*mc_norm_tt1l);
  RooRealVar norm_wjets("norm_wjets","norm_wjets",val_wjets,0.25*mc_norm_wjets,10.*mc_norm_wjets);
  //RooRealVar norm_tt2l("norm_tt2l","norm_tt2l",mc_norm_tt2l,0.25*mc_norm_tt2l,2*mc_norm_tt2l);
  //RooRealVar norm_rare("norm_rare","norm_rare",mc_norm_rare,0.25*mc_norm_rare,2*mc_norm_rare);
  // possibility to study a systematic on it
  if(setup.do_xs_tt2l_sys) mc_norm_tt2l*=setup.xs_sysfactor;
  if(setup.do_xs_rare_sys) mc_norm_rare*=setup.xs_sysfactor;
  RooConstVar norm_tt2l("norm_tt2l","norm_tt2l",mc_norm_tt2l);
  RooConstVar norm_rare("norm_rare","norm_rare",mc_norm_rare);

  RooAddPdf model("model","model",
  	RooArgList(*pdf_tt1l,*pdf_tt2l,*pdf_Wjets,*pdf_rare),
	RooArgList(norm_tt1l,norm_tt2l,norm_wjets,norm_rare)) ;
  //model.Print();

  
  RooDataHist *data_CR1_peak_lowM3b = GetRooData(fin,region,varname,&var);

  fin->Close();
  
  
  //--  Constraints on single top and rare --//
  float RelUncert = 0.2;
  // Construct another Gaussian constraint p.d.f on single top bkg
  // Construct another Gaussian constraint p.d.f on "rare" bkg
  RooGaussian constr_rare("constr_rare","constr_rare",norm_rare,RooConst(mc_norm_rare),RooConst(RelUncert*mc_norm_rare)) ; 
  // Construct another Gaussian constraint p.d.f on "tt2l" bkg
  RooGaussian constr_tt2l("constr_tt2l","constr_tt2l",norm_tt2l,RooConst(mc_norm_tt2l),RooConst(RelUncert*mc_norm_tt2l)) ; 

  // P e r f o r m   t em p l a t e   f i t
  // ---------------------------------------------------

  //Minimizer(type,algo)           -- Choose minimization package and algorithm to use. Default is MINUIT/MIGRAD through the RooMinimizer
  //                                 interface, but rare can be specified (through RooMinimizer interface). Select OldMinuit to use
  //                                 MINUIT through the old RooMinuit interface
  //
  //                                        Type         Algorithm
  //                                        ------       ---------
  //                                        OldMinuit    migrad, simplex, minimize (=migrad+simplex), migradimproved (=migrad+improve)
  //                                        Minuit       migrad, simplex, minimize (=migrad+simplex), migradimproved (=migrad+improve)
  //                                        Minuit2      migrad, simplex, minimize, scan
  //                                        GSLMultiMin  conjugatefr, conjugatepr, bfgs, bfgs2, steepestdescent
  //                                        GSLSimAn     -

  
  // ---  Perform simultaneous fit of model to data and model_ctl to data_ctl --//
  //RooFitResult* res = model.fitTo(*data_CR1_peak_lowM3b,Save());
  RooFitResult* res = model.fitTo(*data_CR1_peak_lowM3b,ExternalConstraints(constr_rare),ExternalConstraints(constr_tt2l),PrintLevel(-1),Save(),
  	Minimizer(setup.type.c_str(),setup.algo.c_str()));
  
  //--- Writing the results ---///
  FitResult fitRes;
  fitRes.Reset();
  fitRes.tt1l_norm = mc_norm_tt1l;
  fitRes.SF_tt1l = GetSF(res,"norm_tt1l");
  fitRes.SF_wjets = GetSF(res,"norm_wjets");
  fitRes.edm = res->edm();
  fitRes.correlation = res->correlationMatrix()[0][1];
  fitRes.conditions = conditions;

  fitRes.Print();
  return fitRes;
 
  // Plot data and complete PDF overlaid
  /*
  RooPlot* xframe  = var.frame(Title("Component plotting of pdf=(sig1+sig2)+(bkg1+bkg2)")) ;
  data_CR1_peak_lowM3b->plotOn(xframe) ;
  model.plotOn(xframe) ;
  model.plotOn(xframe,Components("pdf_1ltop"),LineColor(kRed)) ;
  model.plotOn(xframe,Components("pdf_ttbar_2l"),LineColor(kCyan)) ;
  model.plotOn(xframe,Components("pdf_W+jets"),LineColor(kGreen)) ;
  model.plotOn(xframe,Components("pdf_rare"),LineColor(kPink)) ;
  TCanvas* c = new TCanvas();
  xframe->Draw();
 */

}

struct FitUncert{
	string name;
	float SF_tt1l_uncert;
	float SF_wjets_uncert;
};

struct SummaryResult{
	FitResult nominal;
	vector<FitUncert> uncert;
	void Print(){
		cout<<"#############################"<<endl;
		cout<<"#      Summary results      #"<<endl;
		nominal.Print();
		cout<<"#############################"<<endl;
		cout<<"# Systematic uncertainties #"<<endl;
		cout<<"# name\t SF_tt1l\t SF_wjets"<<endl;
		for(unsigned int i=0;i<uncert.size();i++){
			cout<<"# "<<uncert[i].name<<"\t"<<uncert[i].SF_tt1l_uncert<<"\t"<<uncert[i].SF_wjets_uncert<<endl;
		}
		cout<<"#############################"<<endl;
	}
};


int main()
{


  bool bGenSyst = false;
  bool bBinningSyst = false;
  bool bChi2Tests = false;  

  //init random 
  rand_ = new TRandom();

  // Create observables
  RooRealVar var("Mlb","Mlb",0,600) ;
  string varname("Mlb");
  

  TFile* fin = TFile::Open("plots4CR1/ttbar_madgraph/1DDataMCComparison.root");
  
  SummaryResult summary;

  FitSetup setup;
  FitResult res;
  string conditions;
  FitUncert uncert;

  float min=0;
  float max=0;

  float value_up_SF_tt1l = 0;
  float value_down_SF_tt1l = 0;
  float value_up_SF_wjets = 0;
  float value_down_SF_wjets = 0;
 

  summary.nominal = res;

  //--- tail --//
  setup.Reset(); conditions="tail", setup.region="0btag_MTtail";
  res = doFit(setup,conditions); 
  theDoctor::Figure SFR_tt1l=theDoctor::Figure(res.SF_tt1l.first,res.SF_tt1l.second);
  theDoctor::Figure SFR_wjets=theDoctor::Figure(res.SF_wjets.first,res.SF_wjets.second);
  
  //--- nominal ---//
  setup.Reset(); conditions="peak", setup.region="0btag_MTpeak";
  res = doFit(setup,conditions); 
  SFR_tt1l/=theDoctor::Figure(res.SF_tt1l.first,res.SF_tt1l.second);
  SFR_wjets/=theDoctor::Figure(res.SF_wjets.first,res.SF_wjets.second);
  
  cout<<"0btag-SFR SFR_tt1l = "<<SFR_tt1l.Print()<<" SFR_wjets = "<<SFR_wjets.Print()<<endl;

  //--- preselection --//
  //setup.Reset(); conditions="preselection", setup.region="presel_MTpeak";
  setup.Reset(); conditions="preselection", setup.region="MTpeak";
  res = doFit(setup,conditions); 

  //--- xs(tt2l) ---//
  setup.Reset(); conditions="xs_tt2l";
  setup.do_xs_tt2l_sys = true;
  setup.xs_sysfactor = 0.8;	res = doFit(setup,conditions); value_down_SF_tt1l = res.SF_tt1l.first ; value_down_SF_wjets = res.SF_wjets.first; 
  setup.xs_sysfactor = 1.2; res = doFit(setup,conditions); value_up_SF_tt1l = res.SF_tt1l.first ; value_up_SF_wjets = res.SF_wjets.first;
  uncert.name = conditions;
  uncert.SF_tt1l_uncert = fabs((value_up_SF_tt1l-value_down_SF_tt1l)/2.);
  uncert.SF_wjets_uncert = fabs((value_up_SF_wjets-value_down_SF_wjets)/2.);
  summary.uncert.push_back(uncert);

  //--- xs(rare) ---//
  setup.Reset(); conditions="xs_rare";
  setup.do_xs_rare_sys = true;
  setup.xs_sysfactor = 0.8;	res = doFit(setup,conditions); value_down_SF_tt1l = res.SF_tt1l.first ; value_down_SF_wjets = res.SF_wjets.first; 
  setup.xs_sysfactor = 1.2; res = doFit(setup,conditions); value_up_SF_tt1l = res.SF_tt1l.first ; value_up_SF_wjets = res.SF_wjets.first;
  uncert.name = conditions;
  uncert.SF_tt1l_uncert = fabs((value_up_SF_tt1l-value_down_SF_tt1l)/2.);
  uncert.SF_wjets_uncert = fabs((value_up_SF_wjets-value_down_SF_wjets)/2.);
  cout<<uncert.SF_tt1l_uncert<<" "<<uncert.SF_wjets_uncert<<" "<<value_up_SF_tt1l<<" "<<value_down_SF_tt1l<<endl;
  summary.uncert.push_back(uncert);

  //--- choice of the minimizer --//
  //                                        Minuit2      migrad, simplex, minimize, scan
  //                                        GSLMultiMin  conjugatefr, conjugatepr, bfgs, bfgs2, steepestdescent
  setup.Reset(); conditions="algo";
  //setup.type="Minuit2"; setup.algo="simplex",	res = doFit(setup,conditions); value_down_SF_tt1l = res.SF_tt1l.first ; value_down_SF_wjets = res.SF_wjets.first; 
  //setup.type="Minuit2"; setup.algo="scan",	res = doFit(setup,conditions); value_down_SF_tt1l = res.SF_tt1l.first ; value_down_SF_wjets = res.SF_wjets.first; 
  //setup.type="GSLMultiMin"; setup.algo="bfgs2",	res = doFit(setup,conditions); value_down_SF_tt1l = res.SF_tt1l.first ; value_down_SF_wjets = res.SF_wjets.first; 
  //setup.type="GSLMultiMin"; setup.algo="conjugatepr",	res = doFit(setup,conditions); SF_tt1l_algo.push_back(res.SF_tt1l.first) ; SF_wjets_algo.push_back(res.SF_wjets.first); 
  vector<float> SF_tt1l_algo;
  vector<float> SF_wjets_algo;
  setup.type="Minuit2"; setup.algo="minimize",	res = doFit(setup,conditions); SF_tt1l_algo.push_back(res.SF_tt1l.first) ; SF_wjets_algo.push_back(res.SF_wjets.first); 
  setup.type="GSLMultiMin"; setup.algo="conjugatefr",	res = doFit(setup,conditions); SF_tt1l_algo.push_back(res.SF_tt1l.first) ; SF_wjets_algo.push_back(res.SF_wjets.first); 
  setup.type="GSLMultiMin"; setup.algo="bfgs",	res = doFit(setup,conditions); SF_tt1l_algo.push_back(res.SF_tt1l.first) ; SF_wjets_algo.push_back(res.SF_wjets.first); 
  setup.type="GSLMultiMin"; setup.algo="steepestdescent",	res = doFit(setup,conditions); SF_tt1l_algo.push_back(res.SF_tt1l.first) ; SF_wjets_algo.push_back(res.SF_wjets.first); 
  uncert.name = conditions;
  min = *std::min_element(SF_tt1l_algo.data(),SF_tt1l_algo.data()+SF_tt1l_algo.size());
  max = *std::max_element(SF_tt1l_algo.data(),SF_tt1l_algo.data()+SF_tt1l_algo.size());
  uncert.SF_tt1l_uncert = fabs((max-min)/2.);
  min = *std::min_element(SF_wjets_algo.data(),SF_wjets_algo.data()+SF_wjets_algo.size());
  max = *std::max_element(SF_wjets_algo.data(),SF_wjets_algo.data()+SF_wjets_algo.size());
  uncert.SF_wjets_uncert = fabs((max-min)/2.);
  summary.uncert.push_back(uncert);

  //--- MC statistic --//
  setup.Reset(); conditions="MCstat"; uncert.name = conditions;
  setup.do_mcstat = true;
  SF_tt1l_algo.clear();
  SF_wjets_algo.clear();
  for(int i=0;i<200;i++){
	res = doFit(setup,conditions); SF_tt1l_algo.push_back(res.SF_tt1l.first) ; SF_wjets_algo.push_back(res.SF_wjets.first);
  }
  uncert.SF_tt1l_uncert = TMath::RMS(SF_tt1l_algo.size(),SF_tt1l_algo.data());
  uncert.SF_wjets_uncert = TMath::RMS(SF_wjets_algo.size(),SF_wjets_algo.data());
  summary.uncert.push_back(uncert);

  //--- Conditions initial of the fit --//
  setup.Reset(); conditions="InitCond"; uncert.name = conditions;
  setup.do_init_uncert=true;
  setup.init_tt1l = 0.5; setup.init_wjets = 1.5;
  res = doFit(setup,conditions); value_down_SF_tt1l = res.SF_tt1l.first*setup.init_tt1l ; value_down_SF_wjets = res.SF_wjets.first*setup.init_wjets;
  setup.init_tt1l = 1.5; setup.init_wjets = 0.5;
  res = doFit(setup,conditions); value_up_SF_tt1l = res.SF_tt1l.first*setup.init_tt1l ; value_up_SF_wjets = res.SF_wjets.first*setup.init_wjets;
  uncert.SF_tt1l_uncert = fabs((value_down_SF_tt1l-value_up_SF_tt1l)/2.); 
  uncert.SF_wjets_uncert = fabs((value_down_SF_wjets-value_up_SF_wjets)/2.); 
  summary.uncert.push_back(uncert);
  
  
  //--- JES uncertainty --//
  setup.Reset(); conditions="JES"; uncert.name = conditions;
  setup.varname="Mlb_JESdown"; setup.varMin=0; setup.varMax=600;
  res = doFit(setup,conditions); value_down_SF_tt1l = res.SF_tt1l.first ; value_down_SF_wjets = res.SF_wjets.first;
  setup.varname="Mlb_JESup"; setup.varMin=0; setup.varMax=600;
  res = doFit(setup,conditions); value_up_SF_tt1l = res.SF_tt1l.first ; value_up_SF_wjets = res.SF_wjets.first;
  uncert.SF_tt1l_uncert = fabs((value_down_SF_tt1l-value_up_SF_tt1l)/2.); 
  uncert.SF_wjets_uncert = fabs((value_down_SF_wjets-value_up_SF_wjets)/2.); 
  summary.uncert.push_back(uncert);
  


  //---- Generator systematics --//
  	//--- Scale --//
  if(bGenSyst){
  
  setup.Reset(); conditions="scale"; uncert.name = conditions;
  res = doFit(setup,conditions,"plots4CR1/ttbar_madgraph_scaledown/1DDataMCComparison.root"); value_down_SF_tt1l = res.SF_tt1l.first ; value_down_SF_wjets = res.SF_wjets.first;
  res = doFit(setup,conditions,"plots4CR1/ttbar_madgraph_scaleup/1DDataMCComparison.root"); value_up_SF_tt1l = res.SF_tt1l.first ; value_up_SF_wjets = res.SF_wjets.first;
  uncert.SF_tt1l_uncert = fabs((value_down_SF_tt1l-value_up_SF_tt1l)/2.); 
  uncert.SF_wjets_uncert = fabs((value_down_SF_wjets-value_up_SF_wjets)/2.); 
  summary.uncert.push_back(uncert);

  	//--- matching --//
  setup.Reset(); conditions="matching"; uncert.name = conditions;
  res = doFit(setup,conditions,"plots4CR1/ttbar_madgraph_matchingdown/1DDataMCComparison.root"); value_down_SF_tt1l = res.SF_tt1l.first ; value_down_SF_wjets = res.SF_wjets.first;
  res = doFit(setup,conditions,"plots4CR1/ttbar_madgraph_matchingup/1DDataMCComparison.root"); value_up_SF_tt1l = res.SF_tt1l.first ; value_up_SF_wjets = res.SF_wjets.first;
  uncert.SF_tt1l_uncert = fabs((value_down_SF_tt1l-value_up_SF_tt1l)/2.); 
  uncert.SF_wjets_uncert = fabs((value_down_SF_wjets-value_up_SF_wjets)/2.); 
  summary.uncert.push_back(uncert);

  	//--- Scale --//
  setup.Reset(); conditions="topmass"; uncert.name = conditions;
  res = doFit(setup,conditions,"plots4CR1/ttbar_madgraph_mass166-5/1DDataMCComparison.root"); value_down_SF_tt1l = res.SF_tt1l.first ; value_down_SF_wjets = res.SF_wjets.first;
  res = doFit(setup,conditions,"plots4CR1/ttbar_madgraph_mass178-5/1DDataMCComparison.root"); value_up_SF_tt1l = res.SF_tt1l.first ; value_up_SF_wjets = res.SF_wjets.first;
  uncert.SF_tt1l_uncert = fabs((value_down_SF_tt1l-value_up_SF_tt1l)/2.); 
  uncert.SF_wjets_uncert = fabs((value_down_SF_wjets-value_up_SF_wjets)/2.); 
  summary.uncert.push_back(uncert);

  	//--- powheg --//
  setup.Reset(); conditions="powheg"; uncert.name = conditions;
  float vtt1l = 0;
  float vwjets = 0;
  res = doFit(setup,conditions,"plots4CR1/ttbar_powheg/1DDataMCComparison.root"); vtt1l = res.SF_tt1l.first ; vwjets = res.SF_wjets.first;
  uncert.SF_tt1l_uncert = fabs((vtt1l-summary.nominal.SF_tt1l.first)); 
  uncert.SF_wjets_uncert = fabs((vwjets-summary.nominal.SF_wjets.first)/2.); 
  summary.uncert.push_back(uncert);

	//--- top pt reweighting --//
  setup.Reset(); conditions="ptreweight"; uncert.name = conditions;
  res = doFit(setup,conditions,"plots4CR1/ttbar_madgraph_w_ptreweight/1DDataMCComparison.root"); vtt1l = res.SF_tt1l.first ; vwjets = res.SF_wjets.first;
  uncert.SF_tt1l_uncert = fabs((vtt1l-summary.nominal.SF_tt1l.first)); 
  uncert.SF_wjets_uncert = fabs((vwjets-summary.nominal.SF_wjets.first)/2.); 
  summary.uncert.push_back(uncert);
  }
  
  //--- binning --//
 if(bBinningSyst){
  setup.Reset(); conditions="binning"; uncert.name = conditions;
  setup.varname="M3b_binningdown";
  res = doFit(setup,conditions); value_down_SF_tt1l = res.SF_tt1l.first ; value_down_SF_wjets = res.SF_wjets.first;
  setup.varname="M3b_binningup";
  res = doFit(setup,conditions); value_up_SF_tt1l = res.SF_tt1l.first ; value_up_SF_wjets = res.SF_wjets.first;
  uncert.SF_tt1l_uncert = fabs((value_down_SF_tt1l-value_up_SF_tt1l)/2.);
  uncert.SF_wjets_uncert = fabs((value_down_SF_wjets-value_up_SF_wjets)/2.); 
  summary.uncert.push_back(uncert);
 }
  
  //--- choice of the variable --//
  setup.Reset(); conditions="variables"; uncert.name = conditions;
  /*
  setup.varname="R"; setup.varMin=0; setup.varMax=1;
  res = doFit(setup,conditions); 
  setup.varname="hadronicChi2"; setup.varMin=0; setup.varMax=10;
  res = doFit(setup,conditions); 
  setup.varname="deltaRLeptonB"; setup.varMin=0; setup.varMax=5;
  res = doFit(setup,conditions); 
  setup.varname="deltaPhiMETJets"; setup.varMin=0; setup.varMax=6;
  res = doFit(setup,conditions); 
  */
  /*
  setup.varname="Mlb"; setup.varMin=0; setup.varMax=600;
  res = doFit(setup,conditions); 
  setup.varname="leadingJetPt"; setup.varMin=0; setup.varMax=600;
  res = doFit(setup,conditions); 
  setup.varname="leadingBPt"; setup.varMin=0; setup.varMax=600;
  res = doFit(setup,conditions); 
  setup.varname="M3b_modif"; setup.varMin=0; setup.varMax=1200;
  res = doFit(setup,conditions); 
  //setup.varname="CSVdiscri"; setup.varMin=-1.5; setup.varMax=1.5;
  //res = doFit(setup,conditions); 
  */
  /*
  setup.varname="R"; setup.varMin=0; setup.varMax=1;
  res = doFit(setup,conditions); 
  setup.varname="hadronicChi2"; setup.varMin=0; setup.varMax=10;
  res = doFit(setup,conditions); 
  setup.varname="deltaRLeptonB"; setup.varMin=0; setup.varMax=5;
  res = doFit(setup,conditions); 
  setup.varname="deltaPhiMETJets"; setup.varMin=0; setup.varMax=6;
  res = doFit(setup,conditions); 
  */
  setup.Reset(); conditions="variables2"; setup.region="0btag_MTpeak"; uncert.name = conditions;
  setup.varname="Mlb"; setup.varMin=0; setup.varMax=600;
  res = doFit(setup,conditions); 
  setup.varname="M3b"; setup.region="0btag_MTpeak";
  res = doFit(setup,conditions); 


  //-- Apply the template fit on Njets = 4 && Njets >=5  
  // This will change the fraction W+jets && ttbar
  setup.Reset(); conditions="Njets"; setup.region="0btag_MTpeak_4j"; uncert.name = conditions; res = doFit(setup,conditions); 
  setup.Reset(); conditions="Njets"; setup.region="0btag_MTpeak_5j"; uncert.name = conditions; res = doFit(setup,conditions); 
  setup.Reset(); conditions="Njets"; setup.region="0btag_MTtail_4j"; uncert.name = conditions; res = doFit(setup,conditions); 
  setup.Reset(); conditions="Njets"; setup.region="0btag_MTtail_5j"; uncert.name = conditions; res = doFit(setup,conditions); 

  //---  Perform the template for MT = 120 --//
  setup.Reset(); conditions="C&C"; setup.region="0btag_MTinverted120"; uncert.name = conditions; res = doFit(setup,conditions); 
  setup.Reset(); conditions="C&C"; setup.region="0btag_MTtail120"; uncert.name = conditions; res = doFit(setup,conditions); 
  

  //---------------------------------------//
  //-- Apply the template fit on each SR --//
  //---------------------------------------//



  vector<string> signalRegions = {"LowBDT_T2tt_1",  "LowBDT_T2tt_2",  "LowBDT_T2tt_5",  "LowBDT_T2bw075_1",  "LowBDT_T2bw075_2",  "LowBDT_T2bw075_3",  "LowBDT_T2bw075_5",  "LowBDT_T2bw050_1",  "LowBDT_T2bw050_3",  "LowBDT_T2bw050_4",  "LowBDT_T2bw050_5",  "LowBDT_T2bw050_6",  "LowBDT_T2bw025_1",  "LowBDT_T2bw025_3",  "LowBDT_T2bw025_4",  "LowBDT_T2bw025_6"};
  vector<string> signalRegions_MTpeak = {"LowBDT_MTpeak_T2tt_1",  "LowBDT_MTpeak_T2tt_2",  "LowBDT_MTpeak_T2tt_5",  "LowBDT_MTpeak_T2bw075_1",  "LowBDT_MTpeak_T2bw075_2",  "LowBDT_MTpeak_T2bw075_3",  "LowBDT_MTpeak_T2bw075_5",  "LowBDT_MTpeak_T2bw050_1",  "LowBDT_MTpeak_T2bw050_3",  "LowBDT_MTpeak_T2bw050_4",  "LowBDT_MTpeak_T2bw050_5",  "LowBDT_MTpeak_T2bw050_6",  "LowBDT_MTpeak_T2bw025_1",  "LowBDT_MTpeak_T2bw025_3",  "LowBDT_MTpeak_T2bw025_4",  "LowBDT_MTpeak_T2bw025_6"};
  vector<string> signalRegions_MTpeak_NoBtag = {"LowBDT_MTPeakNoBtag_T2tt_1",  "LowBDT_MTPeakNoBtag_T2tt_2",  "LowBDT_MTPeakNoBtag_T2tt_5",  "LowBDT_MTPeakNoBtag_T2bw075_1",  "LowBDT_MTPeakNoBtag_T2bw075_2",  "LowBDT_MTPeakNoBtag_T2bw075_3",  "LowBDT_MTPeakNoBtag_T2bw075_5",  "LowBDT_MTPeakNoBtag_T2bw050_1",  "LowBDT_MTPeakNoBtag_T2bw050_3",  "LowBDT_MTPeakNoBtag_T2bw050_4",  "LowBDT_MTPeakNoBtag_T2bw050_5",  "LowBDT_MTPeakNoBtag_T2bw050_6",  "LowBDT_MTPeakNoBtag_T2bw025_1",  "LowBDT_MTPeakNoBtag_T2bw025_3",  "LowBDT_MTPeakNoBtag_T2bw025_4",  "LowBDT_MTPeakNoBtag_T2bw025_6"};
  vector<string> signalRegions_MTpeak_OneBtag = {"LowBDT_MTPeakOneBtag_T2tt_1",  "LowBDT_MTPeakOneBtag_T2tt_2",  "LowBDT_MTPeakOneBtag_T2tt_5",  "LowBDT_MTPeakOneBtag_T2bw075_1",  "LowBDT_MTPeakOneBtag_T2bw075_2",  "LowBDT_MTPeakOneBtag_T2bw075_3",  "LowBDT_MTPeakOneBtag_T2bw075_5",  "LowBDT_MTPeakOneBtag_T2bw050_1",  "LowBDT_MTPeakOneBtag_T2bw050_3",  "LowBDT_MTPeakOneBtag_T2bw050_4",  "LowBDT_MTPeakOneBtag_T2bw050_5",  "LowBDT_MTPeakOneBtag_T2bw050_6",  "LowBDT_MTPeakOneBtag_T2bw025_1",  "LowBDT_MTPeakOneBtag_T2bw025_3",  "LowBDT_MTPeakOneBtag_T2bw025_4",  "LowBDT_MTPeakOneBtag_T2bw025_6"};
  
  //Create histos
  TH1F h_SF_MTpeak_BDT_tt1l("h_SF_MTpeak_BDT_tt1l","",signalRegions.size(),0,signalRegions.size());
  TH1F h_SF_MTtail_BDT_tt1l("h_SF_MTtail_BDT_tt1l","",signalRegions.size(),0,signalRegions.size());
  TH1F h_SFR_BDT_tt1l("h_SFR_BDT_tt1l","",signalRegions.size(),0,signalRegions.size());
  TH1F h_SF_MTpeak_BDT_wjets("h_SF_MTpeak_BDT_wjets","",signalRegions.size(),0,signalRegions.size());
  TH1F h_SF_MTtail_BDT_wjets("h_SF_MTtail_BDT_wjets","",signalRegions.size(),0,signalRegions.size());
  TH1F h_SFR_BDT_wjets("h_SFR_BDT_wjets","",signalRegions.size(),0,signalRegions.size());
   
  varname="Mlb";
  
  float mean_SFtt1l = 0;
  float rms_SFtt1l = 0;
  float MaxStatUncert_SFtt1l = 0;

  float mean_SFwjets = 0;
  float rms_SFwjets = 0;
  float MaxStatUncert_SFwjets = 0;

  for(unsigned int i=0;i<signalRegions.size();i++){
  	cout<<"%%%%%%%%%%%%%%%%%% "<<signalRegions[i]<<endl;

	string label = signalRegions[i].substr(7);
	theDoctor::Figure SFR_tt1l;
	theDoctor::Figure SFR_wjets;
	
	//MT tail
	setup.Reset(); conditions="sigRegions_tail"; setup.region=signalRegions[i]; uncert.name = conditions; setup.varname=varname; setup.varMin=0; setup.varMax=600;
  	res = doFit(setup,conditions); 
	SFR_tt1l=theDoctor::Figure(res.SF_tt1l.first,res.SF_tt1l.second);
	SFR_wjets=theDoctor::Figure(res.SF_wjets.first,res.SF_wjets.second);
	h_SF_MTtail_BDT_tt1l.SetBinContent(i+1,res.SF_tt1l.first);
	h_SF_MTtail_BDT_tt1l.SetBinError(i+1,res.SF_tt1l.second);
	h_SF_MTtail_BDT_tt1l.GetXaxis()->SetBinLabel(i+1,label.c_str());
	h_SF_MTtail_BDT_wjets.SetBinContent(i+1,res.SF_wjets.first);
	h_SF_MTtail_BDT_wjets.SetBinError(i+1,res.SF_wjets.second);
	h_SF_MTtail_BDT_wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());
        
	//MT peak
	setup.Reset(); conditions="sigRegions_peak"; setup.region=signalRegions_MTpeak[i]; uncert.name = conditions; setup.varname=varname; setup.varMin=0; setup.varMax=600;
  	res = doFit(setup,conditions); 
	h_SF_MTpeak_BDT_tt1l.SetBinContent(i+1,res.SF_tt1l.first);
	h_SF_MTpeak_BDT_tt1l.SetBinError(i+1,res.SF_tt1l.second);
	h_SF_MTpeak_BDT_tt1l.GetXaxis()->SetBinLabel(i+1,label.c_str());
	h_SF_MTpeak_BDT_wjets.SetBinContent(i+1,res.SF_wjets.first);
	h_SF_MTpeak_BDT_wjets.SetBinError(i+1,res.SF_wjets.second);
	h_SF_MTpeak_BDT_wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());
  	
        //Now compute the ration : SF_tail/SF_peak
	SFR_tt1l/=theDoctor::Figure(res.SF_tt1l.first,res.SF_tt1l.second);
        SFR_wjets/=theDoctor::Figure(res.SF_wjets.first,res.SF_wjets.second);

        //-- do some additionnal test as function of the b-tag multiplicity
	
	//MT peak (no btag req)
	setup.Reset(); conditions="sigRegions_peak_NoBtag"; setup.region=signalRegions_MTpeak_NoBtag[i]; uncert.name = conditions; setup.varname=varname; setup.varMin=0; setup.varMax=600;
  	res = doFit(setup,conditions); 
	
	//MT peak (one btag req)
	setup.Reset(); conditions="sigRegions_peak_OneBtag"; setup.region=signalRegions_MTpeak_OneBtag[i]; uncert.name = conditions; setup.varname=varname; setup.varMin=0; setup.varMax=600;
  	res = doFit(setup,conditions); 
        
	//-- End of additionnal tests

	//Computation of mean/rms/ ..
	//It is based on  the ratio SF_tail/SF_peak
	//-- tt1l
	mean_SFtt1l+=SFR_tt1l.value();
	rms_SFtt1l+=(SFR_tt1l.value()*SFR_tt1l.value());
	if(SFR_tt1l.error()>(MaxStatUncert_SFtt1l/SFR_tt1l.value()))  MaxStatUncert_SFtt1l=SFR_tt1l.error()/SFR_tt1l.value();
	//-- W+jets
	mean_SFwjets+=SFR_wjets.value();
	rms_SFwjets+=(SFR_wjets.value()*SFR_wjets.value());
	if(SFR_wjets.error()>(MaxStatUncert_SFwjets/SFR_wjets.value()))  MaxStatUncert_SFwjets=SFR_wjets.error()/SFR_wjets.value();
        //-----------------------------------

	
	
	//SFR
	h_SFR_BDT_tt1l.SetBinContent(i+1,SFR_tt1l.value());
	h_SFR_BDT_tt1l.SetBinError(i+1,SFR_tt1l.error());
	h_SFR_BDT_tt1l.GetXaxis()->SetBinLabel(i+1,label.c_str());
	h_SFR_BDT_wjets.SetBinContent(i+1,SFR_wjets.value());
	h_SFR_BDT_wjets.SetBinError(i+1,SFR_wjets.error());
	h_SFR_BDT_wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());
  
  	
	cout<<"signRegions: SFR_tt1l: "<<SFR_tt1l.Print()<<" SFR_wjets: "<<SFR_wjets.Print()<<endl;
  }
  //Save plots in roofile
  TFile fCR1_BDT("CR1_BDT.root","RECREATE");
  h_SF_MTpeak_BDT_tt1l.Write();
  h_SF_MTpeak_BDT_wjets.Write();
  h_SF_MTtail_BDT_tt1l.Write();
  h_SF_MTtail_BDT_wjets.Write();
  h_SFR_BDT_tt1l.Write();
  h_SFR_BDT_wjets.Write();
  
  //---------------------------------------//
  mean_SFtt1l/=signalRegions.size();
  rms_SFtt1l/=signalRegions.size();
  rms_SFtt1l-=(mean_SFtt1l*mean_SFtt1l);
  theDoctor::Figure BDT_SFtt1l(mean_SFtt1l,sqrt(rms_SFtt1l+MaxStatUncert_SFtt1l*MaxStatUncert_SFtt1l*mean_SFtt1l*mean_SFtt1l));
  //---------------------------------------//
  mean_SFwjets/=signalRegions.size();
  rms_SFwjets/=signalRegions.size();
  rms_SFwjets-=(mean_SFwjets*mean_SFwjets);
  theDoctor::Figure BDT_SFwjets(mean_SFwjets,sqrt(rms_SFwjets+MaxStatUncert_SFwjets*MaxStatUncert_SFwjets*mean_SFwjets*mean_SFwjets));
  //---------------------------------------//
  
  
  vector<string> sigReglabels(signalRegions.size());
  for(unsigned int i=0;i<signalRegions.size();i++){
  	sigReglabels[i] = signalRegions[i].substr(3);
  } 
  vector<string> columns = {"SFR_tt1l","SFR_wjets"};
  theDoctor::Table SFR_BDT(columns,sigReglabels,columns,sigReglabels);


  for(unsigned int i=0;i<signalRegions.size();i++){
  	SFR_BDT.Set("SFR_tt1l",sigReglabels[i],BDT_SFtt1l);
  	SFR_BDT.Set("SFR_wjets",sigReglabels[i],BDT_SFwjets);
  }
  SFR_BDT.Print("scaleFactors/SFR_BDT.tab",4);
  //---------------------------------------//

  vector<string> signalRegions_CC = {"CR0btag_MTtail_MT_120","CR0btag_MTtail_MT_125", "CR0btag_MTtail_MT_130","CR0btag_MTtail_MT_140", "CR0btag_MTtail_MT_150", "CR0btag_MTtail_MT_160","CR0btag_MTtail_MET_200"          ,  "CR0btag_MTtail_MET_250"          ,  "CR0btag_MTtail_MET_300"          ,  "CR0btag_MTtail_MET_320"          ,  "CR0btag_MTtail_METoverSqrtHT_6" ,  "CR0btag_MTtail_METoverSqrtHT_7" ,  "CR0btag_MTtail_METoverSqrtHT_8" ,  "CR0btag_MTtail_METoverSqrtHT_9" ,  "CR0btag_MTtail_METoverSqrtHT_10",  "CR0btag_MTtail_METoverSqrtHT_12",  "CR0btag_MTtail_BPt_100"         , "CR0btag_MTtail_BPt_150",  "CR0btag_MTtail_BPt_180"          ,  "CR0btag_MTtail_DPhi_02"          ,  "CR0btag_MTtail_DPhi_08"          ,  "CR0btag_MTtail_ISRJet"          ,  "CR0btag_MTtail_MT2W_180"         ,  "CR0btag_MTtail_MT2W_190"         };//,  "CR0btag_MTtail_MT2W_200"};
  vector<string> signalRegions_CC_MTpeak = {"0btag_MTpeak", "0btag_MTpeak",  "0btag_MTpeak",   "0btag_MTpeak",  "0btag_MTpeak",  "0btag_MTpeak", "CR0btag_MTpeak_MET_200"          ,  "CR0btag_MTpeak_MET_250"          ,  "CR0btag_MTpeak_MET_300"          ,  "CR0btag_MTpeak_MET_320"          ,  "CR0btag_MTpeak_METoverSqrtHT_6" ,  "CR0btag_MTpeak_METoverSqrtHT_7" ,  "CR0btag_MTpeak_METoverSqrtHT_8" ,  "CR0btag_MTpeak_METoverSqrtHT_9" ,  "CR0btag_MTpeak_METoverSqrtHT_10",  "CR0btag_MTpeak_METoverSqrtHT_12",  "CR0btag_MTpeak_BPt_100"         ,  "CR0btag_MTtail_BPt_150", "CR0btag_MTpeak_BPt_180"          ,  "CR0btag_MTpeak_DPhi_02"          ,  "CR0btag_MTpeak_DPhi_08"          ,  "CR0btag_MTpeak_ISRJet"          ,  "CR0btag_MTpeak_MT2W_180"         ,  "CR0btag_MTpeak_MT2W_190"         };//,  "CR0btag_MTpeak_MT2W_200"};
  
  std::map<string,vector<string> > CC_Cuts;
  CC_Cuts["cutAndCount_T2tt_offShellLoose"]    		= {"MT_125"};
  CC_Cuts["cutAndCount_T2tt_offShellTight"]   		= {"MT_130","MET_300"};
  CC_Cuts["cutAndCount_T2tt_lowDeltaM"]    			= {"MT_140","METoverSqrtHT_8"};
  CC_Cuts["cutAndCount_T2tt_mediumDeltaM"] 			= {"MT_140","MET_200"};
  CC_Cuts["cutAndCount_T2tt_highDeltaM"] 			= {"MT_130","MET_350"};
  CC_Cuts["cutAndCount_T2bw025_veryOffShell_loose"] 	= {"MT_120","METoverSqrtHT_9"};
  CC_Cuts["cutAndCount_T2bw025_offShell_looSS"] 		= {"MT_120","METoverSqrtHT_7"};
  CC_Cuts["cutAndCount_T2bw025_lowDeltaM_tight"] 		= {"MT_120","METoverSqrtHT_6"};
  CC_Cuts["cutAndCount_T2bw025_highDeltaM"] 		= {"MT_140","METoverSqrtHT_10"};
  CC_Cuts["cutAndCount_T2bw050_offShell_loose"] 		= {"MT_120","METoverSqrtHT_9"};
  CC_Cuts["cutAndCount_T2bw050_lowMass"] 			= {"MT_120","METoverSqrtHT_6"};
  CC_Cuts["cutAndCount_T2bw050_mediumDeltaM_loose"] 	= {"MT_150","METoverSqrtHT_7"};
  CC_Cuts["cutAndCount_T2bw050_highDeltaM"] 	= {"MT_160","METoverSqrtHT_10"};
  CC_Cuts["cutAndCount_T2bw075_lowDeltaM_tight"] 		= {"MT_120","METoverSqrtHT_12"};
  CC_Cuts["cutAndCount_T2bw075_mediumDeltaM"] 		= {"MT_120","METoverSqrtHT_10"};
  CC_Cuts["cutAndCount_T2bw075_highDeltaM"] 		= {"MT_140","MET_320"};

/*
bool cutAndCount_T2tt_offShellLoose(bool applyMTCut) { return cutAndCount_T2tt( -1,  8,  125 * applyMTCut, -1,   -1, 999999, true ); }
bool cutAndCount_T2tt_offShellTight(bool applyMTCut) { return cutAndCount_T2tt( 300, -1, 130 * applyMTCut, -1,   -1, 999999, true ); }
bool cutAndCount_T2tt_lowDeltaM    (bool applyMTCut) { return cutAndCount_T2tt( -1,  8,  140 * applyMTCut, -1,  0.8, 5,      false); }
bool cutAndCount_T2tt_mediumDeltaM (bool applyMTCut) { return cutAndCount_T2tt( 200, -1, 140 * applyMTCut, 180, 0.8, 3,      false); }
bool cutAndCount_T2tt_highDeltaM   (bool applyMTCut) { return cutAndCount_T2tt( 350, -1, 130 * applyMTCut, 190,  -1, 999999, false); }

bool cutAndCount_T2bw025_veryOffShell_loose(bool applyMTCut) { return cutAndCount_T2bw(-1,   9,    120 * applyMTCut, -1,  -1,  0.2, true ); }
bool cutAndCount_T2bw025_offShell_loose(bool applyMTCut)     { return cutAndCount_T2bw(-1,   7,    120 * applyMTCut, 200, 150, 0.8, false); }
bool cutAndCount_T2bw025_lowDeltaM_tight(bool applyMTCut)    { return cutAndCount_T2bw(-1,   6,    120 * applyMTCut, 200, 180, 0.8, false); }
bool cutAndCount_T2bw025_highDeltaM(bool applyMTCut)         { return cutAndCount_T2bw(-1,  10,    140 * applyMTCut, 200, 180, 0.8, false); }

                                                                                    // MET METsig   MT                MT2W BPt dPhi ISRjet
bool cutAndCount_T2bw050_offShell_loose(bool applyMTCut)     { return cutAndCount_T2bw(-1,   9,   120 * applyMTCut, -1,  -1, 0.2, true ); }
bool cutAndCount_T2bw050_lowMass(bool applyMTCut)            { return cutAndCount_T2bw(-1,   6,   120 * applyMTCut, 200,100, 0.8, false); }
bool cutAndCount_T2bw050_mediumDeltaM_loose(bool applyMTCut) { return cutAndCount_T2bw(-1,   7,   150 * applyMTCut, 200,150, 0.8, false); }
bool cutAndCount_T2bw050_highDeltaM(bool applyMTCut)         { return cutAndCount_T2bw(-1,  10,   160 * applyMTCut, 200,180, 0.8, false); }

                                                                                    // MET METsig   MT                MT2W BPt dPhi ISRjet
bool cutAndCount_T2bw075_lowDeltaM_tight(bool applyMTCut)    { return cutAndCount_T2bw(-1,  12,    120 * applyMTCut, -1,  -1, 0.8, true);  }
bool cutAndCount_T2bw075_mediumDeltaM(bool applyMTCut)       { return cutAndCount_T2bw(-1,  10,    140 * applyMTCut, 180, -1, 0.8, false); }
bool cutAndCount_T2bw075_highDeltaM(bool applyMTCut)         { return cutAndCount_T2bw(320, -1,    160 * applyMTCut, 200, -1, 0.8, false); }
*/

  std::map<string,theDoctor::Figure> SFR_CC_tt1l_map;
  std::map<string,theDoctor::Figure> SFR_CC_wjets_map;

  //Create histos
  TH1F h_SF_MTpeak_CC_tt1l("h_SF_MTpeak_CC_tt1l","",signalRegions_CC.size(),0,signalRegions_CC.size());
  TH1F h_SF_MTtail_CC_tt1l("h_SF_MTtail_CC_tt1l","",signalRegions_CC.size(),0,signalRegions_CC.size());
  TH1F h_SFR_CC_tt1l("h_SFR_CC_tt1l","",signalRegions_CC.size(),0,signalRegions_CC.size());
  TH1F h_SF_MTpeak_CC_wjets("h_SF_MTpeak_CC_wjets","",signalRegions_CC.size(),0,signalRegions_CC.size());
  TH1F h_SF_MTtail_CC_wjets("h_SF_MTtail_CC_wjets","",signalRegions_CC.size(),0,signalRegions_CC.size());
  TH1F h_SFR_CC_wjets("h_SFR_CC_wjets","",signalRegions_CC.size(),0,signalRegions_CC.size());
  
  for(unsigned int i=0;i<signalRegions_CC.size();i++){
  	cout<<"%%%%%%%%%%%%%%%%%% "<<signalRegions_CC[i]<<endl;

	string label = signalRegions_CC[i].substr(15);
	theDoctor::Figure SFR_tt1l;
	theDoctor::Figure SFR_wjets;
	
	//MT tail
	setup.Reset(); conditions="sigRegions_CC_tail"; setup.region=signalRegions_CC[i]; uncert.name = conditions; setup.varname=varname; setup.varMin=0; setup.varMax=600;
  	res = doFit(setup,conditions); 
	SFR_tt1l=theDoctor::Figure(res.SF_tt1l.first,res.SF_tt1l.second);
	SFR_wjets=theDoctor::Figure(res.SF_wjets.first,res.SF_wjets.second);
	h_SF_MTtail_CC_tt1l.SetBinContent(i+1,res.SF_tt1l.first);
	h_SF_MTtail_CC_tt1l.SetBinError(i+1,res.SF_tt1l.second);
	h_SF_MTtail_CC_tt1l.GetXaxis()->SetBinLabel(i+1,label.c_str());
	h_SF_MTtail_CC_wjets.SetBinContent(i+1,res.SF_wjets.first);
	h_SF_MTtail_CC_wjets.SetBinError(i+1,res.SF_wjets.second);
	h_SF_MTtail_CC_wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());
        
	//MT peak
	setup.Reset(); conditions="sigRegions_CC_peak"; setup.region=signalRegions_CC_MTpeak[i]; uncert.name = conditions; setup.varname=varname; setup.varMin=0; setup.varMax=600;
  	res = doFit(setup,conditions); 
	h_SF_MTpeak_CC_tt1l.SetBinContent(i+1,res.SF_tt1l.first);
	h_SF_MTpeak_CC_tt1l.SetBinError(i+1,res.SF_tt1l.second);
	h_SF_MTpeak_CC_tt1l.GetXaxis()->SetBinLabel(i+1,label.c_str());
	h_SF_MTpeak_CC_wjets.SetBinContent(i+1,res.SF_wjets.first);
	h_SF_MTpeak_CC_wjets.SetBinError(i+1,res.SF_wjets.second);
	h_SF_MTpeak_CC_wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());
  	
	//MT peak (no btag req)
	//setup.Reset(); conditions="sigRegions_peak_NoBtag"; setup.region=signalRegions_MTpeak_NoBtag[i]; uncert.name = conditions; setup.varname=varname; setup.varMin=0; setup.varMax=600;
  	//res = doFit(setup,conditions); 

        //Now compute the ration : SF_tail/SF_peak
	SFR_tt1l/=theDoctor::Figure(res.SF_tt1l.first,res.SF_tt1l.second);
        SFR_wjets/=theDoctor::Figure(res.SF_wjets.first,res.SF_wjets.second);

	//It is based on the ratio SF_tail/SF_peak
	SFR_CC_tt1l_map[label] = SFR_tt1l;
	SFR_CC_wjets_map[label] = SFR_wjets;
        
	

	//SFR
	h_SFR_CC_tt1l.SetBinContent(i+1,SFR_tt1l.value());
	h_SFR_CC_tt1l.SetBinError(i+1,SFR_tt1l.error());
	h_SFR_CC_tt1l.GetXaxis()->SetBinLabel(i+1,label.c_str());
	h_SFR_CC_wjets.SetBinContent(i+1,SFR_wjets.value());
	h_SFR_CC_wjets.SetBinError(i+1,SFR_wjets.error());
	h_SFR_CC_wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());
  
  	
	cout<<"signRegions (CC): SFR_tt1l: "<<SFR_tt1l.Print()<<" SFR_wjets: "<<SFR_wjets.Print()<<endl;
  }

  //Save plots in roofile
  TFile fCR1_CC("CR1_CC.root","RECREATE");
  h_SF_MTpeak_CC_tt1l.Write();
  h_SF_MTpeak_CC_wjets.Write();
  h_SF_MTtail_CC_tt1l.Write();
  h_SF_MTtail_CC_wjets.Write();
  h_SFR_CC_tt1l.Write();
  h_SFR_CC_wjets.Write();

  //---------------------------------------------
  // Results for C&C
  vector<string> listOfCCRegions = {
  "cutAndCount_T2tt_offShellLoose",    	        
  "cutAndCount_T2tt_offShellTight",   		
  "cutAndCount_T2tt_lowDeltaM",    			
  "cutAndCount_T2tt_mediumDeltaM", 			
  "cutAndCount_T2tt_highDeltaM", 			
  "cutAndCount_T2bw025_veryOffShell_loose", 	
  "cutAndCount_T2bw025_offShell_looSS", 		
  "cutAndCount_T2bw025_lowDeltaM_tight", 		
  "cutAndCount_T2bw025_highDeltaM", 		
  "cutAndCount_T2bw050_offShell_loose", 		
  "cutAndCount_T2bw050_lowMass", 			
  "cutAndCount_T2bw050_mediumDeltaM_loose", 	
  "cutAndCount_T2bw050_highDeltaM", 	
  "cutAndCount_T2bw075_lowDeltaM_tight", 		
  "cutAndCount_T2bw075_mediumDeltaM" 		
  };
  vector<string> cuts;

  theDoctor::Table SFR_CC(columns,listOfCCRegions,columns,listOfCCRegions);
  for(unsigned int r=0;r<listOfCCRegions.size();r++){
  	cuts = CC_Cuts[listOfCCRegions[r]];	
 	theDoctor::Figure SFR_CC_tt1l;
 	theDoctor::Figure SFR_CC_wjets;
  	for(unsigned i=0;i<cuts.size();i++)
  	{
  		cout<<cuts[i]<<" "<<SFR_CC_tt1l_map[cuts[i]].Print()<<endl;
		if(i==0) SFR_CC_tt1l = SFR_CC_tt1l_map[cuts[i]];
  		else SFR_CC_tt1l = theDoctor::Figure(SFR_CC_tt1l.value(),sqrt(pow(SFR_CC_tt1l.error(),2)+pow(SFR_CC_tt1l.value()-SFR_CC_tt1l_map[cuts[i].c_str()].value(),2)));
		
		if(i==0) SFR_CC_wjets = SFR_CC_wjets_map[cuts[i]];
  		else SFR_CC_wjets = theDoctor::Figure(SFR_CC_wjets.value(),sqrt(pow(SFR_CC_wjets.error(),2)+pow(SFR_CC_wjets.value()-SFR_CC_wjets_map[cuts[i].c_str()].value(),2)));
	}
  	SFR_CC.Set("SFR_tt1l",listOfCCRegions[r],SFR_CC_tt1l);
  	SFR_CC.Set("SFR_wjets",listOfCCRegions[r],SFR_CC_wjets);
  	cout<<"TOO "<<listOfCCRegions[r]<<" "<<SFR_CC_tt1l.Print()<<endl;
  }


  SFR_CC.Print("scaleFactors/SFR_CC.tab",4);
  //---------------------------------------//
  //---------------------------------------------
  


  //---------------------------------------------
  //  Perform an estimation for SFR C&C
  //---------------------------------------------

  vector<string> signalRegionsEstim_CC = {
  "CR0bag_MTtail_T2tt_offShellLoose",         "CR0bag_MTtail_T2tt_offShellTight",         "CR0bag_MTtail_T2tt_lowDeltaM",             "CR0bag_MTtail_T2tt_mediumDeltaM",          "CR0bag_MTtail_T2tt_highDeltaM",            
  "CR0bag_MTtail_T2bw025_veryOffShell_loose", "CR0bag_MTtail_T2bw025_offShell_loose",     "CR0bag_MTtail_T2bw025_lowDeltaM_tight",    "CR0bag_MTtail_T2bw025_highDeltaM",         "CR0bag_MTtail_T2bw050_offShell_loose",     "CR0bag_MTtail_T2bw050_lowMass",            "CR0bag_MTtail_T2bw050_mediumDeltaM_loose", "CR0bag_MTtail_T2bw050_highDeltaM",         "CR0bag_MTtail_T2bw075_lowDeltaM_tight",    "CR0bag_MTtail_T2bw075_mediumDeltaM",       "CR0bag_MTtail_T2bw075_highDeltaM"};

  vector<string> signalRegionsEstim_CC_MTpeak = {
  "CR0bag_MTpeak_T2tt_offShellLoose",         "CR0bag_MTpeak_T2tt_offShellTight",         "CR0bag_MTpeak_T2tt_lowDeltaM",             "CR0bag_MTpeak_T2tt_mediumDeltaM",          "CR0bag_MTpeak_T2tt_highDeltaM",            
  "CR0bag_MTpeak_T2bw025_veryOffShell_loose", "CR0bag_MTpeak_T2bw025_offShell_loose",     "CR0bag_MTpeak_T2bw025_lowDeltaM_tight",    "CR0bag_MTpeak_T2bw025_highDeltaM",         "CR0bag_MTpeak_T2bw050_offShell_loose",     "CR0bag_MTpeak_T2bw050_lowMass",            "CR0bag_MTpeak_T2bw050_mediumDeltaM_loose", "CR0bag_MTpeak_T2bw050_highDeltaM",         "CR0bag_MTpeak_T2bw075_lowDeltaM_tight",    "CR0bag_MTpeak_T2bw075_mediumDeltaM",       "CR0bag_MTpeak_T2bw075_highDeltaM"};


  //Create histos
  TH1F h_SF_MTpeakEstim_CC_tt1l("h_SF_MTpeakEstim_CC_tt1l","",signalRegionsEstim_CC.size(),0,signalRegionsEstim_CC.size());
  TH1F h_SF_MTtailEstim_CC_tt1l("h_SF_MTtailEstim_CC_tt1l","",signalRegionsEstim_CC.size(),0,signalRegionsEstim_CC.size());
  TH1F h_SFREstim_CC_tt1l("h_SFREstim_CC_tt1l","",signalRegionsEstim_CC.size(),0,signalRegionsEstim_CC.size());
  TH1F h_SF_MTpeakEstim_CC_wjets("h_SF_MTpeakEstim_CC_wjets","",signalRegionsEstim_CC.size(),0,signalRegionsEstim_CC.size());
  TH1F h_SF_MTtailEstim_CC_wjets("h_SF_MTtailEstim_CC_wjets","",signalRegionsEstim_CC.size(),0,signalRegionsEstim_CC.size());
  TH1F h_SFREstim_CC_wjets("h_SFREstim_CC_wjets","",signalRegionsEstim_CC.size(),0,signalRegionsEstim_CC.size());
  
  for(unsigned int i=0;i<signalRegionsEstim_CC.size();i++){
  	cout<<"%%%%%%%%%%%%%%%%%% "<<signalRegionsEstim_CC[i]<<endl;

	string label = signalRegionsEstim_CC[i].substr(15);
	theDoctor::Figure SFR_tt1l;
	theDoctor::Figure SFR_wjets;
	
	//MT tail
	setup.Reset(); conditions="sigRegionsEstim_CC_tail"; setup.region=signalRegionsEstim_CC[i]; uncert.name = conditions; setup.varname=varname; setup.varMin=0; setup.varMax=600;
        setup.varname="M3b_small";
  	res = doFit(setup,conditions); 
	SFR_tt1l=theDoctor::Figure(res.SF_tt1l.first,res.SF_tt1l.second);
	SFR_wjets=theDoctor::Figure(res.SF_wjets.first,res.SF_wjets.second);
	h_SF_MTtailEstim_CC_tt1l.SetBinContent(i+1,res.SF_tt1l.first);
	h_SF_MTtailEstim_CC_tt1l.SetBinError(i+1,res.SF_tt1l.second);
	h_SF_MTtailEstim_CC_tt1l.GetXaxis()->SetBinLabel(i+1,label.c_str());
	h_SF_MTtailEstim_CC_wjets.SetBinContent(i+1,res.SF_wjets.first);
	h_SF_MTtailEstim_CC_wjets.SetBinError(i+1,res.SF_wjets.second);
	h_SF_MTtailEstim_CC_wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());
        
	//MT peak
	setup.Reset(); conditions="sigRegionsEstim_CC_peak"; setup.region=signalRegionsEstim_CC_MTpeak[i]; uncert.name = conditions; setup.varname=varname; setup.varMin=0; setup.varMax=600;
        setup.varname="M3b_small";
  	res = doFit(setup,conditions); 
	h_SF_MTpeakEstim_CC_tt1l.SetBinContent(i+1,res.SF_tt1l.first);
	h_SF_MTpeakEstim_CC_tt1l.SetBinError(i+1,res.SF_tt1l.second);
	h_SF_MTpeakEstim_CC_tt1l.GetXaxis()->SetBinLabel(i+1,label.c_str());
	h_SF_MTpeakEstim_CC_wjets.SetBinContent(i+1,res.SF_wjets.first);
	h_SF_MTpeakEstim_CC_wjets.SetBinError(i+1,res.SF_wjets.second);
	h_SF_MTpeakEstim_CC_wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());
        SFR_tt1l/=theDoctor::Figure(res.SF_tt1l.first,res.SF_tt1l.second);
        SFR_wjets/=theDoctor::Figure(res.SF_wjets.first,res.SF_tt1l.second);
  	
	//MT peak (no btag req)
	//setup.Reset(); conditions="sigRegions_peak_NoBtag"; setup.region=signalRegions_MTpeak_NoBtag[i]; uncert.name = conditions; setup.varname=varname; setup.varMin=0; setup.varMax=600;
  	//res = doFit(setup,conditions); 

	//SFR
	h_SFREstim_CC_tt1l.SetBinContent(i+1,SFR_tt1l.value());
	h_SFREstim_CC_tt1l.SetBinError(i+1,SFR_tt1l.error());
	h_SFREstim_CC_tt1l.GetXaxis()->SetBinLabel(i+1,label.c_str());
	h_SFREstim_CC_wjets.SetBinContent(i+1,SFR_wjets.value());
	h_SFREstim_CC_wjets.SetBinError(i+1,SFR_wjets.error());
	h_SFREstim_CC_wjets.GetXaxis()->SetBinLabel(i+1,label.c_str());
  
  	
	cout<<"signRegions (CC) - "<<signalRegionsEstim_CC_MTpeak[i]<<": SFR_tt1l: "<<SFR_tt1l.Print()<<" SFR_wjets: "<<SFR_wjets.Print()<<endl;
  }
  //Save plots in roofile
  TFile fCR1Estim_CC("CR1Estim_CC.root","RECREATE");
  h_SF_MTpeakEstim_CC_tt1l.Write();
  h_SF_MTpeakEstim_CC_wjets.Write();
  h_SF_MTtailEstim_CC_tt1l.Write();
  h_SF_MTtailEstim_CC_wjets.Write();
  h_SFREstim_CC_tt1l.Write();
  h_SFREstim_CC_wjets.Write();




  //---------------------------------------//
  //-- split region --//,   /* 
  /* 
  setup.Reset(); conditions = "split-region";
  setup.region = "0btagLD_MTtail";
  res = doFit(setup,conditions); 
  setup.region = "0btagHD_MTtail";
  res = doFit(setup,conditions); 
  
 

  //---  choice of MT test cut --//
  setup.Reset(); setup.region = "0btag_MT80"; conditions="MT80"; 
  res = doFit(setup,conditions); 
  setup.Reset(); setup.region = "0btag_MT90"; conditions="MT90"; 
  res = doFit(setup,conditions); 
  setup.Reset(); setup.region = "0btag_MT110"; conditions="M110"; 
  res = doFit(setup,conditions); 
  setup.Reset(); setup.region = "0btag_MT120"; conditions="MT120"; 
  res = doFit(setup,conditions); 
  setup.Reset(); setup.region = "0btag_MT130"; conditions="MT130"; 
  res = doFit(setup,conditions); 
  //setup.varname="M3b_modif";//test with that var ! -- does not work .. 
  res = doFit(setup,conditions); 
  setup.Reset(); setup.region = "0btag_MT140"; conditions="MT140"; setup.varname = "M3b_binningdown";
  setup.type="GSLMultiMin"; setup.algo="steepestdescent";
  setup.type="GSLMultiMin"; setup.algo="conjugatefr";
  res = doFit(setup,conditions); 
  setup.Reset(); setup.region = "0btag_MT160"; conditions="MT160"; setup.varname = "M3b_binningdown";
  setup.type="GSLMultiMin"; setup.algo="steepestdescent";
  res = doFit(setup,conditions); 
  */
  //---- choice of MT test cut with Mlb ---//
  /*
  setup.Reset(); setup.region = "0btag_MT80"; conditions="MT80"; setup.varname="Mlb"; 
  res = doFit(setup,conditions); 
  setup.Reset(); setup.region = "0btag_MT90"; conditions="MT90"; setup.varname="Mlb";
  res = doFit(setup,conditions); 
  setup.Reset(); setup.region = "0btag_MTtail"; conditions="MT100"; setup.varname="Mlb";
  res = doFit(setup,conditions); 
  setup.Reset(); setup.region = "0btag_MT110"; conditions="M110"; setup.varname="Mlb";
  res = doFit(setup,conditions); 
  setup.Reset(); setup.region = "0btag_MT120"; conditions="MT120"; setup.varname="Mlb";
  res = doFit(setup,conditions); 
  setup.Reset(); setup.region = "0btag_MT130"; conditions="MT130"; setup.varname="Mlb";
  res = doFit(setup,conditions); 
  */
  
  
  //--- Perform Chi2 test for several variables and CR --//
  if(bChi2Tests){
  string region="0btag_MTpeak";
  cout<<"#### Chi2 test - "<<region<<endl;
  Chi2Test(fin,region,"M3b");
  Chi2Test(fin,region,"Mlb");
  region="0btag_MTtail";
  Chi2Test(fin,region,"Mlb");
  region="presel_MTpeak";
  cout<<"#### Chi2 test - "<<region<<endl;
  Chi2Test(fin,region,"M3b");
  Chi2Test(fin,region,"Mlb");
  region="presel_noMET";
  cout<<"#### Chi2 test - "<<region<<endl;
  Chi2Test(fin,region,"M3b");
  Chi2Test(fin,region,"Mlb");
  Chi2Test(fin,region,"Mlb");
  region="presel_MTinverted";
  cout<<"#### Chi2 test -presel_MTinverted-  M3b"<<region<<endl;
  Chi2Test(fin,region,"M3b");
  Chi2Test(fin,region,"Mlb");
  }


  summary.Print();
}  

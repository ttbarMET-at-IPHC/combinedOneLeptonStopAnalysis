#ifndef TOPNESS_H
#define TOPNESS_H

#include "TFitter.h"
#include "TMath.h"
#include <vector>
#include "Math/LorentzVector.h"
#include <math.h>
#include "TLorentzVector.h"
#include <sstream>
#include <fstream>

using namespace std;

double round(double input, int digits){
    return ceil( ( input * pow( 10,digits ) ) - 0.5 ) / pow( 10,digits );
}

struct topness_param{
  double mW;		// mean W mass
  double mT;		// mean top mass
  double aW;		// unccertainty on W (l+nu)
  double aT;		// uncerainty on top (b+l+nu)
  double aTm;		// uncertainty related to the constraint on the top where a W is missing
  double aCM;		// uncertainty trelated to constraint on energy total of ttbar
  double aCM_mean;	// mean value of the total ttbar energy
  bool use_cox_box; 	// help to transform aCM constrain into a gaussian constrain
  bool usePtPz;		// change CM term (use pt, pz instead of Mass);
  double aPt;		// uncertainty on pt(ttbar) term
  double aPz;		// uncertainty on pz(ttbar) term
  double aPt_mean;	// mean value of pt(ttbar) term
  bool use_pow2;	// pow2 - default is pow4
}topness_param_;

void default_param(topness_param& tp){
	tp.mW = 81.;
	tp.mT = 172;
	tp.aW = 5;
	tp.aT = 15.;
	tp.aTm = 15.;
	tp.aCM = 1000.;
	tp.aCM_mean = 2*tp.mT;
	tp.use_cox_box = false;
	tp.usePtPz = false;
	tp.aPt = 80;
	tp.aPt_mean = 460;
	tp.aPz = 500.;
	tp.use_pow2 = false;
	//tp.use_pow2 = true;
}

struct topness_min{
  double tWL;
  double tTL;
  double tTM;
  double tCM;
  double tPt;
  double tPz;
  double topness;
}topness_terms;

void LoadParam(string filename){
	ifstream ifile(filename.c_str());
	stringstream ss;
	ss  << ifile.rdbuf();ss >>topness_param_.aW;
	ss >>topness_param_.aT;
	ss >>topness_param_.aTm;
	ss >>topness_param_.aCM;
	ss >>topness_param_.aCM_mean;
	ss >>topness_param_.use_cox_box;
	ss >>topness_param_.usePtPz;
	ss >>topness_param_.aPt;
	ss >>topness_param_.aPt_mean;
	ss >>topness_param_.aPz;
}
void DumpParam(){
	cout << "aW = "<<topness_param_.aW<<endl;
	cout << "aT = " <<topness_param_.aT << endl;
	cout << "aTm = " <<topness_param_.aTm << endl;
	cout << "aCm = " <<topness_param_.aCM << endl;
	cout << "aCm_mean = " <<topness_param_.aCM_mean << endl;
	cout << "coxbox = " <<topness_param_.use_cox_box << endl;
	cout << "PtPz = " <<topness_param_.usePtPz << endl;
	cout << "aPt = " <<topness_param_.aPt << endl;
	cout << "aPt_mean = " <<topness_param_.aPt_mean << endl;
	cout << "aPz = " <<topness_param_.aPz << endl;
}

//extern "C" float topnessMinimization(TLorentzVector met, TLorentzVector lep, TLorentzVector bjet1, TLorentzVector bjet2, int version=1);
float topnessMinimization(
  double lep_pt, double lep_eta, double lep_phi, double lep_m,
  double bjet1_pt, double bjet1_eta, double bjet1_phi, double bjet1_m,
  double bjet2_pt, double bjet2_eta, double bjet2_phi, double bjet2_m,
  double met_et, double met_phi,
  double &lost_W_pt, double & lost_w_pz, double& nu_pz,
  double &tWL, double &tTL, double& tTM );
/*
extern "C" float topnessMinimization( TLorentzVecto
  double lep_pt, double lep_eta, double lep_phi, double lep_e,
  double bjet1_pt, double bjet1_eta, double bjet1_phi, double bjet1_e,
  double bjet2_pt, double bjet2_eta, double bjet2_phi, double bjet2_e,
  double met, double met_phi);
*/
double topnessFunction(double pwx_, double pwy_, double pwz_, double pnz_,
                       double plx_, double ply_, double plz_, double ple_,
                       double pb1x_, double pb1y_, double pb1z_, double pb1e_,
                       double pb2x_, double pb2y_, double pb2z_, double pb2e_,
                       double pmx_, double pmy_, double pmz_, double pme_);
		       //topness_param& param_);

void minuitFunctionWrapper(int& nDim, double* gout, double& result, double* par, int flg) {
  result = topnessFunction(par[0],par[1],par[2],par[3],
                           par[4],par[5],par[6],par[7],
                           par[8],par[9],par[10],par[11],
                           par[12],par[13],par[14],par[15],
                           par[16],par[17],par[18],par[19]);
			   //param_);
}


#endif

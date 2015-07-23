#ifndef TOPNESS2_H
#define TOPNESS2_H

#include "TFitter.h"
#include "TMath.h"
#include <vector>
#include "Math/LorentzVector.h"
#include <math.h>
#include "TLorentzVector.h"
#include <sstream>
#include <fstream>

using namespace std;

double round2(double input, int digits){
    return ceil( ( input * pow( 10,digits ) ) - 0.5 ) / pow( 10,digits );
}

struct topness2_param{
  double mW;		// mean W mass
  double mT;		// mean top mass
  double aW;		// unccertainty on W (l+nu)
  double aT;		// uncerainty on top (b+l+nu)
  double aTm;		// uncertainty related to the constraint on the top where a W is missing
  double aCM;		// uncertainty trelated to constraint on energy total of ttbar
  double aCM_mean;	// mean value of the total ttbar energy
  bool use_cox_box; 	// help to transform aCM constrain into a gaussian constrain
  bool usePtPz;		// change CM term (use pt, pz instead of Mass);
  bool usePz;		// change CM term (use pt, pz instead of Mass);
  double aPt;		// uncertainty on pt(ttbar) term
  double aPz;		// uncertainty on pz(ttbar) term
  double aPt_mean;	// mean value of pt(ttbar) term
  double aMET;		// relative MET resolution
  double bMET;		// relative MET bias
  bool use_pow2;	// pow2 - default is pow4
}topness2_param_;

void default_param2(topness2_param& tp){
	tp.mW = 81.;
	tp.mT = 172;
	tp.aW = 5;
	tp.aT = 15.;
	tp.aTm = 15.;
	tp.aCM = 1000.;
	tp.aCM_mean = 2*tp.mT;
	tp.use_cox_box = false;
	tp.usePtPz = false;
	tp.usePz = false;
	tp.aPt = 80;
	tp.aPt_mean = 460;
	tp.aPz = 500.;
	tp.use_pow2 = true;
	//tp.use_pow2 = true;
	tp.aMET = 0.36;
	tp.bMET = 0;
}

struct topness2_min{
  double tWL;
  double tTL;
  double tTM;
  double tCM;
  double tPt;
  double tPz;
  double tMET;
  double topness;
}topness2_terms;

void LoadParam2(string filename){
	ifstream ifile(filename.c_str());
	stringstream ss;
	ss  << ifile.rdbuf();ss >>topness2_param_.aW;
	ss >>topness2_param_.aT;
	ss >>topness2_param_.aTm;
	ss >>topness2_param_.aCM;
	ss >>topness2_param_.aCM_mean;
	ss >>topness2_param_.use_cox_box;
	ss >>topness2_param_.usePtPz;
	ss >>topness2_param_.usePz;
	ss >>topness2_param_.aPt;
	ss >>topness2_param_.aPt_mean;
	ss >>topness2_param_.aPz;
	ss >>topness2_param_.aMET;
	ss >>topness2_param_.bMET;
}
void DumpParam22(){
	cout << "aW = "<<topness2_param_.aW<<endl;
	cout << "aT = " <<topness2_param_.aT << endl;
	cout << "aTm = " <<topness2_param_.aTm << endl;
	cout << "aCm = " <<topness2_param_.aCM << endl;
	cout << "aCm_mean = " <<topness2_param_.aCM_mean << endl;
	cout << "coxbox = " <<topness2_param_.use_cox_box << endl;
	cout << "PtPz = " <<topness2_param_.usePtPz << endl;
	cout << "aPt = " <<topness2_param_.aPt << endl;
	cout << "aPt_mean = " <<topness2_param_.aPt_mean << endl;
	cout << "aPz = " <<topness2_param_.aPz << endl;
}

//extern "C" float topnessMinimization(TLorentzVector met, TLorentzVector lep, TLorentzVector bjet1, TLorentzVector bjet2, int version=1);
float topness2Minimization(
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
double topness2Function(double pwx_, double pwy_, double pwz_,
                       double pnx_, double pny_, double pnz,
		       double plx_, double ply_, double plz_, double ple_,
                       double pb1x_, double pb1y_, double pb1z_, double pb1e_,
                       double pb2x_, double pb2y_, double pb2z_, double pb2e_,
		       double pmx_, double pmy_, double pmz_, double pme_);
		       //topness2_param& param_);

void minuitFunctionWrapper2(int& nDim, double* gout, double& result, double* par, int flg) {
  result = topness2Function(par[0],par[1],par[2],par[3],
                           par[4],par[5],par[6],par[7],
                           par[8],par[9],par[10],par[11],
                           par[12],par[13],par[14],par[15],
                           par[16],par[17],par[18],par[19],
			   par[20],par[21]);
			   //param_);
}

#endif

#include "topness.h"
#include <iostream>

using namespace std;

//THIS is the function you want to call to get the variable. Others are helper.
//float topnessMinimization(TLorentzVector met, TLorentzVector lep, TLorentzVector bjet1, TLorentzVector bjet2) {
// Using extern "C" for compiler to not mangle the name of the function
 

//extern "C" 
float topnessMinimization(
  double lep_pt, double lep_eta, double lep_phi, double lep_m,
  double bjet1_pt, double bjet1_eta, double bjet1_phi, double bjet1_m,
  double bjet2_pt, double bjet2_eta, double bjet2_phi, double bjet2_m,
  double met_et, double met_phi,
  double &lost_W_pt, double & lost_w_pz, double& nu_pz,////){
  double &tWL, double &tTL, double& tTM ){
  //topness_param& p){

  TFitter *minimizer=new TFitter(4) ;
  double p1 = -1;
  minimizer->ExecuteCommand("SET PRINTOUT",&p1,1);
  minimizer->SetFCN(minuitFunctionWrapper);

  //Fill TlorentzVector
  TLorentzVector met;
  met.SetPtEtaPhiE(met_et, 0, met_phi, met_et);
  TLorentzVector lep;
  lep.SetPtEtaPhiM(lep_pt, lep_eta, lep_phi, lep_m);
  TLorentzVector bjet1;
  bjet1.SetPtEtaPhiM(bjet1_pt, bjet1_eta, bjet1_phi, bjet1_m);
  TLorentzVector bjet2;
  bjet2.SetPtEtaPhiM(bjet2_pt, bjet2_eta, bjet2_phi, bjet2_m);
 
  // get variables for Topness and use a rounding
 
  int mydigit = 3;
  double iLpx = round(lep.Px(),mydigit);
  double iLpy = round(lep.Py(),mydigit);
  double iLpz = round(lep.Pz(),mydigit);
  double iLpe = round(lep.E(),mydigit);
  double iB1px = round(bjet1.Px(),mydigit);
  double iB1py = round(bjet1.Py(),mydigit);
  double iB1pz = round(bjet1.Pz(),mydigit);
  double iB1pe = round(bjet1.E(),mydigit);
  double iB2px = round(bjet2.Px(),mydigit);
  double iB2py = round(bjet2.Py(),mydigit);
  double iB2pz = round(bjet2.Pz(),mydigit);
  double iB2pe = round(bjet2.E(),mydigit);
  double iMpx = round(met.Px(),mydigit);
  double iMpy = round(met.Py(),mydigit);
  double iMpz = round(met.Pz(),mydigit);
  double iMpe = round(met.E(),mydigit);

  //cout<<iLpx<<" "<<iLpy<<" "<<iLpz<<" "<<iLpe<<endl;
  //cout<<iB1px<<" "<<iB1py<<" "<<iB1pz<<" "<<iB1pe<<endl;
  //cout<<iB2px<<" "<<iB2py<<" "<<iB2pz<<" "<<iB2pe<<endl;
  //cout<<iMpx<<" "<<iMpy<<" "<<iMpz<<" "<<iMpe<<endl;
  
  // Define parameters [param number, param name, init val, estimated distance to min, bla, bla] // 300,3000,-3000,3000
  minimizer->SetParameter(0,"pwx",0,500,-3000,3000);
  minimizer->SetParameter(1,"pwy",0,500,-3000,3000);
  minimizer->SetParameter(2,"pwz",0,500,-3000,3000);
  minimizer->SetParameter(3,"pnz",0,500,-3000,3000);
  // fixed parameters
  minimizer->SetParameter(4,"plx",iLpx,0,iLpx-0.001,iLpx+0.001);
  minimizer->SetParameter(5,"ply",iLpy,0,iLpy-0.001,iLpy+0.001);
  minimizer->SetParameter(6,"plz",iLpz,0,iLpz-0.001,iLpz+0.001);
  minimizer->SetParameter(7,"ple",iLpe,0,iLpe-0.001,iLpe+0.001);
  minimizer->SetParameter(8,"pb1x",iB1px,0,iB1px-0.001,iB1px+0.001);
  minimizer->SetParameter(9,"pb1y",iB1py,0,iB1py-0.001,iB1py+0.001);
  minimizer->SetParameter(10,"pb1z",iB1pz,0,iB1pz-0.001,iB1pz+0.001);
  minimizer->SetParameter(11,"pb1e",iB1pe,0,iB1pe-0.001,iB1pe+0.001);
  minimizer->SetParameter(12,"pb2x",iB2px,0,iB2px-0.001,iB2px+0.001);
  minimizer->SetParameter(13,"pb2y",iB2py,0,iB2py-0.001,iB2py+0.001);
  minimizer->SetParameter(14,"pb2z",iB2pz,0,iB2pz-0.001,iB2pz+0.001);
  minimizer->SetParameter(15,"pb2e",iB2pe,0,iB2pe-0.001,iB2pe+0.001);
  minimizer->SetParameter(16,"pmx",iMpx,0,iMpx-0.001,iMpx+0.001);
  minimizer->SetParameter(17,"pmy",iMpy,0,iMpy-0.001,iMpy+0.001);
  minimizer->SetParameter(18,"pmz",iMpz,0,iMpz-0.001,iMpz+0.001);
  minimizer->SetParameter(19,"pme",iMpe,0,iMpe-0.001,iMpe+0.001);
  minimizer->FixParameter(4);
  minimizer->FixParameter(5);
  minimizer->FixParameter(6);
  minimizer->FixParameter(7);
  minimizer->FixParameter(8);
  minimizer->FixParameter(9);
  minimizer->FixParameter(10);
  minimizer->FixParameter(11);
  minimizer->FixParameter(12);
  minimizer->FixParameter(13);
  minimizer->FixParameter(14);
  minimizer->FixParameter(15);
  minimizer->FixParameter(16);
  minimizer->FixParameter(17);
  minimizer->FixParameter(18);
  minimizer->FixParameter(19);
  // Run the simplex minimizer to get close to the minimum [no good precision, but robust]
  // For signal regions, no difference seen with migrad, but more stable
  minimizer->ExecuteCommand("SIMPLEX",0,0);
  //minimizer->ExecuteCommand("MIGRAD",0,0);
  //Get the best fit values
  double pwx_fit = minimizer->GetParameter(0);
  double pwy_fit = minimizer->GetParameter(1);
  double pwz_fit = minimizer->GetParameter(2);
  double pnz_fit = minimizer->GetParameter(3);
  double plx_fit = minimizer->GetParameter(4);
  double ply_fit = minimizer->GetParameter(5);
  double plz_fit = minimizer->GetParameter(6);
  double ple_fit = minimizer->GetParameter(7);
  double pb1x_fit = minimizer->GetParameter(8);
  double pb1y_fit = minimizer->GetParameter(9);
  double pb1z_fit = minimizer->GetParameter(10);
  double pb1e_fit = minimizer->GetParameter(11);
  double pb2x_fit = minimizer->GetParameter(12);
  double pb2y_fit = minimizer->GetParameter(13);
  double pb2z_fit = minimizer->GetParameter(14);
  double pb2e_fit = minimizer->GetParameter(15);
  double pmx_fit = minimizer->GetParameter(16);
  double pmy_fit = minimizer->GetParameter(17);
  double pmz_fit = minimizer->GetParameter(18);
  double pme_fit = minimizer->GetParameter(19);
  delete minimizer;
  
  TLorentzVector vW; vW.SetPxPyPzE(pwx_fit,pwy_fit,pwz_fit,(sqrt((topness_param_.mW*topness_param_.mW)+(pwx_fit*pwx_fit)+(pwy_fit*pwy_fit)+(pwz_fit*pwz_fit)))) ;
  TLorentzVector vL; vL.SetPxPyPzE(plx_fit,ply_fit,plz_fit,ple_fit);
  TLorentzVector vB1; vB1.SetPxPyPzE(pb1x_fit,pb1y_fit,pb1z_fit,pb1e_fit);
  TLorentzVector vB2; vB2.SetPxPyPzE(pb2x_fit,pb2y_fit,pb2z_fit,pb2e_fit);
  TLorentzVector vMET; vMET.SetPxPyPzE(pmx_fit,pmy_fit,pmz_fit,pme_fit);
  TLorentzVector vN; vN.SetPxPyPzE((pmx_fit-pwx_fit),(pmy_fit-pwy_fit),pmz_fit,(sqrt(pow((pmx_fit-pwx_fit),2)+pow((pmy_fit-pwy_fit),2)+pow(pmz_fit,2))));
  // construct the w-term (lep)
  tWL = ( pow( ((topness_param_.mW*topness_param_.mW) - ((vL+vN).M2())),2) ) / (pow(topness_param_.aW,4));
  //cout<<"topness - tWL term: "<<(vL+vN).M()<<" "<<tWL<<endl;
  // construct the tL-term [seen lepton]
  tTL = ( pow( ((topness_param_.mT*topness_param_.mT) - ((vL+vB1+vN).M2())),2) ) / (pow(topness_param_.aT,4));
  // construct the tM-term [miss lepton]
  tTM = ( pow( ((topness_param_.mT*topness_param_.mT) - ((vB2+vW).M2())),2) ) / (pow(topness_param_.aTm,4));
  // construct the CM-term
  
  //send as reference
  lost_W_pt = sqrt(pow(pwx_fit,2)+pow(pwy_fit,2));
  lost_w_pz = pwz_fit;
  nu_pz = pmz_fit;
  /*
  cout<< "### FIT : " << tWL<< " " << tTL << " " << tWL << " " ;
  cout << topnessFunction(pwx_fit,pwy_fit,pwz_fit,pnz_fit,
			     iLpx,iLpy,iLpz,iLpe,
			     iB1px,iB1py,iB1pz,iB1pe,
			     iB2px,iB2py,iB2pz,iB2pe,
			     iMpx,iMpy,iMpz,iMpe) << endl;//,
  */
  // get the function value at best fit
  return log(topnessFunction(pwx_fit,pwy_fit,pwz_fit,pnz_fit,
			     iLpx,iLpy,iLpz,iLpe,
			     iB1px,iB1py,iB1pz,iB1pe,
			     iB2px,iB2py,iB2pz,iB2pe,
			     iMpx,iMpy,iMpz,iMpe));//,
			     //p));

} // ~ end of Topness Minimization()

// full function
double topnessFunction(double pwx_, double pwy_, double pwz_, double pnz_,
                       double plx_, double ply_, double plz_, double ple_,
                       double pb1x_, double pb1y_, double pb1z_, double pb1e_,
                       double pb2x_, double pb2y_, double pb2z_, double pb2e_,
                       double pmx_, double pmy_, double pmz_, double pme_) {
		       //topness_param& p) {

  /*
  const double mW = 81.;
  const double mT = 172.;
  const double aW = 5.;
  const double aT = 15.;
  const double aTm = 15.;
  const double aCM = 1000.;
  const double aCM_mean = 2*mT;
  */

  // construct the lorentz vectors
  TLorentzVector vW; vW.SetPxPyPzE(pwx_,pwy_,pwz_,(sqrt((topness_param_.mW*topness_param_.mW)+(pwx_*pwx_)+(pwy_*pwy_)+(pwz_*pwz_)))) ;
  TLorentzVector vL; vL.SetPxPyPzE(plx_,ply_,plz_,ple_);
  TLorentzVector vB1; vB1.SetPxPyPzE(pb1x_,pb1y_,pb1z_,pb1e_);
  TLorentzVector vB2; vB2.SetPxPyPzE(pb2x_,pb2y_,pb2z_,pb2e_);
  TLorentzVector vMET; vMET.SetPxPyPzE(pmx_,pmy_,pmz_,pme_);
  TLorentzVector vN; vN.SetPxPyPzE((pmx_-pwx_),(pmy_-pwy_),pnz_,(sqrt(pow((pmx_-pwx_),2)+pow((pmy_-pwy_),2)+pow(pnz_,2))));
  // construct the w-term (lost)
  //double tWM = ( pow( ((mW*mW) - (vW.M2())),2) ) / (pow(aW,4));//zero by construction
  // construct the w-term (lep)
  double tWL = ( pow( ((topness_param_.mW*topness_param_.mW) - ((vL+vN).M2())),2) ) / (pow(topness_param_.aW,4));
  if(topness_param_.use_pow2){
  	tWL = ( pow( ((topness_param_.mW) - ((vL+vN).M())),2) ) / (pow(topness_param_.aW,2));
  }
  // construct the tL-term [seen lepton]
  double tTL = ( pow( ((topness_param_.mT*topness_param_.mT) - ((vL+vB1+vN).M2())),2) ) / (pow(topness_param_.aT,4));
  if(topness_param_.use_pow2){
  	tTL = ( pow( ((topness_param_.mT) - ((vL+vB1+vN).M())),2) ) / (pow(topness_param_.aT,2));
  }
  // construct the tM-term [miss lepton]
  double tTM = ( pow( ((topness_param_.mT*topness_param_.mT) - ((vB2+vW).M2())),2) ) / (pow(topness_param_.aTm,4));
  if(topness_param_.use_pow2){
  	tTM = ( pow( ((topness_param_.mT) - ((vB2+vW).M())),2) ) / (pow(topness_param_.aTm,2));
  }
  // construct the CM-term
  double tCM = 0;
  double tPt = 0;
  double tPz = 0;
  if(topness_param_.use_cox_box){
  	double E_tot = (vL+vN+vW+vB1+vB2).E();
	//Cox-box method
	double term = (pow(E_tot,0.05)-1)/(6.25E-4);
  	//tCM = (pow( (pow(topness_param_.aCM_mean,2) - term*term),2))  / (pow(topness_param_.aCM,4));
  	tCM = (pow( (topness_param_.aCM_mean) - term,2))  / (pow(topness_param_.aCM,2));
  	if(topness_param_.use_pow2){
  		tCM = (pow( ((topness_param_.aCM_mean) - term),2))  / (pow(topness_param_.aCM,2));
  	}
  	//cout<<"use cox box"<<endl;
  }
  else{ 
  	tCM = ( pow( (pow(topness_param_.aCM_mean,2) - ((vL+vN+vW+vB1+vB2).M2())),2) ) / (pow(topness_param_.aCM,4));
  	if(topness_param_.use_pow2){
  		tCM = ( pow( ((topness_param_.aCM_mean) - ((vL+vN+vW+vB1+vB2).M())),2) ) / (pow(topness_param_.aCM,2));
	}
  }
  // calculate Topness
  double Topness = 0;
  if(!topness_param_.usePtPz){
  	//if(topness_param_.use_pow2)
	//	Topness = sqrt(tWL) + sqrt(tTL) + sqrt(tTM) + sqrt(tCM);
	//else
		Topness = tWL + tTL + tTM + tCM;
  }
  else{
  	//cout<<"use ptpz"<<endl;
  	//tPz = ( pow( (((vL+vN+vW+vB1+vB2).Pz())),2) ) / (pow(topness_param_.aPz,4));
  	tPz = ( pow( (((vL+vN+vW+vB1+vB2).Pz())),2) ) / (pow(topness_param_.aPz,2));
	double Pt_tot = (vL+vN+vW+vB1+vB2).Pt();
	//Cox-box method
	double term = (pow(Pt_tot,0.05)-1)/(6.25E-4);
	//tPt = (pow( (pow(topness_param_.aPt_mean,2) - term*term),2))  / (pow(topness_param_.aPt,4));
	tPt = (pow( (topness_param_.aPt_mean) - term,2))  / (pow(topness_param_.aPt,2));
  	//if(topness_param_.use_pow2)
	//	Topness = sqrt(tWL) + sqrt(tTL) + sqrt(tTM) + sqrt(tCM);
  	//else
		Topness = tWL + tTL + tTM + tPz + tPt;
  }
  //cout << "topness: "<<  tWL << " " <<  tTL << " " << tTM << " " << tCM << " and " << Topness <<endl;
  // copy values in the global variable
  topness_terms.tWL = tWL;
  topness_terms.tTL = tTL;
  topness_terms.tTM = tTM;
  topness_terms.tCM = tCM;
  topness_terms.tPt = tPt;
  topness_terms.tPz = tPz;
  topness_terms.topness = Topness;

  return Topness;
}

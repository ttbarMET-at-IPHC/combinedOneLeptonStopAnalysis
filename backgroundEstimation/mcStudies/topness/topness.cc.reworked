#include "topness.h"

//THIS is the function you want to call to get the variable. Others are helper.
//float topnessMinimization(TLorentzVector met, TLorentzVector lep, TLorentzVector bjet1, TLorentzVector bjet2) {
// Using extern "C" for compiler to not mangle the name of the function
float topnessMinimization(
  double lep_pt, double lep_eta, double lep_phi, double lep_e,
  double bjet1_pt, double bjet1_eta, double bjet1_phi, double bjet1_e,
  double bjet2_pt, double bjet2_eta, double bjet2_phi, double bjet2_e,
  double met_et, double met_phi){
  
  TFitter *minimizer=new TFitter(4) ;
  double p1 = -1;
  minimizer->ExecuteCommand("SET PRINTOUT",&p1,1);
  minimizer->SetFCN(minuitFunctionWrapper);

  //Fill TlorentzVector
  TLorentzVector met;
  met.SetPtEtaPhiE(met_et, 0, met_phi, met_et);
  TLorentzVector lep;
  lep.SetPtEtaPhiE(lep_pt, lep_eta, lep_phi, lep_e);
  TLorentzVector bjet1;
  bjet1.SetPtEtaPhiE(bjet1_pt, bjet1_eta, bjet1_phi, bjet1_e);
  TLorentzVector bjet2;
  bjet2.SetPtEtaPhiE(bjet1_pt, bjet2_eta, bjet2_phi, bjet2_e);
  
  // get variables for Topness
  double iLpx = lep.Px();
  double iLpy = lep.Py();
  double iLpz = lep.Pz();
  double iLpe = lep.E();
  double iB1px = bjet1.Px();
  double iB1py = bjet1.Py();
  double iB1pz = bjet1.Pz();
  double iB1pe = bjet1.E();
  double iB2px = bjet2.Px();
  double iB2py = bjet2.Py();
  double iB2pz = bjet2.Pz();
  double iB2pe = bjet2.E();
  double iMpx = met.Px();
  double iMpy = met.Py();
  double iMpz = met.Pz();
  double iMpe = met.E();
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
  //Get the best fit values
  double pwx_fit = minimizer->GetParameter(0);
  double pwy_fit = minimizer->GetParameter(1);
  double pwz_fit = minimizer->GetParameter(2);
  double pnz_fit = minimizer->GetParameter(3);
  delete minimizer;
  // get the function value at best fit
  return log(topnessFunction(pwx_fit,pwy_fit,pwz_fit,pnz_fit,
			     iLpx,iLpy,iLpz,iLpe,
			     iB1px,iB1py,iB1pz,iB1pe,
			     iB2px,iB2py,iB2pz,iB2pe,
			     iMpx,iMpy,iMpz,iMpe
			     ));

} // ~ end of Topness Minimization()

// full function
double topnessFunction(double pwx_, double pwy_, double pwz_, double pnz_,
                       double plx_, double ply_, double plz_, double ple_,
                       double pb1x_, double pb1y_, double pb1z_, double pb1e_,
                       double pb2x_, double pb2y_, double pb2z_, double pb2e_,
                       double pmx_, double pmy_, double pmz_, double pme_) {
  const double mW = 81.;
  const double mT = 172.;
  const double aW = 5.;
  const double aT = 15.;
  const double aCM = 1000.;
    
  // construct the lorentz vectors
  TLorentzVector vW; vW.SetPxPyPzE(pwx_,pwy_,pwz_,(sqrt((mW*mW)+(pwx_*pwx_)+(pwy_*pwy_)+(pwz_*pwz_)))) ;
  TLorentzVector vL; vL.SetPxPyPzE(plx_,ply_,plz_,ple_);
  TLorentzVector vB1; vB1.SetPxPyPzE(pb1x_,pb1y_,pb1z_,pb1e_);
  TLorentzVector vB2; vB2.SetPxPyPzE(pb2x_,pb2y_,pb2z_,pb2e_);
  TLorentzVector vMET; vMET.SetPxPyPzE(pmx_,pmy_,pmz_,pme_);
  TLorentzVector vN; vN.SetPxPyPzE((pmx_-pwx_),(pmy_-pwy_),pnz_,(sqrt(pow((pmx_-pwx_),2)+pow((pmy_-pwy_),2)+pow(pnz_,2))));
  // construct the w-term (lost)
  //double tWM = ( pow( ((mW*mW) - (vW.M2())),2) ) / (pow(aW,4));//zero by construction
  // construct the w-term (lep)
  double tWL = ( pow( ((mW*mW) - ((vL+vN).M2())),2) ) / (pow(aW,4));
  // construct the tL-term [seen lepton]
  double tTL = ( pow( ((mT*mT) - ((vL+vB1+vN).M2())),2) ) / (pow(aT,4));
  // construct the tM-term [miss lepton]
  double tTM = ( pow( ((mT*mT) - ((vB2+vW).M2())),2) ) / (pow(aT,4));
  // construct the CM-term
  double tCM = ( pow( ((4*(mT*mT)) - ((vL+vN+vW+vB1+vB2).M2())),2) ) / (pow(aCM,4));
  // calculate Topness
  double Topness = tWL + tTL + tTM + tCM;
  return Topness;
}

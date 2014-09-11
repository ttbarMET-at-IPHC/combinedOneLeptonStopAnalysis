#ifndef TRIGGER_EFFIFIENCY_REWEIGHTING
#define TRIGGER_EFFIFIENCY_REWEIGHTING

float singleLeptonTriggerWeight(int flavor, float pt, float eta)
{

  //electron efficiencies
  if (flavor == 11) 
  {
    if ( fabs(eta)<1.5) 
    {
      if ( pt>=20 && pt<22 )   return 0.00;
      if ( pt>=22 && pt<24 )   return 0.00;
      if ( pt>=24 && pt<26 )   return 0.00;
      if ( pt>=26 && pt<28 )   return 0.08;
      if ( pt>=28 && pt<30 )   return 0.61;
      if ( pt>=30 && pt<32 )   return 0.86;
      if ( pt>=32 && pt<34 )   return 0.88;
      if ( pt>=34 && pt<36 )   return 0.90;
      if ( pt>=36 && pt<38 )   return 0.91;
      if ( pt>=38 && pt<40 )   return 0.92;
      if ( pt>=40 && pt<50 )   return 0.94;
      if ( pt>=50 && pt<60 )   return 0.95;
      if ( pt>=60 && pt<80 )   return 0.96;
      if ( pt>=80 && pt<100 )  return 0.96;
      if ( pt>=100 && pt<150 ) return 0.96;
      if ( pt>=150 && pt<200 ) return 0.97;
      if ( pt>=200 )           return 0.97;
    } 
    else if ( fabs(eta)>=1.5 && fabs(eta)<2.1) 
    {
      if ( pt>=20 && pt<22 )   return 0.00;
      if ( pt>=22 && pt<24 )   return 0.00;
      if ( pt>=24 && pt<26 )   return 0.02;
      if ( pt>=26 && pt<28 )   return 0.18;
      if ( pt>=28 && pt<30 )   return 0.50;
      if ( pt>=30 && pt<32 )   return 0.63;
      if ( pt>=32 && pt<34 )   return 0.68;
      if ( pt>=34 && pt<36 )   return 0.70;
      if ( pt>=36 && pt<38 )   return 0.72;
      if ( pt>=38 && pt<40 )   return 0.74;
      if ( pt>=40 && pt<50 )   return 0.76;
      if ( pt>=50 && pt<60 )   return 0.77;
      if ( pt>=60 && pt<80 )   return 0.78;
      if ( pt>=80 && pt<100 )  return 0.80;
      if ( pt>=100 && pt<150 ) return 0.79;
      if ( pt>=150 && pt<200 ) return 0.76;
      if ( pt>=200 )           return 0.81;
    }
  } 
  //muon efficiencies
  else if (flavor == 13) 
  {
    if ( fabs(eta)<0.8 ) 
    {
      if (pt>=20 && pt<22)   return 0.00;     
      if (pt>=22 && pt<24)   return 0.03;      
      if (pt>=24 && pt<26)   return 0.87;
      if (pt>=26 && pt<28)   return 0.90;
      if (pt>=28 && pt<30)   return 0.91;
      if (pt>=30 && pt<32)   return 0.91;
      if (pt>=32 && pt<34)   return 0.92;
      if (pt>=34 && pt<36)   return 0.93;
      if (pt>=36 && pt<38)   return 0.93;
      if (pt>=38 && pt<40)   return 0.93;
      if (pt>=40 && pt<50)   return 0.94;
      if (pt>=50 && pt<60)   return 0.95;
      if (pt>=60 && pt<80)   return 0.95;
      if (pt>=80 && pt<100)  return 0.94;
      if (pt>=100 && pt<150) return 0.94;
      if (pt>=150 && pt<200) return 0.93;
      if (pt>=200)           return 0.92;
    } 
    else if ( fabs(eta)>=0.8 && fabs(eta)<1.5 ) 
    {
      if (pt>=20 && pt<22)   return 0.00;
      if (pt>=22 && pt<24)   return 0.05;
      if (pt>=24 && pt<26)   return 0.78;
      if (pt>=26 && pt<28)   return 0.81;
      if (pt>=28 && pt<30)   return 0.81;
      if (pt>=30 && pt<32)   return 0.81;
      if (pt>=32 && pt<34)   return 0.82;
      if (pt>=34 && pt<36)   return 0.82;
      if (pt>=36 && pt<38)   return 0.83;
      if (pt>=38 && pt<40)   return 0.83;
      if (pt>=40 && pt<50)   return 0.84;
      if (pt>=50 && pt<60)   return 0.84;
      if (pt>=60 && pt<80)   return 0.84;
      if (pt>=80 && pt<100)  return 0.84;
      if (pt>=100 && pt<150) return 0.84;
      if (pt>=150 && pt<200) return 0.84;
      if (pt>=200)           return 0.82;
    } 
    else if ( fabs(eta)>=1.5 && fabs(eta)<2.1 ) 
    {
      if (pt>=20 && pt<22)   return 0.00;
      if (pt>=22 && pt<24)   return 0.11;
      if (pt>=24 && pt<26)   return 0.76;
      if (pt>=26 && pt<28)   return 0.78;
      if (pt>=28 && pt<30)   return 0.79;
      if (pt>=30 && pt<32)   return 0.80;
      if (pt>=32 && pt<34)   return 0.80;
      if (pt>=34 && pt<36)   return 0.81;
      if (pt>=36 && pt<38)   return 0.81;
      if (pt>=38 && pt<40)   return 0.82;
      if (pt>=40 && pt<50)   return 0.82;
      if (pt>=50 && pt<60)   return 0.83;
      if (pt>=60 && pt<80)   return 0.83;
      if (pt>=80 && pt<100)  return 0.83;
      if (pt>=100 && pt<150) return 0.83;
      if (pt>=150 && pt<200) return 0.82;
      if (pt>=200)           return 0.82;
    }
  }

  return 1.;

}
     
/// Lepton turn on for the cross trigger
double IsoMu17_eta2p1(float pt, float eta){
  if ( fabs(eta)<0.8 ) 
  {
    if (pt>=20 && pt<22)  return  0.94;     
    if (pt>=22 && pt<24)  return  0.97;      
    if (pt>=24 && pt<26)  return  0.96;
  } 
  else if ( fabs(eta)>=0.8 && fabs(eta)<1.5 ) 
  {
    if (pt>=20 && pt<22)  return  0.87;
    if (pt>=22 && pt<24)  return  0.90;
    if (pt>=24 && pt<26)  return  0.91;
  }
  else if ( fabs(eta)>=1.5 && fabs(eta)<2.1 ) 
  {
    if (pt>=20 && pt<22)  return  0.76;
    if (pt>=22 && pt<24)  return  0.83;
    if (pt>=24 && pt<26)  return  0.87;
  }

  return 1.;
}

/// Jet Turn ons PFNoPUJet30 Runs > 199608
TF1 PFNoPUJet30("PFNoPUJet30","0.6412/(0.6414 + exp(9.173-x*0.3877))");
/// Jet Turn ons PFNoPUJet20 198049 < Runs < 199608
TF1 PFNoPUJet20("PFNoPUJet20","1/(1 + exp(1.393-x*0.2067))");
/// Jet Turn ons PFJet30 Runs < 198049 
TF1 PFJet30("PFJet30","1/(1 + exp(10.95-x*0.4503))");

float crossTriggerWeight(float leptonPt, float leptonEta, float fourthJetPt)
{

    // Weight = lepton part * jet part
    
    return IsoMu17_eta2p1(leptonPt,leptonEta)
         * (0.074 * PFJet30.Eval(fourthJetPt)           // trigger was used during 7.4%  of data taking
          + 0.288 * PFNoPUJet20.Eval(fourthJetPt)       // trigger was used during 28.8% of data taking
          + 0.638 * PFNoPUJet30.Eval(fourthJetPt));     // trigger was used during 63.8% of data taking
}

#endif 

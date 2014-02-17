
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
     
/*
  Turn on curves for the x-triggers
  The efficiency for the complete trigger can be calculated as the product of the efficiencies for each element of the triggers
  Total_Eff = EffMuon(pt)*EffJet1(pt)*EffJet2(pt)*EffJet3(pt)  
*/

/////FIXME  This turn ons are not yet the final word. They will be updated


/// Lepton turn ons

double IsoMu17_eta2p1_TurnOn(double pt){
  TF1 *f1 = new TF1("f1","0.9397/(1.051 + exp(7.322-0.4911*x))");
  return f1->Eval(pt);
}
double IsoMu17_eta2p1_TurnOn_Plus1Sigma(double pt){
  TF1 *f1 = new TF1("f1","1.5923/(1.7806 + exp(8.635-0.55555*x))");
  return f1->Eval(pt);
}
double IsoMu17_eta2p1_TurnOn_Minus1Sigma(double pt){
  TF1 *f1 = new TF1("f1","0.2871/(0.3214 + exp(6.009-0.42665x))");
  return f1->Eval(pt);
}


double IsoMu17_eta2p1_TurnOn(double pt, double eta){
  if ( fabs(eta)<0.8 ) {
    if (pt>=20 && pt<22)  return  0.94;     
    if (pt>=22 && pt<24)  return  0.97;      
    if (pt>=24 && pt<26)  return  0.96;
  } 
  else if ( fabs(eta)>=0.8 && fabs(eta)<1.5 ) {
    if (pt>=20 && pt<22)  return  0.87;
    if (pt>=22 && pt<24)  return  0.90;
    if (pt>=24 && pt<26)  return  0.91;
  }
  else if ( fabs(eta)>=1.5 && fabs(eta)<2.1 ) {
    if (pt>=20 && pt<22)  return  0.76;
    if (pt>=22 && pt<24)  return  0.83;
    if (pt>=24 && pt<26)  return  0.87;
  }

  return 1.;
}

double IsoMu17_eta2p1_TurnOn_Plus1Sigma(double pt, double eta){
  if ( fabs(eta)<0.8 ) {
    if (pt>=20 && pt<22)  return  0.96;     
    if (pt>=22 && pt<24)  return  0.97;      
    if (pt>=24 && pt<26)  return  0.97;
  } 
  else if ( fabs(eta)>=0.8 && fabs(eta)<1.5 ) {
    if (pt>=20 && pt<22)  return  0.88;
    if (pt>=22 && pt<24)  return  0.91;
    if (pt>=24 && pt<26)  return  0.92;
  }
  else if ( fabs(eta)>=1.5 && fabs(eta)<2.1 ) {
    if (pt>=20 && pt<22)  return  0.79;
    if (pt>=22 && pt<24)  return  0.86;
    if (pt>=24 && pt<26)  return  0.89;
  }
  return 1.;
}

double IsoMu17_eta2p1_TurnOn_Minus1Sigma(double pt, double eta){
  if ( fabs(eta)<0.8 ) {
    if (pt>=20 && pt<22)  return  0.93;     
    if (pt>=22 && pt<24)  return  0.95;      
    if (pt>=24 && pt<26)  return  0.95;
  } 
  else if ( fabs(eta)>=0.8 && fabs(eta)<1.5 ) {
    if (pt>=20 && pt<22)  return  0.85;
    if (pt>=22 && pt<24)  return  0.88;
    if (pt>=24 && pt<26)  return  0.90;
  }
  else if ( fabs(eta)>=1.5 && fabs(eta)<2.1 ) {
    if (pt>=20 && pt<22)  return  0.73;
    if (pt>=22 && pt<24)  return  0.80;
    if (pt>=24 && pt<26)  return  0.85;
  }
  return 1.;
}


/// Jet Turn ons PFNoPUJet30 Runs > 199608
double PFNoPUJet30_TurnOn(double pt){
  TF1 *f1 = new TF1("f1","0.6412/(0.6414 + exp(9.173-x*0.3877))");
  return f1->Eval(pt);
}
double PFNoPUJet30_TurnOn_Plus1Sigma(double pt){
  TF1 *f1 = new TF1("f1","0.6423/(0.6425 + exp(10.553-x*0.42636))");
  return f1->Eval(pt);
}
double PFNoPUJet30_TurnOn_Minus1Sigma(double pt){
  TF1 *f1 = new TF1("f1","0.6401/(0.6403 + exp(7.793-x*0.34904))");
  return f1->Eval(pt);
}

/// Jet Turn ons PFNoPUJet20 198049 < Runs < 199608
double PFNoPUJet20_TurnOn(double pt){
  TF1 *f1 = new TF1("f1","1/(1 + exp(1.393-x*0.2067))");
  return f1->Eval(pt);
}
double PFNoPUJet20_TurnOn_Plus1Sigma(double pt){
  TF1 *f1 = new TF1("f1","1/(1 + exp(1.3949-x*0.20675))");
  return f1->Eval(pt);
}
double PFNoPUJet20_TurnOn_Minus1Sigma(double pt){
  TF1 *f1 = new TF1("f1","1/(1 + exp(1.392-x*0.2066))");
  return f1->Eval(pt);
}

/// Jet Turn ons PFJet30 Runs < 198049 
double PFJet30_TurnOn(double pt){
  TF1 *f1 = new TF1("f1","1/(1 + exp(10.95-x*0.4503))");
  return f1->Eval(pt);
}
double PFJet30_TurnOn_Plus1Sigma(double pt){
  TF1 *f1 = new TF1("f1","1.007914/(1.007913 + exp(13.97-x*0.53961))");
  return f1->Eval(pt);
}
double PFJet30_TurnOn_Minus1Sigma(double pt){
  TF1 *f1 = new TF1("f1","0.992086/(0.992087 + exp(7.93-x*0.36099))");
  return f1->Eval(pt);
}


float crossTriggerWeight(float leptonPt, float leadingJetPt, float secondJetPt, float thirdJetPt)
{
    return IsoMu17_eta2p1_TurnOn(leptonPt)
         * (PFJet30_TurnOn(leadingJetPt)     * PFJet30_TurnOn(secondJetPt) * PFJet30_TurnOn(thirdJetPt)
          + PFNoPUJet30_TurnOn(leadingJetPt) * PFNoPUJet30_TurnOn(secondJetPt) * PFNoPUJet20_TurnOn(thirdJetPt)
          + PFNoPUJet30_TurnOn(leadingJetPt) * PFNoPUJet30_TurnOn(secondJetPt) * PFNoPUJet30_TurnOn(thirdJetPt)
           ) / 3.0;
}

 

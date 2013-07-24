
#include <cmath>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>

#include "Math/DistFuncMathCore.h"
#include "Math/DistFuncMathMore.h"
#include "Math/Vector2D.h"
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"
#include "Math/VectorUtil.h"
#include "Math/SVector.h"

#include <TBranch.h>
#include <TLorentzVector.h>

#include "MiniFWLite/DataFormats.h"
#include "MiniFWLite/EDProduct.h"
#include "MiniFWLite/MiniFWLiteDict.h"
#include "MiniFWLite/Wrapper.h"

#include <iostream>
using namespace std;

#include "../microTupling/MicroTuple_Format_Synchro0722.h"

#define INPUT "/opt/sbg/data/data4/cms/aaubin/analysisProd_July13-v1/store/synchro/baby_RelValTTbar_CMSSW_5_3_2_patch1-START53_V7A-v1_GEN-SIM-RECO.root"

#define OUTPUT "./microTuple_babyStopSynchro.root"

// #########################################################################
//                              Main function
// #########################################################################

int main (int argc, char *argv[])
{

  cout << endl;
  cout << "   ┌────────────────────────────────────┐  " << endl;
  cout << "   │   Starting baby tuple conversion   │  " << endl;
  cout << "   └────────────────────────────────────┘  " << endl; 
  cout << endl;

  // ########################################
  // ##       Run over the datasets        ##
  // ########################################

  // Input tree   
  TFile fInput(INPUT,"UPDATE");
  TTree* theInputTree = (TTree*) fInput.Get("t"); 
   
  // Output tree
  microEvent myEvent;
  TFile fOutput(OUTPUT,"RECREATE");
  TTree* theOutputTree=new TTree("microTuple","microTuple");
  theOutputTree->Branch("microEvents",&myEvent,MICROEVENT_FORMATROOT);

  // ########################################
  // ##        Run over the events         ##
  // ########################################

  float aFloat0;  float aFloat1;  float aFloat2;  float aFloat3;  float aFloat4;  
  float aFloat5;  float aFloat6;  float aFloat7;  float aFloat8;  float aFloat9;
  int anInt0;  int anInt1;  int anInt2;  int anInt3;  int anInt4;  int anInt5;  int anInt6;
  UInt_t         anUInt;

  ROOT::Math::LorentzVector< ROOT::Math::PxPyPzE4D<float> >* aVector0 = 0;
  ROOT::Math::LorentzVector< ROOT::Math::PxPyPzE4D<float> >* aVector1 = 0;
  vector< ROOT::Math::LorentzVector< ROOT::Math::PxPyPzE4D<float> > >* aVector2  = 0;

  for (int i = 0 ; i < theInputTree->GetEntries() ; i++)
  {
	  // Get the i-th entry
      theInputTree->SetBranchAddress("event",           &anUInt);  
      theInputTree->SetBranchAddress("isomu24",         &anInt0);  
      theInputTree->SetBranchAddress("ele27wp80",       &anInt1);  
      theInputTree->SetBranchAddress("lep1",            &aVector0);
      theInputTree->SetBranchAddress("pflep1",          &aVector1);
      theInputTree->SetBranchAddress("isopf1",          &aFloat0); 
      theInputTree->SetBranchAddress("eoverpin",        &aFloat1); 
      theInputTree->SetBranchAddress("id1",             &anInt2);  
      theInputTree->SetBranchAddress("mini_met",        &aFloat2); 
      theInputTree->SetBranchAddress("mini_mt",         &aFloat3); 
      theInputTree->SetBranchAddress("mini_njets",      &anInt3);  
      theInputTree->SetBranchAddress("mini_nb",         &anInt4);  
      theInputTree->SetBranchAddress("mini_passisotrk", &anInt5);  
      theInputTree->SetBranchAddress("mini_passtauveto",&anInt6);  
      theInputTree->SetBranchAddress("mini_dphimjmin",  &aFloat4); 
      theInputTree->SetBranchAddress("mini_chi2",       &aFloat5); 
      theInputTree->SetBranchAddress("mini_mt2w",       &aFloat6); 
      theInputTree->SetBranchAddress("mini_pt_b",       &aFloat7); 
      theInputTree->SetBranchAddress("mini_htratiom",   &aFloat8); 
      theInputTree->SetBranchAddress("mini_dRleptB1",   &aFloat9); 
      theInputTree->SetBranchAddress("pfjets",          &aVector2); 

      theInputTree->GetEntry(i); 
      
      myEvent.event        = anUInt;
      myEvent.triggerMu    = anInt0;
      myEvent.triggerEl    = anInt1;
      myEvent.leptonPt     = aVector0->Pt();
      myEvent.leptonEta    = aVector0->Eta();
      myEvent.leptonPhi    = aVector0->Phi();
      myEvent.leptonE      = aVector0->E();
      myEvent.leptonPFPt   = aVector1->Pt();
      myEvent.leptonIso    = aFloat0;
      myEvent.leptonEpin   = aFloat1;
      myEvent.leptonPDG    = anInt2;
      myEvent.MET          = aFloat2;
      myEvent.MT           = aFloat3;
      myEvent.nJets        = anInt3;
      myEvent.nB           = anInt4;
      myEvent.isoTrackVeto = anInt5;
      myEvent.tauVeto      = anInt6;
      myEvent.dPhiMETjet   = aFloat4;
      myEvent.hadronicChi2 = aFloat5;
      myEvent.MT2W         = aFloat6;
      myEvent.leadingBPt   = aFloat7;
      myEvent.HTratio      = aFloat8;
      myEvent.dRleptonB    = aFloat9;

      int i;
      for (i = 0 ; i < (*aVector2).size() ; i++)
      {
          if (i >= 10) break;

          myEvent.jets_Pt[i] = (*aVector2)[i].Pt();
          myEvent.jets_Phi[i] = (*aVector2)[i].Phi();
          myEvent.jets_Eta[i] = (*aVector2)[i].Eta();
      }

      if (i < 10)
      {
          myEvent.jets_Pt[i] = -1;
          myEvent.jets_Phi[i] = -999;
          myEvent.jets_Eta[i] = -999;
      }

      theOutputTree->Fill();
  } 

  theOutputTree->Print();
  theOutputTree->Write(0, TObject::kOverwrite); 

  return (0);

}



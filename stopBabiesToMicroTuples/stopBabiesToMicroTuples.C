
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

#include "../microTupling/formats/Synchro1025.h"

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

  ROOT::Math::LorentzVector< ROOT::Math::PxPyPzE4D<float> >* lepton = 0;
  ROOT::Math::LorentzVector< ROOT::Math::PxPyPzE4D<float> >* leptonPF = 0;
  vector< ROOT::Math::LorentzVector< ROOT::Math::PxPyPzE4D<float> > >* jets  = 0;
  vector< float >* jets_sigma        = 0;
  vector< float >* jets_CSV          = 0;
  vector< float >* jets_CSVreshaped  = 0;
  vector< float >* jets_PUId         = 0;

  theInputTree->SetBranchAddress("event",             &(myEvent.event        ));  
  theInputTree->SetBranchAddress("isomu24",           &(myEvent.triggerMu    ));  
  theInputTree->SetBranchAddress("ele27wp80",         &(myEvent.triggerEl    ));  
  theInputTree->SetBranchAddress("isopf1",            &(myEvent.leptonIso    )); 
  theInputTree->SetBranchAddress("eoverpin",          &(myEvent.leptonEpin   )); 
  theInputTree->SetBranchAddress("id1",               &(myEvent.leptonPDG    ));  
  theInputTree->SetBranchAddress("mini_met",          &(myEvent.MET          )); 
  theInputTree->SetBranchAddress("mini_mt",           &(myEvent.MT           ));
  theInputTree->SetBranchAddress("mini_njets",        &(myEvent.nJets        ));  
  theInputTree->SetBranchAddress("mini_nb",           &(myEvent.nB           ));  
  theInputTree->SetBranchAddress("mini_passisotrk",   &(myEvent.isoTrackVeto ));  
  theInputTree->SetBranchAddress("mini_passtauveto",  &(myEvent.tauVeto      ));  
  theInputTree->SetBranchAddress("mini_dphimjmin",    &(myEvent.dPhiMETjet   )); 
  theInputTree->SetBranchAddress("mini_chi2",         &(myEvent.hadronicChi2 )); 
  theInputTree->SetBranchAddress("mini_mt2w",         &(myEvent.MT2W         )); 
  theInputTree->SetBranchAddress("mini_pt_b",         &(myEvent.leadingBPt   )); 
  theInputTree->SetBranchAddress("mini_htratiom",     &(myEvent.HTratio      )); 
  theInputTree->SetBranchAddress("mini_dRleptB1",     &(myEvent.dRleptonB    ));
  theInputTree->SetBranchAddress("lep1",              &lepton                 );
  theInputTree->SetBranchAddress("pflep1",            &leptonPF               );
  theInputTree->SetBranchAddress("pfjets",            &jets                   ); 
  theInputTree->SetBranchAddress("pfjets_sigma",      &jets_sigma             ); 
  theInputTree->SetBranchAddress("pfjets_csv",        &jets_CSV               ); 
  theInputTree->SetBranchAddress("pfjets_csvreshape", &jets_CSVreshaped       ); 
  theInputTree->SetBranchAddress("pfjets_mva5xPUid",  &jets_PUId              ); 
  
  theInputTree->SetBranchAddress("pfmet",             &(myEvent.rawMET       )); 
  
  theInputTree->SetBranchAddress("mini_metup",        &(myEvent.MET_up       )); 
  theInputTree->SetBranchAddress("mini_metdown",      &(myEvent.MET_down     )); 
  theInputTree->SetBranchAddress("mini_mtup",         &(myEvent.MT_up        )); 
  theInputTree->SetBranchAddress("mini_mtdown",       &(myEvent.MT_down      )); 
  
  theInputTree->SetBranchAddress("mini_puweight",     &(myEvent.pileUpWeight )); 

  for (int i = 0 ; i < theInputTree->GetEntries() ; i++)
  {
	  // Get the i-th entry
      theInputTree->GetEntry(i); 
      
      myEvent.leptonPt     = lepton->Pt();
      myEvent.leptonEta    = lepton->Eta();
      myEvent.leptonPhi    = lepton->Phi();
      myEvent.leptonE      = lepton->E();
      myEvent.leptonPFPt   = leptonPF->Pt();

      for (int i = 0 ; i < 10 ; i++)
      {
          if (i < (*jets).size())
          {
              myEvent.jets_Pt[i]          = (*jets)[i].Pt();
              myEvent.jets_Phi[i]         = (*jets)[i].Phi();
              myEvent.jets_Eta[i]         = (*jets)[i].Eta();
              myEvent.jets_Sigma[i]       = (*jets_sigma)[i];
              myEvent.jets_CSV[i]         = (*jets_CSV)[i];
              myEvent.jets_CSVreshaped[i] = (*jets_CSVreshaped)[i];
              myEvent.jets_PUId[i]        = (*jets_PUId)[i];
          }
          else
          {
              myEvent.jets_Pt[i]          = -1;
              myEvent.jets_Phi[i]         = -999;
              myEvent.jets_Eta[i]         = -999;
              myEvent.jets_Sigma[i]       = -999;
              myEvent.jets_CSV[i]         = -999; 
              myEvent.jets_CSVreshaped[i] = -999;
              myEvent.jets_PUId[i]        = -999;
          }
      }

      theOutputTree->Fill();
  } 

  //theOutputTree->Print();
  theOutputTree->Write(0, TObject::kOverwrite); 

  cout << endl;
  cout << "   ┌──────────┐  " << endl;
  cout << "   │   Done   │  " << endl;
  cout << "   └──────────┘  " << endl; 
  cout << endl;

  return (0);

}



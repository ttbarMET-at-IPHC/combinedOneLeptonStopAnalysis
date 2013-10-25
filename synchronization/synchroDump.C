
#include <cmath>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TBranch.h>
#include <TLorentzVector.h>

#include <iostream>
using namespace std;

#include "../microTupling/MicroTuple_Format_Synchro0902.h"

#define INPUT "../store/microTuples_Synchro0902/fnal.root"

// #########################################################################
//                              Main function
// #########################################################################

int main (int argc, char *argv[])
{

  cout << endl;
  cout << "   ┌───────────────────┐  " << endl;
  cout << "   │   Starting dump   │  " << endl;
  cout << "   └───────────────────┘  " << endl; 
  cout << endl;

  microEvent myEvent;

  // Input tree   
  TFile fInput(INPUT,"UPDATE");
  TTree* theTree = (TTree*) fInput.Get("microTuple"); 
  theTree->SetBranchAddress("microEvents",&myEvent);

  // ########################################
  // ##        Run over the events         ##
  // ########################################

  for (int i = 0 ; i < theTree->GetEntries() ; i++)
  {
	  // Get the i-th entry
      theTree->GetEntry(i);

       cout << " ---------- " << endl;
       cout << "  event " << myEvent.event << endl;
       cout << " trigger (e,µ) : (" << myEvent.triggerEl << "," << myEvent.triggerMu << ")" << endl;
       cout << " lepton (pt,eta,phi,e) : (" << myEvent.leptonPt << "," << myEvent.leptonEta << "," << myEvent.leptonPhi << "," << myEvent.leptonE << ")" << endl;
       cout << "        (PFpt,iso,Epin) : (" << myEvent.leptonPFPt << "," << myEvent.leptonIso << "," << myEvent.leptonEpin << "," << myEvent.leptonPDG << ")" << endl;
       for (int i = 0 ; i < 10 ; i++)
       {
           if (myEvent.jets_Pt[i] == -1) break;
           cout << " jet: " << i << " pt: " << myEvent.jets_Pt[i] << " eta: " << myEvent.jets_Eta[i] << " phi: " << myEvent.jets_Phi[i] << " sigma: " << myEvent.jets_Sigma[i] << endl;
       }
       cout << " nJets : " << myEvent.nJets << " ; nB : " << myEvent.nB << endl;
       cout << " MET : " << myEvent.MET << " ; MT : " << myEvent.MT << " ; MT2W " << myEvent.MT2W << endl;
       cout << " dPhiMETjet : " << myEvent.dPhiMETjet << " ; chi2 : " << myEvent.hadronicChi2 << " HTRatio : " << myEvent.HTratio << endl;
  }

  return (0);

}




#include <cmath>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TBranch.h>
#include <TLorentzVector.h>

#include <iostream>
using namespace std;

#include "interface/SonicScrewdriver.h" 
using namespace theDoctor;

// Define format and input files
#include "../babyReader/Reader_prefinal1024.h"
#include "../microTupling/formats/Synchro1025.h"

#define INPUT_IPHC "synchronizationBaby1102.root"
#define INPUT_FNAL "../store/microTuples_Synchro1025/fnal.root"

bool inclusiveChannelSelector() { return true; }
bool Selection_presel() { return true; }

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

  // ####################
  // ##   Init tools   ##
  // ####################
  
	 // Create a sonic Screwdriver
 	 SonicScrewdriver mySonic;
 	 SonicScrewdriver mySonic_jets;

  // ##########################
  // ##   Create Variables   ##
  // ##########################

     float triggerE;           mySonic.AddVariable("triggerE",   "#Delta triggerE",        "", 3,-1.5,1.5,  &(triggerE),                "logY=true");
     float triggerM;           mySonic.AddVariable("triggerM",   "#Delta triggerM",        "", 3,-1.5,1.5,  &(triggerM),                "logY=true");
     
     float leptonPt;           mySonic.AddVariable("leptonPt",   "#Delta lepton p_{T}",    "", 21,-0.5,0.5, &(leptonPt),                "logY=true");
     float leptonEta;          mySonic.AddVariable("leptonEta",  "#Delta lepton #eta",     "", 21,-0.5,0.5, &(leptonEta),               "logY=true");
     float leptonPhi;          mySonic.AddVariable("leptonPhi",  "#Delta lepton #phi",     "", 21,-0.5,0.5, &(leptonPhi),               "logY=true");
     float leptonE;            mySonic.AddVariable("leptonE",    "#Delta lepton E",        "", 21,-0.5,0.5, &(leptonE),                 "logY=true");
     float nJets;              mySonic.AddVariable("nJets",      "#Delta nJets",           "", 7,-3.5,3.5,  &(nJets),                   "logY=true");
     float nBtag;              mySonic.AddVariable("nBTag",      "#Delta nBTag",           "", 7,-3.5,3.5,  &(nBtag),                   "logY=true");
     
     float MET;                mySonic.AddVariable("MET",        "#Delta MET",             "", 21,-0.5,0.5, &(MET),                     "logY=true");
     float METup;              mySonic.AddVariable("METup",      "#Delta MET_{up}",        "", 21,-0.5,0.5, &(METup),                   "logY=true");
     float METdown;            mySonic.AddVariable("METdown",    "#Delta MET_{down}",      "", 21,-0.5,0.5, &(METdown),                 "logY=true");
     float MT;                 mySonic.AddVariable("MT",         "#Delta MT",              "", 21,-0.5,0.5, &(MT),                      "logY=true");
     float MTup;               mySonic.AddVariable("MTup",       "#Delta MT_{up}",         "", 21,-0.5,0.5, &(MTup),                    "logY=true");
     float MTdown;             mySonic.AddVariable("MTdown",     "#Delta MT_{down}",       "", 21,-0.5,0.5, &(MTdown),                   "logY=true");
     float MT2W;               mySonic.AddVariable("MT2W",       "#Delta MT2W",            "", 21,-0.5,0.5, &(MT2W),                    "logY=true");
     float dPhiMETjet;         mySonic.AddVariable("dPhiMETjet", "#Delta dPhiMetJet",      "", 21,-0.5,0.5, &(dPhiMETjet),              "logY=true");
     float Chi2;               mySonic.AddVariable("Chi2",       "#Delta Chi2",            "", 21,-0.5,0.5, &(Chi2),                    "logY=true");
     float HTRatio;            mySonic.AddVariable("HTRatio",    "#Delta HTRatio",         "", 21,-0.5,0.5, &(HTRatio),                 "logY=true");
     
     float trackVeto;          mySonic.AddVariable("trackVeto",  "#Delta trackVeto",       "", 3,-1.5,1.5,  &(trackVeto),               "logY=true");
     float tauVeto;            mySonic.AddVariable("tauVeto",    "#Delta tauVeto",         "", 3,-1.5,1.5,  &(tauVeto),                 "logY=true");
     
     float jetPt;              mySonic_jets.AddVariable("pT",         "#Delta p_{T}",           "", 21,-1.5,1.5, &(jetPt),              "logY=true");
     float jetEta;             mySonic_jets.AddVariable("Eta",        "#Delta #eta",            "", 21,-1.5,1.5, &(jetEta),             "logY=true");
     float jetPhi;             mySonic_jets.AddVariable("Phi",        "#Delta #phi",            "", 21,-1.5,1.5, &(jetPhi),             "logY=true");
     float jetCSV;             mySonic_jets.AddVariable("CSV",        "#Delta CSV",             "", 21,-1.5,1.5, &(jetCSV),             "logY=true");
     float jetCSVreshaped;     mySonic_jets.AddVariable("CSVreshaped","#Delta CSV reshaped",    "", 21,-1.5,1.5, &(jetCSVreshaped),     "logY=true");
     
  // #########################################################
  // ##   Create ProcessClasses (and associated datasets)   ##
  // #########################################################

     mySonic.AddProcessClass("diff","UC/FNAL vs IPHC","background",COLORPLOT_AZURE);
     mySonic.AddRegion("presel","Preselection",&Selection_presel);  
	 mySonic.AddChannel("inclusiveChannel","",&inclusiveChannelSelector);
     
     mySonic_jets.AddProcessClass("diff","UC/FNAL vs IPHC","background",COLORPLOT_AZURE);
     mySonic_jets.AddRegion("presel","Preselection",&Selection_presel);  
	 mySonic_jets.AddChannel("inclusiveChannel","",&inclusiveChannelSelector);
  	 
  	 mySonic.Create1DHistos();
     mySonic.SchedulePlots("1DSuperpRenorm");
         
   	 mySonic_jets.Create1DHistos();
     mySonic_jets.SchedulePlots("1DSuperpRenorm");

  // #################################
  // ##        Fill the map         ##
  // #################################
  
     babyEvent  myEventIPHC;
     microEvent myEventFNAL;

     TFile fInputFNAL(INPUT_FNAL,"UPDATE");
     TTree* theTreeFNAL = (TTree*) fInputFNAL.Get("microTuple"); 
     theTreeFNAL->SetBranchAddress("microEvents",&myEventFNAL);

     TFile fInputIPHC(INPUT_IPHC,"UPDATE");
     TTree* theTreeIPHC = (TTree*) fInputIPHC.Get("babyTuple"); 
     intermediatePointers pointers;
     InitializeBranches(theTreeIPHC,&myEventIPHC,&pointers);

     vector<int> eventsIPHC;
     for (int i = 0 ; i < theTreeIPHC->GetEntries() ; i++) { theTreeIPHC->GetEntry(i); eventsIPHC.push_back(myEventIPHC.event); }

         int nReject = 0;
     vector<int> eventsFNAL;
     for (int i = 0 ; i < theTreeFNAL->GetEntries() ; i++) 
     { 

         theTreeFNAL->GetEntry(i); 
         eventsFNAL.push_back(myEventFNAL.event);

         int eventIPHC = -1;
         for (int j = 0 ; j < eventsIPHC.size() ; j++)
         {
             if (eventsIPHC[j] == myEventFNAL.event) { eventIPHC = j; break; }
         }

         if (eventIPHC == -1) continue;
         ReadEvent(theTreeIPHC,eventIPHC,&pointers,&myEventIPHC);

         triggerE   = (myEventFNAL.triggerEl    - myEventIPHC.triggerElec   );
         triggerM   = (myEventFNAL.triggerMu    - myEventIPHC.triggerMuon   );

         leptonPt   = (myEventFNAL.leptonPt     - myEventIPHC.leadingLepton.Pt()  ) / myEventFNAL.leptonPt    ;
         leptonEta  = (myEventFNAL.leptonEta    - myEventIPHC.leadingLepton.Eta() ) / myEventFNAL.leptonEta   ;
         leptonPhi  = (myEventFNAL.leptonPhi    - myEventIPHC.leadingLepton.Phi() ) / myEventFNAL.leptonPhi   ;
         leptonE    = (myEventFNAL.leptonE      - myEventIPHC.leadingLepton.E()   ) / myEventFNAL.leptonE     ;
         nJets      = (myEventFNAL.nJets        - myEventIPHC.nJets       );
         nBtag      = (myEventFNAL.nB           - myEventIPHC.nBTag       );

         MET        = (myEventFNAL.MET          - myEventIPHC.MET             ) / myEventFNAL.MET         ;
         METup      = (myEventFNAL.MET_up       - myEventIPHC.MET_JESup       ) / myEventFNAL.MET_up      ;
         METdown    = (myEventFNAL.MET_down     - myEventIPHC.MET_JESdown     ) / myEventFNAL.MET_down    ;
         MT         = (myEventFNAL.MT           - myEventIPHC.MT              ) / myEventFNAL.MT          ;
         MTup       = (myEventFNAL.MT_up        - myEventIPHC.MT_JESup        ) / myEventFNAL.MT_up       ;
         MTdown     = (myEventFNAL.MT_down      - myEventIPHC.MT_JESdown      ) / myEventFNAL.MT_down     ;
         MT2W       = (myEventFNAL.MT2W         - myEventIPHC.MT2W            ) / myEventFNAL.MT2W        ;
         dPhiMETjet = (myEventFNAL.dPhiMETjet   - myEventIPHC.deltaPhiMETJets ) / myEventFNAL.dPhiMETjet  ;
         Chi2       = (myEventFNAL.hadronicChi2 - myEventIPHC.hadronicChi2    ) / myEventFNAL.hadronicChi2;
         HTRatio    = (myEventFNAL.HTratio      - myEventIPHC.HTRatio         ) / myEventFNAL.HTratio     ;

         trackVeto  = (myEventFNAL.isoTrackVeto - myEventIPHC.isolatedTrackVeto); 
         tauVeto    = (myEventFNAL.tauVeto      - myEventIPHC.tauVeto          );      

         mySonic.AutoFillProcessClass("diff");

         cout << "Chi2 : " << myEventFNAL.hadronicChi2      - myEventIPHC.hadronicChi2        << endl;

         METup      = (myEventFNAL.MET_up       - myEventIPHC.MET_JESup       ) / myEventFNAL.MET_up      ;
         int jSelect = 0;
         for (int j = 0 ; jSelect < myEventIPHC.nJets ; j++)
         {
//             if     (abs(myEventFNAL.jets_Eta[j]) < 2.5)
//                     if (myEventFNAL.jets_PUId[j] <  0.41) { nReject++; continue; }
//             else    
//                     if (myEventFNAL.jets_PUId[j] < -0.10) { nReject++; continue; }

             jSelect++;
             bool found = false;
             for (int k = 0 ; k < 10 ; k++)
             {
                 jetEta         = myEventFNAL.jets_Eta[k]         - myEventIPHC.jets[j].Eta();  
                 jetPhi         = myEventFNAL.jets_Phi[k]         - myEventIPHC.jets[j].Phi(); 

                 if ((abs(jetEta) < 0.05) && (abs(jetPhi) < 0.05))
                 {
                     jetPt          = myEventFNAL.jets_Pt[k]          - myEventIPHC.jets[j].Pt(); 
                     jetCSV         = myEventFNAL.jets_CSV[k]         - myEventIPHC.jets_CSV_raw[j]; 
                     jetCSVreshaped = myEventFNAL.jets_CSVreshaped[k] - myEventIPHC.jets_CSV_reshaped[j]; 
                     mySonic_jets.AutoFillProcessClass("diff");
                     found = true;
                     break;
                 }
             }

             if (!found)
             {
                 for (int l = 0 ; l < max((int) myEventFNAL.nJets,(int) myEventIPHC.nJets) ; l++)
                 {
                     cout << "l = " << l << " ; UC/IPHC ; pT : " << myEventFNAL.jets_Pt[l]  << "/" << myEventIPHC.jets[l].Pt()
                         << " eta : " << myEventFNAL.jets_Eta[l] << "/" << myEventIPHC.jets[l].Eta()
                         << " phi : " << myEventFNAL.jets_Phi[l] << "/" << myEventIPHC.jets[l].Phi()
                         << " puid : " << myEventFNAL.jets_PUId[l]
                         << endl;

                 }
             }
         }
     }

     cout << "   > Making plots..." << endl;
     mySonic.MakePlots();
     mySonic_jets.MakePlots();
     cout << "   > Saving plots..." << endl;
     mySonic.WritePlots("../plots/checkBabySynchro/","");
     mySonic_jets.WritePlots("../plots/checkBabySynchro_jets/","");


     return (0);

}



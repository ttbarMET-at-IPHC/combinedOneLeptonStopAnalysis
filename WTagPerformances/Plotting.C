
#include <cmath>
#include <TTree.h>
#include <TBranch.h>
#include <TLorentzVector.h>

using namespace std;


#include "interface/Table.h" 
#include "interface/SonicScrewdriver.h" 
using namespace theDoctor;

#define FOLDER_MICROTUPLES "../store/microTuples_WTag0925/"
#include "../microTupling/MicroTuple_Format_WTag0925.h" 
microEvent* myEventPointer;

bool WTagged(float mass, float pT, float tau2OverTau1,float dRlep, float dRbjet)
{
    if ((mass > 60) && (mass < 130) && (dRlep > 0.6) && (dRbjet > 0.6))
         return true;
    else return false;
}

float deltaR(float genEta, float genPhi, float recoEta, float recoPhi)
{
    float dEta = genEta - recoEta;
    float dPhi = TVector2::Phi_mpi_pi(genPhi - recoPhi);
    float dR = sqrt(dEta*dEta + dPhi*dPhi);
    return dR;
}

// #########################################################################
//                          Region selectors
// #########################################################################

bool inclusiveChannelSelector() { return true; }

bool Selection_presel() { return true; }

float* massPointer;
float* pTPointer;
bool Selection_pTAndMass() 
{ 
    if ((*massPointer > 60) && (*massPointer < 130) && (*pTPointer > 200))
        return true; 
    else
        return false;
}

// #########################################################################
//                              Main function
// #########################################################################

int main (int argc, char *argv[])
{

  cout << endl;
  cout << "   ┌──────────────────────────────┐  " << endl;
  cout << "   │   Starting plot generation   │  " << endl;
  cout << "   └──────────────────────────────┘  " << endl; 
  cout << endl;

  // ####################
  // ##   Init tools   ##
  // ####################
  
	 // Create a sonic Screwdriver
 	 SonicScrewdriver mySonic;

  	 // Create a container for the event
	 microEvent myEvent;
	 myEventPointer = &myEvent;

  // ##########################
  // ##   Create Variables   ##
  // ##########################

     float genW_pT;                   mySonic.AddVariable("PtGen",                   "p_{T}(gen W)",                          "GeV",    24,0,600,  &(genW_pT),                  "");
     float genW_hasMatchedCA8;        mySonic.AddVariable("genW_hasMatchedCA8",        "gen W has a matched CA8 jet",           "",     2,-0.5,1.5,  &(genW_hasMatchedCA8),       "");
     float genW_hasMatchedTaggedCA8;  mySonic.AddVariable("genW_hasMatchedTaggedCA8",  "gen W has a matched tagged CA8 jet",    "",     2,-0.5,1.5,  &(genW_hasMatchedTaggedCA8), "");
     float CA8jet_mass;      mySonic.AddVariable("mass",       "mass(CA8 jet)",                         "GeV",    20,0,200,  &(CA8jet_mass),     "");
     float CA8jet_pT;        mySonic.AddVariable("Pt",         "p_{T}(CA8 jet)",                        "GeV",    24,0,600,  &(CA8jet_pT),       "");
     float CA8jet_dR;        mySonic.AddVariable("dR",         "#DeltaR(CA8 jet,gen W)",                   "",      40,0,2,  &(CA8jet_dR),       "");
     float CA8jet_dRlep;     mySonic.AddVariable("dRlep",      "#DeltaR(CA8 jet,sel. l)",                  "",      40,0,2,  &(CA8jet_dRlep),    "");
     float CA8jet_dRbjet;    mySonic.AddVariable("dRbjet",     "#DeltaR(CA8 jet,b jet)",                   "",      40,0,2,  &(CA8jet_dRbjet),    "");
     float CA8jet_tagged;    mySonic.AddVariable("tagged",     "W-tag flag",                               "",  2,-0.5,1.5,  &(CA8jet_tagged),   "");
     float deltaPtRel;       mySonic.AddVariable("deltaPtRel", "p_{T}(CA8 jet) - p_{T}(gen W) / p_{T}(gen W)",  "", 20,-0.3,0.3,  &(deltaPtRel), "");
     float subjetinessRatio; mySonic.AddVariable("subjetinessRatio", "#tau_{2} / #tau_{1}",                    "",      20,0,1,  &(subjetinessRatio),"");
  
     massPointer = &CA8jet_mass;
     pTPointer   = &CA8jet_pT;

  // #########################################################
  // ##   Create ProcessClasses (and associated datasets)   ##
  // #########################################################

      mySonic.AddProcessClass("matched","matched #DeltaR<0.1","background",COLORPLOT_ORANGE);
      mySonic.AddProcessClass("fakes",  "fakes",  "background",COLORPLOT_AZURE);
      mySonic.AddProcessClass("gen",    "gen",    "data",COLORPLOT_GREEN);

  // #######################################
  // ##    Create Regions and channels    ##
  // #######################################

     mySonic.AddRegion("presel","Preselection",&Selection_presel);  
     mySonic.AddRegion("pT > 200, 60 < mass < 130","",&Selection_pTAndMass);  
	 mySonic.AddChannel("inclusiveChannel","",&inclusiveChannelSelector);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################
  
	 // Create histograms
  	 mySonic.Create1DHistos();

  	 mySonic.Add2DHisto("Pt","tagged");
  	 mySonic.Add2DHisto("Pt","mass");
  	 mySonic.Add2DHisto("dR","mass");
  	 mySonic.Add2DHisto("dRlep","mass");
     mySonic.Add2DHisto("PtGen","genW_hasMatchedCA8");
     mySonic.Add2DHisto("PtGen","genW_hasMatchedTaggedCA8");

  	 // Schedule plots
  	 mySonic.SchedulePlots("1DSuperpRenorm");
  	 mySonic.SchedulePlots("2D");
    
     // (Mis)tagging efficiency plots
     mySonic.SchedulePlots("2DProjectedTo1D","varX=Pt,varY=tagged,projectionType=mean,labelY=Efficiency");

     mySonic.SchedulePlots("2DProjectedTo1D","varX=PtGen,varY=genW_hasMatchedCA8,projectionType=mean,labelY=hasMatched");
     mySonic.SchedulePlots("2DProjectedTo1D","varX=PtGen,varY=genW_hasMatchedTaggedCA8,projectionType=mean,labelY=Efficiency");

  // ########################################
  // ##       Run over the datasets        ##
  // ########################################

     // Open the tree
     TFile f((string(FOLDER_MICROTUPLES)+"ttbar.root").c_str());
     TTree* theTree = (TTree*) f.Get("microTuple"); 
     theTree->SetBranchAddress("microEvents",&myEvent);
    
  // ########################################
  // ##        Run over the events         ##
  // ########################################

     int numberOfCA8JetHighPtMatched = 0;
     int numberOfCA8JetHighPtMatchedTagged = 0;

  for (int i = 0 ; i < theTree->GetEntries() ; i++)
  {
      if (i % 10000 == 0) cout << "i = " << i << endl;
	  // Get the i-th entry
      theTree->GetEntry(i);

      if (myEvent.genW_pT == -1.0)    continue;

      genW_pT = -1;
      genW_hasMatchedCA8 = -1;
      genW_hasMatchedTaggedCA8 = -1;

      bool foundMatchedCA8 = false;
      bool foundMatchedTaggedCA8 = false;
      for (int j = 0 ; j < 10 ; j++)
      {
        if (myEvent.recoW_pT[j] == 0.0) break;
        if (abs(myEvent.recoW_pT[j]) > 2000.0) break;

         CA8jet_mass = myEvent.recoW_Mass[j];
         CA8jet_pT   = myEvent.recoW_pT[j] ;
         CA8jet_dR   = deltaR(myEvent.genW_Eta,
                              myEvent.genW_Phi,
                              myEvent.recoW_Eta[j],
                              myEvent.recoW_Phi[j]);
         
         subjetinessRatio = myEvent.recoW_Tau2[j] / myEvent.recoW_Tau1[j];

         if (CA8jet_dR > 1.99) CA8jet_dR = 1.99;

         CA8jet_dRlep = deltaR(myEvent.lep_eta,
                               myEvent.lep_phi,
                               myEvent.recoW_Eta[j],
                               myEvent.recoW_Phi[j]);
         if (CA8jet_dRlep > 1.99) CA8jet_dRlep = 1.99;

         CA8jet_dRbjet = 1.99;
         
         for (int k = 0 ; k < myEvent.nJets ; k++)
         {
            if (myEvent.jet_bTag[k] != 1.0) continue;

            float deltaRTemp = deltaR(myEvent.jet_eta[k],
                                      myEvent.jet_phi[k],
                                      myEvent.recoW_Eta[j],
                                      myEvent.recoW_Phi[j]);
            
            if (CA8jet_dRbjet > deltaRTemp)
                CA8jet_dRbjet = deltaRTemp;
         }

         CA8jet_tagged = WTagged(CA8jet_mass,CA8jet_pT,subjetinessRatio,CA8jet_dRlep,CA8jet_dRbjet);
         deltaPtRel = (CA8jet_pT - myEvent.genW_pT) / myEvent.genW_pT;


         if (CA8jet_dR < 0.1) 
         {
             mySonic.AutoFillProcessClass("matched");
             foundMatchedCA8 = true;
             if (CA8jet_tagged >= 1.0) foundMatchedTaggedCA8 = true;
         
             if (CA8jet_pT >= 200)
             {
                numberOfCA8JetHighPtMatched++;
                if (CA8jet_tagged >= 1.0) numberOfCA8JetHighPtMatchedTagged++;
             }
         }
         else 
         {
             mySonic.AutoFillProcessClass("fakes");
         }

      }

      genW_pT = myEvent.genW_pT;
      if (foundMatchedCA8) genW_hasMatchedCA8 = 1;
      else                 genW_hasMatchedCA8 = 0;

      if (foundMatchedTaggedCA8) genW_hasMatchedTaggedCA8 = 1;
      else                       genW_hasMatchedTaggedCA8 = 0;
      
      mySonic.AutoFillProcessClass("gen");

  } 

  cout << "numberOfCA8JetHighPtMatched = " << numberOfCA8JetHighPtMatched << endl;
  cout << "numberOfCA8JetHighPtMatchedTagged = " <<numberOfCA8JetHighPtMatchedTagged << endl;
  cout << "ratio = " << (float) numberOfCA8JetHighPtMatched / (float) numberOfCA8JetHighPtMatchedTagged << endl;


  // ###################################
  // ##   Make plots and write them   ##
  // ###################################
 
  cout << "   > Making plots..." << endl;
  mySonic.MakePlots();
  cout << "   > Saving plots..." << endl;
  //mySonic.WritePlots("../plots/WTagPerformances/","","exportPngAndEps=true");
  mySonic.WritePlots("../plots/WTagPerformances/","");

  cout << endl;
  cout << "   ┌──────────────────────────────┐  " << endl;
  cout << "   │   Plot generation completed  │  " << endl;
  cout << "   └──────────────────────────────┘  " << endl; 
  cout << endl;

  return (0);

}



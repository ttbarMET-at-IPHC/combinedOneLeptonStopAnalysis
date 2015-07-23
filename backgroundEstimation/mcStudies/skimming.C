#define USE_GEN_INFO
//#define USE_GEN_INFO_EXT
#define USE_LEP1
//#define USE_LEP1_EXT
#define USE_LEP2
//#define USE_LEP2_EXT
#define USE_JETS
//#define USE_JETS_EXT
#define USE_PV
#define USE_WEIGHTS
#define USE_VAR_BASELINE
#define USE_GLOBAL_VAR
//#define USE_OLD_VAR

#include <algorithm>

#include "Format.h"
#include "../common/common.h"
#include "topness_tt2l_mc_reso.h"


//#define FOLDER_INPUT  "/opt/sbg/data/data1/cms/echabert/Stop2015/BabyTuples/v1_30_06/"
//#define FOLDER_OUTPUT "/opt/sbg/data/data1/cms/echabert/Stop2015/BabyTuples/v1_30_06/skimming_noMETcut/"

#define FOLDER_INPUT  "/opt/sbg/data/data1/cms/echabert/Stop2015/BabyTuples/v1_20_07/"
#define FOLDER_OUTPUT "/opt/sbg/data/data1/cms/echabert/Stop2015/BabyTuples/v1_20_07/skimming/"

// #########################################################################
//                              Main function
// #########################################################################

int main (int argc, char *argv[])
{
    if (argc <= 1)
    {
        cout << "No dataset specified in argument" << endl;
        return -1;
    }

    string dataset = argv[1];

    // ################################
    // ##       Open the tree        ##
    // ################################

    // Input tree
    TFile fInput((FOLDER_INPUT+dataset+".root").c_str(),"READ");
    TTree* theInputTree = (TTree*) fInput.Get("babyTuple");
    InitializeBranchesForReading(theInputTree,&myEvent);
    // Output tree
    TFile fOutput((FOLDER_OUTPUT+dataset+".root").c_str(),"RECREATE");
    TTree* theOutputTree = new TTree("babyTuple","");
    InitializeBranchesForWritting(theOutputTree,&myEvent);


     // ##########################
     // ##   Create Variables   ##
     // ##########################

     tt2l_mc_info mcinfo;
     tt2l_reso res;
     float minDPhi_jmet;
    
    // #########################
    // ##   Add var. in branch #
    // #########################
    theOutputTree->Branch("genMET", &mcinfo.genMET); 
    theOutputTree->Branch("Pz_ttbar", &mcinfo.Pz_ttbar); 
    theOutputTree->Branch("Pt_ttbar", &mcinfo.Pt_ttbar); 
    theOutputTree->Branch("E_ttbar", &mcinfo.CM_Q2); 
    theOutputTree->Branch("M_ttbar", &mcinfo.CM_M2); 
    theOutputTree->Branch("ttbar_decay", &mcinfo.ttbar_decay); 
    theOutputTree->Branch("nofISR", &res.nofISR); 
    theOutputTree->Branch("index_b", &res.index_b); 
    theOutputTree->Branch("index_bbar", &res.index_bbar); 
    theOutputTree->Branch("matched", &res.matched); 
    theOutputTree->Branch("isr_id", "std::vector<Int_t>", &mcinfo.isr_id); 
    theOutputTree->Branch("isr_p4", "std::vector<TLorentzVector>", &mcinfo.isr_p4); 
    theOutputTree->Branch("neutralinos", "std::vector<TLorentzVector>", &mcinfo.neutralinos); 
    theOutputTree->Branch("neutrinos", "std::vector<TLorentzVector>", &mcinfo.neutrinos); 
    theOutputTree->Branch("minDPhi_jmet",&minDPhi_jmet); 


      int nEntries = theInputTree->GetEntries();
      //int nEntries = 1000;
      for (int i = 0 ; i < nEntries ; i++)
      {
          if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,dataset);

          // Get the i-th entry
          ReadEvent(theInputTree,i,&myEvent);

          //Topness study
          if(goesInBaselineSearchSR()){
          	ResetMCInfo(mcinfo);
		ResetRes(res);
		Fill_tt2l_mc_info(mcinfo,myEvent);
          	if(mcinfo.top_lepdecay && mcinfo.atop_lepdecay){
          		fill_t2l_reso(res, mcinfo,myEvent);
          	}
        	//compute minDPhi_jmet
		minDPhi_jmet = 9999;
		if(myEvent.jet_pt.size()>=2){
			minDPhi_jmet = abs(TVector2::Phi_mpi_pi(myEvent.pfmet_phi-myEvent.jet_phi[0]));
			float tmp = TVector2::Phi_mpi_pi(myEvent.pfmet_phi-myEvent.jet_phi[1]);
			if(abs(tmp)<abs(minDPhi_jmet)) minDPhi_jmet = abs(tmp);
		}
		cout<<"fill the tree !"<<endl;
		theOutputTree->Fill();
          }
      }
    cout << endl;
    theOutputTree->Write(0, TObject::kOverwrite);

    return (0);
  
}


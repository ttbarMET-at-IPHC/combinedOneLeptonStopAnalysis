#ifndef FORMAT_H
#define FORMAT_H

#include "../common/common.h"

void InitializeBranchesForWritting(TTree* theTree, babyEvent* myEvent)
{
    
    //not in the common format but needed
    //theTree->Branch("scale1fb",                &(myEvent->scale1fb));
    //theTree->Branch("crossSection",            &(myEvent->crossSection));
    ////////////////
    //
    //used for weight
    theTree->Branch("totalNumberOfInitialEvent", &(myEvent->totalNumberOfInitialEvent));

    // multiplicities
    theTree->Branch("nvetoleps",               &(myEvent->nvetoleps));
    theTree->Branch("ngoodjets",               &(myEvent->ngoodjets));
    theTree->Branch("ngoodbtags",              &(myEvent->ngoodbtags));
    theTree->Branch("ngoodleps",               &(myEvent->ngoodleps));
    
    // vetoes
    theTree->Branch("PassTauVeto", &myEvent->PassTauVeto);
    theTree->Branch("PassTrackVeto", &myEvent->PassTrackVeto);
    
    //gen info
    theTree->Branch("genlepsfromtop",          &(myEvent->genlepsfromtop));
    
    //trigger
    theTree->Branch("HLT_SingleMu",            &(myEvent->HLT_SingleMu));
    theTree->Branch("HLT_SingleE",             &(myEvent->HLT_SingleE));
    
    
    //needed to define the channel !
    theTree->Branch("lep1_pdgid",              &(myEvent->lep1_pdgid));
    #ifdef USE_LEP1
    theTree->Branch("lep1_pt",                 &(myEvent->lep1_pt));
    theTree->Branch("lep1_eta",                &(myEvent->lep1_eta));
    theTree->Branch("lep1_phi",                &(myEvent->lep1_phi));
    theTree->Branch("lep1_mass",               &(myEvent->lep1_mass));
    #endif
    #ifdef USE_LEP1_EXT
    theTree->Branch("lep1_d0",                 &(myEvent->lep1_d0));
    theTree->Branch("lep1_dz",                 &(myEvent->lep1_dz));
    theTree->Branch("lep1_passVeto",           &(myEvent->lep1_passVeto));
    theTree->Branch("lep1_passMediumID",       &(myEvent->lep1_passMediumID));
    theTree->Branch("lep1_MiniIso",            &(myEvent->lep1_MiniIso));
    #endif
    #ifdef USE_LEP2
    theTree->Branch("lep2_pt",                 &(myEvent->lep2_pt));
    theTree->Branch("lep2_eta",                &(myEvent->lep2_eta));
    theTree->Branch("lep2_phi",                &(myEvent->lep2_phi));
    theTree->Branch("lep2_mass",               &(myEvent->lep2_mass));
    theTree->Branch("lep2_pdgid",              &(myEvent->lep2_pdgid));
    #endif
    #ifdef USE_LEP2_EXT
    theTree->Branch("lep2_d0",                 &(myEvent->lep2_d0));
    theTree->Branch("lep2_dz",                 &(myEvent->lep2_dz));
    theTree->Branch("lep2_passVeto",           &(myEvent->lep2_passVeto));
    theTree->Branch("lep2_passMediumID",       &(myEvent->lep2_passMediumID));
    theTree->Branch("lep2_MiniIso",            &(myEvent->lep2_MiniIso));
    #endif

    #ifdef USE_JETS
    theTree->Branch("ak4pfjets_pt",            "std::vector<Float_t>", &(myEvent->pointerForak4pfjets_pt));
    theTree->Branch("ak4pfjets_phi",           "std::vector<Float_t>", &(myEvent->pointerForak4pfjets_phi));
    theTree->Branch("ak4pfjets_eta",           "std::vector<Float_t>", &(myEvent->pointerForak4pfjets_eta));
    theTree->Branch("ak4pfjets_mass",          "std::vector<Float_t>", &(myEvent->pointerForak4pfjets_mass));
    theTree->Branch("ak4pfjets_CSV",           "std::vector<Float_t>", &(myEvent->pointerForak4pfjets_CSV));
    #endif
    #ifdef USE_JETS_EXT
    theTree->Branch("ak4pfjets_puid",          "std::vector<Float_t>", &(myEvent->pointerForak4pfjets_puid));
    theTree->Branch("ak4pfjets_loose_pfid",    "std::vector<Float_t>", &(myEvent->pointerForak4pfjets_loose_pfid));
    theTree->Branch("dphi_ak4pfjets_met",      "std::vector<Float_t>", &(myEvent->dphi_ak4pfjets_met));
    theTree->Branch("ak4pfjets_rho",           &(myEvent->ak4pfjets_rho));
    #endif

    #ifdef USE_PV
    theTree->Branch("pv_ndof",                 &(myEvent->pv_ndof));
    theTree->Branch("pv_isFake",               &(myEvent->pv_isFake));
    theTree->Branch("pv_rho",                  &(myEvent->pv_rho));
    theTree->Branch("pv_z",                    &(myEvent->pv_z));
    #endif 

    #ifdef USE_WEIGHTS
    theTree->Branch("lep_sf",                  &(myEvent->lep_sf));
    theTree->Branch("btag_sf",                 &(myEvent->btag_sf));
    theTree->Branch("pu_weight",               &(myEvent->pu_weight));
    #endif

    #ifdef USE_VAR_BASELINE
    theTree->Branch("pfmet_phi",               &(myEvent->pfmet_phi));
    theTree->Branch("pfmet",                   &(myEvent->pfmet));
    theTree->Branch("MT2W",                    &(myEvent->MT2W));
    theTree->Branch("mt_met_lep",              &(myEvent->mt_met_lep));
    theTree->Branch("hadronic_top_chi2",       &(myEvent->hadronic_top_chi2));
    theTree->Branch("dphi_Wlep",               &(myEvent->dphi_Wlep));
    #endif

    #ifdef USE_GLOBAL_VAR
    theTree->Branch("ak4_htssm",               &(myEvent->ak4_htssm));
    theTree->Branch("ak4_htosm",               &(myEvent->ak4_htosm));
    theTree->Branch("ak4_HT",                  &(myEvent->ak4_HT));
    theTree->Branch("dR_lep_leadb",            &(myEvent->dR_lep_leadb));
    theTree->Branch("Mjjj",                    &(myEvent->Mjjj));
    theTree->Branch("Mlb",                     &(myEvent->Mlb));
    theTree->Branch("Mlb_leadb",               &(myEvent->Mlb_leadb));
    theTree->Branch("topness",                 &(myEvent->topness));
    #endif

   
   //*/
    #ifdef USE_GEN_INFO
    theTree->Branch("gen_pt", &(myEvent->pointerForgen_pt));
    theTree->Branch("gen_eta",  &(myEvent->pointerForgen_eta));
    theTree->Branch("gen_phi",  &(myEvent->pointerForgen_phi));
    theTree->Branch("gen_m", &(myEvent->pointerForgen_m));
    theTree->Branch("gen_status", &(myEvent->pointerForgen_status));
    theTree->Branch("gen_id",  &(myEvent->pointerForgen_id));
    theTree->Branch("gen_daughter_n",  &(myEvent->pointerForgen_daughter_n));
    theTree->Branch("gen_daughter_index", &(myEvent->pointerForgen_daughter_index));
    #endif
    #ifdef USE_GEN_INFO_EXT
    theTree->Branch("gen_mother_index",  &(myEvent->pointerForgen_mother_index));
    theTree->Branch("gen_charge",  &(myEvent->pointerForgen_charge));
    theTree->Branch("gen_index",  &(myEvent->pointerForgen_index));
    #endif
}


#endif

#ifndef babyFormat
#define babyFormat

typedef struct 
{
    // ------------
    // General info
    // ------------

    Int_t run;                                  // Run number
    Int_t lumi;                                 // Lumiblock number
    Int_t event;                                // Event number

    // ------------
    // Trigger info
    // ------------
    
    Bool_t triggerMuon;                         // Single-Muon trigger has been fired
    Bool_t xtriggerMuon;                        // Muon cross-trigger  has been fired
    Bool_t triggerElec;                         // Single-Elec trigger has been fired
    
    Bool_t triggerDoubleElec;                   // Double-Elec trigger has been fired
    Bool_t triggerDoubleMuon;                   // Double-Muon trigger has been fired
    Bool_t triggerMuonElec;                     // Muon-Elec   trigger has been fired

    // ------------
    // Leptons info
    // ------------

    Short_t        numberOfLepton;              // Number of selected leptons
    TLorentzVector leadingLepton;               // p4 of the leading selected lepton
    Short_t        leadingLeptonPDGId;          // pdgid of the leading selected lepton
    TLorentzVector secondLepton;                // p4 of the second lepton
    Short_t        secondLeptonPDGId;           // pdgid of the second lepton

    Bool_t         isolatedTrackVeto;           // Event pass/fail the isolated track veto
    Bool_t         tauVeto;                     // Event pass/fail the tau veto
    
    // ------------
    // Jets info
    // ------------

    Short_t                nJets;               // Number of selected jets
    Short_t                nBTag;               // Number of selected jets b-tagged
    vector<TLorentzVector> jets;                // p4 of the selected jets
    vector<Float_t>        jets_CSV_raw;        // CSV value of the selected jets, before reshaping 
    vector<Float_t>        jets_CSV_reshaped;   // CSV value of the selected jets, after reshaping
    vector<Int_t>          jets_partonFlav;     // Parton flavour of the selected jets

    // ------------
    // "High-level" /BDT variables
    // ------------

    Float_t MET;                                // Type-1 + phi-corrected PF MET
    Float_t MT;                                 // transverse mass of leading lepton + MET
    Float_t deltaPhiMETJets;                    // DeltaPhi(MET,first two leading jets)
    Float_t hadronicChi2;                       // Hadronic Chi^2
    Float_t MT2W;                               // MT2W
    Float_t HT;                                 // HT
    Float_t HTRatio;                            // Fraction of HT in the same hemisphere as MET
    Float_t leadingBPt;                         // pT of the leading b-jet
    Float_t leadingLeptonPt;                    // pT of the leading lepton
    Float_t leadingJetPt;                       // pT of the leading jet
    Float_t Mlb;                                // invariant mass of leading lepton + jet with highest CSV
    Float_t Mlb_hemi;                           // invariant mass of leading lepton + b-jet closest to lepton
    Float_t M3b;                                // invariant mass of the 3jets dR-furthest from lepton
    Float_t deltaRLeptonLeadingB;               // DeltaR(leading lepton, leading b-jet)
    Float_t METoverSqrtHT;                      // MET/sqrt(HT)
    Float_t HTPlusLeptonPtPlusMET;              // HT + pT(leading lepton) + MET

    // ------------
    // W-tagging info
    // ------------

    Short_t nWTag;                              // Number of W-tagged jets
    Float_t leadingWjetPt;                      // pT of the leading W-tagged jet

    // ------------
    // Signal-specific infos
    // ------------

    Float_t mStop;                              // Stop mass
    Float_t mNeutralino;                        // Neutralino mass
    Float_t mCharginoParameter;                 // Chargino mass parameter for T2bw (0.25, 0.5 or 0.75)
 
    // ------------
    // Generator-level infos
    // ------------
   
    Int_t   numberOfGenLepton;                  // Number of MC leptons (e/mu/tau, leptons from tau decay are not taken into account)

    vector<TLorentzVector> genParticles;        // p4 of status=3 particles
    vector<Int_t>          genParticlesPDGId;   // PDG of status=3 particles
    vector<Int_t>          genParticlesMother;  // Mother index of the status=3 particles

    // ------------
    // Weights and infos related to weights
    // ------------

    Int_t   numberOfInitialEvents;              // Number of events for the given dataset or signal mass point, before any skimming/selection applied
    Float_t crossSection;                       // Cross section for the given dataset or signal mass point
    Int_t   numberOfTruePU;                     // Number of true pile-up (MC, used in weightPileup)
    Int_t   numberOfPrimaryVertices;            // Number of primary vertices, used to check pile-up data/MC aggreement

    Float_t weightCrossSection;                 // = (crossSection / numberOfInitialEvents) ; user should multiply this weight by the desired lumi
    Float_t weightPileUp;                       // Weight for PU-reweighting
    Float_t weightISRmodeling;                  // Weight for ISR modeling
    Float_t weightTopPt;                        // Weight for top Pt reweighting
    Float_t weightTriggerEfficiency;            // Weight for singleLepton trigger efficiency

    Float_t weightT2ttLeftHanded;               // Polarization reweighting
    Float_t weightT2ttRightHanded;              // Polarization reweighting
    Float_t weightT2bwPolarization_ss;          // Polarization reweighting
    Float_t weightT2bwPolarization_ls;          // Polarization reweighting
    Float_t weightT2bwPolarization_sl;          // Polarization reweighting
    Float_t weightT2bwPolarization_sr;          // Polarization reweighting
    Float_t weightT2bwPolarization_rs;          // Polarization reweighting
    Float_t weightT2bwPolarization_ll;          // Polarization reweighting
    Float_t weightT2bwPolarization_lr;          // Polarization reweighting
    Float_t weightT2bwPolarization_rl;          // Polarization reweighting
    Float_t weightT2bwPolarization_rr;          // Polarization reweighting

    // ------------
    // Infos for systematic effects
    // ------------

        // Jet energy scale

    Float_t MET_JESup;                          // Type-1 + phi-corrected PF MET                              (+1sigma JES applied)
    Float_t MT_JESup;                           // transverse mass of leading lepton + MET                    (+1sigma JES applied)
    Float_t deltaPhiMETJets_JESup;              // DeltaPhi(MET,first two leading jets)                       (+1sigma JES applied)
    Float_t hadronicChi2_JESup;                 // Hadronic Chi^2                                             (+1sigma JES applied)
    Float_t MT2W_JESup;                         // MT2W                                                       (+1sigma JES applied)
    Float_t HT_JESup;                           // HT                                                         (+1sigma JES applied)
    Float_t HTRatio_JESup;                      // Fraction of HT in the same hemisphere as MET               (+1sigma JES applied)
    Float_t leadingBPt_JESup;                   // pT of the leading b-jet                                    (+1sigma JES applied)
    Float_t leadingJetPt_JESup;                 // pT of the leading jet                                      (+1sigma JES applied)
    Float_t Mlb_JESup;                          // invariant mass of leading lepton + jet with highest CSV    (+1sigma JES applied)
    Float_t Mlb_hemi_JESup;                     // invariant mass of leading lepton + b-jet closest to lepton (+1sigma JES applied)
    Float_t M3b_JESup;                          // invariant mass of the 3jets dR-furthest from lepton        (+1sigma JES applied)
    Float_t deltaRLeptonLeadingB_JESup;         // DeltaR(leading lepton, leading b-jet)                      (+1sigma JES applied)
    Float_t METoverSqrtHT_JESup;                // MET/sqrt(HT)                                               (+1sigma JES applied)
    Float_t HTPlusLeptonPtPlusMET_JESup;        // HT + pT(leading lepton) + MET                              (+1sigma JES applied)
    Short_t nJets_JESup;                        // Jet multiplicity with                                      (+1sigma JES applied)
    Float_t weightTriggerEfficiency_JESup;      // Weight for singleLepton trigger efficiency                 (+1sigma JES applied)

    Float_t MET_JESdown;                        // Type-1 - phi-corrected PF MET                              (-1sigma JES applied)
    Float_t MT_JESdown;                         // transverse mass of leading lepton - MET                    (-1sigma JES applied)
    Float_t deltaPhiMETJets_JESdown;            // DeltaPhi(MET,first two leading jets)                       (-1sigma JES applied)
    Float_t hadronicChi2_JESdown;               // Hadronic Chi^2                                             (-1sigma JES applied)
    Float_t MT2W_JESdown;                       // MT2W                                                       (-1sigma JES applied)
    Float_t HT_JESdown;                         // HT                                                         (-1sigma JES applied)
    Float_t HTRatio_JESdown;                    // Fraction of HT in the same hemisphere as MET               (-1sigma JES applied)
    Float_t leadingBPt_JESdown;                 // pT of the leading b-jet                                    (-1sigma JES applied)
    Float_t leadingJetPt_JESdown;               // pT of the leading jet                                      (-1sigma JES applied)
    Float_t Mlb_JESdown;                        // invariant mass of leading lepton - jet with highest CSV    (-1sigma JES applied)
    Float_t Mlb_hemi_JESdown;                   // invariant mass of leading lepton - b-jet closest to lepton (-1sigma JES applied)
    Float_t M3b_JESdown;                        // invariant mass of the 3jets dR-furthest from lepton        (-1sigma JES applied)
    Float_t deltaRLeptonLeadingB_JESdown;       // DeltaR(leading lepton, leading b-jet)                      (-1sigma JES applied)
    Float_t METoverSqrtHT_JESdown;              // MET/sqrt(HT)                                               (-1sigma JES applied)
    Float_t HTPlusLeptonPtPlusMET_JESdown;      // HT - pT(leading lepton) - MET                              (-1sigma JES applied)
    Short_t nJets_JESdown;                      // Jet multiplicity with                                      (-1sigma JES applied)
    Float_t weightTriggerEfficiency_JESdown;    // Weight for singleLepton trigger efficiency                 (-1sigma JES applied)

        // CSV reshaping
        
    vector<Float_t> jets_CSV_reshapedUpBC;
    vector<Float_t> jets_CSV_reshapedDownBC;
    vector<Float_t> jets_CSV_reshapedUpLight;
    vector<Float_t> jets_CSV_reshapedDownLight;

        // Non-selected jets (with pT > 10, eta < 4.7)

    vector<TLorentzVector> nonSelectedJets;              // p4 of the non selected jets (with pT > 10, eta < 4.7)
    vector<Float_t>        nonSelectedJets_CSV_raw;      // CSV value of the non selected jets, before reshaping 
    vector<Float_t>        nonSelectedJets_CSV_reshaped; // CSV value of the non selected jets, after reshaping
    vector<Int_t>          nonSelectedJets_partonFlav;   // Parton flavour of the non selected jets

        // Non selected leptons (with pT > 10, eta < 2.4, and electron/muon ID applied)

    vector<TLorentzVector> nonSelectedLeptons;              // p4 of the non selected leptons
    vector<Short_t>        nonSelectedLeptonsPDGId;         // pdgid of the non selected leptons 

        // Raw MET (used as a cross check for when applying MET filters after production)

    Float_t rawPFMET;                           // Raw MET from PF-based algorithm 

        // Infos for PDF uncertainties
   
    Float_t x_firstIncomingParton;              // Momentum fraction carried by the first incoming parton
    Float_t x_secondIncomingParton;             // Momentum fraction carried by the second incoming parton
    Int_t   flavor_firstIncomingParton;         // PDGId of the first incoming parton
    Int_t   flavor_secondIncomingParton;        // PDGId of the second incoming parton
    Float_t scalePDF;                           // The PDF scale

} babyEvent;

typedef struct
{

    TLorentzVector*         pointerToLeadingLepton;
    TLorentzVector*         pointerToSecondLepton;
    vector<TLorentzVector>* pointerToJets;
    vector<float>*          pointerToJets_CSV_raw;
    vector<float>*          pointerToJets_CSV_reshaped;
    vector<Int_t>*          pointerToJets_partonFlav;
    vector<TLorentzVector>* pointerToGenParticles;
    vector<Int_t>*          pointerToGenParticlesPDGId;
    vector<Int_t>*          pointerToGenParticlesMother;
    vector<Float_t>*        pointerToJets_CSV_reshapedUpBC;
    vector<Float_t>*        pointerToJets_CSV_reshapedDownBC;
    vector<Float_t>*        pointerToJets_CSV_reshapedUpLight;
    vector<Float_t>*        pointerToJets_CSV_reshapedDownLight;
    vector<TLorentzVector>* pointerToNonSelectedJets;              
    vector<Float_t>*        pointerToNonSelectedJets_CSV_raw;      
    vector<Float_t>*        pointerToNonSelectedJets_CSV_reshaped; 
    vector<Int_t>*          pointerToNonSelectedJets_partonFlav;   
    vector<TLorentzVector>* pointerToNonSelectedLeptons;              
    vector<Short_t>*        pointerToNonSelectedLeptonsPDGId;   

} intermediatePointers;

void ReadEvent(TTree* theTree, long int i, intermediatePointers* pointers, babyEvent* myEvent)
{
      theTree->GetEntry(i);

      
      myEvent->leadingLepton                = *(pointers->pointerToLeadingLepton);
      myEvent->secondLepton                 = *(pointers->pointerToSecondLepton);
     /* 
      myEvent->jets                         = *(pointers->pointerToJets);
      myEvent->jets_CSV_raw                 = *(pointers->pointerToJets_CSV_raw);
      myEvent->jets_CSV_reshaped            = *(pointers->pointerToJets_CSV_reshaped);
      myEvent->jets_partonFlav              = *(pointers->pointerToJets_partonFlav);
      myEvent->genParticles                 = *(pointers->pointerToGenParticles);
      myEvent->genParticlesPDGId            = *(pointers->pointerToGenParticlesPDGId);
      myEvent->genParticlesMother           = *(pointers->pointerToGenParticlesMother);
      
      myEvent->jets_CSV_reshapedUpBC        = *(pointers->pointerToJets_CSV_reshapedUpBC);
      myEvent->jets_CSV_reshapedDownBC      = *(pointers->pointerToJets_CSV_reshapedDownBC);
      myEvent->jets_CSV_reshapedUpLight     = *(pointers->pointerToJets_CSV_reshapedUpLight);
      myEvent->jets_CSV_reshapedDownLight   = *(pointers->pointerToJets_CSV_reshapedDownLight);
      myEvent->nonSelectedJets              = *(pointers->pointerToNonSelectedJets);              
      myEvent->nonSelectedJets_CSV_raw      = *(pointers->pointerToNonSelectedJets_CSV_raw);      
      myEvent->nonSelectedJets_CSV_reshaped = *(pointers->pointerToNonSelectedJets_CSV_reshaped); 
      myEvent->nonSelectedJets_partonFlav   = *(pointers->pointerToNonSelectedJets_partonFlav);   
    */

}

void InitializeBranch(TTree* theTree, const char* branch, void* target)
{
    theTree->SetBranchAddress(branch,target);
}
void InitializeBranch(TTree* theTree, const char* branch, void** target)
{
    theTree->SetBranchAddress(branch,target);
}

void DisableBranch(TTree* theTree, const char* branch, void* target)
{
    theTree->SetBranchAddress(branch,target);
    theTree->SetBranchStatus(branch,0);
}

void DisableBranch(TTree* theTree, const char* branch, void** target)
{
    theTree->SetBranchAddress(branch,target);
    theTree->SetBranchStatus(branch,0);
}

void InitializeBranches(TTree* theTree, babyEvent* myEvent,intermediatePointers* pointers)
{
    //theTree->SetBranchStatus("*",kFALSE);

    InitializeBranch(theTree,"run",                                          &(myEvent->run));
    InitializeBranch(theTree,"lumi",                                         &(myEvent->lumi));
    InitializeBranch(theTree,"event",                                        &(myEvent->event));
    
    InitializeBranch(theTree,"triggerMuon",                                  &(myEvent->triggerMuon));
    InitializeBranch(theTree,"xtriggerMuon",                                 &(myEvent->xtriggerMuon));
    InitializeBranch(theTree,"triggerElec",                                  &(myEvent->triggerElec));
    InitializeBranch(theTree,"triggerDoubleElec",                            &(myEvent->triggerDoubleElec));
    InitializeBranch(theTree,"triggerDoubleMuon",                            &(myEvent->triggerDoubleMuon));
    InitializeBranch(theTree,"triggerMuonElec",                              &(myEvent->triggerMuonElec));
   
    InitializeBranch(theTree,"numberOfLepton",                               &(myEvent->numberOfLepton));
    pointers->pointerToLeadingLepton = 0;
    InitializeBranch(theTree,"leadingLepton",                                (void**) &(pointers->pointerToLeadingLepton));
    InitializeBranch(theTree,"leadingLeptonPDGId",                           &(myEvent->leadingLeptonPDGId));
    pointers->pointerToSecondLepton = 0;
    InitializeBranch(theTree,"secondLepton",                                 (void**) &(pointers->pointerToSecondLepton));
    InitializeBranch(theTree,"secondLeptonPDGId",                            &(myEvent->secondLeptonPDGId));
    InitializeBranch(theTree,"isolatedTrackVeto",                            &(myEvent->isolatedTrackVeto));
    InitializeBranch(theTree,"tauVeto",                                      &(myEvent->tauVeto));
    
    InitializeBranch(theTree,"nJets",                                        &(myEvent->nJets));
    InitializeBranch(theTree,"nBTag",                                        &(myEvent->nBTag));

    /*
    pointers->pointerToJets = 0;
    InitializeBranch(theTree,"jets",                                         (void**) &(pointers->pointerToJets));
    pointers->pointerToJets_CSV_raw = 0;
    InitializeBranch(theTree,"jets_CSV_raw",                                 (void**) &(pointers->pointerToJets_CSV_raw));
    pointers->pointerToJets_CSV_reshaped = 0;
    InitializeBranch(theTree,"jets_CSV_reshaped",                            (void**) &(pointers->pointerToJets_CSV_reshaped));
    pointers->pointerToJets_partonFlav = 0;
    DisableBranch(theTree,"jets_partonFlav",                              (void**) &(pointers->pointerToJets_partonFlav));
    */

    InitializeBranch(theTree,"MET",                                          &(myEvent->MET));
    InitializeBranch(theTree,"MT",                                           &(myEvent->MT));
    InitializeBranch(theTree,"deltaPhiMETJets",                              &(myEvent->deltaPhiMETJets));
    InitializeBranch(theTree,"hadronicChi2",                                 &(myEvent->hadronicChi2));
    InitializeBranch(theTree,"MT2W",                                         &(myEvent->MT2W));
    InitializeBranch(theTree,"HT",                                           &(myEvent->HT));
    InitializeBranch(theTree,"HTRatio",                                      &(myEvent->HTRatio));
    InitializeBranch(theTree,"leadingBPt",                                   &(myEvent->leadingBPt));
    InitializeBranch(theTree,"leadingLeptonPt",                              &(myEvent->leadingLeptonPt));
    InitializeBranch(theTree,"leadingJetPt",                                 &(myEvent->leadingJetPt));
    InitializeBranch(theTree,"Mlb",                                          &(myEvent->Mlb));
    InitializeBranch(theTree,"Mlb_hemi",                                     &(myEvent->Mlb_hemi));
    InitializeBranch(theTree,"M3b",                                          &(myEvent->M3b));
    InitializeBranch(theTree,"deltaRLeptonLeadingB",                         &(myEvent->deltaRLeptonLeadingB));
    InitializeBranch(theTree,"METoverSqrtHT",                                &(myEvent->METoverSqrtHT));
    InitializeBranch(theTree,"HTPlusLeptonPtPlusMET",                        &(myEvent->HTPlusLeptonPtPlusMET));
    
    //DisableBranch(theTree,"nWTag",                                        &(myEvent->nWTag));
    //DisableBranch(theTree,"leadingWjetPt",                                &(myEvent->leadingWjetPt));
    
    InitializeBranch(theTree,"mStop",                                        &(myEvent->mStop));
    InitializeBranch(theTree,"mNeutralino",                                  &(myEvent->mNeutralino));
    //DisableBranch(theTree,"mCharginoParameter",                           &(myEvent->mCharginoParameter));
    
    InitializeBranch(theTree,"numberOfGenLepton",                            &(myEvent->numberOfGenLepton));

    /*
    pointers->pointerToGenParticles = 0;
    DisableBranch(theTree,"genParticles",                                 (void**) &(pointers->pointerToGenParticles));
    pointers->pointerToGenParticlesPDGId = 0;
    DisableBranch(theTree,"genParticlesPDGId",                            (void**) &(pointers->pointerToGenParticlesPDGId));
    pointers->pointerToGenParticlesMother = 0;
    DisableBranch(theTree,"genParticlesMother",                           (void**) &(pointers->pointerToGenParticlesMother));
    */

    //DisableBranch(theTree,"numberOfInitialEvents",                        &(myEvent->numberOfInitialEvents));
    InitializeBranch(theTree,"crossSection",                                 &(myEvent->crossSection));
    //DisableBranch(theTree,"numberOfTruePU",                               &(myEvent->numberOfTruePU));
    //DisableBranch(theTree,"numberOfPrimaryVertices",                      &(myEvent->numberOfPrimaryVertices));
    InitializeBranch(theTree,"weightCrossSection",                           &(myEvent->weightCrossSection));
    InitializeBranch(theTree,"weightPileUp",                                 &(myEvent->weightPileUp));
    InitializeBranch(theTree,"weightISRmodeling",                            &(myEvent->weightISRmodeling));
    InitializeBranch(theTree,"weightTopPt",                                  &(myEvent->weightTopPt));
    InitializeBranch(theTree,"weightTriggerEfficiency",                      &(myEvent->weightTriggerEfficiency));

    /*
    DisableBranch(theTree,"weightT2ttLeftHanded",                         &(myEvent->weightT2ttLeftHanded));
    DisableBranch(theTree,"weightT2ttRightHanded",                        &(myEvent->weightT2ttRightHanded));
    DisableBranch(theTree,"weightT2bwPolarization_ss",                    &(myEvent->weightT2bwPolarization_ss));
    DisableBranch(theTree,"weightT2bwPolarization_ls",                    &(myEvent->weightT2bwPolarization_ls));
    DisableBranch(theTree,"weightT2bwPolarization_sl",                    &(myEvent->weightT2bwPolarization_sl));
    DisableBranch(theTree,"weightT2bwPolarization_sr",                    &(myEvent->weightT2bwPolarization_sr));
    DisableBranch(theTree,"weightT2bwPolarization_rs",                    &(myEvent->weightT2bwPolarization_rs));
    DisableBranch(theTree,"weightT2bwPolarization_ll",                    &(myEvent->weightT2bwPolarization_ll));
    DisableBranch(theTree,"weightT2bwPolarization_lr",                    &(myEvent->weightT2bwPolarization_lr));
    DisableBranch(theTree,"weightT2bwPolarization_rl",                    &(myEvent->weightT2bwPolarization_rl));
    DisableBranch(theTree,"weightT2bwPolarization_rr",                    &(myEvent->weightT2bwPolarization_rr));
    
    DisableBranch(theTree,"MET_JESup",                                    &(myEvent->MET_JESup));
    DisableBranch(theTree,"MT_JESup",                                     &(myEvent->MT_JESup));
    DisableBranch(theTree,"deltaPhiMETJets_JESup",                        &(myEvent->deltaPhiMETJets_JESup));
    DisableBranch(theTree,"hadronicChi2_JESup",                           &(myEvent->hadronicChi2_JESup));
    DisableBranch(theTree,"MT2W_JESup",                                   &(myEvent->MT2W_JESup));
    DisableBranch(theTree,"HT_JESup",                                     &(myEvent->HT_JESup));
    DisableBranch(theTree,"HTRatio_JESup",                                &(myEvent->HTRatio_JESup));
    DisableBranch(theTree,"leadingBPt_JESup",                             &(myEvent->leadingBPt_JESup));
    DisableBranch(theTree,"leadingJetPt_JESup",                           &(myEvent->leadingJetPt_JESup));
    DisableBranch(theTree,"Mlb_JESup",                                    &(myEvent->Mlb_JESup));
    DisableBranch(theTree,"Mlb_hemi_JESup",                               &(myEvent->Mlb_hemi_JESup));
    DisableBranch(theTree,"M3b_JESup",                                    &(myEvent->M3b_JESup));
    DisableBranch(theTree,"deltaRLeptonLeadingB_JESup",                   &(myEvent->deltaRLeptonLeadingB_JESup));
    DisableBranch(theTree,"METoverSqrtHT_JESup",                          &(myEvent->METoverSqrtHT_JESup));
    DisableBranch(theTree,"HTPlusLeptonPtPlusMET_JESup",                  &(myEvent->HTPlusLeptonPtPlusMET_JESup));
    DisableBranch(theTree,"nJets_JESup",                                  &(myEvent->nJets_JESup));
    DisableBranch(theTree,"weightTriggerEfficiency_JESup",                &(myEvent->weightTriggerEfficiency_JESup));
 
    DisableBranch(theTree,"MET_JESdown",                                  &(myEvent->MET_JESdown));
    DisableBranch(theTree,"MT_JESdown",                                   &(myEvent->MT_JESdown));
    DisableBranch(theTree,"deltaPhiMETJets_JESdown",                      &(myEvent->deltaPhiMETJets_JESdown));
    DisableBranch(theTree,"hadronicChi2_JESdown",                         &(myEvent->hadronicChi2_JESdown));
    DisableBranch(theTree,"MT2W_JESdown",                                 &(myEvent->MT2W_JESdown));
    DisableBranch(theTree,"HT_JESdown",                                   &(myEvent->HT_JESdown));
    DisableBranch(theTree,"HTRatio_JESdown",                              &(myEvent->HTRatio_JESdown));
    DisableBranch(theTree,"leadingBPt_JESdown",                           &(myEvent->leadingBPt_JESdown));
    DisableBranch(theTree,"leadingJetPt_JESdown",                         &(myEvent->leadingJetPt_JESdown));
    DisableBranch(theTree,"Mlb_JESdown",                                  &(myEvent->Mlb_JESdown));
    DisableBranch(theTree,"Mlb_hemi_JESdown",                             &(myEvent->Mlb_hemi_JESdown));
    DisableBranch(theTree,"M3b_JESdown",                                  &(myEvent->M3b_JESdown));
    DisableBranch(theTree,"deltaRLeptonLeadingB_JESdown",                 &(myEvent->deltaRLeptonLeadingB_JESdown));
    DisableBranch(theTree,"METoverSqrtHT_JESdown",                        &(myEvent->METoverSqrtHT_JESdown));
    DisableBranch(theTree,"HTPlusLeptonPtPlusMET_JESdown",                &(myEvent->HTPlusLeptonPtPlusMET_JESdown));
    DisableBranch(theTree,"nJets_JESdown",                                &(myEvent->nJets_JESdown));
    DisableBranch(theTree,"weightTriggerEfficiency_JESdown",              &(myEvent->weightTriggerEfficiency_JESdown));

    pointers->pointerToJets_CSV_reshapedUpBC        = 0;
    DisableBranch(theTree,"jets_CSV_reshapedUpBC",                        (void**) &(pointers->pointerToJets_CSV_reshapedUpBC));
    pointers->pointerToJets_CSV_reshapedDownBC      = 0;                                
    DisableBranch(theTree,"jets_CSV_reshapedDownBC",                      (void**) &(pointers->pointerToJets_CSV_reshapedDownBC));
    pointers->pointerToJets_CSV_reshapedUpLight     = 0;                                
    DisableBranch(theTree,"jets_CSV_reshapedUpLight",                     (void**) &(pointers->pointerToJets_CSV_reshapedUpLight));
    pointers->pointerToJets_CSV_reshapedDownLight   = 0;                                
    DisableBranch(theTree,"jets_CSV_reshapedDownLight",                   (void**) &(pointers->pointerToJets_CSV_reshapedDownLight));
                                                                                        
    pointers->pointerToNonSelectedJets              = 0;                                
    DisableBranch(theTree,"nonSelectedJets",                              (void**) &(pointers->pointerToNonSelectedJets)); 
    pointers->pointerToNonSelectedJets_CSV_raw      = 0;                                
    DisableBranch(theTree,"nonSelectedJets_CSV_raw",                      (void**) &(pointers->pointerToNonSelectedJets_CSV_raw));
    pointers->pointerToNonSelectedJets_CSV_reshaped = 0;                                
    DisableBranch(theTree,"nonSelectedJets_CSV_reshaped",                 (void**) &(pointers->pointerToNonSelectedJets_CSV_reshaped));
    pointers->pointerToNonSelectedJets_partonFlav   = 0;                                
    DisableBranch(theTree,"nonSelectedJets_partonFlav",                   (void**) &(pointers->pointerToNonSelectedJets_partonFlav));

    pointers->pointerToNonSelectedLeptons           = 0;                                
    DisableBranch(theTree,"nonSelectedLeptons",                           (void**) &(pointers->pointerToNonSelectedLeptons)); 
    pointers->pointerToNonSelectedLeptonsPDGId      = 0;                                
    DisableBranch(theTree,"nonSelectedLeptonsPDGId",                      (void**) &(pointers->pointerToNonSelectedLeptonsPDGId));
    
    DisableBranch(theTree,"rawPFMET",                                     &(myEvent->rawPFMET));

    DisableBranch(theTree,"x_firstIncomingParton",                        &(myEvent->x_firstIncomingParton));                       
    DisableBranch(theTree,"x_secondIncomingParton",                       &(myEvent->x_secondIncomingParton));     
    DisableBranch(theTree,"flavor_firstIncomingParton",                   &(myEvent->flavor_firstIncomingParton)); 
    DisableBranch(theTree,"flavor_secondIncomingParton",                  &(myEvent->flavor_secondIncomingParton));
    DisableBranch(theTree,"scalePDF",                                     &(myEvent->scalePDF));                   
    */
}





#endif


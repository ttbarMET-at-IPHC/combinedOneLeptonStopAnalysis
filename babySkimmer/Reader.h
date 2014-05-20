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

      myEvent->nonSelectedLeptons           = *(pointers->pointerToNonSelectedLeptons); 
      myEvent->nonSelectedLeptonsPDGId      = *(pointers->pointerToNonSelectedLeptonsPDGId); 

}

void InitializeBranchesForReading(TTree* theTree, babyEvent* myEvent,intermediatePointers* pointers)
{
    theTree->SetBranchAddress("run",                                          &(myEvent->run));
    theTree->SetBranchAddress("lumi",                                         &(myEvent->lumi));
    theTree->SetBranchAddress("event",                                        &(myEvent->event));
    
    theTree->SetBranchAddress("triggerMuon",                                  &(myEvent->triggerMuon));
    theTree->SetBranchAddress("xtriggerMuon",                                 &(myEvent->xtriggerMuon));
    theTree->SetBranchAddress("triggerElec",                                  &(myEvent->triggerElec));
    theTree->SetBranchAddress("triggerDoubleElec",                            &(myEvent->triggerDoubleElec));
    theTree->SetBranchAddress("triggerDoubleMuon",                            &(myEvent->triggerDoubleMuon));
    theTree->SetBranchAddress("triggerMuonElec",                              &(myEvent->triggerMuonElec));
    
    theTree->SetBranchAddress("numberOfLepton",                               &(myEvent->numberOfLepton));
    pointers->pointerToLeadingLepton = 0;
    theTree->SetBranchAddress("leadingLepton",                                &(pointers->pointerToLeadingLepton));
    theTree->SetBranchAddress("leadingLeptonPDGId",                           &(myEvent->leadingLeptonPDGId));
    pointers->pointerToSecondLepton = 0;
    theTree->SetBranchAddress("secondLepton",                                 &(pointers->pointerToSecondLepton));
    theTree->SetBranchAddress("secondLeptonPDGId",                            &(myEvent->secondLeptonPDGId));
    theTree->SetBranchAddress("isolatedTrackVeto",                            &(myEvent->isolatedTrackVeto));
    theTree->SetBranchAddress("tauVeto",                                      &(myEvent->tauVeto));
    
    theTree->SetBranchAddress("nJets",                                        &(myEvent->nJets));
    theTree->SetBranchAddress("nBTag",                                        &(myEvent->nBTag));

    pointers->pointerToJets = 0;
    theTree->SetBranchAddress("jets",                                         &(pointers->pointerToJets));
    pointers->pointerToJets_CSV_raw = 0;
    theTree->SetBranchAddress("jets_CSV_raw",                                 &(pointers->pointerToJets_CSV_raw));
    pointers->pointerToJets_CSV_reshaped = 0;
    theTree->SetBranchAddress("jets_CSV_reshaped",                            &(pointers->pointerToJets_CSV_reshaped));
    pointers->pointerToJets_partonFlav = 0;
    theTree->SetBranchAddress("jets_partonFlav",                              &(pointers->pointerToJets_partonFlav));

    theTree->SetBranchAddress("MET",                                          &(myEvent->MET));
    theTree->SetBranchAddress("MT",                                           &(myEvent->MT));
    theTree->SetBranchAddress("deltaPhiMETJets",                              &(myEvent->deltaPhiMETJets));
    theTree->SetBranchAddress("hadronicChi2",                                 &(myEvent->hadronicChi2));
    theTree->SetBranchAddress("MT2W",                                         &(myEvent->MT2W));
    theTree->SetBranchAddress("HT",                                           &(myEvent->HT));
    theTree->SetBranchAddress("HTRatio",                                      &(myEvent->HTRatio));
    theTree->SetBranchAddress("leadingBPt",                                   &(myEvent->leadingBPt));
    theTree->SetBranchAddress("leadingLeptonPt",                              &(myEvent->leadingLeptonPt));
    theTree->SetBranchAddress("leadingJetPt",                                 &(myEvent->leadingJetPt));
    theTree->SetBranchAddress("Mlb",                                          &(myEvent->Mlb));
    theTree->SetBranchAddress("Mlb_hemi",                                     &(myEvent->Mlb_hemi));
    theTree->SetBranchAddress("M3b",                                          &(myEvent->M3b));
    theTree->SetBranchAddress("deltaRLeptonLeadingB",                         &(myEvent->deltaRLeptonLeadingB));
    theTree->SetBranchAddress("METoverSqrtHT",                                &(myEvent->METoverSqrtHT));
    theTree->SetBranchAddress("HTPlusLeptonPtPlusMET",                        &(myEvent->HTPlusLeptonPtPlusMET));
    
    theTree->SetBranchAddress("nWTag",                                        &(myEvent->nWTag));
    theTree->SetBranchAddress("leadingWjetPt",                                &(myEvent->leadingWjetPt));
    
    theTree->SetBranchAddress("mStop",                                        &(myEvent->mStop));
    theTree->SetBranchAddress("mNeutralino",                                  &(myEvent->mNeutralino));
    theTree->SetBranchAddress("mCharginoParameter",                           &(myEvent->mCharginoParameter));
    
    theTree->SetBranchAddress("numberOfGenLepton",                            &(myEvent->numberOfGenLepton));

    pointers->pointerToGenParticles = 0;
    theTree->SetBranchAddress("genParticles",                                 &(pointers->pointerToGenParticles));
    pointers->pointerToGenParticlesPDGId = 0;
    theTree->SetBranchAddress("genParticlesPDGId",                            &(pointers->pointerToGenParticlesPDGId));
    pointers->pointerToGenParticlesMother = 0;
    theTree->SetBranchAddress("genParticlesMother",                           &(pointers->pointerToGenParticlesMother));

    theTree->SetBranchAddress("numberOfInitialEvents",                        &(myEvent->numberOfInitialEvents));
    theTree->SetBranchAddress("crossSection",                                 &(myEvent->crossSection));
    theTree->SetBranchAddress("numberOfTruePU",                               &(myEvent->numberOfTruePU));
    theTree->SetBranchAddress("numberOfPrimaryVertices",                      &(myEvent->numberOfPrimaryVertices));
    theTree->SetBranchAddress("weightCrossSection",                           &(myEvent->weightCrossSection));
    theTree->SetBranchAddress("weightPileUp",                                 &(myEvent->weightPileUp));
    theTree->SetBranchAddress("weightISRmodeling",                            &(myEvent->weightISRmodeling));
    theTree->SetBranchAddress("weightTopPt",                                  &(myEvent->weightTopPt));
    theTree->SetBranchAddress("weightTriggerEfficiency",                      &(myEvent->weightTriggerEfficiency));
    theTree->SetBranchAddress("weightT2ttLeftHanded",                         &(myEvent->weightT2ttLeftHanded));
    theTree->SetBranchAddress("weightT2ttRightHanded",                        &(myEvent->weightT2ttRightHanded));
    theTree->SetBranchAddress("weightT2bwPolarization_ss",                    &(myEvent->weightT2bwPolarization_ss));
    theTree->SetBranchAddress("weightT2bwPolarization_ls",                    &(myEvent->weightT2bwPolarization_ls));
    theTree->SetBranchAddress("weightT2bwPolarization_sl",                    &(myEvent->weightT2bwPolarization_sl));
    theTree->SetBranchAddress("weightT2bwPolarization_sr",                    &(myEvent->weightT2bwPolarization_sr));
    theTree->SetBranchAddress("weightT2bwPolarization_rs",                    &(myEvent->weightT2bwPolarization_rs));
    theTree->SetBranchAddress("weightT2bwPolarization_ll",                    &(myEvent->weightT2bwPolarization_ll));
    theTree->SetBranchAddress("weightT2bwPolarization_lr",                    &(myEvent->weightT2bwPolarization_lr));
    theTree->SetBranchAddress("weightT2bwPolarization_rl",                    &(myEvent->weightT2bwPolarization_rl));
    theTree->SetBranchAddress("weightT2bwPolarization_rr",                    &(myEvent->weightT2bwPolarization_rr));
    
    theTree->SetBranchAddress("MET_JESup",                                    &(myEvent->MET_JESup));
    theTree->SetBranchAddress("MT_JESup",                                     &(myEvent->MT_JESup));
    theTree->SetBranchAddress("deltaPhiMETJets_JESup",                        &(myEvent->deltaPhiMETJets_JESup));
    theTree->SetBranchAddress("hadronicChi2_JESup",                           &(myEvent->hadronicChi2_JESup));
    theTree->SetBranchAddress("MT2W_JESup",                                   &(myEvent->MT2W_JESup));
    theTree->SetBranchAddress("HT_JESup",                                     &(myEvent->HT_JESup));
    theTree->SetBranchAddress("HTRatio_JESup",                                &(myEvent->HTRatio_JESup));
    theTree->SetBranchAddress("leadingBPt_JESup",                             &(myEvent->leadingBPt_JESup));
    theTree->SetBranchAddress("leadingJetPt_JESup",                           &(myEvent->leadingJetPt_JESup));
    theTree->SetBranchAddress("Mlb_JESup",                                    &(myEvent->Mlb_JESup));
    theTree->SetBranchAddress("Mlb_hemi_JESup",                               &(myEvent->Mlb_hemi_JESup));
    theTree->SetBranchAddress("M3b_JESup",                                    &(myEvent->M3b_JESup));
    theTree->SetBranchAddress("deltaRLeptonLeadingB_JESup",                   &(myEvent->deltaRLeptonLeadingB_JESup));
    theTree->SetBranchAddress("METoverSqrtHT_JESup",                          &(myEvent->METoverSqrtHT_JESup));
    theTree->SetBranchAddress("HTPlusLeptonPtPlusMET_JESup",                  &(myEvent->HTPlusLeptonPtPlusMET_JESup));
    theTree->SetBranchAddress("nJets_JESup",                                  &(myEvent->nJets_JESup));
    theTree->SetBranchAddress("weightTriggerEfficiency_JESup",                &(myEvent->weightTriggerEfficiency_JESup));
 
    theTree->SetBranchAddress("MET_JESdown",                                  &(myEvent->MET_JESdown));
    theTree->SetBranchAddress("MT_JESdown",                                   &(myEvent->MT_JESdown));
    theTree->SetBranchAddress("deltaPhiMETJets_JESdown",                      &(myEvent->deltaPhiMETJets_JESdown));
    theTree->SetBranchAddress("hadronicChi2_JESdown",                         &(myEvent->hadronicChi2_JESdown));
    theTree->SetBranchAddress("MT2W_JESdown",                                 &(myEvent->MT2W_JESdown));
    theTree->SetBranchAddress("HT_JESdown",                                   &(myEvent->HT_JESdown));
    theTree->SetBranchAddress("HTRatio_JESdown",                              &(myEvent->HTRatio_JESdown));
    theTree->SetBranchAddress("leadingBPt_JESdown",                           &(myEvent->leadingBPt_JESdown));
    theTree->SetBranchAddress("leadingJetPt_JESdown",                         &(myEvent->leadingJetPt_JESdown));
    theTree->SetBranchAddress("Mlb_JESdown",                                  &(myEvent->Mlb_JESdown));
    theTree->SetBranchAddress("Mlb_hemi_JESdown",                             &(myEvent->Mlb_hemi_JESdown));
    theTree->SetBranchAddress("M3b_JESdown",                                  &(myEvent->M3b_JESdown));
    theTree->SetBranchAddress("deltaRLeptonLeadingB_JESdown",                 &(myEvent->deltaRLeptonLeadingB_JESdown));
    theTree->SetBranchAddress("METoverSqrtHT_JESdown",                        &(myEvent->METoverSqrtHT_JESdown));
    theTree->SetBranchAddress("HTPlusLeptonPtPlusMET_JESdown",                &(myEvent->HTPlusLeptonPtPlusMET_JESdown));
    theTree->SetBranchAddress("nJets_JESdown",                                &(myEvent->nJets_JESdown));
    theTree->SetBranchAddress("weightTriggerEfficiency_JESdown",              &(myEvent->weightTriggerEfficiency_JESdown));

    pointers->pointerToJets_CSV_reshapedUpBC        = 0;
    theTree->SetBranchAddress("jets_CSV_reshapedUpBC",                        &(pointers->pointerToJets_CSV_reshapedUpBC));
    pointers->pointerToJets_CSV_reshapedDownBC      = 0;                                
    theTree->SetBranchAddress("jets_CSV_reshapedDownBC",                      &(pointers->pointerToJets_CSV_reshapedDownBC));
    pointers->pointerToJets_CSV_reshapedUpLight     = 0;                                
    theTree->SetBranchAddress("jets_CSV_reshapedUpLight",                     &(pointers->pointerToJets_CSV_reshapedUpLight));
    pointers->pointerToJets_CSV_reshapedDownLight   = 0;                                
    theTree->SetBranchAddress("jets_CSV_reshapedDownLight",                   &(pointers->pointerToJets_CSV_reshapedDownLight));
                                                                                        
    pointers->pointerToNonSelectedJets              = 0;                                
    theTree->SetBranchAddress("nonSelectedJets",                              &(pointers->pointerToNonSelectedJets)); 
    pointers->pointerToNonSelectedJets_CSV_raw      = 0;                                
    theTree->SetBranchAddress("nonSelectedJets_CSV_raw",                      &(pointers->pointerToNonSelectedJets_CSV_raw));
    pointers->pointerToNonSelectedJets_CSV_reshaped = 0;                                
    theTree->SetBranchAddress("nonSelectedJets_CSV_reshaped",                 &(pointers->pointerToNonSelectedJets_CSV_reshaped));
    pointers->pointerToNonSelectedJets_partonFlav   = 0;                                
    theTree->SetBranchAddress("nonSelectedJets_partonFlav",                   &(pointers->pointerToNonSelectedJets_partonFlav));

    pointers->pointerToNonSelectedLeptons           = 0;                                
    theTree->SetBranchAddress("nonSelectedLeptons",                           &(pointers->pointerToNonSelectedLeptons)); 
    pointers->pointerToNonSelectedLeptonsPDGId      = 0;                                
    theTree->SetBranchAddress("nonSelectedLeptonsPDGId",                      &(pointers->pointerToNonSelectedLeptonsPDGId));
    
    theTree->SetBranchAddress("rawPFMET",                                     &(myEvent->rawPFMET));

    theTree->SetBranchAddress("x_firstIncomingParton",                        &(myEvent->x_firstIncomingParton));                       
    theTree->SetBranchAddress("x_secondIncomingParton",                       &(myEvent->x_secondIncomingParton));     
    theTree->SetBranchAddress("flavor_firstIncomingParton",                   &(myEvent->flavor_firstIncomingParton)); 
    theTree->SetBranchAddress("flavor_secondIncomingParton",                  &(myEvent->flavor_secondIncomingParton));
    theTree->SetBranchAddress("scalePDF",                                     &(myEvent->scalePDF));                   
}

void InitializeBranchesForWriting(TTree* theTree, babyEvent* myEvent)
{
    theTree->Branch("run",                                          &(myEvent->run));
    theTree->Branch("lumi",                                         &(myEvent->lumi));
    theTree->Branch("event",                                        &(myEvent->event));
    
    theTree->Branch("triggerMuon",                                  &(myEvent->triggerMuon));
    theTree->Branch("xtriggerMuon",                                 &(myEvent->xtriggerMuon));
    theTree->Branch("triggerElec",                                  &(myEvent->triggerElec));
    theTree->Branch("triggerDoubleElec",                            &(myEvent->triggerDoubleElec));
    theTree->Branch("triggerDoubleMuon",                            &(myEvent->triggerDoubleMuon));
    theTree->Branch("triggerMuonElec",                              &(myEvent->triggerMuonElec));
    
    theTree->Branch("numberOfLepton",                               &(myEvent->numberOfLepton));
    theTree->Branch("leadingLepton","TLorentzVector",               &(myEvent->leadingLepton));
    theTree->Branch("leadingLeptonPDGId",                           &(myEvent->leadingLeptonPDGId));
    theTree->Branch("secondLepton","TLorentzVector",                &(myEvent->secondLepton));
    theTree->Branch("secondLeptonPDGId",                            &(myEvent->secondLeptonPDGId));
    theTree->Branch("isolatedTrackVeto",                            &(myEvent->isolatedTrackVeto));
    theTree->Branch("tauVeto",                                      &(myEvent->tauVeto));
    
    theTree->Branch("nJets",                                        &(myEvent->nJets));
    theTree->Branch("nBTag",                                        &(myEvent->nBTag));
    theTree->Branch("jets","std::vector<TLorentzVector>",           &(myEvent->jets));
    theTree->Branch("jets_CSV_raw",     "std::vector<Float_t>",     &(myEvent->jets_CSV_raw));
    theTree->Branch("jets_CSV_reshaped","std::vector<Float_t>",     &(myEvent->jets_CSV_reshaped));
    theTree->Branch("jets_partonFlav",  "std::vector<Int_t>",       &(myEvent->jets_partonFlav));

    theTree->Branch("MET",                                          &(myEvent->MET));
    theTree->Branch("MT",                                           &(myEvent->MT));
    theTree->Branch("deltaPhiMETJets",                              &(myEvent->deltaPhiMETJets));
    theTree->Branch("hadronicChi2",                                 &(myEvent->hadronicChi2));
    theTree->Branch("MT2W",                                         &(myEvent->MT2W));
    theTree->Branch("HT",                                           &(myEvent->HT));
    theTree->Branch("HTRatio",                                      &(myEvent->HTRatio));
    theTree->Branch("leadingBPt",                                   &(myEvent->leadingBPt));
    theTree->Branch("leadingLeptonPt",                              &(myEvent->leadingLeptonPt));
    theTree->Branch("leadingJetPt",                                 &(myEvent->leadingJetPt));
    theTree->Branch("Mlb",                                          &(myEvent->Mlb));
    theTree->Branch("Mlb_hemi",                                     &(myEvent->Mlb_hemi));
    theTree->Branch("M3b",                                          &(myEvent->M3b));
    theTree->Branch("deltaRLeptonLeadingB",                         &(myEvent->deltaRLeptonLeadingB));
    theTree->Branch("METoverSqrtHT",                                &(myEvent->METoverSqrtHT));
    theTree->Branch("HTPlusLeptonPtPlusMET",                        &(myEvent->HTPlusLeptonPtPlusMET));
    
    theTree->Branch("nWTag",                                        &(myEvent->nWTag));
    theTree->Branch("leadingWjetPt",                                &(myEvent->leadingWjetPt));
    
    theTree->Branch("mStop",                                        &(myEvent->mStop));
    theTree->Branch("mNeutralino",                                  &(myEvent->mNeutralino));
    theTree->Branch("mCharginoParameter",                           &(myEvent->mCharginoParameter));
    
    theTree->Branch("numberOfGenLepton",                            &(myEvent->numberOfGenLepton));
    theTree->Branch("genParticles","std::vector<TLorentzVector>",   &(myEvent->genParticles));
    theTree->Branch("genParticlesPDGId","std::vector<Int_t>",       &(myEvent->genParticlesPDGId));
    theTree->Branch("genParticlesMother","std::vector<Int_t>",      &(myEvent->genParticlesMother));
    
    theTree->Branch("numberOfInitialEvents",                        &(myEvent->numberOfInitialEvents));
    theTree->Branch("crossSection",                                 &(myEvent->crossSection));
    theTree->Branch("numberOfTruePU",                               &(myEvent->numberOfTruePU));
    theTree->Branch("numberOfPrimaryVertices",                      &(myEvent->numberOfPrimaryVertices));
    theTree->Branch("weightCrossSection",                           &(myEvent->weightCrossSection));
    theTree->Branch("weightPileUp",                                 &(myEvent->weightPileUp));
    theTree->Branch("weightISRmodeling",                            &(myEvent->weightISRmodeling));
    theTree->Branch("weightTopPt",                                  &(myEvent->weightTopPt));
    theTree->Branch("weightTriggerEfficiency",                      &(myEvent->weightTriggerEfficiency));
    theTree->Branch("weightT2ttLeftHanded",                         &(myEvent->weightT2ttLeftHanded));
    theTree->Branch("weightT2ttRightHanded",                        &(myEvent->weightT2ttRightHanded));
    theTree->Branch("weightT2bwPolarization_ss",                    &(myEvent->weightT2bwPolarization_ss));
    theTree->Branch("weightT2bwPolarization_ls",                    &(myEvent->weightT2bwPolarization_ss));
    theTree->Branch("weightT2bwPolarization_sl",                    &(myEvent->weightT2bwPolarization_ss));
    theTree->Branch("weightT2bwPolarization_sr",                    &(myEvent->weightT2bwPolarization_ss));
    theTree->Branch("weightT2bwPolarization_rs",                    &(myEvent->weightT2bwPolarization_ss));
    theTree->Branch("weightT2bwPolarization_ll",                    &(myEvent->weightT2bwPolarization_ll));
    theTree->Branch("weightT2bwPolarization_lr",                    &(myEvent->weightT2bwPolarization_lr));
    theTree->Branch("weightT2bwPolarization_rl",                    &(myEvent->weightT2bwPolarization_rl));
    theTree->Branch("weightT2bwPolarization_rr",                    &(myEvent->weightT2bwPolarization_rr));
   
    theTree->Branch("MET_JESup",                                    &(myEvent->MET_JESup));
    theTree->Branch("MT_JESup",                                     &(myEvent->MT_JESup));
    theTree->Branch("deltaPhiMETJets_JESup",                        &(myEvent->deltaPhiMETJets_JESup));
    theTree->Branch("hadronicChi2_JESup",                           &(myEvent->hadronicChi2_JESup));
    theTree->Branch("MT2W_JESup",                                   &(myEvent->MT2W_JESup));
    theTree->Branch("HT_JESup",                                     &(myEvent->HT_JESup));
    theTree->Branch("HTRatio_JESup",                                &(myEvent->HTRatio_JESup));
    theTree->Branch("leadingBPt_JESup",                             &(myEvent->leadingBPt_JESup));
    theTree->Branch("leadingJetPt_JESup",                           &(myEvent->leadingJetPt_JESup));
    theTree->Branch("Mlb_JESup",                                    &(myEvent->Mlb_JESup));
    theTree->Branch("Mlb_hemi_JESup",                               &(myEvent->Mlb_hemi_JESup));
    theTree->Branch("M3b_JESup",                                    &(myEvent->M3b_JESup));
    theTree->Branch("deltaRLeptonLeadingB_JESup",                   &(myEvent->deltaRLeptonLeadingB_JESup));
    theTree->Branch("METoverSqrtHT_JESup",                          &(myEvent->METoverSqrtHT_JESup));
    theTree->Branch("HTPlusLeptonPtPlusMET_JESup",                  &(myEvent->HTPlusLeptonPtPlusMET_JESup));
    theTree->Branch("nJets_JESup",                                  &(myEvent->nJets_JESup));
    theTree->Branch("weightTriggerEfficiency_JESup",                &(myEvent->weightTriggerEfficiency_JESup));
    
    theTree->Branch("MET_JESdown",                                  &(myEvent->MET_JESdown));
    theTree->Branch("MT_JESdown",                                   &(myEvent->MT_JESdown));
    theTree->Branch("deltaPhiMETJets_JESdown",                      &(myEvent->deltaPhiMETJets_JESdown));
    theTree->Branch("hadronicChi2_JESdown",                         &(myEvent->hadronicChi2_JESdown));
    theTree->Branch("MT2W_JESdown",                                 &(myEvent->MT2W_JESdown));
    theTree->Branch("HT_JESdown",                                   &(myEvent->HT_JESdown));
    theTree->Branch("HTRatio_JESdown",                              &(myEvent->HTRatio_JESdown));
    theTree->Branch("leadingBPt_JESdown",                           &(myEvent->leadingBPt_JESdown));
    theTree->Branch("leadingJetPt_JESdown",                         &(myEvent->leadingJetPt_JESdown));
    theTree->Branch("Mlb_JESdown",                                  &(myEvent->Mlb_JESdown));
    theTree->Branch("Mlb_hemi_JESdown",                             &(myEvent->Mlb_hemi_JESdown));
    theTree->Branch("M3b_JESdown",                                  &(myEvent->M3b_JESdown));
    theTree->Branch("deltaRLeptonLeadingB_JESdown",                 &(myEvent->deltaRLeptonLeadingB_JESdown));
    theTree->Branch("METoverSqrtHT_JESdown",                        &(myEvent->METoverSqrtHT_JESdown));
    theTree->Branch("HTPlusLeptonPtPlusMET_JESdown",                &(myEvent->HTPlusLeptonPtPlusMET_JESdown));
    theTree->Branch("nJets_JESdown",                                &(myEvent->nJets_JESdown));
    theTree->Branch("weightTriggerEfficiency_JESdown",              &(myEvent->weightTriggerEfficiency_JESdown));

    theTree->Branch("jets_CSV_reshapedUpBC",       "std::vector<Float_t>",          &(myEvent->jets_CSV_reshapedUpBC));
    theTree->Branch("jets_CSV_reshapedDownBC",     "std::vector<Float_t>",          &(myEvent->jets_CSV_reshapedDownBC));
    theTree->Branch("jets_CSV_reshapedUpLight",    "std::vector<Float_t>",          &(myEvent->jets_CSV_reshapedUpLight));
    theTree->Branch("jets_CSV_reshapedDownLight",  "std::vector<Float_t>",          &(myEvent->jets_CSV_reshapedDownLight));

    theTree->Branch("nonSelectedJets",             "std::vector<TLorentzVector>",   &(myEvent->nonSelectedJets));
    theTree->Branch("nonSelectedJets_CSV_raw",     "std::vector<Float_t>",          &(myEvent->nonSelectedJets_CSV_raw));
    theTree->Branch("nonSelectedJets_CSV_reshaped","std::vector<Float_t>",          &(myEvent->nonSelectedJets_CSV_reshaped));
    theTree->Branch("nonSelectedJets_partonFlav",  "std::vector<Int_t>",            &(myEvent->nonSelectedJets_partonFlav));
    
    theTree->Branch("nonSelectedLeptons",          "std::vector<TLorentzVector>",   &(myEvent->nonSelectedLeptons));
    theTree->Branch("nonSelectedLeptonsPDGId",     "std::vector<Short_t>",          &(myEvent->nonSelectedLeptonsPDGId));
    
    theTree->Branch("rawPFMET",                     &(myEvent->rawPFMET));

    theTree->Branch("x_firstIncomingParton",        &(myEvent->x_firstIncomingParton));       
    theTree->Branch("x_secondIncomingParton",       &(myEvent->x_secondIncomingParton));      
    theTree->Branch("flavor_firstIncomingParton",   &(myEvent->flavor_firstIncomingParton));  
    theTree->Branch("flavor_secondIncomingParton",  &(myEvent->flavor_secondIncomingParton)); 
    theTree->Branch("scalePDF",                     &(myEvent->scalePDF));                   

}



#endif


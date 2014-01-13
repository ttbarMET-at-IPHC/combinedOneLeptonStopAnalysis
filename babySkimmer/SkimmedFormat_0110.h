#ifndef babyFormatSkimmed
#define babyFormatSkimmed

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

} babyEventSkimmed;

void InitializeBranches(TTree* theTree, babyEventSkimmed* myEvent)
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
}


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

} intermediatePointersSkimmed;


void ReadEvent(TTree* theTree, long int i, intermediatePointersSkimmed* pointers, babyEventSkimmed* myEvent)
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
}

/*
void InitializeBranches(TTree* theTree, babyEventSkimmed* myEvent,intermediatePointers* pointers)
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
}
*/




#endif


#ifndef babyFormat
#define babyFormat


typedef struct 
{
    // ------------
    // General info
    // ------------

    Int_t run;                              // Run number
    Int_t lumi;                             // Lumiblock number
    Int_t event;                            // Event number

    // ------------
    // Trigger info
    // ------------
    
    Bool_t triggerMuon;                     // Single-Muon trigger has been fired
    Bool_t xtriggerMuon;                    // Muon cross-trigger  has been fired
    Bool_t triggerElec;                     // Single-Elec trigger has been fired
    
    Bool_t triggerDoubleElec;               // Double-Elec trigger has been fired
    Bool_t triggerDoubleMuon;               // Double-Muon trigger has been fired
    Bool_t triggerMuonElec;                 // Muon-Elec   trigger has been fired

    // ------------
    // Leptons info
    // ------------

    Short_t        numberOfLepton;          // Number of selected leptons
    TLorentzVector leadingLepton;           // p4 of the leading selected lepton
    Short_t        leadingLeptonPDGId;      // pdgid of the leading selected lepton
    TLorentzVector secondLepton;            // p4 of the second lepton
    Short_t        secondLeptonPDGId;       // pdgid of the second lepton

    Bool_t         isolatedTrackVeto;       // Event pass/fail the isolated track veto
    Bool_t         tauVeto;                 // Event pass/fail the tau veto
    
    // ------------
    // Jets info
    // ------------

    Short_t                nJets;           // Number of selected jets
    Short_t                nBTag;           // Number of selected jets b-tagged
    vector<TLorentzVector> jets;            // p4 of the selected jets
    vector<Float_t>        jets_CSV;        // CSV value of the selected jets           // TODO : b-tag reweighting

    // ------------
    // "High-level" /BDT variables
    // ------------

    Float_t MET;                            // Type-1 + phi-corrected PF MET
    Float_t MT;                             // transverse mass of leading lepton + MET
    Float_t deltaPhiMETJets;                // DeltaPhi(MET,first two leading jets)
    Float_t hadronicChi2;                   // Hadronic Chi^2
    Float_t MT2W;                           // MT2W
    Float_t HT;                             // HT
    Float_t HTRatio;                        // Fraction of HT in the same hemisphere as MET
    Float_t leadingBPt;                     // pT of the leading b-jet
    Float_t leadingLeptonPt;                // pT of the leading lepton
    Float_t leadingJetPt;                   // pT of the leading jet
    Float_t Mlb;                            // invariant mass of leading lepton + jet with highest CSV
    Float_t Mlb_hemi;                       // invariant mass of leading lepton + b-jet closest to lepton
    Float_t M3b;                            // invariant mass of the 3jets dR-furthest from lepton
    Float_t deltaRLeptonLeadingB;           // DeltaR(leading lepton, leading b-jet)
    Float_t METoverSqrtHT;                  // MET/sqrt(HT)
    Float_t HTPlusLeptonPtPlusMET;          // HT + pT(leading lepton) + MET

    // ------------
    // W-tagging info
    // ------------

    Short_t nWTag;                          // Number of W-tagged jets
    Float_t leadingWjetPt;                  // pT of the leading W-tagged jet

    // ------------
    // Signal-specific infos
    // ------------

    Float_t mStop;                          // Stop mass
    Float_t mNeutralino;                    // Neutralino mass
    Float_t mCharginoParameter;             // Chargino mass parameter for T2bw (0.25, 0.5 or 0.75)
 
    // ------------
    // Generator-level infos
    // ------------
   
    Int_t   numberOfGenLepton;              // Number of MC leptons (e/µ/tau, leptons from tau decay are not taken into account)
    Float_t genMET;                         // MET at generator level (definition ?), (TODO)
    Float_t genMETPhi;                      // Phi of the MET at generator level (TODO)

    vector<TLorentzVector> genParticles;       // p4 of status=3 particles
    vector<Int_t>          genParticlesPDGId;  // PDG of status=3 particles
    vector<Int_t>          genParticlesMother; // Mother index of the status=3 particles

    // ------------
    // Weights and infos related to weights
    // ------------

    Int_t   numberOfInitialEvents;          // Number of events for the given dataset or signal mass point, before any skimming/selection applied
    Float_t crossSection;                   // Cross section for the given dataset or signal mass point
    Int_t   numberOfTruePU;                 // Number of true pile-up (MC, used in weightPileup)
    Int_t   numberOfPrimaryVertices;        // Number of primary vertices, used to check pile-up data/MC aggreement

    Float_t weightCrossSection;             // = (crossSection / numberOfInitialEvents) ; user should multiply this weight by the desired lumi
    Float_t weightPileUp;                   // Weight for PU-reweighting, TODO
    Float_t weightISRmodeling;              // Weight for ISR modeling, TODO

    Float_t weightT2ttLeftHanded;           // Polarization reweighting, TODO
    Float_t weightT2ttRightHanded;          // Polarization reweighting, TODO
    Float_t weightT2bwPolarization_ss;      // Polarization reweighting, TODO
    Float_t weightT2bwPolarization_ll;      // Polarization reweighting, TODO
    Float_t weightT2bwPolarization_lr;      // Polarization reweighting, TODO
    Float_t weightT2bwPolarization_rl;      // Polarization reweighting, TODO
    Float_t weightT2bwPolarization_rr;      // Polarization reweighting, TODO
    
    Float_t efficiencySingleLeptonTrigger;  // Single-lepton trigger efficiency
    Float_t efficiencyDoubleLeptonTrigger;  // Double-lepton trigger efficiency 

    // ------------
    // Infos for systematic effects (TODO)
    // ------------

    Float_t MET_JESup;                      // Type-1 + phi-corrected PF MET                              (+1sigma JES applied)
    Float_t MT_JESup;                       // transverse mass of leading lepton + MET                    (+1sigma JES applied)
    Float_t deltaPhiMETJets_JESup;          // DeltaPhi(MET,first two leading jets)                       (+1sigma JES applied)
    Float_t hadronicChi2_JESup;             // Hadronic Chi^2                                             (+1sigma JES applied)
    Float_t MT2W_JESup;                     // MT2W                                                       (+1sigma JES applied)
    Float_t HT_JESup;                       // HT                                                         (+1sigma JES applied)
    Float_t HTRatio_JESup;                  // Fraction of HT in the same hemisphere as MET               (+1sigma JES applied)
    Float_t leadingBPt_JESup;               // pT of the leading b-jet                                    (+1sigma JES applied)
    Float_t leadingJetPt_JESup;             // pT of the leading jet                                      (+1sigma JES applied)
    Float_t Mlb_JESup;                      // invariant mass of leading lepton + jet with highest CSV    (+1sigma JES applied)
    Float_t Mlb_hemi_JESup;                 // invariant mass of leading lepton + b-jet closest to lepton (+1sigma JES applied)
    Float_t M3b_JESup;                      // invariant mass of the 3jets dR-furthest from lepton        (+1sigma JES applied)
    Float_t deltaRLeptonLeadingB_JESup;     // DeltaR(leading lepton, leading b-jet)                      (+1sigma JES applied)
    Float_t METoverSqrtHT_JESup;            // MET/sqrt(HT)                                               (+1sigma JES applied)
    Float_t HTPlusLeptonPtPlusMET_JESup;    // HT + pT(leading lepton) + MET                              (+1sigma JES applied)
    Short_t nJets_JESup;                    // Jet multiplicity with                                      (+1sigma JES applied)

    Float_t MET_JESdown;                    // Type-1 - phi-corrected PF MET                              (-1sigma JES applied)
    Float_t MT_JESdown;                     // transverse mass of leading lepton - MET                    (-1sigma JES applied)
    Float_t deltaPhiMETJets_JESdown;        // DeltaPhi(MET,first two leading jets)                       (-1sigma JES applied)
    Float_t hadronicChi2_JESdown;           // Hadronic Chi^2                                             (-1sigma JES applied)
    Float_t MT2W_JESdown;                   // MT2W                                                       (-1sigma JES applied)
    Float_t HT_JESdown;                             // HT                                                         (-1sigma JES applied)
    Float_t HTRatio_JESdown;                // Fraction of HT in the same hemisphere as MET               (-1sigma JES applied)
    Float_t leadingBPt_JESdown;             // pT of the leading b-jet                                    (-1sigma JES applied)
    Float_t leadingJetPt_JESdown;           // pT of the leading jet                                      (-1sigma JES applied)
    Float_t Mlb_JESdown;                    // invariant mass of leading lepton - jet with highest CSV    (-1sigma JES applied)
    Float_t Mlb_hemi_JESdown;               // invariant mass of leading lepton - b-jet closest to lepton (-1sigma JES applied)
    Float_t M3b_JESdown;                    // invariant mass of the 3jets dR-furthest from lepton        (-1sigma JES applied)
    Float_t deltaRLeptonLeadingB_JESdown;   // DeltaR(leading lepton, leading b-jet)                      (-1sigma JES applied)
    Float_t METoverSqrtHT_JESdown;          // MET/sqrt(HT)                                               (-1sigma JES applied)
    Float_t HTPlusLeptonPtPlusMET_JESdown;  // HT - pT(leading lepton) - MET                              (-1sigma JES applied)
    Short_t nJets_JESdown;                  // Jet multiplicity with                                      (-1sigma JES applied)

} babyEvent;






//
// At this point, I'd like to take a moment to speak to you about ROOT and TTrees.
//
// ROOT does not make the life of physicists easier. Such a quality would mean that
// the framework you are using does not require expert skills in C/C++ programming and
// pointer handling to achieve what is supposed to be a basic feature. Here, I just
// want to access the TLorentzVector stored in my TTree. Turns out, when you want to
// read a float, it's okay, just give the pointer to the float. But here, you want to
// read a object inherited from TObject, therefore you have to give the *address* of a 
// pointer. - Why ? - I understand that the internal structure might require such a thing,
// but I think it would really be much easier for everybody to find an other way to 
// implement this, instead of letting every single user go through twenty different 
// forums to figure out this small detail that makes everything more complicated.
//
// Now, I want to try to keep my code clean, which is also something ROOT doesn't help to.
// The best solution I found right now is to have this additional structure taking care 
// of the intermediate pointers, and then remap the content to the babyEvent member that I
// actually want it to go to, every time I read a new event.
//
typedef struct
{

    TLorentzVector*         pointerToLeadingLepton;
    TLorentzVector*         pointerToSecondLepton;
    vector<TLorentzVector>* pointerToJets;
    vector<float>*          pointerToJets_CSV;
    vector<TLorentzVector>* pointerToGenParticles;
    vector<Int_t>*          pointerToGenParticlesPDGId;
    vector<Int_t>*          pointerToGenParticlesMother;

} intermediatePointers;

void ReadEvent(TTree* theTree, long int i, intermediatePointers* pointers, babyEvent* myEvent)
{
      theTree->GetEntry(i);

      myEvent->leadingLepton      = *(pointers->pointerToLeadingLepton);
      myEvent->secondLepton       = *(pointers->pointerToSecondLepton);
      myEvent->jets               = *(pointers->pointerToJets);
      myEvent->jets_CSV           = *(pointers->pointerToJets_CSV);
      myEvent->genParticles       = *(pointers->pointerToGenParticles);
      myEvent->genParticlesPDGId  = *(pointers->pointerToGenParticlesPDGId);
      myEvent->genParticlesMother = *(pointers->pointerToGenParticlesMother);

}

void InitializeBranches(TTree* theTree, babyEvent* myEvent,intermediatePointers* pointers)
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
    theTree->SetBranchAddress("leadingLepton",&(pointers->pointerToLeadingLepton));
    theTree->SetBranchAddress("leadingLeptonPDGId",                           &(myEvent->leadingLeptonPDGId));
    pointers->pointerToSecondLepton = 0;
    theTree->SetBranchAddress("secondLepton",&(pointers->pointerToSecondLepton));
    theTree->SetBranchAddress("secondLeptonPDGId",                            &(myEvent->secondLeptonPDGId));
    theTree->SetBranchAddress("isolatedTrackVeto",                            &(myEvent->isolatedTrackVeto));
    theTree->SetBranchAddress("tauVeto",                                      &(myEvent->tauVeto));
    
    theTree->SetBranchAddress("nJets",                                        &(myEvent->nJets));
    theTree->SetBranchAddress("nBTag",                                        &(myEvent->nBTag));

    pointers->pointerToJets = 0;
    theTree->SetBranchAddress("jets",&(pointers->pointerToJets));
    pointers->pointerToJets_CSV = 0;
    theTree->SetBranchAddress("jets_CSV",&(pointers->pointerToJets_CSV));

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
    theTree->SetBranchAddress("genMET",                                       &(myEvent->genMET));
    theTree->SetBranchAddress("genMETPhi",                                    &(myEvent->genMETPhi));

    pointers->pointerToGenParticles = 0;
    theTree->SetBranchAddress("genParticles",&(pointers->pointerToGenParticles));
    pointers->pointerToGenParticlesPDGId = 0;
    theTree->SetBranchAddress("genParticlesPDGId",&(pointers->pointerToGenParticlesPDGId));
    pointers->pointerToGenParticlesMother = 0;
    theTree->SetBranchAddress("genParticlesMother",&(pointers->pointerToGenParticlesMother));

    theTree->SetBranchAddress("numberOfInitialEvents",                        &(myEvent->numberOfInitialEvents));
    theTree->SetBranchAddress("crossSection",                                 &(myEvent->crossSection));
    theTree->SetBranchAddress("numberOfTruePU",                               &(myEvent->numberOfTruePU));
    theTree->SetBranchAddress("numberOfPrimaryVertices",                      &(myEvent->numberOfPrimaryVertices));
    theTree->SetBranchAddress("weightCrossSection",                           &(myEvent->weightCrossSection));
    theTree->SetBranchAddress("weightPileUp",                                 &(myEvent->weightPileUp));
    theTree->SetBranchAddress("weightISRmodeling",                            &(myEvent->weightISRmodeling));
    theTree->SetBranchAddress("weightT2ttLeftHanded",                         &(myEvent->weightT2ttLeftHanded));
    theTree->SetBranchAddress("weightT2ttRightHanded",                        &(myEvent->weightT2ttRightHanded));
    theTree->SetBranchAddress("weightT2bwPolarization_ss",                    &(myEvent->weightT2bwPolarization_ss));
    theTree->SetBranchAddress("weightT2bwPolarization_ll",                    &(myEvent->weightT2bwPolarization_ll));
    theTree->SetBranchAddress("weightT2bwPolarization_lr",                    &(myEvent->weightT2bwPolarization_lr));
    theTree->SetBranchAddress("weightT2bwPolarization_rl",                    &(myEvent->weightT2bwPolarization_rl));
    theTree->SetBranchAddress("weightT2bwPolarization_rr",                    &(myEvent->weightT2bwPolarization_rr));
    theTree->SetBranchAddress("efficiencySingleLeptonTrigger",                &(myEvent->efficiencySingleLeptonTrigger));
    theTree->SetBranchAddress("efficiencyDoubleLeptonTrigger",                &(myEvent->efficiencyDoubleLeptonTrigger));
    
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

}





#endif


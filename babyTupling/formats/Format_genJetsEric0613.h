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
    float          leadingLeptonIdEfficiency;   // lepton ID efficiency weight
    float          leadingLeptonIsoScaleFactor; // lepton isolation scale factor

    TLorentzVector secondLepton;                // p4 of the second lepton
    Short_t        secondLeptonPDGId;           // pdgid of the second lepton
    float          secondLeptonIdEfficiency;    // lepton ID efficiency weight
    float          secondLeptonIsoScaleFactor;  // lepton isolation scale factor

    Bool_t         isolatedTrackVeto;           // Event pass/fail the isolated track veto
    Bool_t         tauVeto;                     // Event pass/fail the tau veto
    
    // ------------
    // Jets info
    // ------------

    Short_t                nJets;               // Number of selected jets
    Short_t                nBTag;               // Number of selected jets b-tagged
    vector<TLorentzVector> jets;                // p4 of the selected jets
    vector<TLorentzVector> jets_gen;            // p4 of the selected jets (gen jet matched)
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

    Short_t                nBTag_JESup;               // Number of selected jets b-tagged                     (+1sigma JES applied)
    vector<TLorentzVector> jets_JESup;                // p4 of the selected jets                              (+1sigma JES applied)
    vector<TLorentzVector> jets_gen_JESup;            // p4 of the selected jets                              (+1sigma JES applied) (gen jet matched)
    vector<Float_t>        jets_CSV_raw_JESup;        // CSV value of the selected jets, before reshaping     (+1sigma JES applied)
    vector<Float_t>        jets_CSV_reshaped_JESup;   // CSV value of the selected jets, after reshaping      (+1sigma JES applied)


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

    Short_t                nBTag_JESdown;               // Number of selected jets b-tagged                   (-1sigma JES applied)
    vector<TLorentzVector> jets_JESdown;                // p4 of the selected jets                            (-1sigma JES applied)
    vector<TLorentzVector> jets_gen_JESdown;            // p4 of the selected jets                            (-1sigma JES applied) (gen jet matched)
    vector<Float_t>        jets_CSV_raw_JESdown;        // CSV value of the selected jets, before reshaping   (-1sigma JES applied)
    vector<Float_t>        jets_CSV_reshaped_JESdown;   // CSV value of the selected jets, after reshaping    (-1sigma JES applied)

        // CSV reshaping variations
        
    vector<Float_t> jets_CSV_reshapedUpBC;
    vector<Float_t> jets_CSV_reshapedDownBC;
    vector<Float_t> jets_CSV_reshapedUpLight;
    vector<Float_t> jets_CSV_reshapedDownLight;

        // Non-selected jets (with pT > 10, eta < 4.7)

    vector<TLorentzVector> nonSelectedJets;              // p4 of the non selected jets (with pT > 10, eta < 4.7)
    vector<TLorentzVector> nonSelectedJets_gen;          // p4 of the non selected jets (with pT > 10, eta < 4.7) (gen jet matched)
    vector<Float_t>        nonSelectedJets_CSV_raw;      // CSV value of the non selected jets, before reshaping 
    vector<Float_t>        nonSelectedJets_CSV_reshaped; // CSV value of the non selected jets, after reshaping
    vector<Int_t>          nonSelectedJets_partonFlav;   // Parton flavour of the non selected jets

        // Non selected leptons (with pT > 10, eta < 2.4, and electron/muon ID applied)

    vector<TLorentzVector> nonSelectedLeptons;              // p4 of the non selected leptons
    vector<Short_t>        nonSelectedLeptonsPDGId;         // pdgid of the non selected leptons 

        // Raw MET (used as a cross check for when applying MET filters after production)

    Float_t rawPFMET;                           // Raw MET from PF-based algorithm 
    
        // Phi of the (corrected) MET

    Float_t METPhi;                             // Type-1 + phi-corrected PF MET

        // Infos for PDF uncertainties
   
    Float_t x_firstIncomingParton;              // Momentum fraction carried by the first incoming parton
    Float_t x_secondIncomingParton;             // Momentum fraction carried by the second incoming parton
    Int_t   flavor_firstIncomingParton;         // PDGId of the first incoming parton
    Int_t   flavor_secondIncomingParton;        // PDGId of the second incoming parton
    Float_t scalePDF;                           // The PDF scale


} babyEvent;

#endif
  


// ###################################################
// #     This function is called by the proof        #
// #   job - this is where you fill the microtuple   #
// ###################################################
#ifdef isCompilingTheBabyTupler

TH2F* mapT2tt;
TH2F* mapT2bw025;
TH2F* mapT2bw050;
TH2F* mapT2bw075;

void ProofJob::LoadCorrectionFiles()
{

    sel.setBTagReshapingInput(     anaEL->GetInfo("Analysis","Corrections","bTagReshaping")          );
    sel.setMCJetCorrectionsInput(  anaEL->GetInfo("Analysis","Corrections","jetEnergyCorrectionMC")  );
    sel.setDataJetCorrectionsInput(anaEL->GetInfo("Analysis","Corrections","jetEnergyCorrectionData"));
    sel.setPileUpReweightingInput( anaEL->GetInfo("Analysis","Corrections","pileUp")                 );

    sel.loadCorrections();
    

    TFile* fileMapT2tt    = new TFile("/opt/sbg/data/data4/cms/aaubin/analysisStudy_July13/CMSSW_5_3_2_patch4/src/NTuple/NTupleAnalysis/macros/stopAnalysis/babyTupling/../inputs/signalInitialNumberOfEventsMap/mapT2tt.root");
    TFile* fileMapT2bw025 = new TFile("/opt/sbg/data/data4/cms/aaubin/analysisStudy_July13/CMSSW_5_3_2_patch4/src/NTuple/NTupleAnalysis/macros/stopAnalysis/babyTupling/../inputs/signalInitialNumberOfEventsMap/mapT2bw-025.root");
    TFile* fileMapT2bw050 = new TFile("/opt/sbg/data/data4/cms/aaubin/analysisStudy_July13/CMSSW_5_3_2_patch4/src/NTuple/NTupleAnalysis/macros/stopAnalysis/babyTupling/../inputs/signalInitialNumberOfEventsMap/mapT2bw-050.root");
    TFile* fileMapT2bw075 = new TFile("/opt/sbg/data/data4/cms/aaubin/analysisStudy_July13/CMSSW_5_3_2_patch4/src/NTuple/NTupleAnalysis/macros/stopAnalysis/babyTupling/../inputs/signalInitialNumberOfEventsMap/mapT2bw-075.root");

    fileMapT2tt   ->GetObject("mStopVSmNeutralino",mapT2tt   );
    fileMapT2bw025->GetObject("mStopVSmNeutralino",mapT2bw025);
    fileMapT2bw050->GetObject("mStopVSmNeutralino",mapT2bw050);
    fileMapT2bw075->GetObject("mStopVSmNeutralino",mapT2bw075);
}

void ProofJob::InitializeBranches(TTree* theTree_, babyEvent* myEvent_)
{
    theTree_->Branch("run",                                          &(myEvent_->run));
    theTree_->Branch("lumi",                                         &(myEvent_->lumi));
    theTree_->Branch("event",                                        &(myEvent_->event));
    
    theTree_->Branch("triggerMuon",                                  &(myEvent_->triggerMuon));
    theTree_->Branch("xtriggerMuon",                                 &(myEvent_->xtriggerMuon));
    theTree_->Branch("triggerElec",                                  &(myEvent_->triggerElec));
    theTree_->Branch("triggerDoubleElec",                            &(myEvent_->triggerDoubleElec));
    theTree_->Branch("triggerDoubleMuon",                            &(myEvent_->triggerDoubleMuon));
    theTree_->Branch("triggerMuonElec",                              &(myEvent_->triggerMuonElec));
    
    theTree_->Branch("numberOfLepton",                               &(myEvent_->numberOfLepton));
    theTree_->Branch("leadingLepton","TLorentzVector",               &(myEvent_->leadingLepton));
    theTree_->Branch("leadingLeptonPDGId",                           &(myEvent_->leadingLeptonPDGId));
    theTree_->Branch("leadingLeptonIdEfficiency",                    &(myEvent_->leadingLeptonIdEfficiency));
    theTree_->Branch("leadingLeptonIsoScaleFactor",                  &(myEvent_->leadingLeptonIsoScaleFactor));
    theTree_->Branch("secondLepton","TLorentzVector",                &(myEvent_->secondLepton));
    theTree_->Branch("secondLeptonPDGId",                            &(myEvent_->secondLeptonPDGId));
    theTree_->Branch("secondLeptonIdEfficiency",                     &(myEvent_->secondLeptonIdEfficiency));
    theTree_->Branch("secondLeptonIsoScaleFactor",                   &(myEvent_->secondLeptonIsoScaleFactor));
    theTree_->Branch("isolatedTrackVeto",                            &(myEvent_->isolatedTrackVeto));
    theTree_->Branch("tauVeto",                                      &(myEvent_->tauVeto));
   
    theTree_->Branch("nJets",                                        &(myEvent_->nJets));
    theTree_->Branch("nBTag",                                        &(myEvent_->nBTag));
    theTree_->Branch("jets","std::vector<TLorentzVector>",           &(myEvent_->jets));
    theTree_->Branch("jets_gen","std::vector<TLorentzVector>",       &(myEvent_->jets_gen));
    theTree_->Branch("jets_CSV_raw",     "std::vector<Float_t>",     &(myEvent_->jets_CSV_raw));
    theTree_->Branch("jets_CSV_reshaped","std::vector<Float_t>",     &(myEvent_->jets_CSV_reshaped));
    theTree_->Branch("jets_partonFlav",  "std::vector<Int_t>",       &(myEvent_->jets_partonFlav));

    theTree_->Branch("MET",                                          &(myEvent_->MET));
    theTree_->Branch("MT",                                           &(myEvent_->MT));
    theTree_->Branch("deltaPhiMETJets",                              &(myEvent_->deltaPhiMETJets));
    theTree_->Branch("hadronicChi2",                                 &(myEvent_->hadronicChi2));
    theTree_->Branch("MT2W",                                         &(myEvent_->MT2W));
    theTree_->Branch("HT",                                           &(myEvent_->HT));
    theTree_->Branch("HTRatio",                                      &(myEvent_->HTRatio));
    theTree_->Branch("leadingBPt",                                   &(myEvent_->leadingBPt));
    theTree_->Branch("leadingLeptonPt",                              &(myEvent_->leadingLeptonPt));
    theTree_->Branch("leadingJetPt",                                 &(myEvent_->leadingJetPt));
    theTree_->Branch("Mlb",                                          &(myEvent_->Mlb));
    theTree_->Branch("Mlb_hemi",                                     &(myEvent_->Mlb_hemi));
    theTree_->Branch("M3b",                                          &(myEvent_->M3b));
    theTree_->Branch("deltaRLeptonLeadingB",                         &(myEvent_->deltaRLeptonLeadingB));
    theTree_->Branch("METoverSqrtHT",                                &(myEvent_->METoverSqrtHT));
    theTree_->Branch("HTPlusLeptonPtPlusMET",                        &(myEvent_->HTPlusLeptonPtPlusMET));
    
    theTree_->Branch("nWTag",                                        &(myEvent_->nWTag));
    theTree_->Branch("leadingWjetPt",                                &(myEvent_->leadingWjetPt));
    
    theTree_->Branch("mStop",                                        &(myEvent_->mStop));
    theTree_->Branch("mNeutralino",                                  &(myEvent_->mNeutralino));
    theTree_->Branch("mCharginoParameter",                           &(myEvent_->mCharginoParameter));
    
    theTree_->Branch("numberOfGenLepton",                            &(myEvent_->numberOfGenLepton));
    theTree_->Branch("genParticles","std::vector<TLorentzVector>",   &(myEvent_->genParticles));
    theTree_->Branch("genParticlesPDGId","std::vector<Int_t>",       &(myEvent_->genParticlesPDGId));
    theTree_->Branch("genParticlesMother","std::vector<Int_t>",      &(myEvent_->genParticlesMother));
    
    theTree_->Branch("numberOfInitialEvents",                        &(myEvent_->numberOfInitialEvents));
    theTree_->Branch("crossSection",                                 &(myEvent_->crossSection));
    theTree_->Branch("numberOfTruePU",                               &(myEvent_->numberOfTruePU));
    theTree_->Branch("numberOfPrimaryVertices",                      &(myEvent_->numberOfPrimaryVertices));
    theTree_->Branch("weightCrossSection",                           &(myEvent_->weightCrossSection));
    theTree_->Branch("weightPileUp",                                 &(myEvent_->weightPileUp));
    theTree_->Branch("weightISRmodeling",                            &(myEvent_->weightISRmodeling));
    theTree_->Branch("weightTopPt",                                  &(myEvent_->weightTopPt));
    theTree_->Branch("weightTriggerEfficiency",                      &(myEvent_->weightTriggerEfficiency));
    theTree_->Branch("weightT2ttLeftHanded",                         &(myEvent_->weightT2ttLeftHanded));
    theTree_->Branch("weightT2ttRightHanded",                        &(myEvent_->weightT2ttRightHanded));
    theTree_->Branch("weightT2bwPolarization_ss",                    &(myEvent_->weightT2bwPolarization_ss));
    theTree_->Branch("weightT2bwPolarization_ls",                    &(myEvent_->weightT2bwPolarization_ss));
    theTree_->Branch("weightT2bwPolarization_sl",                    &(myEvent_->weightT2bwPolarization_ss));
    theTree_->Branch("weightT2bwPolarization_sr",                    &(myEvent_->weightT2bwPolarization_ss));
    theTree_->Branch("weightT2bwPolarization_rs",                    &(myEvent_->weightT2bwPolarization_ss));
    theTree_->Branch("weightT2bwPolarization_ll",                    &(myEvent_->weightT2bwPolarization_ll));
    theTree_->Branch("weightT2bwPolarization_lr",                    &(myEvent_->weightT2bwPolarization_lr));
    theTree_->Branch("weightT2bwPolarization_rl",                    &(myEvent_->weightT2bwPolarization_rl));
    theTree_->Branch("weightT2bwPolarization_rr",                    &(myEvent_->weightT2bwPolarization_rr));
   
    theTree_->Branch("MET_JESup",                                    &(myEvent_->MET_JESup));
    theTree_->Branch("MT_JESup",                                     &(myEvent_->MT_JESup));
    theTree_->Branch("deltaPhiMETJets_JESup",                        &(myEvent_->deltaPhiMETJets_JESup));
    theTree_->Branch("hadronicChi2_JESup",                           &(myEvent_->hadronicChi2_JESup));
    theTree_->Branch("MT2W_JESup",                                   &(myEvent_->MT2W_JESup));
    theTree_->Branch("HT_JESup",                                     &(myEvent_->HT_JESup));
    theTree_->Branch("HTRatio_JESup",                                &(myEvent_->HTRatio_JESup));
    theTree_->Branch("leadingBPt_JESup",                             &(myEvent_->leadingBPt_JESup));
    theTree_->Branch("leadingJetPt_JESup",                           &(myEvent_->leadingJetPt_JESup));
    theTree_->Branch("Mlb_JESup",                                    &(myEvent_->Mlb_JESup));
    theTree_->Branch("Mlb_hemi_JESup",                               &(myEvent_->Mlb_hemi_JESup));
    theTree_->Branch("M3b_JESup",                                    &(myEvent_->M3b_JESup));
    theTree_->Branch("deltaRLeptonLeadingB_JESup",                   &(myEvent_->deltaRLeptonLeadingB_JESup));
    theTree_->Branch("METoverSqrtHT_JESup",                          &(myEvent_->METoverSqrtHT_JESup));
    theTree_->Branch("HTPlusLeptonPtPlusMET_JESup",                  &(myEvent_->HTPlusLeptonPtPlusMET_JESup));
    theTree_->Branch("nJets_JESup",                                  &(myEvent_->nJets_JESup));
    theTree_->Branch("weightTriggerEfficiency_JESup",                &(myEvent_->weightTriggerEfficiency_JESup));
    
    theTree_->Branch("nBTag_JESup",                                  &(myEvent_->nBTag_JESup));
    theTree_->Branch("jets_JESup",                                   &(myEvent_->jets_JESup));
    theTree_->Branch("jets_gen_JESup",                               &(myEvent_->jets_gen_JESup));
    theTree_->Branch("jets_CSV_raw_JESup",                           &(myEvent_->jets_CSV_raw_JESup));
    theTree_->Branch("jets_CSV_reshaped_JESup",                      &(myEvent_->jets_CSV_reshaped_JESup));
   
    theTree_->Branch("MET_JESdown",                                  &(myEvent_->MET_JESdown));
    theTree_->Branch("MT_JESdown",                                   &(myEvent_->MT_JESdown));
    theTree_->Branch("deltaPhiMETJets_JESdown",                      &(myEvent_->deltaPhiMETJets_JESdown));
    theTree_->Branch("hadronicChi2_JESdown",                         &(myEvent_->hadronicChi2_JESdown));
    theTree_->Branch("MT2W_JESdown",                                 &(myEvent_->MT2W_JESdown));
    theTree_->Branch("HT_JESdown",                                   &(myEvent_->HT_JESdown));
    theTree_->Branch("HTRatio_JESdown",                              &(myEvent_->HTRatio_JESdown));
    theTree_->Branch("leadingBPt_JESdown",                           &(myEvent_->leadingBPt_JESdown));
    theTree_->Branch("leadingJetPt_JESdown",                         &(myEvent_->leadingJetPt_JESdown));
    theTree_->Branch("Mlb_JESdown",                                  &(myEvent_->Mlb_JESdown));
    theTree_->Branch("Mlb_hemi_JESdown",                             &(myEvent_->Mlb_hemi_JESdown));
    theTree_->Branch("M3b_JESdown",                                  &(myEvent_->M3b_JESdown));
    theTree_->Branch("deltaRLeptonLeadingB_JESdown",                 &(myEvent_->deltaRLeptonLeadingB_JESdown));
    theTree_->Branch("METoverSqrtHT_JESdown",                        &(myEvent_->METoverSqrtHT_JESdown));
    theTree_->Branch("HTPlusLeptonPtPlusMET_JESdown",                &(myEvent_->HTPlusLeptonPtPlusMET_JESdown));
    theTree_->Branch("nJets_JESdown",                                &(myEvent_->nJets_JESdown));
    theTree_->Branch("weightTriggerEfficiency_JESdown",              &(myEvent_->weightTriggerEfficiency_JESdown));

    theTree_->Branch("nBTag_JESdown",                                &(myEvent_->nBTag_JESdown));
    theTree_->Branch("jets_JESdown",                                 &(myEvent_->jets_JESdown));
    theTree_->Branch("jets_gen_JESdown",                             &(myEvent_->jets_gen_JESdown));
    theTree_->Branch("jets_CSV_raw_JESdown",                         &(myEvent_->jets_CSV_raw_JESdown));
    theTree_->Branch("jets_CSV_reshaped_JESdown",                    &(myEvent_->jets_CSV_reshaped_JESdown));

    theTree_->Branch("jets_CSV_reshapedUpBC",       "std::vector<Float_t>",          &(myEvent_->jets_CSV_reshapedUpBC));
    theTree_->Branch("jets_CSV_reshapedDownBC",     "std::vector<Float_t>",          &(myEvent_->jets_CSV_reshapedDownBC));
    theTree_->Branch("jets_CSV_reshapedUpLight",    "std::vector<Float_t>",          &(myEvent_->jets_CSV_reshapedUpLight));
    theTree_->Branch("jets_CSV_reshapedDownLight",  "std::vector<Float_t>",          &(myEvent_->jets_CSV_reshapedDownLight));

    theTree_->Branch("nonSelectedJets",             "std::vector<TLorentzVector>",   &(myEvent_->nonSelectedJets));
    theTree_->Branch("nonSelectedJets_gen",         "std::vector<TLorentzVector>",   &(myEvent_->nonSelectedJets_gen));
    theTree_->Branch("nonSelectedJets_CSV_raw",     "std::vector<Float_t>",          &(myEvent_->nonSelectedJets_CSV_raw));
    theTree_->Branch("nonSelectedJets_CSV_reshaped","std::vector<Float_t>",          &(myEvent_->nonSelectedJets_CSV_reshaped));
    theTree_->Branch("nonSelectedJets_partonFlav",  "std::vector<Int_t>",            &(myEvent_->nonSelectedJets_partonFlav));
    
    theTree_->Branch("nonSelectedLeptons",          "std::vector<TLorentzVector>",   &(myEvent_->nonSelectedLeptons));
    theTree_->Branch("nonSelectedLeptonsPDGId",     "std::vector<Short_t>",          &(myEvent_->nonSelectedLeptonsPDGId));
    
    theTree_->Branch("rawPFMET",                     &(myEvent_->rawPFMET));
    theTree_->Branch("METPhi",                       &(myEvent_->METPhi));

    theTree_->Branch("x_firstIncomingParton",        &(myEvent_->x_firstIncomingParton));       
    theTree_->Branch("x_secondIncomingParton",       &(myEvent_->x_secondIncomingParton));      
    theTree_->Branch("flavor_firstIncomingParton",   &(myEvent_->flavor_firstIncomingParton));  
    theTree_->Branch("flavor_secondIncomingParton",  &(myEvent_->flavor_secondIncomingParton)); 
    theTree_->Branch("scalePDF",                     &(myEvent_->scalePDF));                   

}

#define PI 3.141592653
#include "../inputs/polarizationReweighting.h"
#include "../inputs/triggerEfficiencyReweighting.h"
#include "../inputs/triggerEfficiencyReweighting.h"
#include "../inputs/electronIDReweighting.h"
#include "../inputs/stopCrossSections.h"

bool badSignalEventFilter(string datasetName,StopMCinfo* stopMCinfo);

Bool_t ProofJob::Process(Long64_t entry)
{
    // ########################################
    // #  Load the event from the input tree  #
    // ########################################

    fChain->GetTree()->GetEntry(entry); 
    branch->GetEntry(entry);
    IPHCTree::NTTransient::InitializeAfterReading(event);
    sel.LoadEvent(event);
    string datasetName_(dataset->Name());

    // ####################
    // #   Get MC infos   #
    // ####################

    bool runningOnData = dataset->isData();

    IPHCTree::NTMonteCarlo mcInfo;
    vector<IPHCTree::NTGenParticle> MCParticles;
    if (!runningOnData)
    {
        mcInfo = *((sel).GetPointer2MC());    
        MCParticles = mcInfo.genParticles;
        stopMCinfo->LoadEvent(event);
    }

    // ###############################
    // #  Filter bad signal events   #
    // ###############################
  
    if (badSignalEventFilter(datasetName_,stopMCinfo) == false) return false;

    // ######################
    // #  Apply selection   #
    // ######################
    
    if (sel.passEventSelection(runningOnData) == false) return false;

    // #####################
    // #   General infos   #
    // #####################

    myEvent.run   = event->general.runNb;  
    myEvent.lumi  = event->general.lumiblock;
    myEvent.event = event->general.eventNb;

    // #####################
    // #   Trigger infos   #
    // #####################

    myEvent.triggerMuon       = sel.passTrigger("singleMuon");
    myEvent.triggerElec       = sel.passTrigger("singleElec");
    myEvent.xtriggerMuon      = sel.passTrigger("crossMuon" );
    myEvent.triggerDoubleMuon = sel.passTrigger("doubleMuon");
    myEvent.triggerDoubleElec = sel.passTrigger("doubleElec");
    myEvent.triggerMuonElec   = sel.passTrigger("muonElec"  );  

    // #####################
    // #   Leptons infos   #
    // #####################

    myEvent.numberOfLepton              = sel.getTheNumberOfSelectedLeptons();
    
    myEvent.leadingLepton               = sel.getTheLeadingLepton();
    myEvent.leadingLeptonPDGId          = sel.getTheLeadingLeptonPDGId();
    myEvent.leadingLeptonIdEfficiency   = leptonIDEfficiency(myEvent.leadingLeptonPDGId,myEvent.leadingLepton.Pt(),myEvent.leadingLepton.Eta()).first;
    myEvent.leadingLeptonIsoScaleFactor = leptonIsoScaleFactor(myEvent.leadingLeptonPDGId,myEvent.leadingLepton.Pt(),myEvent.leadingLepton.Eta()).first;

    myEvent.secondLepton                = sel.getTheSecondLepton();
    myEvent.secondLeptonPDGId           = sel.getTheSecondLeptonPDGId();
    if (myEvent.numberOfLepton >= 2)
    {
        myEvent.secondLeptonIdEfficiency    = leptonIDEfficiency(myEvent.secondLeptonPDGId,myEvent.secondLepton.Pt(),myEvent.secondLepton.Eta()).first;
        myEvent.secondLeptonIsoScaleFactor  = leptonIsoScaleFactor(myEvent.secondLeptonPDGId,myEvent.secondLepton.Pt(),myEvent.secondLepton.Eta()).first;
    }
    else
    {
        myEvent.secondLeptonIdEfficiency    = 1.0;
        myEvent.secondLeptonIsoScaleFactor  = 1.0;
    }

    float leptonCharge;
    if (myEvent.leadingLeptonPDGId > 0) leptonCharge = -1.0;
    else                                leptonCharge = +1.0; 
    myEvent.isolatedTrackVeto           = sel.GetSUSYstopIsolatedTrackVeto(myEvent.leadingLepton,leptonCharge);
    myEvent.tauVeto                     = sel.GetSUSYstopTauVeto(myEvent.leadingLepton,leptonCharge);

    // ####################
    // #  Fill jets info  #
    // ####################

    myEvent.nJets        = sel.GetJetsForAna().size();
    myEvent.nBTag        = sel.GetBJetsForAna().size();

    myEvent.jets.clear();
    myEvent.jets_gen.clear();
    myEvent.jets_partonFlav.clear();
    myEvent.jets_CSV_raw.clear();
    myEvent.jets_CSV_reshaped.clear();
    myEvent.jets_CSV_reshapedUpBC.clear();
    myEvent.jets_CSV_reshapedDownBC.clear();
    myEvent.jets_CSV_reshapedUpLight.clear();
    myEvent.jets_CSV_reshapedDownLight.clear();

    std::vector<IPHCTree::NTJet> jets = sel.GetJetsForAna();
    for (unsigned int i = 0 ; i < jets.size() ; i++)
    {
        myEvent.jets.push_back(jets[i].p4);
        myEvent.jets_gen.push_back(jets[i].p4Gen);
        myEvent.jets_partonFlav.push_back(jets[i].partonFlavour);

        myEvent.jets_CSV_raw.push_back(              jets[i].bTag["combinedSecondaryVertexBJetTags"]                 );
        myEvent.jets_CSV_reshaped.push_back(         jets[i].bTag["zz1combinedSecondaryVertexBJetTagsReshapeNominal"]  );
        myEvent.jets_CSV_reshapedUpBC.push_back(     jets[i].bTag["zz2combinedSecondaryVertexBJetTagsReshapeUpBC"]     );
        myEvent.jets_CSV_reshapedDownBC.push_back(   jets[i].bTag["zz3combinedSecondaryVertexBJetTagsReshapeDownBC"]   );
        myEvent.jets_CSV_reshapedUpLight.push_back(  jets[i].bTag["zz4combinedSecondaryVertexBJetTagsReshapeUpLight"]  );
        myEvent.jets_CSV_reshapedDownLight.push_back(jets[i].bTag["zz5combinedSecondaryVertexBJetTagsReshapeDownLight"]);
    }

    // #############################
    // #  "High-level" variables   #
    // #############################

    myEvent.MET                   = sel.Met();
    myEvent.MT                    = sel.MT_wleptonic();
    myEvent.deltaPhiMETJets       = sel.DPhi_MET_leadingJets();
    myEvent.hadronicChi2          = sel.HadronicChi2(runningOnData);
    myEvent.MT2W                  = sel.MT2W();
    myEvent.HTRatio               = sel.HT_ratio();
    myEvent.HT                    = sel.HT();
    myEvent.METoverSqrtHT         = myEvent.MET / sqrt(myEvent.HT);
    myEvent.leadingLeptonPt       = myEvent.leadingLepton.Pt();
    myEvent.HTPlusLeptonPtPlusMET = myEvent.HT + myEvent.leadingLeptonPt + myEvent.MET;
    TLorentzVector leadingBJet    = sel.leadingBJet();
    myEvent.leadingBPt            = leadingBJet.Pt();
    myEvent.deltaRLeptonLeadingB  = leadingBJet.DeltaR(myEvent.leadingLepton);
    myEvent.leadingJetPt          = sel.leadingJet().P();
    myEvent.M3b                   = sel.M3b();
    myEvent.Mlb                   = (myEvent.leadingLepton + sel.leadingJetByCSV(runningOnData)).M();
    myEvent.Mlb_hemi              = (myEvent.leadingLepton + sel.bJetClosestToLeadingLepton()).M();

    // ####################
    // #   Signal infos   #
    // ####################

    if ((datasetName_.find("T2tt") != string::npos) 
    ||  (datasetName_.find("T2bw")  != string::npos)) 
    {
        myEvent.mStop       = stopMCinfo->GetStopMass();
        myEvent.mNeutralino = stopMCinfo->GetNeutralinoMass();

        if (datasetName_.find("T2tt") != string::npos)           myEvent.mCharginoParameter = -1.0;
        else if (datasetName_.find("T2bw-025") != string::npos)  myEvent.mCharginoParameter = 0.25;
        else if (datasetName_.find("T2bw-050") != string::npos)  myEvent.mCharginoParameter = 0.50; 
        else if (datasetName_.find("T2bw-075") != string::npos)  myEvent.mCharginoParameter = 0.75;
    }
    else
    {
        myEvent.mStop              = -1.0;
        myEvent.mNeutralino        = -1.0;
        myEvent.mCharginoParameter = -1.0;
    }

    // ################################################
    // #  Weights and weighting-related informations  #
    // ################################################

    // Cross section

    if ((datasetName_.find("T2tt") != string::npos) 
     || (datasetName_.find("T2bw") != string::npos))
    {
        myEvent.crossSection          = stopCrossSection(myEvent.mStop).first;

        TH2F* signalMap = 0;

        if (datasetName_.find("T2tt") != string::npos)          signalMap = mapT2tt; 
        else if (datasetName_.find("T2bw-025") != string::npos) signalMap = mapT2bw025;
        else if (datasetName_.find("T2bw-050") != string::npos) signalMap = mapT2bw050;
        else if (datasetName_.find("T2bw-075") != string::npos) signalMap = mapT2bw075;
       
        myEvent.numberOfInitialEvents = signalMap->GetBinContent(signalMap->FindBin(myEvent.mStop,myEvent.mNeutralino));
    }
    else
    {
        myEvent.crossSection          = dataset->Xsection();
        myEvent.numberOfInitialEvents = dataset->getNumberOfEventsBeforeMTSkimmer();
    }
    
    myEvent.weightCrossSection = myEvent.crossSection / myEvent.numberOfInitialEvents;

    // Pile-up

    // Don't take PU weight into account for data
    if (runningOnData)
    {
        myEvent.numberOfTruePU = 1.0;
        myEvent.weightPileUp = 1.0;
    }
    else
    {

        myEvent.numberOfTruePU = sel.getTnpv();
        myEvent.weightPileUp = sel.getPileUpWeight(myEvent.numberOfTruePU,runningOnData);
    }

    myEvent.numberOfPrimaryVertices = sel.GetVertex().size();;

    // ISR

    float ISRboost = 0.0;
    if ((datasetName_.find("T2tt") != string::npos) 
     || (datasetName_.find("T2bw") != string::npos))
    {
        TLorentzVector StopStopBar;
        for (unsigned int i = 0 ; i < MCParticles.size() ; i++)  
            if (abs(MCParticles[i].id) == 1000006) StopStopBar += MCParticles[i].p4;
        ISRboost = StopStopBar.Pt();
    }
    else if ((datasetName_ == "ttbar"                      )
          || (datasetName_ == "ttbar_madgraph_1l"          )
          || (datasetName_ == "ttbar_madgraph_2l"          )
          || (datasetName_ == "ttbar_madgraph_mass166-5"   )
          || (datasetName_ == "ttbar_madgraph_mass178-5"   )
          || (datasetName_ == "ttbar_madgraph_matchingdown")
          || (datasetName_ == "ttbar_madgraph_matchingup"  )
          || (datasetName_ == "ttbar_madgraph_scaledown"   )
          || (datasetName_ == "ttbar_madgraph_scaleup"     ))
    {
        TLorentzVector TopTopBar;
        for (unsigned int i = 0 ; i < MCParticles.size() ; i++)  
            if (abs(MCParticles[i].id) == 6) TopTopBar += MCParticles[i].p4;
        ISRboost = TopTopBar.Pt();
    }

    myEvent.weightISRmodeling = 1.0;
    if ((ISRboost > 120.0) && (ISRboost < 150.0)) myEvent.weightISRmodeling = 0.95;
    if ((ISRboost > 150.0) && (ISRboost < 250.0)) myEvent.weightISRmodeling = 0.90;
    if  (ISRboost > 250.0)                        myEvent.weightISRmodeling = 0.80;

    // Top Pt weight
    
    myEvent.weightTopPt = 1.0;
    if ((datasetName_ == "ttbar"                      )
     || (datasetName_ == "ttbar_madgraph_1l"          )
     || (datasetName_ == "ttbar_madgraph_2l"          )
     || (datasetName_ == "ttbar_madgraph_mass166-5"   )
     || (datasetName_ == "ttbar_madgraph_mass178-5"   )
     || (datasetName_ == "ttbar_madgraph_matchingdown")
     || (datasetName_ == "ttbar_madgraph_matchingup"  )
     || (datasetName_ == "ttbar_madgraph_scaledown"   )
     || (datasetName_ == "ttbar_madgraph_scaleup"     ))
    {
        float TopPt = 0.0;
        for (unsigned int i = 0 ; i < MCParticles.size() ; i++)  
            if (MCParticles[i].id == 6) { TopPt = MCParticles[i].p4.Pt(); break; }

        myEvent.weightTopPt = exp(0.156 - 0.00137 * TopPt); 
    }
    
    // Trigger efficiency weight

    if (abs(myEvent.leadingLeptonPDGId) == 11) 
        myEvent.weightTriggerEfficiency = singleLeptonTriggerWeight(11,myEvent.leadingLepton.Pt(),
                                                                       myEvent.leadingLepton.Eta());
    else if ((abs(myEvent.leadingLeptonPDGId) == 13) && (myEvent.leadingLepton.Pt() > 26))
        myEvent.weightTriggerEfficiency = singleLeptonTriggerWeight(13,myEvent.leadingLepton.Pt(),
                                                                       myEvent.leadingLepton.Eta());
    else if (myEvent.nJets >= 4)
        myEvent.weightTriggerEfficiency = crossTriggerWeight(myEvent.leadingLepton.Pt(),
                                                             myEvent.leadingLepton.Eta(),
                                                             myEvent.jets[3].Pt());
    else
        myEvent.weightTriggerEfficiency = -1.0;

    // Signal polarization

    if (datasetName_.find("T2tt") != string::npos) 
    {
        myEvent.weightT2ttLeftHanded  = Reweight_Stop_to_TopChi0(MCParticles, 0., -1);
        myEvent.weightT2ttRightHanded = Reweight_Stop_to_TopChi0(MCParticles, 0.,  1);
    }
    else
    {
        myEvent.weightT2ttLeftHanded  = 1.0;
        myEvent.weightT2ttRightHanded = 1.0; 
    }

    if (datasetName_.find("T2bw") != string::npos)
    {
        myEvent.weightT2bwPolarization_lr = Reweight_T2bW( 0      , 0      , MCParticles );
        myEvent.weightT2bwPolarization_ls = Reweight_T2bW( 0      , PI/4.0 , MCParticles );
        myEvent.weightT2bwPolarization_ll = Reweight_T2bW( 0      , PI/2.0 , MCParticles );
        myEvent.weightT2bwPolarization_sr = Reweight_T2bW( PI/4.0 , 0      , MCParticles );
        myEvent.weightT2bwPolarization_ss = Reweight_T2bW( PI/4.0 , PI/4.0 , MCParticles );
        myEvent.weightT2bwPolarization_sl = Reweight_T2bW( PI/4.0 , PI/2.0 , MCParticles );
        myEvent.weightT2bwPolarization_rr = Reweight_T2bW( PI/2.0 , 0      , MCParticles );
        myEvent.weightT2bwPolarization_rs = Reweight_T2bW( PI/2.0 , PI/4.0 , MCParticles );
        myEvent.weightT2bwPolarization_rl = Reweight_T2bW( PI/2.0 , PI/2.0 , MCParticles );
    }
    else
    {
        myEvent.weightT2bwPolarization_lr = 1.0; 
        myEvent.weightT2bwPolarization_ls = 1.0;
        myEvent.weightT2bwPolarization_ll = 1.0;
        myEvent.weightT2bwPolarization_sr = 1.0;
        myEvent.weightT2bwPolarization_ss = 1.0;
        myEvent.weightT2bwPolarization_sl = 1.0;
        myEvent.weightT2bwPolarization_rr = 1.0;
        myEvent.weightT2bwPolarization_rs = 1.0;
        myEvent.weightT2bwPolarization_rl = 1.0;
    }

    // ###################################
    // #   Generator-level information   #
    // ###################################

    myEvent.genParticles.clear();
    myEvent.genParticlesPDGId.clear();
    myEvent.genParticlesMother.clear();

    if (runningOnData)
    {
        myEvent.numberOfGenLepton = -1;
    }
    else
    {

        // TMEME decoding
        int TMEME = mcInfo.TMEME; 
        int  MEME = TMEME % 10000; 
        int   EME =  MEME % 1000; 
        int    ME =   EME % 100; 
        int     E =    ME % 10;

        myEvent.numberOfGenLepton = TMEME/10000 + ME/10 + E/1;

        for (unsigned int i = 0 ; i < MCParticles.size() ; i++)  
        {
            myEvent.genParticles.push_back(MCParticles[i].p4);
            myEvent.genParticlesPDGId.push_back(MCParticles[i].id);
            myEvent.genParticlesMother.push_back(MCParticles[i].motherIndex_);
        }
    }

    // ################
    // #  Fill W-tag  #
    // ################         

    myEvent.nWTag = 0;
    myEvent.leadingWjetPt = -1.0;
    std::vector<IPHCTree::NTJet> WCand = sel.GetHeavyTagJets();
    for (unsigned int i = 0 ; i < WCand.size() ; i++)
    {
        // Pt
        if (WCand[i].p4.Pt() < 200) continue;

        // Mass window
        if ((WCand[i].p4.M() < 60) || (WCand[i].p4.M() > 100)) continue;

        // Lepton overlap removal
        if (WCand[i].p4.DeltaR(myEvent.leadingLepton) < 0.6) continue; 

        // B-jet overlap removal
        float minDeltaRBJet = 99.0;
        for (int j = 0 ; j < myEvent.nBTag ; j++)
        {
            if (minDeltaRBJet > WCand[i].p4.DeltaR(sel.GetBJetsForAna()[j].p4))
                minDeltaRBJet = WCand[i].p4.DeltaR(sel.GetBJetsForAna()[j].p4);
        }
        if (minDeltaRBJet > 0.6)
        {
            myEvent.nWTag = 1.0;
            if (myEvent.leadingWjetPt < WCand[i].p4.Pt())
                myEvent.leadingWjetPt = WCand[i].p4.Pt();
        }

    }
    // #####################################
    // #  Info for studies of systematics  #
    // #####################################

        // Non selected jets

    myEvent.nonSelectedJets.clear();     
    myEvent.nonSelectedJets_gen.clear();     
    myEvent.nonSelectedJets_CSV_raw.clear();    
    myEvent.nonSelectedJets_CSV_reshaped.clear(); 
    myEvent.nonSelectedJets_partonFlav.clear();

    std::vector<IPHCTree::NTJet> goodJets = sel.GetGoodJets();
    for (unsigned int i = 0 ; i < goodJets.size() ; i++)
    {
        bool goodJetIsSelected = false;
        for (unsigned int j = 0 ; j < jets.size() ; j++)
        {
            if (jets[j].p4.Pt() == goodJets[i].p4.Pt()) goodJetIsSelected = true;
        }

        if (goodJetIsSelected) continue;

        myEvent.nonSelectedJets.push_back(goodJets[i].p4);
        myEvent.nonSelectedJets_gen.push_back(goodJets[i].p4Gen);
        myEvent.nonSelectedJets_CSV_raw.push_back(goodJets[i].bTag["combinedSecondaryVertexBJetTags"]);
        myEvent.nonSelectedJets_CSV_reshaped.push_back(goodJets[i].bTag["zz1combinedSecondaryVertexBJetTagsReshapeNominal"]); 
        myEvent.nonSelectedJets_partonFlav.push_back(goodJets[i].partonFlavour);
    }

        // Non selected leptons
    myEvent.nonSelectedLeptons.clear();
    myEvent.nonSelectedLeptonsPDGId.clear();

    std::vector<IPHCTree::NTElectron> goodElectrons = sel.GetGoodElectrons();
    std::vector<IPHCTree::NTMuon>     goodMuons = sel.GetGoodMuons();

    for (unsigned i = 0 ; i < goodElectrons.size() ; i++)
    {
        bool goodElectronIsSelected = false;
        if (goodElectrons[i].p4.Pt() == myEvent.leadingLepton.Pt()) goodElectronIsSelected = true; 
        if (goodElectrons[i].p4.Pt() == myEvent.secondLepton.Pt()) goodElectronIsSelected = true;

        if (goodElectronIsSelected) continue;

        myEvent.nonSelectedLeptons.push_back(goodElectrons[i].p4);
        myEvent.nonSelectedLeptonsPDGId.push_back(goodElectrons[i].charge * (-11));
    }

    for (unsigned i = 0 ; i < goodMuons.size() ; i++)
    {
        bool goodMuonIsSelected = false;
        if (goodMuons[i].p4.Pt() == myEvent.leadingLepton.Pt()) goodMuonIsSelected = true; 
        if (goodMuons[i].p4.Pt() == myEvent.secondLepton.Pt()) goodMuonIsSelected = true;

        if (goodMuonIsSelected) continue;

        myEvent.nonSelectedLeptons.push_back(goodMuons[i].p4);
        myEvent.nonSelectedLeptonsPDGId.push_back(goodMuons[i].charge * (-13));
    }

        // Raw MET
        
    myEvent.rawPFMET = sel.rawMet();
    
        // MET Phi

    myEvent.METPhi   = sel.MetPhi();

        // PDF infos

    if (runningOnData == false)
    {
        myEvent.x_firstIncomingParton       = mcInfo.x.first;
        myEvent.x_secondIncomingParton      = mcInfo.x.second;
        myEvent.flavor_firstIncomingParton  = mcInfo.partonFlavor.first;
        myEvent.flavor_secondIncomingParton = mcInfo.partonFlavor.second;
        myEvent.scalePDF                    = mcInfo.Q_scale;
    }

    sel.doObjectSelection(runningOnData,1);
    sel.FillKinematicP4();
   
    myEvent.MET_JESup                       = sel.Met();
    myEvent.MT_JESup                        = sel.MT_wleptonic();
    myEvent.deltaPhiMETJets_JESup           = sel.DPhi_MET_leadingJets();
    myEvent.hadronicChi2_JESup              = sel.HadronicChi2(runningOnData);
    myEvent.MT2W_JESup                      = sel.MT2W();
    myEvent.HTRatio_JESup                   = sel.HT_ratio();
    myEvent.HT_JESup                        = sel.HT();
    myEvent.METoverSqrtHT_JESup             = myEvent.MET_JESup / sqrt(myEvent.HT_JESup);
    leadingBJet = sel.leadingBJet();
    myEvent.leadingBPt_JESup                = leadingBJet.Pt();
    myEvent.deltaRLeptonLeadingB_JESup = leadingBJet.DeltaR(myEvent.leadingLepton);
    myEvent.leadingJetPt_JESup              = sel.leadingJet().P();
    myEvent.M3b_JESup                       = sel.M3b();
    myEvent.Mlb_JESup                       = (myEvent.leadingLepton + sel.leadingJetByCSV(runningOnData)).M();
    myEvent.Mlb_hemi_JESup                  = (myEvent.leadingLepton + sel.bJetClosestToLeadingLepton()).M();
    myEvent.HTPlusLeptonPtPlusMET_JESup     = myEvent.HT_JESup + myEvent.leadingLeptonPt + myEvent.MET_JESup;
    myEvent.nJets_JESup                     = sel.GetJetsForAna().size();

    if (abs(myEvent.leadingLeptonPDGId) == 11) 
        myEvent.weightTriggerEfficiency_JESup = singleLeptonTriggerWeight(11,myEvent.leadingLepton.Pt(),
                                                                             myEvent.leadingLepton.Eta());
    else if ((abs(myEvent.leadingLeptonPDGId) == 13) && (myEvent.leadingLepton.Pt() > 26))
        myEvent.weightTriggerEfficiency_JESup = singleLeptonTriggerWeight(13,myEvent.leadingLepton.Pt(),
                                                                             myEvent.leadingLepton.Eta());
    else if (myEvent.nJets_JESup >= 4)
        myEvent.weightTriggerEfficiency_JESup = crossTriggerWeight(myEvent.leadingLepton.Pt(),
                                                                   myEvent.leadingLepton.Eta(),
                                                                   myEvent.jets[3].Pt());
    else
        myEvent.weightTriggerEfficiency_JESup = -1.0;
    
    myEvent.nBTag_JESup        = sel.GetBJetsForAna().size();
    myEvent.jets_JESup.clear();
    myEvent.jets_gen_JESup.clear();
    myEvent.jets_CSV_raw_JESup.clear();
    myEvent.jets_CSV_reshaped_JESup.clear();

    jets = sel.GetJetsForAna();
    for (unsigned int i = 0 ; i < jets.size() ; i++)
    {
        myEvent.jets_JESup.push_back(jets[i].p4);
        myEvent.jets_gen_JESup.push_back(jets[i].p4Gen);
        myEvent.jets_CSV_raw_JESup.push_back(     jets[i].bTag["combinedSecondaryVertexBJetTags"]                 );
        myEvent.jets_CSV_reshaped_JESup.push_back(jets[i].bTag["zz1combinedSecondaryVertexBJetTagsReshapeNominal"]);
    }


    sel.doObjectSelection(runningOnData,-1);
    sel.FillKinematicP4();
    
    myEvent.MET_JESdown                     = sel.Met();
    myEvent.MT_JESdown                      = sel.MT_wleptonic();
    myEvent.deltaPhiMETJets_JESdown         = sel.DPhi_MET_leadingJets();
    myEvent.hadronicChi2_JESdown            = sel.HadronicChi2(runningOnData);
    myEvent.MT2W_JESdown                    = sel.MT2W();
    myEvent.HTRatio_JESdown                 = sel.HT_ratio();
    myEvent.HT_JESdown                      = sel.HT();
    myEvent.METoverSqrtHT_JESdown           = myEvent.MET_JESdown / sqrt(myEvent.HT_JESdown);
    leadingBJet = sel.leadingBJet();
    myEvent.leadingBPt_JESdown              = leadingBJet.Pt();
    myEvent.deltaRLeptonLeadingB_JESdown = leadingBJet.DeltaR(myEvent.leadingLepton);
    myEvent.leadingJetPt_JESdown            = sel.leadingJet().P();
    myEvent.M3b_JESdown                     = sel.M3b();
    myEvent.Mlb_JESdown                     = (myEvent.leadingLepton + sel.leadingJetByCSV(runningOnData)).M();
    myEvent.Mlb_hemi_JESdown                = (myEvent.leadingLepton + sel.bJetClosestToLeadingLepton()).M();
    myEvent.HTPlusLeptonPtPlusMET_JESdown   = myEvent.HT_JESdown + myEvent.leadingLeptonPt + myEvent.MET_JESdown;
    myEvent.nJets_JESdown                   = sel.GetJetsForAna().size();

    if (abs(myEvent.leadingLeptonPDGId) == 11) 
        myEvent.weightTriggerEfficiency_JESdown = singleLeptonTriggerWeight(11,myEvent.leadingLepton.Pt(),
                                                                               myEvent.leadingLepton.Eta());
    else if ((abs(myEvent.leadingLeptonPDGId) == 13) && (myEvent.leadingLepton.Pt() > 26))
        myEvent.weightTriggerEfficiency_JESdown = singleLeptonTriggerWeight(13,myEvent.leadingLepton.Pt(),
                                                                               myEvent.leadingLepton.Eta());
    else if (myEvent.nJets_JESdown >= 4)
        myEvent.weightTriggerEfficiency_JESdown = crossTriggerWeight(myEvent.leadingLepton.Pt(),
                                                                     myEvent.leadingLepton.Eta(),
                                                                     myEvent.jets[3].Pt());
    else
        myEvent.weightTriggerEfficiency_JESdown = -1.0;

    myEvent.nBTag_JESdown        = sel.GetBJetsForAna().size();
    myEvent.jets_JESdown.clear();
    myEvent.jets_gen_JESdown.clear();
    myEvent.jets_CSV_raw_JESdown.clear();
    myEvent.jets_CSV_reshaped_JESdown.clear();

    jets = sel.GetJetsForAna();
    for (unsigned int i = 0 ; i < jets.size() ; i++)
    {
        myEvent.jets_JESdown.push_back(jets[i].p4);
        myEvent.jets_gen_JESdown.push_back(jets[i].p4Gen);
        myEvent.jets_CSV_raw_JESdown.push_back(     jets[i].bTag["combinedSecondaryVertexBJetTags"]                 );
        myEvent.jets_CSV_reshaped_JESdown.push_back(jets[i].bTag["zz1combinedSecondaryVertexBJetTagsReshapeNominal"]);
    }

    // ###############################
    // #  Add the event to the tree  #
    // ###############################

    theTree->Fill();

    return true;
}

bool badSignalEventFilter(string datasetName,StopMCinfo* stopMCinfo)
{

    if ((datasetName.find("T2tt") != string::npos)
     || (datasetName.find("T2bw") != string::npos))
    {
        if ((datasetName.find("withLSP0")  != string::npos)
         || (datasetName.find("LSP0to700") != string::npos))
        {
            float mStop       = stopMCinfo->GetStopMass();
            float mNeutralino = stopMCinfo->GetNeutralinoMass();

            if (mNeutralino - int(mNeutralino) > 0.000001) return false;
            if (mStop       - int(mStop)       > 0.000001) return false;
            if (int(mNeutralino) % 25 != 0) return false;
            if (int(mStop)       % 25 != 0) return false;

            if ((datasetName.find("withLSP0") != string::npos) && (mNeutralino == 500)) return false;
        }

    }

    return true;
}



#endif


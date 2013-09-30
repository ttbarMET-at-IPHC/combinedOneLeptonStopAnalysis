#ifndef babyFormat
#define babyFormat



typedef struct 
{
    // ------------
    // General info
    // ------------

    Int_t run;
    Int_t lumi;
    Int_t event;

    // ------------
    // Trigger info
    // ------------

    Bool_t triggerMuon;
    Bool_t xtriggerMuon;
    Bool_t triggerElec;
    Bool_t xtriggerElec;

    // ------------
    // Leptons info
    // ------------

    Short_t        numberOfLepton;
    TLorentzVector leadingLepton;
    Short_t        leadingLeptonPDGId;
    TLorentzVector secondLepton;
    Short_t        secondLeptonPDGId;

    Bool_t         isolatedTrackVeto;
    Bool_t         tauVeto;
    
    // ------------
    // Jets info
    // ------------

    Short_t        nJets;
    Short_t        nBTag;
    vector<TLorentzVector> jets;
    vector<float> jets_CSV;

    // ------------
    // "High-level" variables
    // ------------

    Float_t MET;
    Float_t MT;
    Float_t deltaPhiMETJets;
    Float_t hadronicChi2;
    Float_t MT2W;
    Float_t HTRatio;
    Float_t leadingBPt;
    Float_t deltaRLeptonLeadingB;

    // ------------
    // W-tagging info
    // ------------

    Short_t nWTag;
    Float_t leadingWjetPt;

    // ------------
    // Infos for systematic effects
    // ------------
    
    Float_t MET_up;
    Float_t MET_down;
    Float_t MT_up;
    Float_t MT_down;

    // ------------
    // Weights and infos related to weights
    // ------------

    Int_t   numberOfInitialEvents;
    Float_t crossSection;
    Int_t   numberOfTruePU;
    Int_t   numberOfPrimaryVertices;

    Float_t weightCrossSection;
    Float_t weightPileUp;
    Float_t weightISRmodeling;

    Float_t weightT2ttLeftHanded;
    Float_t weightT2ttRightHanded;
    Float_t weightT2bwPolarization_ss;
    Float_t weightT2bwPolarization_ll;
    Float_t weightT2bwPolarization_lr;
    Float_t weightT2bwPolarization_rl;
    Float_t weightT2bwPolarization_rr;
    
    Float_t efficiencySingleLeptonTrigger;
    Float_t efficiencyDoubleLeptonTrigger;

    // ------------
    // Signal-specific infos
    // ------------

    Float_t mStop;
    Float_t mNeutralino;
    Float_t mCharginoParameter;
 
    // ------------
    // Generator-level infos
    // ------------
   
    Int_t   numberOfGenLepton;
    Float_t genMET;                     // Definition ? Info available ?
    Float_t genMETPhi;  

    vector<TLorentzVector> genParticles;
    vector<Int_t>        genParticlesPDGId;
    vector<Int_t>        genParticlesMother;


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

    theTree->SetBranchAddress("run",&(myEvent->run));
    theTree->SetBranchAddress("lumi",&(myEvent->lumi));
    theTree->SetBranchAddress("event",&(myEvent->event));

    theTree->SetBranchAddress("triggerMuon",&(myEvent->triggerMuon));
    //theTree->SetBranchAddress("xtriggerMuon",&(myEvent->xtriggerMuon));     // Need inputs from Alberto/Caroline
    theTree->SetBranchAddress("triggerElec",&(myEvent->triggerElec));
    //theTree->SetBranchAddress("xtriggerElec",&(myEvent->xtriggerElec));     // Need inputs from Alberto/Caroline

    theTree->SetBranchAddress("numberOfLepton",&(myEvent->numberOfLepton));
    
    pointers->pointerToLeadingLepton = 0;
    theTree->SetBranchAddress("leadingLepton",&(pointers->pointerToLeadingLepton));
    theTree->SetBranchAddress("leadingLeptonPDGId",&(myEvent->leadingLeptonPDGId));
    pointers->pointerToSecondLepton = 0;
    theTree->SetBranchAddress("secondLepton",&(pointers->pointerToSecondLepton));
    theTree->SetBranchAddress("secondLeptonPDGId",&(myEvent->secondLeptonPDGId));
    theTree->SetBranchAddress("isolatedTrackVeto",&(myEvent->isolatedTrackVeto));
    theTree->SetBranchAddress("tauVeto",&(myEvent->tauVeto));

    theTree->SetBranchAddress("nJets",&(myEvent->nJets));
    theTree->SetBranchAddress("nBTag",&(myEvent->nBTag));
    pointers->pointerToJets = 0;
    theTree->SetBranchAddress("jets",&(pointers->pointerToJets));
    pointers->pointerToJets_CSV = 0;
    theTree->SetBranchAddress("jets_CSV",&(pointers->pointerToJets_CSV));     // Need inputs from Franscesco/?? for bTag reweighting

    theTree->SetBranchAddress("MET",&(myEvent->MET));
    theTree->SetBranchAddress("MT",&(myEvent->MT));
    theTree->SetBranchAddress("deltaPhiMETJets",&(myEvent->deltaPhiMETJets));
    theTree->SetBranchAddress("hadronicChi2",&(myEvent->hadronicChi2));
    theTree->SetBranchAddress("MT2W",&(myEvent->MT2W));
    theTree->SetBranchAddress("HTRatio",&(myEvent->HTRatio));
    theTree->SetBranchAddress("leadingBPt",&(myEvent->leadingBPt));
    theTree->SetBranchAddress("deltaRLeptonLeadingB",&(myEvent->deltaRLeptonLeadingB));
    
    theTree->SetBranchAddress("nWTag",&(myEvent->nWTag));
    theTree->SetBranchAddress("leadingWjetPt",&(myEvent->leadingWjetPt));

    theTree->SetBranchAddress("MET_up",&(myEvent->MET_up));                   // Need to check how exactly to do this and if it is needed for every variables likely to go in the BDT
    theTree->SetBranchAddress("MET_down",&(myEvent->MET_down));
    theTree->SetBranchAddress("MT_up",&(myEvent->MT_up));
    theTree->SetBranchAddress("MT_down",&(myEvent->MT_down));

    theTree->SetBranchAddress("numberOfInitialEvents",&(myEvent->numberOfInitialEvents));
    theTree->SetBranchAddress("crossSection",&(myEvent->crossSection));
    theTree->SetBranchAddress("numberOfTruePU",&(myEvent->numberOfTruePU));
    theTree->SetBranchAddress("numberOfPrimaryVertices",&(myEvent->numberOfPrimaryVertices));
    theTree->SetBranchAddress("weightCrossSection",&(myEvent->weightCrossSection));
    theTree->SetBranchAddress("weightPileUp",&(myEvent->weightPileUp));
    theTree->SetBranchAddress("weightISRmodeling",&(myEvent->weightISRmodeling));
    theTree->SetBranchAddress("weightT2ttLeftHanded",&(myEvent->weightT2ttLeftHanded));
    theTree->SetBranchAddress("weightT2ttRightHanded",&(myEvent->weightT2ttRightHanded));
    theTree->SetBranchAddress("weightT2bwPolarization_ss",&(myEvent->weightT2bwPolarization_ss));
    theTree->SetBranchAddress("weightT2bwPolarization_ll",&(myEvent->weightT2bwPolarization_ll));
    theTree->SetBranchAddress("weightT2bwPolarization_lr",&(myEvent->weightT2bwPolarization_lr));
    theTree->SetBranchAddress("weightT2bwPolarization_rl",&(myEvent->weightT2bwPolarization_rl));
    theTree->SetBranchAddress("weightT2bwPolarization_rr",&(myEvent->weightT2bwPolarization_rr));
    theTree->SetBranchAddress("efficiencySingleLeptonTrigger",&(myEvent->efficiencySingleLeptonTrigger));
    theTree->SetBranchAddress("efficiencyDoubleLeptonTrigger",&(myEvent->efficiencyDoubleLeptonTrigger));
    
    theTree->SetBranchAddress("mStop",&(myEvent->mStop));
    theTree->SetBranchAddress("mNeutralino",&(myEvent->mNeutralino));
    theTree->SetBranchAddress("mCharginoParameter",&(myEvent->mCharginoParameter));
    
    theTree->SetBranchAddress("numberOfGenLepton",&(myEvent->numberOfGenLepton));
    theTree->SetBranchAddress("genMET",&(myEvent->genMET));
    theTree->SetBranchAddress("genMETPhi",&(myEvent->genMETPhi));

    pointers->pointerToGenParticles = 0;
    theTree->SetBranchAddress("genParticles",&(pointers->pointerToGenParticles));
    pointers->pointerToGenParticlesPDGId = 0;
    theTree->SetBranchAddress("genParticlesPDGId",&(pointers->pointerToGenParticlesPDGId));
    pointers->pointerToGenParticlesMother = 0;
    theTree->SetBranchAddress("genParticlesMother",&(pointers->pointerToGenParticlesMother));
}




#endif


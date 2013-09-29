#ifndef microTupleFormat
#define microTupleFormat

// If you want to change the format being used by
// the proof jobs, you should edit the file
// ProofMicroTupler_Format.h


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
    Float_t        leadingLeptonIsolation;
    Short_t        leadingLeptonPDGId;
    TLorentzVector secondLepton;
    Float_t        secondLeptonIsolation;
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
    Float_t genMET;
    Float_t genMETPhi;

    vector<TLorentzVector> genParticles;
    vector<Short_t>        genParticlesPDGId;
    vector<Short_t>        genParticlesMother;


} babyEvent;

#endif
  


// ###################################################
// #     This function is called by the proof        #
// #   job - this is where you fill the microtuple   #
// ###################################################
#ifdef isCompilingTheBabyTupler

void ProofJob::InitializeBranches(TTree* theTree, babyEvent* myEvent)
{
    theTree->Branch("run",&(myEvent->run));
    theTree->Branch("lumi",&(myEvent->lumi));
    theTree->Branch("event",&(myEvent->event));

    theTree->Branch("triggerMuon",&(myEvent->triggerMuon));
    //theTree->Branch("xtriggerMuon",&(myEvent->xtriggerMuon));
    theTree->Branch("triggerElec",&(myEvent->triggerElec));
    //theTree->Branch("xtriggerElec",&(myEvent->xtriggerElec));

    theTree->Branch("numberOfLepton",&(myEvent->numberOfLepton));
    theTree->Branch("leadingLepton",&(myEvent->leadingLepton));
    theTree->Branch("leadingLeptonIsolation",&(myEvent->leadingLeptonIsolation));
    theTree->Branch("leadingLeptonPDGId",&(myEvent->leadingLeptonPDGId));
    theTree->Branch("secondLepton",&(myEvent->secondLepton));
    theTree->Branch("secondLeptonIsolation",&(myEvent->secondLeptonIsolation));
    theTree->Branch("secondLeptonPDGId",&(myEvent->secondLeptonPDGId));
    theTree->Branch("isolatedTrackVeto",&(myEvent->isolatedTrackVeto));
    theTree->Branch("tauVeto",&(myEvent->tauVeto));

    theTree->Branch("nJets",&(myEvent->nJets));
    theTree->Branch("nBTag",&(myEvent->nBTag));
    theTree->Branch("jets",&(myEvent->jets));
    theTree->Branch("jets_CSV",&(myEvent->jets_CSV));

    theTree->Branch("MET",&(myEvent->MET));
    theTree->Branch("MT",&(myEvent->MT));
    theTree->Branch("deltaPhiMETJets",&(myEvent->deltaPhiMETJets));
    theTree->Branch("hadronicChi2",&(myEvent->hadronicChi2));
    theTree->Branch("MT2W",&(myEvent->MT2W));
    theTree->Branch("HTRatio",&(myEvent->HTRatio));
    theTree->Branch("leadingBPt",&(myEvent->leadingBPt));
    theTree->Branch("deltaRLeptonLeadingB",&(myEvent->deltaRLeptonLeadingB));

    theTree->Branch("MET_up",&(myEvent->MET_up));
    theTree->Branch("MET_down",&(myEvent->MET_down));
    theTree->Branch("MT_up",&(myEvent->MT_up));
    theTree->Branch("MT_down",&(myEvent->MT_down));

    theTree->Branch("numberOfInitialEvents",&(myEvent->numberOfInitialEvents));
    theTree->Branch("crossSection",&(myEvent->crossSection));
    theTree->Branch("numberOfTruePU",&(myEvent->numberOfTruePU));
    theTree->Branch("numberOfPrimaryVertices",&(myEvent->numberOfPrimaryVertices));
    theTree->Branch("weightCrossSection",&(myEvent->weightCrossSection));
    theTree->Branch("weightPileUp",&(myEvent->weightPileUp));
    theTree->Branch("weightISRmodeling",&(myEvent->weightISRmodeling));
    theTree->Branch("weightT2ttLeftHanded",&(myEvent->weightT2ttLeftHanded));
    theTree->Branch("weightT2ttRightHanded",&(myEvent->weightT2ttRightHanded));
    theTree->Branch("weightT2bwPolarization_ss",&(myEvent->weightT2bwPolarization_ss));
    theTree->Branch("weightT2bwPolarization_ll",&(myEvent->weightT2bwPolarization_ll));
    theTree->Branch("weightT2bwPolarization_lr",&(myEvent->weightT2bwPolarization_lr));
    theTree->Branch("weightT2bwPolarization_rl",&(myEvent->weightT2bwPolarization_rl));
    theTree->Branch("weightT2bwPolarization_rr",&(myEvent->weightT2bwPolarization_rr));
    theTree->Branch("efficiencySingleLeptonTrigger",&(myEvent->efficiencySingleLeptonTrigger));
    theTree->Branch("efficiencyDoubleLeptonTrigger",&(myEvent->efficiencyDoubleLeptonTrigger));
    
    theTree->Branch("mStop",&(myEvent->mStop));
    theTree->Branch("mNeutralino",&(myEvent->mNeutralino));
    theTree->Branch("mCharginoParameter",&(myEvent->mCharginoParameter));
    
    theTree->Branch("numberOfGenLepton",&(myEvent->numberOfGenLepton));
    theTree->Branch("genMET",&(myEvent->genMET));
    theTree->Branch("genMETPhi",&(myEvent->genMETPhi));
    theTree->Branch("genParticles",&(myEvent->genParticles));
    theTree->Branch("genParticlesPDGId",&(myEvent->genParticlesPDGId));
    theTree->Branch("genParticlesMother",&(myEvent->genParticlesMother));

}


float stopCrossSection(float inputMass);
    
Bool_t ProofJob::Process(Long64_t entry)
{

  // ########################################
  // #  Load the event from the input tree  #
  // ########################################
  
    fChain->GetTree()->GetEntry(entry); 
    branch->GetEntry(entry);
    IPHCTree::NTTransient::InitializeAfterReading(event);
    sel.LoadEvent(event);

  // ######################
  // #  Apply selection   #
  // ######################

    pair<bool,bool> triggerME;
    if (sel.doFullSelection(dataset,&triggerME) == false) return false;

  // #####################
  // #   General infos   #
  // #####################

    myEvent.run   = event->general.runNb;  
    myEvent.lumi  = event->general.lumiblock;
    myEvent.event = event->general.eventNb;

  // #####################
  // #   General infos   #
  // #####################

    myEvent.triggerMuon = triggerME.first;
    myEvent.triggerElec = triggerME.second;
    
    //myEvent.xtriggerMuon;
    //myEvent.xtriggerElec;

  // #####################
  // #   Leptons infos   #
  // #####################

    /*
    myEvent.numberOfLepton          = ;
    myEvent.leadingLepton           = ;
    myEvent.leadingLeptonIsolation  = ;
    myEvent.leadingLeptonPDGId      = ;
    myEvent.secondLepton            = ;
    myEvent.secondLeptonIsolation   = ;
    myEvent.secondLeptonPDGId       = ;
    myEvent.isolatedTrackVeto       = ;
    myEvent.tauVeto                 = ;
    myEvent.trueNumberOfEvents = dataset->getNumberOfEventsBeforeMTSkimmer();
    */


  // ####################
  // #   Signal infos   #
  // ####################

    if ((dataset->Name() == "T2tt") && (dataset->Name() == "T2bw"))
    {
        stopMCinfo->LoadEvent(event);
        myEvent.mStop       = stopMCinfo->GetStopMass();
        myEvent.mNeutralino = stopMCinfo->GetNeutralinoMass();

        cout << "mStop = " << myEvent.mStop << endl;

    }
    else
    {
        myEvent.mStop       = -1;
        myEvent.mNeutralino = -1;
    }
        




    IPHCTree::NTMonteCarlo mcInfo = *((sel).GetPointer2MC());    
    int TMEME = mcInfo.TMEME; 
    int  MEME = TMEME % 10000; 
    int   EME =  MEME % 1000; 
    int    ME =   EME % 100; 
    int     E =    ME % 10;

    int nMCLepton = TMEME/10000 + ME/10 + E/1;


    // ####################
    // #  Fill jets info  #
    // ####################

    /*
    myEvent.nMCLepton    = nMCLepton; 
    myEvent.nJets        = sel.GetJetsForAna().size();
    myEvent.nBTag        = sel.GetBJetsForAna().size();
    myEvent.MT           = sel.MT_wleptonic();

    myEvent.MET          = sel.Met();
    myEvent.MT2W         = sel.MT2W();
    myEvent.dPhiMETjet   = sel.DPhi_MET_leadingJets(); 
    myEvent.HTratio      = sel.HT_ratio();
    myEvent.HadronicChi2 = sel.HadronicChi2(dataset->isData());
    */
    // ####################
    // #  Get the lepton  #
    // ####################        

    /*
    TLorentzVector lepton_p;
    if (sel.GetMuonsForAna().size()==1) 
        lepton_p = (sel.GetMuonsForAna()[0]).p4;
    else 
        lepton_p = (sel.GetElectronsForAna()[0]).p4;
 
    myEvent.leptonPt = lepton_p.Pt();
    */

    // ##########################
    // #  Get the leading b pT  #
    // ##########################
   
    /*
    myEvent.leadingBPt = -1.0;
    for (int i = 0 ; i < myEvent.nBTag ; i++)
    {
        if (myEvent.leadingBPt < sel.GetBJetsForAna()[i].p4.Pt())
            myEvent.leadingBPt = sel.GetBJetsForAna()[i].p4.Pt();
    }
    */

    // ################
    // #  Fill W-tag  #
    // ################         

    /*
    myEvent.nWTag  = 0;
    myEvent.nWTag2 = 0;
    std::vector<IPHCTree::NTJet> WCand = sel.GetHeavyTagJets();
    for (unsigned int i = 0 ; i < WCand.size() ; i++)
    {
        // Pt
        if (WCand[i].p4.Pt() < 200) continue;

        // Mass window
        if ((WCand[i].p4.M() < 60) || (WCand[i].p4.M() > 100)) continue;

        // Lepton overlap removal
        if (WCand[i].p4.DeltaR(lepton_p) < 0.6) continue; 

        // B-jet overlap removal
        float minDeltaRBJet = 99.0;
        for (int j = 0 ; j < myEvent.nBTag ; j++)
        {
            if (minDeltaRBJet > WCand[i].p4.DeltaR(sel.GetBJetsForAna()[j].p4));
                minDeltaRBJet = WCand[i].p4.DeltaR(sel.GetBJetsForAna()[j].p4);
        }
        if (minDeltaRBJet > 0.6) 
            myEvent.nWTag2 += 1.0;

    }
    */


    // ###############################
    // #  Add the event to the tree  #
    // ###############################
    
    theTree->Fill();

    return true;
}

float stopCrossSection(float inputMass)
{
         if (abs(inputMass - 170) <= 5) return 42.6441;
    else if (abs(inputMass - 180) <= 5) return 31.8695;
    else if (abs(inputMass - 190) <= 5) return 24.1585;
    else if (abs(inputMass - 200) <= 5) return 18.5245;
    else if (abs(inputMass - 210) <= 5) return 14.3201;
    else if (abs(inputMass - 220) <= 5) return 11.1808;
    else if (abs(inputMass - 230) <= 5) return 8.78125;
    else if (abs(inputMass - 240) <= 5) return 6.96892;
    else if (abs(inputMass - 250) <= 5) return 5.57596;
    else if (abs(inputMass - 260) <= 5) return 4.48773;
    else if (abs(inputMass - 270) <= 5) return 3.63085;
    else if (abs(inputMass - 280) <= 5) return 2.95613;
    else if (abs(inputMass - 290) <= 5) return 2.42299;
    else if (abs(inputMass - 300) <= 5) return 1.99608;
    else if (abs(inputMass - 310) <= 5) return 1.64956;
    else if (abs(inputMass - 320) <= 5) return 1.3733;
    else if (abs(inputMass - 330) <= 5) return 1.14277;
    else if (abs(inputMass - 340) <= 5) return 0.959617;
    else if (abs(inputMass - 350) <= 5) return 0.807323;
    else if (abs(inputMass - 360) <= 5) return 0.681346;
    else if (abs(inputMass - 370) <= 5) return 0.576882;
    else if (abs(inputMass - 380) <= 5) return 0.489973;
    else if (abs(inputMass - 390) <= 5) return 0.4176;
    else if (abs(inputMass - 400) <= 5) return 0.35683;
    else if (abs(inputMass - 410) <= 5) return 0.305512;
    else if (abs(inputMass - 420) <= 5) return 0.262683;
    else if (abs(inputMass - 430) <= 5) return 0.226367;
    else if (abs(inputMass - 440) <= 5) return 0.195812;
    else if (abs(inputMass - 450) <= 5) return 0.169668;
    else if (abs(inputMass - 460) <= 5) return 0.147492;
    else if (abs(inputMass - 470) <= 5) return 0.128326;
    else if (abs(inputMass - 480) <= 5) return 0.112241;
    else if (abs(inputMass - 490) <= 5) return 0.0977878;
    else if (abs(inputMass - 500) <= 5) return 0.0855847;
    else if (abs(inputMass - 510) <= 5) return 0.0751004;
    else if (abs(inputMass - 520) <= 5) return 0.0660189;
    else if (abs(inputMass - 530) <= 5) return 0.0580348;
    else if (abs(inputMass - 540) <= 5) return 0.0511747;
    else if (abs(inputMass - 550) <= 5) return 0.0452067;
    else if (abs(inputMass - 560) <= 5) return 0.0399591;
    else if (abs(inputMass - 570) <= 5) return 0.0354242;
    else if (abs(inputMass - 580) <= 5) return 0.0313654;
    else if (abs(inputMass - 590) <= 5) return 0.0279395;
    else if (abs(inputMass - 600) <= 5) return 0.0248009;
    else if (abs(inputMass - 610) <= 5) return 0.0220672;
    else if (abs(inputMass - 620) <= 5) return 0.0196331;
    else if (abs(inputMass - 630) <= 5) return 0.0175075;
    else if (abs(inputMass - 640) <= 5) return 0.0155809;
    else if (abs(inputMass - 650) <= 5) return 0.0139566;
    else if (abs(inputMass - 660) <= 5) return 0.0125393;
    else if (abs(inputMass - 670) <= 5) return 0.0112223;
    else if (abs(inputMass - 680) <= 5) return 0.0100516;
    else if (abs(inputMass - 690) <= 5) return 0.0090306;
    else if (abs(inputMass - 700) <= 5) return 0.0081141;
    else if (abs(inputMass - 710) <= 5) return 0.00730084;
    else if (abs(inputMass - 720) <= 5) return 0.00656729;
    else if (abs(inputMass - 730) <= 5) return 0.00591771;
    else if (abs(inputMass - 740) <= 5) return 0.00532605;
    else if (abs(inputMass - 750) <= 5) return 0.00480639;
    else if (abs(inputMass - 760) <= 5) return 0.00433688;
    else if (abs(inputMass - 770) <= 5) return 0.00391839;
    else if (abs(inputMass - 780) <= 5) return 0.00354211;
    else if (abs(inputMass - 790) <= 5) return 0.00320476;
    else if (abs(inputMass - 800) <= 5) return 0.00289588;
    else if (abs(inputMass - 810) <= 5) return 0.0026184;
    else if (abs(inputMass - 820) <= 5) return 0.00237168;
    else if (abs(inputMass - 830) <= 5) return 0.00214607;
    else if (abs(inputMass - 840) <= 5) return 0.00195172;
    else if (abs(inputMass - 850) <= 5) return 0.00176742;
    else if (abs(inputMass - 860) <= 5) return 0.00160403;
    else if (abs(inputMass - 870) <= 5) return 0.00145772;
    else if (abs(inputMass - 880) <= 5) return 0.00132077;
    else if (abs(inputMass - 890) <= 5) return 0.00120568;
    else if (abs(inputMass - 900) <= 5) return 0.00109501;
    else if (abs(inputMass - 910) <= 5) return 0.000996193;
    else if (abs(inputMass - 920) <= 5) return 0.000907494;
    else if (abs(inputMass - 930) <= 5) return 0.000826533;
    else if (abs(inputMass - 940) <= 5) return 0.000753768;
    else if (abs(inputMass - 950) <= 5) return 0.000687022;
    else if (abs(inputMass - 960) <= 5) return 0.000626876;
    else if (abs(inputMass - 970) <= 5) return 0.000571551;
    else if (abs(inputMass - 980) <= 5) return 0.000522495;
    else if (abs(inputMass - 990) <= 5) return 0.000476255;
    else if (abs(inputMass - 1000) <= 5) return 0.000435488;
    else return 0.0;
}


#endif


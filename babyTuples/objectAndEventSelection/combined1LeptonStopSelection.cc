#include "combined1LeptonStopSelection.h"
using namespace std;

#define DEBUG_MSG std::cout << "DEBUG (" << __FILE__ << ", l." << __LINE__ << ") "

// ----------------------------------------------------------------------------
// Default constructor
// ----------------------------------------------------------------------------

combined1LeptonStopSelection::combined1LeptonStopSelection ()
{
}

void combined1LeptonStopSelection::setBTagReshapingInput(string fileName)       { bTagReshapingInput      = fileName; }          
void combined1LeptonStopSelection::setMCJetCorrectionsInput(string fileName)    { jetCorrectionsMCInput   = fileName; }
void combined1LeptonStopSelection::setDataJetCorrectionsInput(string fileName)  { jetCorrectionsDataInput = fileName; }
void combined1LeptonStopSelection::setPileUpReweightingInput(string fileName)   { pileUpReweightingInput  = fileName; }

void combined1LeptonStopSelection::loadCorrections()
{
    // Load bTagReshaping input

    BTagReshape_nominal   = new BTagShapeInterface(bTagReshapingInput, 0.0, 0.0);
    BTagReshape_upBC      = new BTagShapeInterface(bTagReshapingInput, 1.0, 0.0);
    BTagReshape_downBC    = new BTagShapeInterface(bTagReshapingInput,-1.0, 0.0);
    BTagReshape_upLight   = new BTagShapeInterface(bTagReshapingInput, 0.0, 1.0);
    BTagReshape_downLight = new BTagShapeInterface(bTagReshapingInput, 0.0,-1.0);

    // Load JEC/JES stuff

    JESUncertainty_MC     = new JetCorrectionUncertainty(jetCorrectionsMCInput);
    JESUncertainty_Data   = new JetCorrectionUncertainty(jetCorrectionsDataInput);

    // Load pile-up stuff

    TFile* pileUpReweightingFile = TFile::Open(pileUpReweightingInput.c_str());
    pileUpReweightingHisto = (TH1F*) pileUpReweightingFile->Get("puWeights");
    pileUpReweightingHisto->SetName("h_pu_wgt");
}


void combined1LeptonStopSelection::DoObjectSelection(bool runningOnData, short int JESvariation)
{

    // Clear selected object collections

    goodElectrons.clear();
    goodMuons.clear();
    goodJets.clear();
    selectedJets.clear();
    selectedBJets.clear();
    selectedElectrons.clear();
    selectedMuons.clear();

    // Raw / good objects

    rawJets           = *GetPointer2Jets();
    rawMET            = GetSelectedMET(false,1.0,false,1.0);
    goodMuons         = Get1LeptonStopGoodMuons();
    goodElectrons     = Get1LeptonStopGoodElectrons();
    goodJets          = Get1LeptonStopGoodJets(runningOnData);


    // MET
    NTMET tmpMET      = Get1LeptonStopType1PhiMET(runningOnData);
    theMET            = TLorentzVector(tmpMET.p2.Px(),tmpMET.p2.Py(),0.,tmpMET.p2.Mod());

    // JES variations if asked
    if (JESvariation ==  1) ApplyJESVariation(runningOnData,true);
    if (JESvariation == -1) ApplyJESVariation(runningOnData,false);

    // Selected objects

    selectedMuons     = Get1LeptonStopSelectedMuons();
    selectedElectrons = Get1LeptonStopSelectedElectrons();
    selectedJets      = Get1LeptonStopSelectedJets (runningOnData);
    selectedBJets     = Get1LeptonStopSelectedBJets(runningOnData);

}

bool combined1LeptonStopSelection::PassEventSelection(bool runningOnData)
{

    // #####################
    // #  Object selection #
    // #####################

    DoObjectSelection(runningOnData);

    // ##########################
    // #  Step 0        Filters #
    // ##########################

    if ((getRho() < 0.0) || (getRho() > 40.0)) return false;

    // ##########################
    // #  Step 1        Trigger #
    // ##########################

    bool trigger_singleElec = PassTrigger("singleElec");
    bool trigger_singleMuon = PassTrigger("singleMuon");
    bool trigger_crossMuon  = PassTrigger("crossMuon");
    bool trigger_doubleElec = PassTrigger("doubleElec");
    bool trigger_doubleMuon = PassTrigger("doubleMuon");
    bool trigger_elecMuon   = PassTrigger("muonElec");

    if (
            ((runningOnData) && (trigger_singleElec || trigger_singleMuon || trigger_crossMuon 
                              || trigger_doubleElec || trigger_doubleMuon || trigger_elecMuon))
            || (!runningOnData)  
       )
    {

        // ####################
        // #  Step 2   Lepton #
        // ####################

        if ((selectedElectrons.size() >= 1 || selectedMuons.size() >= 1))
        //if ((selectedElectrons.size() + selectedMuons.size() >= 2))
        {
            numberOfSelectedLeptons = selectedElectrons.size() + selectedMuons.size();

            FillLeadingLeptonsInfo();

            // ##########################
            // #  Step 3       3 jets   #
            // ##########################

            if (selectedJets.size() >= 3) 
            //if (selectedJets.size() >= 1) 
            {
                return true;
            }
        } 
    } 

    return false;
}

// #############################################################################################
// #    _____          _       _           _   _                  _                _           #
// #   |_   _|        | |     | |         | | | |                | |              | |          #
// #     | | ___  ___ | | __ _| |_ ___  __| | | |_ _ __ __ _  ___| | __ __   _____| |_ ___     #
// #     | |/ __|/ _ \| |/ _` | __/ _ \/ _` | | __| '__/ _` |/ __| |/ / \ \ / / _ \ __/ _ \    #
// #    _| |\__ \ (_) | | (_| | ||  __/ (_| | | |_| | | (_| | (__|   <   \ V /  __/ || (_) |   #
// #    \___/___/\___/|_|\__,_|\__\___|\__,_|  \__|_|  \__,_|\___|_|\_\   \_/ \___|\__\___/    #
// #                                                                                           #
// #############################################################################################

bool combined1LeptonStopSelection::Get1LeptonStopIsolatedTrackVeto(TLorentzVector lepton_p, float lepton_charge) const
{
    // Input vector
    std::vector<IPHCTree::NTPFCandidate> vetotracks = GetPFCandidates();

    // Loop over pfcandidates
    for(unsigned int i=0 ; i < vetotracks.size() ; i++)
    {
        // Check pfcandidate doesnt match the selected lepton
        TLorentzVector vetoTrack_p = vetotracks[i].p4;

        if (lepton_p.DeltaR(vetoTrack_p) < 0.1) continue;
        
        bool passCuts = false;

        float pfCandId = vetotracks[i].others["id"];

        if (abs(pfCandId) == 11)
        {
            if ((vetotracks[i].others["gsfPt"] > 5)
                    && (fabs(vetotracks[i].others["gsfdz"]) < 0.05)
                    && (vetotracks[i].trackIso / vetotracks[i].others["gsfPt"] < 0.2))
                passCuts = true;
        }
        else if (abs(pfCandId) == 13)
        {
            if ((vetotracks[i].p4.Pt() > 5)
                    && (fabs(vetotracks[i].dz_firstGoodVertex) < 0.05)
                    && (vetotracks[i].trackIso / vetotracks[i].p4.Pt() < 0.2))    
                passCuts = true;
        }
        else
        {
            if ((vetotracks[i].p4.Pt() > 10)
                    && (fabs(vetotracks[i].dz_firstGoodVertex) < 0.05)
                    && (vetotracks[i].trackIso / vetotracks[i].p4.Pt() < 0.1)
                    && (lepton_charge != vetotracks[i].others["charge_fromID"]))
                passCuts = true;
        }

        if (passCuts == false) continue;

        return false;
    }

    return true;
}

// ############################################
// #   _____                       _          #
// #  |_   _|                     | |         #
// #    | | __ _ _   _  __   _____| |_ ___    #
// #    | |/ _` | | | | \ \ / / _ \ __/ _ \   #
// #    | | (_| | |_| |  \ V /  __/ || (_) |  #
// #    \_/\__,_|\__,_|   \_/ \___|\__\___/   #
// #                                          #
// ############################################

bool combined1LeptonStopSelection::Get1LeptonStopTauVeto(TLorentzVector lepton_p, float lepton_charge) const
{
    std::vector<IPHCTree::NTTau> localTaus = (*GetPointer2Taus());
    for (unsigned int i = 0 ; i < localTaus.size() ; i++)
    {
        // Reject tau candidates with pT < 20 GeV
        if (localTaus[i].p4.Pt() < 20) continue;
        // Reject tau candidates with same charge than selected lepton
        if (localTaus[i].charge == lepton_charge) continue;
        // Reject tau candidates not satisfying IDs
        if (localTaus[i].ID["decayModeFinding"] != 1.0) continue;
        // Reject tau candidates too close from selected lepton
        if (localTaus[i].p4.DeltaR(lepton_p) < 0.4) continue;
        // Apply ID
        if (localTaus[i].ID["byMediumIsolationMVA2"] != 1.0) continue;

        return false; 
    }

    return true;
}

void combined1LeptonStopSelection::FillLeadingLeptonsInfo()
{
    // Reset

    theLeadingLepton *= 0;
    theSecondLepton  *= 0;

    theLeadingLepton_pdgid = -9999;
    theSecondLepton_pdgid  = -9999;
    
    theLeadingLepton_isolation = -1;
    theSecondLepton_isolation  = -1;

    // Fill the values
    
        // First, check muons
    for (unsigned int i = 0 ; i < selectedMuons.size() ; i++)
    {
        if (theLeadingLepton.Pt()  < selectedMuons[i].p4.Pt()) 
        {
            theSecondLepton            = theLeadingLepton;
            theSecondLepton_pdgid      = theLeadingLepton_pdgid;
            theSecondLepton_isolation  = theLeadingLepton_isolation;

            theLeadingLepton           = selectedMuons[i].p4;
            theLeadingLepton_pdgid     = selectedMuons[i].charge * (-13);
            theLeadingLepton_isolation = Get1LeptonStopIsolation(selectedMuons[i]);
        }
        else if (theSecondLepton.Pt()  < selectedMuons[i].p4.Pt())
        {
            theSecondLepton           = selectedMuons[i].p4;
            theSecondLepton_pdgid     = selectedMuons[i].charge * (-13);
            theSecondLepton_isolation = Get1LeptonStopIsolation(selectedMuons[i]);
        }
    }

        // Second, check electrons
    for (unsigned int i = 0 ; i < selectedElectrons.size() ; i++)
    {
        if (theLeadingLepton.Pt()  < selectedElectrons[i].p4.Pt()) 
        {
            theSecondLepton            = theLeadingLepton;
            theSecondLepton_pdgid      = theLeadingLepton_pdgid;
            theSecondLepton_isolation  = theLeadingLepton_isolation;
            
            theLeadingLepton           = selectedElectrons[i].p4;
            theLeadingLepton_pdgid     = selectedElectrons[i].charge * (-11);
            theLeadingLepton_isolation = Get1LeptonStopIsolation(selectedElectrons[i]);
        }
        else if (theSecondLepton.Pt() < selectedElectrons[i].p4.Pt())
        {
            theSecondLepton           = selectedElectrons[i].p4;
            theSecondLepton_pdgid     = selectedElectrons[i].charge * (-11);
            theSecondLepton_isolation = Get1LeptonStopIsolation(selectedElectrons[i]);
        }
    }

}

// ############################################
// #   _____    _                             #
// #  |_   _|  (_)                            #
// #    | |_ __ _  __ _  __ _  ___ _ __ ___   #
// #    | | '__| |/ _` |/ _` |/ _ \ '__/ __|  #
// #    | | |  | | (_| | (_| |  __/ |  \__ \  #
// #    \_/_|  |_|\__, |\__, |\___|_|  |___/  #
// #               __/ | __/ |                #
// #              |___/ |___/                 #
// #                                          #
// ############################################

bool combined1LeptonStopSelection::PassTrigger(string channel)
{
    vector<string> path;

    if (channel == "singleMuon") 
    { 
        path.push_back("HLT_IsoMu24_v*"); 
        path.push_back("HLT_IsoMu24_eta2p1_v*"); 
    }
    if (channel == "singleElec") 
    { 
        path.push_back("HLT_Ele27_WP80_v*"); 
    }
    if (channel == "crossMuon" ) 
    { 
        path.push_back("HLT_IsoMu17_eta2p1_TriCentralPFJet30_v*"); 
        path.push_back("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_30_20_v*");
        path.push_back("HLT_IsoMu17_eta2p1_TriCentralPFNoPUJet30_v*"); 
    }
    if (channel == "doubleMuon") 
    { 
        path.push_back("HLT_Mu17_Mu8_v*"); 
    }
    if (channel == "doubleElec")
    { 
        path.push_back("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*"); 
    }
    if (channel == "muonElec"  ) 
    { 
        path.push_back("HLT_Mu17_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v*"); 
        path.push_back("HLT_Mu8_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL*"); 
    }

    for (unsigned int i = 0 ; i < path.size() ; i++)
    {
        if (CheckPathHasBeenFired(path[i])) return true;
    }

    return false;

}

bool combined1LeptonStopSelection::CheckPathHasBeenFired(string path)
{
    std::vector<IPHCTree::NTTriggerPathType> myPaths;
    GetPointer2Trigger()->GetSubTable(path.c_str(),myPaths);
    for (unsigned int i=0;i<myPaths.size();i++) 
    {
        if (myPaths[i].fired==1) 
        {
            if (myPaths[i].prescale>1) cout << " warning TRIGGER " << myPaths[i].name 
                << " is PRESCALED with a factor " << myPaths[i].prescale 
                    << endl;

            return true;
        }
    }

    return false;

}


// #################################################################
// #   _____                 _                                     #
// #  |  __ \               | |                                    #
// #  | |  \/ ___   ___   __| |  _ __ ___  _   _  ___  _ __  ___   #
// #  | | __ / _ \ / _ \ / _` | | '_ ` _ \| | | |/ _ \| '_ \/ __|  #
// #  | |_\ \ (_) | (_) | (_| | | | | | | | |_| | (_) | | | \__ \  #
// #   \____/\___/ \___/ \__,_| |_| |_| |_|\__,_|\___/|_| |_|___/  #
// #                                                               #
// #################################################################


float combined1LeptonStopSelection::Get1LeptonStopIsolation(IPHCTree::NTMuon muon) const
{
    float Charged = muon.isolation["PF03Char"];
    float Neutral = muon.isolation["PF03Neut"];
    float Photon  = muon.isolation["PF03Phot"];
    float PU      = muon.isolation["PF03PU"];
    float absIso  =  ( Charged 
                       + max( (float) 0.0,
                              (float) (Neutral + Photon- 0.5*PU ) ) );
    
    return absIso;
}

std::vector<IPHCTree::NTMuon> combined1LeptonStopSelection::Get1LeptonStopGoodMuons() const
{

    // Container for output 
    std::vector<IPHCTree::NTMuon> outputVector;

    // Get Muons
    std::vector<IPHCTree::NTMuon> localMuons;
    localMuons = *GetPointer2Muons();

    // Loop over muons
    for(unsigned int i=0;i<localMuons.size();i++)
    {

        if (!localMuons[i].isPFMuon)               continue;
        if (!localMuons[i].isGlobalMuon)           continue;
        if (localMuons[i].p4.Pt()        < 10)     continue;
        if (fabs(localMuons[i].p4.Eta()) > 2.4)    continue;

        float relIso = Get1LeptonStopIsolation(localMuons[i]) 
                     / localMuons[i].p4.Pt(); 
        if (relIso >= 0.15)                        continue;

        if (localMuons[i].Chi2 > 10)               continue;

        if (localMuons[i].NValidHits <= 0)         continue;
        if (localMuons[i].numMatchedStations <= 1) continue;   
        if (localMuons[i].pixelHits <= 0)          continue;

        if (localMuons[i].numTrackerLayersWithMeasurement <= 5) continue;
        if (localMuons[i].dxy_vertex >= 0.02)      continue;
        if (localMuons[i].dz_vertex >= 0.5)        continue;

        outputVector.push_back(localMuons[i]);
    }
    std::sort(outputVector.begin(),outputVector.end(),HighestPt());
    return outputVector;
}

// ##############################################################################
// #   _____      _           _           _                                     #
// #  /  ___|    | |         | |         | |                                    #
// #  \ `--.  ___| | ___  ___| |_ ___  __| |  _ __ ___  _   _  ___  _ __  ___   #
// #   `--. \/ _ \ |/ _ \/ __| __/ _ \/ _` | | '_ ` _ \| | | |/ _ \| '_ \/ __|  #
// #  /\__/ /  __/ |  __/ (__| ||  __/ (_| | | | | | | | |_| | (_) | | | \__ \  #
// #  \____/ \___|_|\___|\___|\__\___|\__,_| |_| |_| |_|\__,_|\___/|_| |_|___/  #
// #                                                                            # 
// ##############################################################################

std::vector<IPHCTree::NTMuon> combined1LeptonStopSelection::Get1LeptonStopSelectedMuons() const
{

    // Container for output
    std::vector<IPHCTree::NTMuon> outputVector;

    // Get muons
    std::vector<IPHCTree::NTMuon> muons = goodMuons;

    // Loop over muons
    for(unsigned int i=0;i<muons.size();i++)
    {

        // Pt and Eta
        if (muons[i].p4.Pt()  < 20) continue;
        if (fabs(muons[i].p4.Eta()) > 2.1) continue;

        // Reco - PF matching
        if (fabs(muons[i].bestMatch_pT - muons[i].p4.Pt()) >= 10) continue;

        // Absolute isolation
        float absIso = Get1LeptonStopIsolation(muons[i]);
        if (absIso > 5) continue;

        outputVector.push_back(muons[i]);
    }

    std::sort(outputVector.begin(),outputVector.end(),HighestPt());
    return outputVector;
}

// ##########################################################################
// #   _____                 _        _           _                         #
// #  |  __ \               | |      | |         | |                        #
// #  | |  \/ ___   ___   __| |   ___| | ___  ___| |_ _ __ ___  _ __  ___   #
// #  | | __ / _ \ / _ \ / _` |  / _ \ |/ _ \/ __| __| '__/ _ \| '_ \/ __|  #
// #  | |_\ \ (_) | (_) | (_| | |  __/ |  __/ (__| |_| | | (_) | | | \__ \  #
// #   \____/\___/ \___/ \__,_|  \___|_|\___|\___|\__|_|  \___/|_| |_|___/  #
// #                                                                        #
// ##########################################################################

float combined1LeptonStopSelection::Get1LeptonStopIsolation(IPHCTree::NTElectron electron) const
{
    float chargedIso  = electron.isolation["RA4Charg"];
    float photonIso   = electron.isolation["RA4Photo"];
    float neutralIso  = electron.isolation["RA4Neutr"];
    float rho         = electron.isolation["rho"];
    float Aeff        = electron.isolation["Aeff"];
    float absIso = (chargedIso + max((float) 0.0, (float) (photonIso + neutralIso - rho * Aeff)));

    return absIso;
}

std::vector<IPHCTree::NTElectron> combined1LeptonStopSelection::Get1LeptonStopGoodElectrons() const
{

    // Output vector
    std::vector<IPHCTree::NTElectron> outputVector;

    // Get new electrons
    std::vector<IPHCTree::NTElectron> localElectrons;
    localElectrons = *GetPointer2Electrons();

    // Loop over electrons
    for(unsigned int i=0;i<localElectrons.size();i++)
    {
        // Check muon overlap
        bool foundMuonOverlap = false;
        for (unsigned int j = 0 ; j < goodMuons.size() ; j++)
        {
            if (localElectrons[i].p4.DeltaR(goodMuons[j].p4) < 0.1) foundMuonOverlap = true;
        }
        if (foundMuonOverlap) continue;

        // p_T
        if (localElectrons[i].p4.Pt()        < 10)       continue;

        // eta cuts
        if (fabs(localElectrons[i].p4.Eta()) >= 2.4)     continue;
        if (  (fabs(localElectrons[i].etaSuperCluster) >= 1.4442) 
                && (fabs(localElectrons[i].etaSuperCluster) <= 1.566)) continue;

        // abs(deltaEta) and abs(deltaPhi)
        if ((localElectrons[i].isEB) && (fabs(localElectrons[i].deltaEtaSuperClusterTrackAtVtx) >= 0.004))    continue;
        if ((localElectrons[i].isEE) && (fabs(localElectrons[i].deltaEtaSuperClusterTrackAtVtx) >= 0.007))    continue;

        if ((localElectrons[i].isEB) && (fabs(localElectrons[i].deltaPhiSuperClusterTrackAtVtx) >= 0.06))     continue;
        if ((localElectrons[i].isEE) && (fabs(localElectrons[i].deltaPhiSuperClusterTrackAtVtx) >= 0.03))     continue;

        // sigmaIetaIeta
        if ((localElectrons[i].isEB) && (localElectrons[i].sigmaIetaIeta >= 0.01))    continue;
        if ((localElectrons[i].isEE) && (localElectrons[i].sigmaIetaIeta >= 0.03))    continue;

        // hadOverEM
        if ((localElectrons[i].isEB) && (localElectrons[i].hadronicOverEm >= 0.12))    continue;
        if ((localElectrons[i].isEE) && (localElectrons[i].hadronicOverEm >= 0.10))    continue;

        // dxy, dz
        if (localElectrons[i].dxy_vertex >= 0.02)           continue;
        if (localElectrons[i].dz_vertex  >= 0.1)            continue;

        // fabs(1/E-1/pin)
        float overE_m_overPin = fabs( (1.0 - localElectrons[i].eSuperClusterOverP) / localElectrons[i].EmEnergy_  );
        if (overE_m_overPin >= 0.05) continue;

        // Rel Iso
        float relIso = Get1LeptonStopIsolation(localElectrons[i]) 
                     / localElectrons[i].p4.Pt();

        if (localElectrons[i].isEB)
            if (relIso > 0.15) continue;

        if ((localElectrons[i].isEE) && (localElectrons[i].p4.Pt() >= 20))
            if (relIso > 0.15) continue;

        if ((localElectrons[i].isEE) && (localElectrons[i].p4.Pt() < 20))
            if (relIso > 0.10) continue;

        // Conversion rejection
        if (localElectrons[i].passConversionVeto == false) continue;
        if (localElectrons[i].missingHits > 1)            continue;

        // Add to selected electrons
        outputVector.push_back(localElectrons[i]);
    }

    // Return output vector after sorting
    std::sort(outputVector.begin(),outputVector.end(),HighestPt());
    return outputVector;
}

// #######################################################################################
// #   _____      _           _           _        _           _                         #
// #  /  ___|    | |         | |         | |      | |         | |                        #
// #  \ `--.  ___| | ___  ___| |_ ___  __| |   ___| | ___  ___| |_ _ __ ___  _ __  ___   #
// #   `--. \/ _ \ |/ _ \/ __| __/ _ \/ _` |  / _ \ |/ _ \/ __| __| '__/ _ \| '_ \/ __|  #
// #  /\__/ /  __/ |  __/ (__| ||  __/ (_| | |  __/ |  __/ (__| |_| | | (_) | | | \__ \  #
// #  \____/ \___|_|\___|\___|\__\___|\__,_|  \___|_|\___|\___|\__|_|  \___/|_| |_|___/  #
// #                                                                                     #
// #######################################################################################
std::vector<IPHCTree::NTElectron> combined1LeptonStopSelection::Get1LeptonStopSelectedElectrons() const
{
    std::vector<IPHCTree::NTElectron> outputVector;

    std::vector<IPHCTree::NTElectron> electrons = goodElectrons;

    for(unsigned int i=0;i<electrons.size();i++)
    {

        // Pt and Eta
        if (electrons[i].p4.Pt() < 20) continue;
        if (fabs(electrons[i].etaSuperCluster) >= 1.4442)     continue;

        // Absolute isolation
        float absIso = Get1LeptonStopIsolation(electrons[i]);
        if (absIso > 5) continue;

        // E/Pin
        if (electrons[i].eSuperClusterOverP > 4) continue;

        // PF - Reco matching
        if (fabs(electrons[i].bestMatch_pT - electrons[i].p4.Pt()) > 10) continue;

        outputVector.push_back(electrons[i]);
    }

    std::sort(outputVector.begin(),outputVector.end(),HighestPt());
    return outputVector;
}

// #########################################################################
// #     ___ _____ _____                   _       _   _                   #
// #    |_  |  ___/  ___|                 (_)     | | (_)                  #
// #      | | |__ \ `--.  __   ____ _ _ __ _  __ _| |_ _  ___  _ __  ___   #
// #      | |  __| `--. \ \ \ / / _` | '__| |/ _` | __| |/ _ \| '_ \/ __|  #
// #  /\__/ / |___/\__/ /  \ V / (_| | |  | | (_| | |_| | (_) | | | \__ \  #
// #  \____/\____/\____/    \_/ \__,_|_|  |_|\__,_|\__|_|\___/|_| |_|___/  #
// #                                                                       #
// #########################################################################

// upOrDown : - true  = up
//            - false = down
void combined1LeptonStopSelection::ApplyJESVariation(bool runningOnData, bool upOrDown)
{
    short int sign = upOrDown ? 1 : -1;

    JetCorrectionUncertainty* JESUncertainty;
    if (runningOnData)  JESUncertainty = JESUncertainty_Data;
    else                JESUncertainty = JESUncertainty_MC;

    // Get the rawMET x and y
    float rawMET_x = theMET.Px();
    float rawMET_y = theMET.Py();

    // deltaMETfromJets : correspond to the variation to be propagated to the MET
    float deltaMETfromJets_x = 0.0;
    float deltaMETfromJets_y = 0.0;

    // sumJets : used to compute the unclustered energy
    float sumJets_x = 0.0;
    float sumJets_y = 0.0;

    for (unsigned int i = 0 ; i < goodJets.size() ; i++)
    {
        JESUncertainty->setJetPt(goodJets[i].p4.Pt());
        JESUncertainty->setJetEta(goodJets[i].p4.Eta());
        float scale = JESUncertainty->getUncertainty(upOrDown);
        
        deltaMETfromJets_x += scale * goodJets[i].p4.Px();
        deltaMETfromJets_y += scale * goodJets[i].p4.Py();

        sumJets_x   += goodJets[i].p4.Px();
        sumJets_y   += goodJets[i].p4.Py();

        // Correct the jets from the JES variation
        goodJets[i].p4 *= (1 + sign * scale);
    }

    // sumLeptons : used to computed unclustered energy
    float sumLeptons_x = 0.0;
    float sumLeptons_y = 0.0;

    for (unsigned int i = 0 ; i < goodMuons.size()     ; i++) { sumLeptons_x += goodMuons[i].p4.Px();     sumLeptons_y += goodMuons[i].p4.Py();     }
    for (unsigned int i = 0 ; i < goodElectrons.size() ; i++) { sumLeptons_x += goodElectrons[i].p4.Px(); sumLeptons_y += goodElectrons[i].p4.Py(); }

    // Compute unclustered energy
    float unclusteredEnergy_x = -1.0 * (rawMET_x + sumJets_x + sumLeptons_x);
    float unclusteredEnergy_y = -1.0 * (rawMET_y + sumJets_y + sumLeptons_y);

    // Compute MET variation from JES, assume 10% uncertainty for unclustered energy
    float rawMETvariation_x = sign * (deltaMETfromJets_x + 0.1 * unclusteredEnergy_x);
    float rawMETvariation_y = sign * (deltaMETfromJets_y + 0.1 * unclusteredEnergy_y);

   /* 
    DEBUG_MSG << " rawMET x : " << rawMET_x  << endl;
    DEBUG_MSG << " rawMET y : " << rawMET_y  << endl;
    DEBUG_MSG << " deltaFromJet x : " << deltaMETfromJets_x << " ; unclustered x : " << unclusteredEnergy_x << endl;
    DEBUG_MSG << " deltaFromJet y : " << deltaMETfromJets_y << " ; unclustered y : " << unclusteredEnergy_y << endl;
    DEBUG_MSG << " variated MET x : " << rawMET_x - rawMETvariation_x  << endl;
    DEBUG_MSG << " variated MET y : " << rawMET_x - rawMETvariation_x  << endl;
   */

    // Replace rawMET with the variated MET
    TVector2 tmpMET(rawMET_x - rawMETvariation_x, \
                    rawMET_y - rawMETvariation_y);

    theMET.SetPxPyPzE(tmpMET.Px(),tmpMET.Py(),0.0,tmpMET.Mod());
}

// ##################################################
// #   _____                 _     _      _         #
// #  |  __ \               | |   (_)    | |        #
// #  | |  \/ ___   ___   __| |    _  ___| |_ ___   #
// #  | | __ / _ \ / _ \ / _` |   | |/ _ \ __/ __|  #
// #  | |_\ \ (_) | (_) | (_| |   | |  __/ |_\__ \  #
// #   \____/\___/ \___/ \__,_|   | |\___|\__|___/  #
// #                             _/ |               #
// #                            |__/                #
// ##################################################

std::vector<IPHCTree::NTJet> combined1LeptonStopSelection::Get1LeptonStopGoodJets(
        int DataType) const
{
    // Container for output
    std::vector<IPHCTree::NTJet> outputVector;

    // # #           Get the jets                #
    for(unsigned int i=0;i<rawJets.size();i++)
    {

        if (rawJets[i].ID["LOOSE"] != 1.) continue; 

        if ((fabs(rawJets[i].p4.Eta()) >= 4.7) || (rawJets[i].p4.Pt() < 10)) continue; 

        double deltaRmu = 10000;
        double deltaRel = 10000;

        for(unsigned int imu=0; imu< goodMuons.size(); imu++)
        {
            double deltaR = rawJets[i].p4.DeltaR(goodMuons[imu].p4);
            if(deltaR < deltaRmu) deltaRmu = deltaR;
        }

        for(unsigned int iel=0; iel< goodElectrons.size(); iel++)
        {
            double deltaR = rawJets[i].p4.DeltaR(goodElectrons[iel].p4);
            if(deltaR < deltaRel) deltaRel = deltaR;
        }

        if( deltaRmu > 0.4  && deltaRel > 0.4)
            outputVector.push_back(rawJets[i]);
    }

    // Add reshaped CSV info
    for (unsigned int i = 0 ; i < outputVector.size() ; i++)
    {
        float nominal   = BTagReshape_nominal  ->reshape(outputVector[i].p4.Eta(), 
                                                         outputVector[i].p4.Pt(), 
                                                         outputVector[i].bTag["combinedSecondaryVertexBJetTags"], 
                                                         outputVector[i].partonFlavour);
        float upBC      = BTagReshape_upBC     ->reshape(outputVector[i].p4.Eta(), 
                                                         outputVector[i].p4.Pt(), 
                                                         outputVector[i].bTag["combinedSecondaryVertexBJetTags"], 
                                                         outputVector[i].partonFlavour);
        float downBC    = BTagReshape_downBC   ->reshape(outputVector[i].p4.Eta(), 
                                                         outputVector[i].p4.Pt(), 
                                                         outputVector[i].bTag["combinedSecondaryVertexBJetTags"], 
                                                         outputVector[i].partonFlavour);
        float upLight   = BTagReshape_upLight  ->reshape(outputVector[i].p4.Eta(), 
                                                         outputVector[i].p4.Pt(), 
                                                         outputVector[i].bTag["combinedSecondaryVertexBJetTags"], 
                                                         outputVector[i].partonFlavour);
        float downLight = BTagReshape_downLight->reshape(outputVector[i].p4.Eta(), 
                                                         outputVector[i].p4.Pt(), 
                                                         outputVector[i].bTag["combinedSecondaryVertexBJetTags"], 
                                                         outputVector[i].partonFlavour);


        outputVector[i].bTag.Add("zz1combinedSecondaryVertexBJetTagsReshapeNominal",  nominal  );
        outputVector[i].bTag.Add("zz2combinedSecondaryVertexBJetTagsReshapeUpBC",    upBC     );
        outputVector[i].bTag.Add("zz3combinedSecondaryVertexBJetTagsReshapeDownBC",   downBC   );
        outputVector[i].bTag.Add("zz4combinedSecondaryVertexBJetTagsReshapeUpLight",  upLight  );
        outputVector[i].bTag.Add("zz5combinedSecondaryVertexBJetTagsReshapeDownLight",downLight);
    }

    std::sort(outputVector.begin(),outputVector.end(),HighestPt());
    return outputVector;

}

// ###############################################################
// #   _____      _           _           _     _      _         #
// #  /  ___|    | |         | |         | |   (_)    | |        #
// #  \ `--.  ___| | ___  ___| |_ ___  __| |    _  ___| |_ ___   #
// #   `--. \/ _ \ |/ _ \/ __| __/ _ \/ _` |   | |/ _ \ __/ __|  #
// #  /\__/ /  __/ |  __/ (__| ||  __/ (_| |   | |  __/ |_\__ \  #
// #  \____/ \___|_|\___|\___|\__\___|\__,_|   | |\___|\__|___/  #
// #                                          _/ |               #
// #                                         |__/                #
// ###############################################################

std::vector<IPHCTree::NTJet> combined1LeptonStopSelection::Get1LeptonStopSelectedJets(
        int DataType) const
{
    // Container for output
    std::vector<IPHCTree::NTJet> outputVector;

    for(unsigned int i=0;i<goodJets.size();i++)
    {

        // Tight ID
        if (goodJets[i].ID["PU_IDTight5x"] != 1.) continue;
        
        // Eta and Pt cuts
        if ((fabs(goodJets[i].p4.Eta()) >= 2.4) || (goodJets[i].p4.Pt()  < 30)) continue;

        outputVector.push_back(goodJets[i]);
    }


    std::sort(outputVector.begin(),outputVector.end(),HighestPt());
    return outputVector;
}
// #######################################################################
// #   _____      _           _           _     _       _      _         #
// #  /  ___|    | |         | |         | |   | |     (_)    | |        #
// #  \ `--.  ___| | ___  ___| |_ ___  __| |   | |__    _  ___| |_ ___   #
// #   `--. \/ _ \ |/ _ \/ __| __/ _ \/ _` |   | '_ \  | |/ _ \ __/ __|  #
// #  /\__/ /  __/ |  __/ (__| ||  __/ (_| |   | |_) | | |  __/ |_\__ \  #
// #  \____/ \___|_|\___|\___|\__\___|\__,_|   |_.__/  | |\___|\__|___/  #
// #                                                  _/ |               #
// #                                                 |__/                #
// #######################################################################
std::vector<IPHCTree::NTJet> combined1LeptonStopSelection::Get1LeptonStopSelectedBJets(
        int DataType) const
{
    std::vector<IPHCTree::NTJet> bJets;

    for (unsigned int j = 0; j < selectedJets.size (); j++)
    {
        // Get discriminant
        float discr;
        if (DataType == 1)
            discr = selectedJets[j].bTag["combinedSecondaryVertexBJetTags"];
        else
            discr = selectedJets[j].bTag["zz1combinedSecondaryVertexBJetTagsReshapeNominal"];

        // Apply CSV medium working point
        if (discr >= 0.679) bJets.push_back(selectedJets[j]);
    }

    return bJets;
}

// ###########################
// #  ___  ___ _____ _____   #
// #  |  \/  ||  ___|_   _|  #
// #  | .  . || |__   | |    #
// #  | |\/| ||  __|  | |    #
// #  | |  | || |___  | |    #
// #  \_|  |_/\____/  \_/    #
// #                         #
// ###########################


IPHCTree::NTMET combined1LeptonStopSelection::Get1LeptonStopType1MET(
        int DataType) const
{


    // # # Actually compute the type1 met from pfMet #

    NTMET rawPfMET = rawMET;

    float metx = rawPfMET.p2.Mod() * cos( rawPfMET.p2.Phi() );
    float mety = rawPfMET.p2.Mod() * sin( rawPfMET.p2.Phi() );

    for (unsigned int i = 0 ; i < goodJets.size() ; i++)
    {

        float l1Corr = goodJets[i].others["corr_L1FastJet"];
        float lastCorr = 999.0;
        if (DataType == 0)
            lastCorr = goodJets[i].others["corr_L3Absolute"];
        else if (DataType == 1)
            lastCorr = goodJets[i].others["corr_L2L3Residual"];

        metx += goodJets[i].p4.Px() * (l1Corr - lastCorr);
        mety += goodJets[i].p4.Py() * (l1Corr - lastCorr);

    }

    rawPfMET.p2.Set(metx,mety);

    return rawPfMET;
}

IPHCTree::NTMET combined1LeptonStopSelection::Get1LeptonStopType1PhiMET(
        int DataType) const
{
    NTMET the_type1met_    = Get1LeptonStopType1MET(DataType);
    NTMET the_type1phimet_ = the_type1met_;

    int Nvtx = GetVertex().size();

    float metx = the_type1met_.p2.Px();
    float mety = the_type1met_.p2.Py();

    float metx_phiCorr = 0.0;
    float mety_phiCorr = 0.0;

    // MC corrections
    if (DataType == 0)
    {
        metx_phiCorr = metx - (+0.1166 + 0.0200*Nvtx);
        mety_phiCorr = mety - (+0.2764 - 0.1280*Nvtx);
    }
    // Data corrections
    else if (DataType == 1)
    {
        metx_phiCorr = metx - (+0.2661 + 0.3217*Nvtx);
        mety_phiCorr = mety - (-0.2251 - 0.1747*Nvtx);
    }

    the_type1phimet_.p2.Set(metx_phiCorr,mety_phiCorr);

    return the_type1phimet_;
}



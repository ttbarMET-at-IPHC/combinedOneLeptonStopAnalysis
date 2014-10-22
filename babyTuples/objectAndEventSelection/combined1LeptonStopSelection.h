#ifndef combined1LeptonStopSelection_h
#define combined1LeptonStopSelection_h

// IPHC headers
#include "Selection/interface/Selection.h"
#include "Selection/interface/SelectionTable.h"
#include "Tools/interface/Dataset.h"

#include "EventReco/interface/StopAnaReco.h"
#include "EventReco/interface/Resolution.h"
#include "EventReco/interface/Mt2Com_bisect.h"
#include "BTagReshaping/interface/BTagReshaping.h"

#include "JetCorrections/interface/JetCorrectionUncertainty.h"

// system include files
#include <memory>
#include <vector>
#include <sstream>

class combined1LeptonStopSelection: public Selection
{

    // -------------------------------------------------------------
    //                       method members
    // -------------------------------------------------------------
    public:

        //! Constructor without argument
        combined1LeptonStopSelection();

        //! Copy constructor
        combined1LeptonStopSelection(const combined1LeptonStopSelection &);

        //! Destructor
        ~combined1LeptonStopSelection()
        { }

        //! LoadEvent
        bool LoadEvent(const NTEvent * event)
        {
            return Event::LoadEvent(event);
        }

        void setBTagReshapingInput(string fileName);
        void setMCJetCorrectionsInput(string fileName);
        void setDataJetCorrectionsInput(string fileName);
        void setPileUpReweightingInput(string fileName);
        void loadCorrections();

        // #####################################
        // #   Accessors to selected objects   #
        // #####################################

        //! Accessor to good electrons and muons
        const std::vector<IPHCTree::NTElectron>& GetGoodElectrons() const { return goodElectrons; }
        const std::vector<IPHCTree::NTMuon>& GetGoodMuons() const { return goodMuons; }

        //! Accessor to good and selected (b-)jets
        const std::vector<IPHCTree::NTJet>& GetGoodJets() const { return goodJets; }
        const std::vector<IPHCTree::NTJet>& GetSelectedJets() const { return selectedJets; }
        const std::vector<IPHCTree::NTJet>& GetSelectedBJets() const { return selectedBJets; }

        // Accessors to selected leptons
        TLorentzVector GetTheLeadingLepton()      { return theLeadingLepton; };
        int            GetTheLeadingLeptonPDGId() { return theLeadingLepton_pdgid; };
        float          GetTheLeadingLeptonIsolation() { return theLeadingLepton_isolation; };
        TLorentzVector GetTheSecondLepton()       { return theSecondLepton; };
        int            GetTheSecondLeptonPDGId()  { return theSecondLepton_pdgid; };
        float          GetTheSecondLeptonIsolation()  { return theSecondLepton_isolation; };

        int getTheNumberOfSelectedLeptons() { return numberOfSelectedLeptons; }

        // #################################
        // #   Selection-related methods   #
        // #################################

        // Perform object and event selection on current event
        void DoObjectSelection(bool runningOnData, short int JESvariation = 0);
        bool PassEventSelection(bool runningOnData);
        // Apply JES +/- 1sigma variation on a given jet collection
        void ApplyJESVariation(bool runningOnData, bool upOrDown);
        // Fill leading lepton infos
        void FillLeadingLeptonsInfo();

        // Triggers
        bool CheckPathHasBeenFired(string path);
        bool PassTrigger(string channel);

        // Compute isolation for leptons
        float Get1LeptonStopIsolation(IPHCTree::NTElectron) const;
        float Get1LeptonStopIsolation(IPHCTree::NTMuon)     const;

        // Perform ood and selected muon selection
        std::vector<IPHCTree::NTMuon> Get1LeptonStopGoodMuons()     const;
        std::vector<IPHCTree::NTMuon> Get1LeptonStopSelectedMuons() const;

        // Perform veto-lepton selections
        bool Get1LeptonStopIsolatedTrackVeto(TLorentzVector lepton_p, float lepton_charge) const;
        bool Get1LeptonStopTauVeto(TLorentzVector lepton_p, float lepton_charge) const;

        // Perform good and selected electrons selection
        std::vector<IPHCTree::NTElectron> Get1LeptonStopGoodElectrons    () const;
        std::vector<IPHCTree::NTElectron> Get1LeptonStopSelectedElectrons() const;

        // Perform good and selected (b-)jets selection
        std::vector<IPHCTree::NTJet> Get1LeptonStopGoodJets(int DataType) const;
        std::vector<IPHCTree::NTJet> Get1LeptonStopSelectedJets(int DataType) const;
        std::vector<IPHCTree::NTJet> Get1LeptonStopSelectedBJets(int DataType) const;

        // Perform MET correction
        IPHCTree::NTMET Get1LeptonStopType1MET(int DataType) const;
        IPHCTree::NTMET Get1LeptonStopType1PhiMET(int DataType) const;

        // #######################################
        // #   High-level variables definition   #
        // #######################################

        float MT_wleptonic() const   { return sqrt( 2.* theLeadingLepton.Pt() * theMET.Pt() *(1. - cos( theLeadingLepton.Phi() - theMET.Phi()) )) ;}
        float Dphi_lmet() const      { return theLeadingLepton.DeltaPhi(theMET);}
        float rawMet() const         { return rawMET.p2.Mod(); }
        float Met() const            { return theMET.Pt(); }
        float MetPhi() const         { return theMET.Phi(); }

        // HT
        float HT() const
        {
            float HT_ = 0;
            for (unsigned int i = 0 ; i < selectedJets.size() ; i++)
                HT_ += selectedJets[i].p4.Pt();

            return HT_;
        }

        // HT ratio
        float HT_ratio() const
        {
            float HT_onTheSideOfMET = 0;
            float HT_total = 0;
            for (unsigned int i = 0 ; i < selectedJets.size() ; i++)
            {
                if (abs(theMET.DeltaPhi(selectedJets[i].p4)) < 3.1415/2.0)
                    HT_onTheSideOfMET += selectedJets[i].p4.Pt();

                HT_total += selectedJets[i].p4.Pt();
            }

            return HT_onTheSideOfMET / HT_total;
        }

        // Hadronic chi2
        float HadronicChi2(bool runningOnData) const
        {
            Resolution Chi2;
            float value = Chi2.GetChi2(GetSelectedJets(),runningOnData);
            return value;
        }

        // MT2W
        float MT2W() const
        {
            Mt2Com_bisect Mt2;
            float value = Mt2.calculateMT2w(GetSelectedJets(),
                    GetSelectedBJets(),
                    theLeadingLepton,
                    theMET.Vect().XYvector(),
                    "MT2w");
            return value;
        }

        // DeltaPhi(MET, two first leading jets)
        float DPhi_MET_leadingJets() const
        {
            TLorentzVector firstLeadingJet;
            TLorentzVector secondLeadingJet;

            for (unsigned int j = 0 ; j < selectedJets.size() ; j++)
            {
                if (selectedJets[j].p4.Pt() > firstLeadingJet.Pt())
                    firstLeadingJet = selectedJets[j].p4;

                else if (selectedJets[j].p4.Pt() > secondLeadingJet.Pt())
                    secondLeadingJet = selectedJets[j].p4;
            }

            if (abs(theMET.DeltaPhi(firstLeadingJet)) < abs(theMET.DeltaPhi(secondLeadingJet)))
                return abs(theMET.DeltaPhi(firstLeadingJet));
            else
                return abs(theMET.DeltaPhi(secondLeadingJet));
        }

        // M3b
        float M3b()
        {
            TLorentzVector sum;
            if(selectedJets.size() == 3)
            {
                sum = selectedJets[0].p4 + selectedJets[1].p4 + selectedJets[2].p4;
            }
            else
            {
                // check which jet is closest to lepton, then take other 3
                double dphimin = 99.;
                int index_closest_jet = -1;
                for(int i=0; i < 4; i++)
                {
                    double dphi = theLeadingLepton.DeltaPhi(selectedJets[i].p4);
                    if (dphi < dphimin)
                    {
                        dphimin = dphi;
                        index_closest_jet = i;
                    }
                }

                for(int i=0; i<4; i++)
                {
                    if(i!=index_closest_jet)
                        sum = sum + selectedJets[i].p4;
                }

            }

            return sum.M();

        }

        TLorentzVector leadingBJet()
        {
            TLorentzVector theLeadingBJet = 0.0;

            for (unsigned int i = 0 ; i < selectedBJets.size() ; i++)
            {
                if (theLeadingBJet.Pt() < selectedBJets[i].p4.Pt())
                    theLeadingBJet      = selectedBJets[i].p4;
            }

            if (theLeadingBJet.Pt() == 0.0) return leadingJet();

            return theLeadingBJet;
        }

        TLorentzVector leadingJet()
        {
            TLorentzVector theLeadingJet = 0.0;

            for (unsigned int i = 0 ; i < selectedJets.size() ; i++)
            {
                if (theLeadingJet.Pt() < selectedJets[i].p4.Pt())
                    theLeadingJet      = selectedJets[i].p4;
            }

            return theLeadingJet;
        }

        TLorentzVector leadingJetByCSV(bool runningOnData)
        {
            TLorentzVector leadingCSVJet = 0.0;
            float maxDiscr = -1.0;

            string discrName;
            if (runningOnData) discrName = "combinedSecondaryVertexBJetTags";
            else               discrName = "zz1combinedSecondaryVertexBJetTagsReshapeNominal";

            for (unsigned int i = 0 ; i < selectedJets.size() ; i++)
            {
                if (maxDiscr < selectedJets[i].bTag[discrName])
                {
                    maxDiscr = selectedJets[i].bTag[discrName];
                    leadingCSVJet = selectedJets[i].p4;
                }
            }

            return leadingCSVJet;
        }

        TLorentzVector bJetClosestToLeadingLepton()
        {
            float minDeltaRLeptonB = 9999.0;
            TLorentzVector closestBJet;

            for (unsigned int i = 0 ; i < selectedBJets.size() ; i++)
            if (minDeltaRLeptonB    > selectedBJets[i].p4.DeltaR(theLeadingLepton))
            {
                closestBJet         = selectedBJets[i].p4;
                minDeltaRLeptonB    = closestBJet.DeltaR(theLeadingLepton);
            }

            if (minDeltaRLeptonB == 9999.0) return leadingJet();
            /*
            for (unsigned int i = 0 ; i < selectedJets.size() ; i++)
            if (minDeltaRLeptonB    > selectedJets[i].p4.DeltaR(theLeadingLepton))
            {
                closestBJet         = selectedJets[i].p4;
                minDeltaRLeptonB    = closestBJet.DeltaR(theLeadingLepton);
            }
            */

            return closestBJet;
        }

        float getPileUpWeight( int nvtx, bool runningOnData )
        {
            // From Verena

            if( runningOnData ) return 1;

            if( nvtx > pileUpReweightingHisto->GetNbinsX() )
                nvtx = pileUpReweightingHisto->GetNbinsX();

            float weight = 0;
            weight = pileUpReweightingHisto->GetBinContent( pileUpReweightingHisto->FindBin(nvtx) );

            //we don't want to kill events bc they have no weight
            if( weight <= 0 ) weight = 1.;
            return weight;

        }


        // -------------------------------------------------------------
        //                        data members
        // -------------------------------------------------------------
    private:

        // Raw objects

        std::vector<IPHCTree::NTJet>      rawJets;
        NTMET                             rawMET;

        // Partially selected objects

        std::vector<IPHCTree::NTElectron> goodElectrons;
        std::vector<IPHCTree::NTMuon>     goodMuons;
        std::vector<IPHCTree::NTJet>      goodJets;

        // Objects for analysis

        int numberOfSelectedLeptons;

        TLorentzVector theLeadingLepton;
        int            theLeadingLepton_pdgid;
        float          theLeadingLepton_isolation;

        TLorentzVector theSecondLepton;
        int            theSecondLepton_pdgid;
        float          theSecondLepton_isolation;

        std::vector<IPHCTree::NTElectron> selectedElectrons;
        std::vector<IPHCTree::NTMuon>     selectedMuons;
        std::vector<IPHCTree::NTJet>      selectedJets;
        std::vector<IPHCTree::NTJet>      selectedBJets;
        TLorentzVector theMET;

        // Corrections managers

        string bTagReshapingInput;
        string jetCorrectionsMCInput;
        string jetCorrectionsDataInput;
        string pileUpReweightingInput;

        TH1F* pileUpReweightingHisto;
        JetCorrectionUncertainty* JESUncertainty_MC;
        JetCorrectionUncertainty* JESUncertainty_Data;

        BTagShapeInterface* BTagReshape_nominal;
        BTagShapeInterface* BTagReshape_upBC;
        BTagShapeInterface* BTagReshape_downBC;
        BTagShapeInterface* BTagReshape_upLight;
        BTagShapeInterface* BTagReshape_downLight;

};

#endif

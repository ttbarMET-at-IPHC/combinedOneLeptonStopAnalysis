
bool findISRJet()
{
    if (myEvent.nJets < 5) return false;

    bool foundISRJet = false;
    for (unsigned int i = 0 ; i < myEvent.jets.size() ; i++)
    {
        // Check jet is high-pt
        if ((myEvent.jets)[i].Pt() < 200) continue;

        // Check jet isn't b-tagged
        if (sampleType == "data") { if ((myEvent.jets_CSV_raw)[i]      > 0.679) continue; }
        else                      { if ((myEvent.jets_CSV_reshaped)[i] > 0.679) continue; }

        foundISRJet = true;
    }

    return foundISRJet;
}

bool cutAndCount_T2tt(float cutMEToverSqrtHT, float cutMT, float cutMT2W, float cutMET, float cutDeltaPhi, float cutHadronicChi2, bool enableISRJetRequirement)
{
    if (myEvent.METoverSqrtHT   < cutMEToverSqrtHT) return false;
    if (myEvent.MT              < cutMT)            return false;
    if (myEvent.MT2W            < cutMT2W)          return false;
    if (myEvent.MET             < cutMET)           return false;
    if (myEvent.deltaPhiMETJets < cutDeltaPhi)      return false;
    if (myEvent.hadronicChi2    > cutHadronicChi2)  return false;
    if ((enableISRJetRequirement) && (!findISRJet()))       return false;

    return true;
}

bool cutAndCount_T2tt_offShellLoose(bool applyMTCut) { return cutAndCount_T2tt(-1, 120 * applyMTCut, -1,  200, -1,  999999, true ); }
bool cutAndCount_T2tt_offShellTight(bool applyMTCut) { return cutAndCount_T2tt(10, 140 * applyMTCut, -1,  -1,  -1,  999999, true ); }
bool cutAndCount_T2tt_lowDeltaM    (bool applyMTCut) { return cutAndCount_T2tt(-1, 130 * applyMTCut, -1,  130, 0.8, 5,      false); }
bool cutAndCount_T2tt_mediumDeltaM (bool applyMTCut) { return cutAndCount_T2tt(10, 140 * applyMTCut, 180, -1,  0.8, 3,      false); }
bool cutAndCount_T2tt_highDeltaM   (bool applyMTCut) { return cutAndCount_T2tt(15, 190 * applyMTCut, 240, -1,  -1,  999999, false); }


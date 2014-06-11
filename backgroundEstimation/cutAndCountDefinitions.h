
bool findISRJet()
{
    if (myEventPointer->nJets < 5) return false;

    bool foundISRJet = false;
    for (unsigned int i = 0 ; i < myEventPointer->jets.size() ; i++)
    {
        // Check jet is high-pt
        if ((myEventPointer->jets)[i].Pt() < 200) continue;

        // Check jet isn't b-tagged
        if (sampleType == "data") { if ((myEventPointer->jets_CSV_raw)[i]      > 0.679) continue; }
        else                      { if ((myEventPointer->jets_CSV_reshaped)[i] > 0.679) continue; }

        foundISRJet = true;
    }

    return foundISRJet;
}

bool cutAndCount_T2tt(float cutMEToverSqrtHT, float cutMT, float cutMT2W, float cutMET, float cutDeltaPhi, float cutHadronicChi2, bool enableISRJetRequirement)
{
    if (myEventPointer->METoverSqrtHT   < cutMEToverSqrtHT) return false;
    if (myEventPointer->MT              < cutMT)            return false;
    if (myEventPointer->MT2W            < cutMT2W)          return false;
    if (myEventPointer->MET             < cutMET)           return false;
    if (myEventPointer->deltaPhiMETJets < cutDeltaPhi)      return false;
    if (myEventPointer->hadronicChi2    > cutHadronicChi2)  return false;
    if ((enableISRJetRequirement) && (!findISRJet()))       return false;

    return true;
}
bool cutAndCount_T2tt_offShellLoose(bool applyMTCut) { return cutAndCount_T2tt(-1, 120 * applyMTCut, -1,  200, -1,  999999, true ); }
bool cutAndCount_T2tt_offShellTight(bool applyMTCut) { return cutAndCount_T2tt(10, 140 * applyMTCut, -1,  -1,  -1,  999999, true ); }
bool cutAndCount_T2tt_lowDeltaM    (bool applyMTCut) { return cutAndCount_T2tt(-1, 130 * applyMTCut, -1,  130, 0.8, 5,      false); }
bool cutAndCount_T2tt_mediumDeltaM (bool applyMTCut) { return cutAndCount_T2tt(10, 140 * applyMTCut, 180, -1,  0.8, 3,      false); }
bool cutAndCount_T2tt_highDeltaM   (bool applyMTCut) { return cutAndCount_T2tt(15, 190 * applyMTCut, 240, -1,  -1,  999999, false); }

#include "backgroundEstimation.h"

#define PLACEHOLDER_CR4_CR5_MODELING_UNCERTAINTY             0.1  // In case CR4/CR5 modeling checks are not available
#define DILEPTON_TTBAR_JET_MULTIPLICITY_MODELING_UNCERTAINTY 0.05 // From Lara/Pedrame// From Lara/Pedrame
#define ISOLATED_TRACK_VETO_UNCERTAINTY                      0.06 // From tag and probe studies
#define TAU_VETO_UNCERTAINTY                                 0.07 // From tau POG

//#define SIGNAL_CONTAMINATION_INPUT "T2tt_475_175"

int main (int argc, char *argv[])
{
    if (argc <= 1) { WARNING_MSG << "No signal region specified" << endl; return -1; }

    string signalRegion = argv[1];
    backgroundEstimation(signalRegion).Run();

    return 0;
}

backgroundEstimation::backgroundEstimation(string signalRegionLabel_)
{
    signalRegionLabel = signalRegionLabel_;

    // ########################
    // #  Read raw MC yields  #
    // ########################

    rawYieldTable = Table("rawYieldTables/"+signalRegionLabel+".tab");
    secondLeptonInAcceptanceYieldTable = Table("secondLeptonInAcceptance/"+signalRegionLabel+".tab");

    // ###################################################################
    // #  Initialize the table containing the raw mc and the prediction  #
    // ###################################################################

    vector<string> predictionTableColumns = { "raw_mc", "prediction" };
    predictionTable = Table(predictionTableColumns, processesTagList);

    // Raw MC
    Figure N1ltop_mc  = rawYieldTable.Get("signalRegion_MTtail","1ltop"   );
    Figure N2ltop_mc  = rawYieldTable.Get("signalRegion_MTtail","ttbar_2l");
    Figure Nwjets_mc  = rawYieldTable.Get("signalRegion_MTtail","W+jets"  );
    Figure Nrare_mc   = rawYieldTable.Get("signalRegion_MTtail","rare"    );
    Figure NSumBkg_mc = N1ltop_mc+N2ltop_mc+Nwjets_mc+Nrare_mc;

    predictionTable.Set("raw_mc","1ltop",    N1ltop_mc );
    predictionTable.Set("raw_mc","ttbar_2l", N2ltop_mc );
    predictionTable.Set("raw_mc","W+jets",   Nwjets_mc );
    predictionTable.Set("raw_mc","rare",     Nrare_mc  );
    predictionTable.Set("raw_mc","totalSM",  NSumBkg_mc);

    // ######################################################
    // #  Initialize the table containing the scale factors #
    // ######################################################

    vector<string> dummy = { "value" };
    scaleFactorTable = Table(dummy,scaleFactorsTagList);


    // ####################################
    // #  Read results from other modules #
    // ####################################

    #ifdef USING_CR4_AND_CR5_CHECKS_FROM_LOOSENED_SIGNAL_REGIONS
    	// Read the uncertainty from CR4 CR5 modeling checks
        Table temporaryTable;

        if (findSubstring(signalRegionLabel,"BDT"))
    	    temporaryTable = Table("../checkCR4CR5modeling/results/uncertainties_BDT.tab");
        else
    	    temporaryTable = Table("../checkCR4CR5modeling/results/uncertainties_CnC.tab");

        ttll_CR4CR5ModelingUncertainty = temporaryTable.Get("value",signalRegionLabel).error();
    #else
    	ttll_CR4CR5ModelingUncertainty = PLACEHOLDER_CR4_CR5_MODELING_UNCERTAINTY;
    #endif

    ttll_nJetsModelingUncertainty = DILEPTON_TTBAR_JET_MULTIPLICITY_MODELING_UNCERTAINTY;

    // ##################################################################
    // #  Initialize the table containing the systematic uncertainties  #
    // ##################################################################

    vector<string> uncertainties = { "absolute", "relative" };
    systematicsUncertainties = Table(uncertainties, systematicsTagList);

    // Reset the systematics flags
    ResetSystematics();
}

void backgroundEstimation::Run()
{
    ComputePredictionWithSystematics();

    scaleFactorTable.        Print("scaleFactors/"+signalRegionLabel+".tab",4);
    systematicsUncertainties.Print("systematics/" +signalRegionLabel+".tab",4);
    predictionTable.         Print("prediction/"  +signalRegionLabel+".tab",4);

}

void backgroundEstimation::ResetSystematics()
{
    ttll_CR4and5_rescale            = 1.0;
    ttll_nJets_rescale              = 1.0;
    ttll_2ndlepVeto_rescale         = 1.0;
    oneLepTopCrossSection_rescale   = 1.0;
    WjetCrossSection_rescale        = 1.0;
    rareCrossSection_rescale        = 1.0;
    #ifdef USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
        SF_MTtail_Wjets_variation = 0;
        SF_MTtail_1ltop_variation = 0;
    #else
        SFR_Wjets_variation             = 0;
        tailToPeakRatio_1lTop_variation = 0;
    #endif
    MTpeakStat_variation            = 0;
    WjetsStat_variation             = 0;
    top1lStat_variation             = 0;
    ttbar2lStat_variation           = 0;
    rareStat_variation              = 0;
}

Figure backgroundEstimation::ComputePrediction()
{
    ComputeSFpre();
    ComputeSFpost();
    ComputeSF0btag();
    ComputeSFvetopeak();

    #ifdef USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
        ImportMTTailCorrectionFromTemplateFitMethod();
    #else
        ComputeMTTailToPeakRatioCorrectionMethod();
    #endif

    FillPredictionTable();
    return predictionTable.Get("prediction","totalSM");
}

void backgroundEstimation::ComputePredictionWithSystematics()
{

    // Loop over the uncertainties
    for (unsigned int s = 0 ; s < systematicsTagList.size() ; s++)
    {
        ResetSystematics();
        string systematic = systematicsTagList[s];
        float uncertainty = 0.0;

        if (systematic == "total") continue;

        else if (systematic ==  "tt->ll_(CR4,CR5)")
        {
            // Determine the uncertainty from values stored in the table
	        ttll_CR4and5_rescale = 1-ttll_CR4CR5ModelingUncertainty; float yieldDown = ComputePrediction().value();
            ttll_CR4and5_rescale = 1+ttll_CR4CR5ModelingUncertainty; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        else if (systematic ==  "tt->ll_(nJets)")
        {
            ttll_nJets_rescale = 1-ttll_nJetsModelingUncertainty; float yieldDown = ComputePrediction().value();
            ttll_nJets_rescale = 1+ttll_nJetsModelingUncertainty; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        else if (systematic ==  "tt->ll_(veto)")
        {
            ttll_2ndlepVeto_rescale = 1-ComputeSecondLeptonVetoUncertainty(); float yieldDown = ComputePrediction().value();
            ttll_2ndlepVeto_rescale = 1+ComputeSecondLeptonVetoUncertainty(); float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        #ifdef USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
        else if (systematic == "1ltop_(cross_section)")
        {
            oneLepTopCrossSection_rescale = 0.9; float yieldDown = ComputePrediction().value();
            oneLepTopCrossSection_rescale = 1.1; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        #endif
        else if (systematic == "W+jets_(cross_section)")
        {
            WjetCrossSection_rescale = 0.5; float yieldDown = ComputePrediction().value();
            WjetCrossSection_rescale = 1.5; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        else if (systematic == "Rare_(cross_section)")
        {
            rareCrossSection_rescale = 0.5; float yieldDown = ComputePrediction().value();
            rareCrossSection_rescale = 1.5; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        #ifdef USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
        else if (systematic == "SF_MTtail_Wjets")
        {
            SF_MTtail_Wjets_variation = -1.0; float yieldDown = ComputePrediction().value();
            SF_MTtail_Wjets_variation =  1.0; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        else if (systematic == "SF_MTtail_1ltop")
        {
            SF_MTtail_1ltop_variation = -1.0; float yieldDown = ComputePrediction().value();
            SF_MTtail_1ltop_variation =  1.0; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        #else
        else if (systematic == "SFR_Wjets")
        {
            SFR_Wjets_variation = -1.0; float yieldDown = ComputePrediction().value();
            SFR_Wjets_variation =  1.0; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        else if (systematic == "1ltop_tailToPeak")
        {
            tailToPeakRatio_1lTop_variation = -1.0; float yieldDown = ComputePrediction().value();
            tailToPeakRatio_1lTop_variation =  1.0; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        #endif
        else if (systematic == "MTpeak")
        {
            MTpeakStat_variation = -1.0; float yieldDown = ComputePrediction().value();
            MTpeakStat_variation =  1.0; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        else if (systematic == "tt->ll_(MCstat)")
        {
            ttbar2lStat_variation = -1.0; float yieldDown = ComputePrediction().value();
            ttbar2lStat_variation =  1.0; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        else if (systematic == "1l_top_(MCstat)")
        {
            top1lStat_variation = -1.0; float yieldDown = ComputePrediction().value();
            top1lStat_variation =  1.0; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        else if (systematic == "W+jets_(MCstat)")
        {
            WjetsStat_variation = -1.0; float yieldDown = ComputePrediction().value();
            WjetsStat_variation =  1.0; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        else if (systematic == "rare_(MCstat)")
        {
            rareStat_variation = -1.0; float yieldDown = ComputePrediction().value();
            rareStat_variation =  1.0; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }

        systematicsUncertainties.Set("absolute",systematic,Figure(uncertainty,0.0));
    }

    // Compute prediction for nominal case
    ResetSystematics();
    float yieldNominal = ComputePrediction().value();

    // Fill total uncertainty

    float totalUncertainty = 0.0;
    for (unsigned int s = 0 ; s < systematicsTagList.size() ; s++)
    {
        string systematic = systematicsTagList[s];
        float u = systematicsUncertainties.Get("absolute",systematic).value();
        totalUncertainty += u*u;
    }
    totalUncertainty = sqrt(totalUncertainty);
    systematicsUncertainties.Set("absolute","total",Figure(totalUncertainty,0.0));

    predictionTable.Set("prediction", "totalSM", Figure(yieldNominal, totalUncertainty));

    // Fill relative uncertainty from absolute

    for (unsigned int s = 0 ; s < systematicsTagList.size() ; s++)
    {
        string systematic = systematicsTagList[s];
        float relUncertainty = systematicsUncertainties.Get("absolute",systematic).value() / yieldNominal;
        if (yieldNominal == 0.0) relUncertainty = 0;
        systematicsUncertainties.Set("relative",systematic,Figure(relUncertainty,0.0));
    }

    // Fill scale factor table

    scaleFactorTable.Set("value","SF_pre",     SF_pre);
    scaleFactorTable.Set("value","SF_post",    SF_post);
    scaleFactorTable.Set("value","SF_0btag",   SF_0btag);
    scaleFactorTable.Set("value","SF_vetopeak",SF_vetopeak);
    #ifdef USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
    scaleFactorTable.Set("value","SF_MTtail_Wjets", SF_MTtail_Wjets);
    scaleFactorTable.Set("value","SF_MTtail_1ltop", SF_MTtail_1ltop);
    #else
    scaleFactorTable.Set("value","SFR_W+jets", SFR_Wjets_mean);
    scaleFactorTable.Set("value","R_W+jets",   R_Wjets_corrected);
    scaleFactorTable.Set("value","R_1ltop",    R_1ltop_mean);
    #endif
    scaleFactorTable.Print("scaleFactors/"+signalRegionLabel+".tab",4);

}

float backgroundEstimation::ComputeSecondLeptonVetoUncertainty()
{
    Figure ttbar_2l_all                     = secondLeptonInAcceptanceYieldTable.Get("preveto_MTtail", "ttbar_2l");
    Figure ttbar_2l_withTrackInAcceptance   = secondLeptonInAcceptanceYieldTable.Get("singleTrack",         "ttbar_2l");
    Figure ttbar_2l_withHadrTauInAcceptance = secondLeptonInAcceptanceYieldTable.Get("hadronicTau",         "ttbar_2l");

    if (ttbar_2l_all.value() == 0) return 0.0;

    Figure fraction_track = ttbar_2l_withTrackInAcceptance   / ttbar_2l_all;
    Figure fraction_tau   = ttbar_2l_withHadrTauInAcceptance / ttbar_2l_all;

    if (fraction_track.value() < 0.02) fraction_track = Figure(0.02,0);
    if (fraction_tau.value()   < 0.02) fraction_tau   = Figure(0.02,0);

    float uncertainty = fraction_track.value() * ISOLATED_TRACK_VETO_UNCERTAINTY
                      + fraction_tau.value()   * TAU_VETO_UNCERTAINTY;

    return uncertainty;
}

void backgroundEstimation::ComputeSFpre()
{
    Figure preveto_1ltop    = rawYieldTable.Get("preveto_MTpeak","1ltop"   ) * oneLepTopCrossSection_rescale;
    Figure preveto_ttbar_2l = rawYieldTable.Get("preveto_MTpeak","ttbar_2l");
    Figure preveto_Wjets    = rawYieldTable.Get("preveto_MTpeak","W+jets"  ) * WjetCrossSection_rescale;
    Figure preveto_rare     = rawYieldTable.Get("preveto_MTpeak","rare"    ) * rareCrossSection_rescale;
    Figure preveto_data     = rawYieldTable.Get("preveto_MTpeak","data"    );

    //Figure preveto_signal   = rawYieldTable.Get("preveto_MTpeak","signal"    );
    //SF_pre = (preveto_data - preveto_rare - preveto_signal)
    //      / (preveto_1ltop + preveto_ttbar_2l + preveto_Wjets);

    SF_pre = (preveto_data - preveto_rare)
          / (preveto_1ltop + preveto_ttbar_2l + preveto_Wjets);
    if (MTpeakStat_variation) SF_pre.keepVariation(MTpeakStat_variation,"noNegativeValue");

}

void backgroundEstimation::ComputeSFpost()
{
    Figure postveto_1ltop    = rawYieldTable.Get("signalRegion_MTpeak","1ltop"   ) * oneLepTopCrossSection_rescale;
    Figure postveto_ttbar_2l = rawYieldTable.Get("signalRegion_MTpeak","ttbar_2l");
    Figure postveto_Wjets    = rawYieldTable.Get("signalRegion_MTpeak","W+jets"  ) * WjetCrossSection_rescale;
    Figure postveto_rare     = rawYieldTable.Get("signalRegion_MTpeak","rare"    ) * rareCrossSection_rescale;
    Figure postveto_data     = rawYieldTable.Get("signalRegion_MTpeak","data"    );

    //Figure postveto_signal   = rawYieldTable.Get("signalRegion_MTpeak","signal"    );
    //SF_post = (postveto_data - postveto_rare - SF_pre * postveto_ttbar_2l - postveto_signal)
    //       / (postveto_1ltop + postveto_Wjets);

    SF_post = (postveto_data - postveto_rare - SF_pre * postveto_ttbar_2l)
           / (postveto_1ltop + postveto_Wjets);

    if (MTpeakStat_variation) SF_post.keepVariation(MTpeakStat_variation,"noNegativeValue");
}

void backgroundEstimation::ComputeSF0btag()
{
    // NB : SF_0btag is used only in the control plots, not in the background prediction iteself

    Figure noBTagPeak_1ltop    = rawYieldTable.Get("0btag_MTpeak","1ltop"   );
    Figure noBTagPeak_ttbar_2l = rawYieldTable.Get("0btag_MTpeak","ttbar_2l");
    Figure noBTagPeak_Wjets    = rawYieldTable.Get("0btag_MTpeak","W+jets"  ) * WjetCrossSection_rescale;
    Figure noBTagPeak_rare     = rawYieldTable.Get("0btag_MTpeak","rare"    ) * rareCrossSection_rescale;
    Figure noBTagPeak_data     = rawYieldTable.Get("0btag_MTpeak","data"    );

    // FIXME shoudln't we apply SF_pre to dilepton here ?
    SF_0btag = (noBTagPeak_data  - noBTagPeak_rare - noBTagPeak_ttbar_2l)
             / (noBTagPeak_1ltop + noBTagPeak_Wjets);
}

void backgroundEstimation::ComputeSFvetopeak()
{
    // NB : SF_vetopeak is used only in the control plots, not in the background prediction iteself

    Figure peakveto_1ltop    = rawYieldTable.Get("reversedVeto_MTpeak","1ltop"   );
    Figure peakveto_ttbar_2l = rawYieldTable.Get("reversedVeto_MTpeak","ttbar_2l");
    Figure peakveto_Wjets    = rawYieldTable.Get("reversedVeto_MTpeak","W+jets"  );
    Figure peakveto_rare     = rawYieldTable.Get("reversedVeto_MTpeak","rare"    );
    Figure peakveto_data     = rawYieldTable.Get("reversedVeto_MTpeak","data"    );

    SF_vetopeak = (peakveto_data  - peakveto_rare - SF_pre * peakveto_ttbar_2l)
                / (peakveto_1ltop + peakveto_Wjets);
}


#ifdef USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
void backgroundEstimation::ImportMTTailCorrectionFromTemplateFitMethod()
{
    Table SFR_table = Table("../MTtailCorrection/results/SF_MTtail.tab");
    //Table SFR_table = Table("../MTtailCorrection/signalContamination/"+string(SIGNAL_CONTAMINATION_INPUT)+"/SF_MTtail.tab");

    // Remove low/medium/highDM suffix in label for BDT's
    string signalRegionLabel_ = signalRegionLabel;
    if (findSubstring(signalRegionLabel_,"BDT"))
    {
        size_t pos;
        pos = signalRegionLabel_.find("_low");
        if (pos != string::npos) signalRegionLabel_ = signalRegionLabel_.substr(0,pos);
        pos = signalRegionLabel_.find("_med");
        if (pos != string::npos) signalRegionLabel_ = signalRegionLabel_.substr(0,pos);
        pos = signalRegionLabel_.find("_high");
        if (pos != string::npos) signalRegionLabel_ = signalRegionLabel_.substr(0,pos);
    }

    SF_MTtail_Wjets = SFR_table.Get("SFR_Wjets",signalRegionLabel_);
    SF_MTtail_1ltop = SFR_table.Get("SFR_1ltop",signalRegionLabel_);

    // Fix for signal contamination crazy values : we assume that SFR_1ltop is >= 1.
    // We keep the same relative uncertainty
    if (SF_MTtail_1ltop.value() < 1) { SF_MTtail_1ltop = Figure(1.0,SF_MTtail_1ltop.value() / SF_MTtail_1ltop.error()); }

    if (SF_MTtail_Wjets_variation) SF_MTtail_Wjets.keepVariation(SF_MTtail_Wjets_variation,"noNegativeValue");
    if (SF_MTtail_1ltop_variation) SF_MTtail_1ltop.keepVariation(SF_MTtail_1ltop_variation,"noNegativeValue");
}
#else
void backgroundEstimation::ComputeMTTailToPeakRatioCorrectionMethod()
{
    Figure signalRegionPeak_1ltop    = rawYieldTable.Get("signalRegion_MTpeak","1ltop"   );
    Figure signalRegionPeak_ttbar_2l = rawYieldTable.Get("signalRegion_MTpeak","ttbar_2l");
    Figure signalRegionPeak_Wjets    = rawYieldTable.Get("signalRegion_MTpeak","W+jets"  ) * WjetCrossSection_rescale;
    Figure signalRegionPeak_rare     = rawYieldTable.Get("signalRegion_MTpeak","rare"    ) * rareCrossSection_rescale;
    Figure signalRegionPeak_data     = rawYieldTable.Get("signalRegion_MTpeak","data"    );

    Figure signalRegionTail_1ltop    = rawYieldTable.Get("signalRegion_MTtail","1ltop"   );
    Figure signalRegionTail_ttbar_2l = rawYieldTable.Get("signalRegion_MTtail","ttbar_2l");
    Figure signalRegionTail_Wjets    = rawYieldTable.Get("signalRegion_MTtail","W+jets"  ) * WjetCrossSection_rescale;
    Figure signalRegionTail_rare     = rawYieldTable.Get("signalRegion_MTtail","rare"    ) * rareCrossSection_rescale;
    Figure signalRegionTail_data     = rawYieldTable.Get("signalRegion_MTtail","data"    );

    Figure noBTagTail_1ltop          = rawYieldTable.Get("0btag_MTtail","1ltop"   );
    Figure noBTagTail_ttbar_2l       = rawYieldTable.Get("0btag_MTtail","ttbar_2l");
    Figure noBTagTail_Wjets          = rawYieldTable.Get("0btag_MTtail","W+jets"  ) * WjetCrossSection_rescale;
    Figure noBTagTail_rare           = rawYieldTable.Get("0btag_MTtail","rare"    ) * rareCrossSection_rescale;
    Figure noBTagTail_data           = rawYieldTable.Get("0btag_MTtail","data"    );

    R_Wjets_mc = (noBTagTail_Wjets + signalRegionTail_Wjets) / (noBTagPeak_Wjets + signalRegionPeak_Wjets);
    R_1ltop_mc = (noBTagTail_1ltop + signalRegionTail_1ltop) / (noBTagPeak_1ltop + signalRegionPeak_1ltop);

    SFR_all    = noBTagTail_data / ((noBTagTail_Wjets + noBTagTail_1ltop)*SF_0btag + noBTagTail_ttbar_2l + noBTagTail_rare);
    SFR_Wonly  = (noBTagTail_data - noBTagTail_1ltop*SF_0btag - noBTagTail_ttbar_2l - noBTagTail_rare) / (noBTagTail_Wjets*SF_0btag);

    SFR_Wjets_mean = Figure((SFR_all.value()+SFR_Wonly.value())/2.0 , (SFR_all.error() + SFR_Wonly.error())/2.0);

    if (SFR_Wjets_variation) SFR_W_mean.keepVariation(SFR_Wjets_variation,"noNegativeValue");

    R_Wjets_corrected  = R_Wjets_mc  * SFR_W_mean.value();
    R_1ltop_corrected = R_1ltop_mc * SFR_W_mean.value();

    // Old method :
    //  R_1ltop_mean = Figure((RW_corrected.value() + R_1ltop_corrected.value()) / 2.0, fabs(RW_corrected.value() - R_1ltop_corrected.value()) / 2.0);
    //
    // NB : We now take the ration R from MC without averaging
    //
    R_1ltop_mean = R_1ltop_corrected;

    if (tailToPeakRatio_1lTop_variation) R_1ltop_mean.keepVariation(tailToPeakRatio_1lTop_variation,"noNegativeValue");
}
#endif

void backgroundEstimation::FillPredictionTable()
{
    #ifdef USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
    Figure N1ltop_tail = rawYieldTable.Get("signalRegion_MTtail","1ltop"   ) * oneLepTopCrossSection_rescale;
    Figure Nwjets_tail = rawYieldTable.Get("signalRegion_MTtail","W+jets"  ) * WjetCrossSection_rescale;
    #else
    Figure N1ltop_peak = rawYieldTable.Get("signalRegion_MTpeak","1ltop"   );
    Figure Nwjets_peak = rawYieldTable.Get("signalRegion_MTpeak","W+jets"  ) * WjetCrossSection_rescale;
    #endif
    Figure N2ltop_tail = rawYieldTable.Get("signalRegion_MTtail","ttbar_2l") * ttll_CR4and5_rescale * ttll_nJets_rescale * ttll_2ndlepVeto_rescale;
    Figure Nrare_tail  = rawYieldTable.Get("signalRegion_MTtail","rare"    ) * rareCrossSection_rescale;

    // MC stat uncertainty
    #ifdef USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
    if (top1lStat_variation)   N1ltop_tail.keepVariation(top1lStat_variation,  "noNegativeValue");
    if (WjetsStat_variation)   Nwjets_tail.keepVariation(WjetsStat_variation,  "noNegativeValue");
    #else
    if (top1lStat_variation)   N1ltop_peak.keepVariation(top1lStat_variation,  "noNegativeValue");
    if (WjetsStat_variation)   Nwjets_peak.keepVariation(WjetsStat_variation,  "noNegativeValue");
    #endif
    if (ttbar2lStat_variation) N2ltop_tail.keepVariation(ttbar2lStat_variation,"noNegativeValue");
    if (rareStat_variation)    Nrare_tail .keepVariation(rareStat_variation,   "noNegativeValue");

    // To have cross section uncertainty in per-process prediction table
    #ifdef USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
    Nwjets_tail *= Figure(1.0, 0.5);
    #else
    Nwjets_peak *= Figure(1.0, 0.5);
    #endif
    Nrare_tail  *= Figure(1.0, 0.5);

    // To have the (temporary and arbitrary) numbers for tt->ll systematics in the per-process prediction table
    N2ltop_tail *= Figure(1.0,ttll_nJetsModelingUncertainty );
    N2ltop_tail *= Figure(1.0,ttll_CR4CR5ModelingUncertainty);
    N2ltop_tail *= Figure(1.0,ComputeSecondLeptonVetoUncertainty());

    // Prediction
    #ifdef USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
        Figure N1ltop_prediction  = N1ltop_tail  * SF_post * SF_MTtail_1ltop;
        Figure Nwjets_prediction  = Nwjets_tail  * SF_post * SF_MTtail_Wjets;
    #else
        Figure N1ltop_prediction  = N1ltop_peak  * SF_post * R_1ltop_mean;
        Figure Nwjets_prediction  = Nwjets_peak  * SF_post * R_Wjets_corrected;
    #endif
    Figure N2ltop_prediction  = N2ltop_tail  * SF_pre;
    Figure Nrare_prediction   = Nrare_tail;


    // Background sum
    Figure NSumBkg_prediction = N1ltop_prediction + N2ltop_prediction + Nwjets_prediction + Nrare_prediction;

    // Fill table
    predictionTable.Set("prediction","1ltop",    N1ltop_prediction  );
    predictionTable.Set("prediction","ttbar_2l", N2ltop_prediction  );
    predictionTable.Set("prediction","W+jets",   Nwjets_prediction  );
    predictionTable.Set("prediction","rare",     Nrare_prediction );
    predictionTable.Set("prediction","totalSM",  NSumBkg_prediction );
}



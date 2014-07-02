#include "backgroundEstimation.h"



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

    // Defines the regions of interesets
    //
    //
    vector<string> regions1 = { "preveto_MTpeak",      "preveto_MTtail",
                             "signalRegion_MTpeak", "signalRegion_MTtail",
                             "0btag_MTpeak",        "0btag_MTtail",        };
    vector<string> regions2 = { "preveto_MTpeak",      "preveto_MTtail",
                             "signalRegion_MTpeak", "signalRegion_MTtail",
                             "0btag_MTpeak",        "0btag_MTtail",
                             "antiveto_MTpeak",     "antiveto_MTtail"};
    vector<string> regions3 = { "2leptons_MTpeak",     "2leptons_MTtail"};

    vector<string> regions;
    if (CR45) {
      regions=regions2;
    }
    else {
      regions=regions1;
    }

/* ---> was before!! strange not to have the same order --> a problem or not?
    vector<string> regions = 
    { 
        "preveto_MTpeak",
        "preveto_MTtail",
        "0btag_MTpeak",
        "0btag_MTtail",
        "signalRegion_MTpeak",
        "signalRegion_MTtail"
    };
*/
    
    // Import the raw yield table from external file
    rawYieldTable = Table("rawYieldTables/"+signalRegionLabel+".tab");
    if (CR45) {
    rawYieldTable_2leptons = Table("rawYieldTables/"+signalRegionLabel+"_2leptons.tab");
    }

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
    if (CR45) {
    scaleFactorTable_2leptons = Table(dummy,scaleFactorsTagList_2leptons);
    }

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

    //scaleFactorTable.PrintLatex(3);
    //systematicsUncertainties.PrintLatex(1,"noError");
    //predictionTable.PrintLatex(1);

    scaleFactorTable.Print("scaleFactors/"+signalRegionLabel+".tab",4);
    systematicsUncertainties.Print("systematics/"+signalRegionLabel+".tab",4);
    predictionTable.Print("prediction/"+signalRegionLabel+".tab",4);

    if (CR45) {
     ResetSystematics();

     ComputeSF2lpeak();
     ComputeSFvetopeak();
     ComputeK3K4();

     ComputeSFpre();
     ComputeSFpost();
     ComputeRandSFR();
     ComputeSF2ltail();
     ComputeSFvetotail();

     scaleFactorTable_2leptons.Set("value","SF2lpeak",SF2lpeak);
     scaleFactorTable_2leptons.Set("value","SF2ltail",SF2ltail);
     scaleFactorTable_2leptons.Set("value","K3",K3);
     scaleFactorTable_2leptons.Set("value","K4",K4);
     scaleFactorTable_2leptons.Set("value","SF_pre",SFpre);
     scaleFactorTable_2leptons.Set("value","SFR_W+jets",SFR_W_mean);
     scaleFactorTable_2leptons.Set("value","SFvetopeak",SFvetopeak);
     scaleFactorTable_2leptons.Set("value","SFvetotail",SFvetotail);
     scaleFactorTable_2leptons.Print("scaleFactors/"+signalRegionLabel+"_2leptons.tab",4);
    }
}

void backgroundEstimation::ResetSystematics()
{
    ttll_CR4and5_rescale            = 1.0;
    ttll_nJets_rescale              = 1.0;
    ttll_2ndlepVeto_rescale         = 1.0;
    WjetCrossSection_rescale        = 1.0;
    rareCrossSection_rescale        = 1.0;
    tailToPeakRatio_1lTop_variation = 0;
    SFR_Wjets_variation             = 0;
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
    ComputeRandSFR();
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
            ttll_CR4and5_rescale = 1-0.1; float yieldDown = ComputePrediction().value(); 
            ttll_CR4and5_rescale = 1+0.1; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        else if (systematic ==  "tt->ll_(nJets)")
        {
            ttll_nJets_rescale = 1-0.17; float yieldDown = ComputePrediction().value(); 
            ttll_nJets_rescale = 1+0.17; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        else if (systematic ==  "tt->ll_(veto)")
        {
            ttll_2ndlepVeto_rescale = 1-0.024; float yieldDown = ComputePrediction().value(); 
            ttll_2ndlepVeto_rescale = 1+0.024; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
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
        else if (systematic == "1ltop_tailToPeak") 
        {
            tailToPeakRatio_1lTop_variation = -1.0; float yieldDown = ComputePrediction().value();
            tailToPeakRatio_1lTop_variation =  1.0; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        else if (systematic == "SFR_Wjets")
        {
            SFR_Wjets_variation = -1.0; float yieldDown = ComputePrediction().value();
            SFR_Wjets_variation =  1.0; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
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
        float absUncertainty = systematicsUncertainties.Get("absolute",systematic).value();
        systematicsUncertainties.Set("relative",systematic,Figure( absUncertainty / yieldNominal,0.0));
    }

    // Fill scale factor table
   
    scaleFactorTable.Set("value","SF_pre",SFpre);
    scaleFactorTable.Set("value","SF_post",SFpost);
    scaleFactorTable.Set("value","SF_0btag",SF_0btag);
    scaleFactorTable.Set("value","SFR_W+jets",SFR_W_mean);
    scaleFactorTable.Set("value","R_W+jets",RW_corrected);
    scaleFactorTable.Set("value","R_1ltop",Rlj_mean);


}

void backgroundEstimation::ComputeSFpre()
{
    Figure preveto_1ltop    = rawYieldTable.Get("preveto_MTpeak","1ltop"   );
    Figure preveto_ttbar_2l = rawYieldTable.Get("preveto_MTpeak","ttbar_2l");
    Figure preveto_Wjets    = rawYieldTable.Get("preveto_MTpeak","W+jets"  ) * WjetCrossSection_rescale;
    Figure preveto_rare     = rawYieldTable.Get("preveto_MTpeak","rare"    ) * rareCrossSection_rescale;
    Figure preveto_data     = rawYieldTable.Get("preveto_MTpeak","data"    );

    SFpre = (preveto_data - preveto_rare) / (preveto_1ltop + preveto_ttbar_2l + preveto_Wjets);
    if (MTpeakStat_variation) SFpre.keepVariation(MTpeakStat_variation);

}

void backgroundEstimation::ComputeSFpost()
{
    Figure postveto_1ltop    = rawYieldTable.Get("signalRegion_MTpeak","1ltop"   );
    Figure postveto_ttbar_2l = rawYieldTable.Get("signalRegion_MTpeak","ttbar_2l");
    Figure postveto_Wjets    = rawYieldTable.Get("signalRegion_MTpeak","W+jets"  ) * WjetCrossSection_rescale;
    Figure postveto_rare     = rawYieldTable.Get("signalRegion_MTpeak","rare"    ) * rareCrossSection_rescale;
    Figure postveto_data     = rawYieldTable.Get("signalRegion_MTpeak","data"    );
    
    SFpost = (postveto_data - postveto_rare - SFpre * postveto_ttbar_2l) / (postveto_1ltop + postveto_Wjets);
    if (MTpeakStat_variation) SFpost.keepVariation(MTpeakStat_variation);
}

void backgroundEstimation::ComputeSF2lpeak()
{
/*
    Figure peak2l_1ltop    = rawYieldTable_2leptons.Get("2leptons_MTpeak","1ltop"   );
    Figure peak2l_ttbar_2l = rawYieldTable_2leptons.Get("2leptons_MTpeak","ttbar_2l");
    Figure peak2l_Wjets    = rawYieldTable_2leptons.Get("2leptons_MTpeak","W+jets"  ); 
    Figure peak2l_rare     = rawYieldTable_2leptons.Get("2leptons_MTpeak","rare"    );
    Figure peak2l_data     = rawYieldTable_2leptons.Get("2leptons_MTpeak","data"    );
*/
/*
    Figure peak2l_1ltop    = rawYieldTable_2leptons.Get("2leptons_MTinv","1ltop"   );
    Figure peak2l_ttbar_2l = rawYieldTable_2leptons.Get("2leptons_MTinv","ttbar_2l");
    Figure peak2l_Wjets    = rawYieldTable_2leptons.Get("2leptons_MTinv","W+jets"  ); 
    Figure peak2l_rare     = rawYieldTable_2leptons.Get("2leptons_MTinv","rare"    );
    Figure peak2l_data     = rawYieldTable_2leptons.Get("2leptons_MTinv","data"    );
*/

    Figure peak2l_1ltop    = rawYieldTable_2leptons.Get("2leptons_MTinv","1ltop"    )
			+    rawYieldTable_2leptons.Get("2leptons_MTtail","1ltop"   );
    Figure peak2l_ttbar_2l = rawYieldTable_2leptons.Get("2leptons_MTinv","ttbar_2l" )
			+    rawYieldTable_2leptons.Get("2leptons_MTtail","ttbar_2l");
    Figure peak2l_Wjets    = rawYieldTable_2leptons.Get("2leptons_MTinv","W+jets"   )
			+    rawYieldTable_2leptons.Get("2leptons_MTtail","W+jets"  ); 
    Figure peak2l_rare     = rawYieldTable_2leptons.Get("2leptons_MTinv","rare"     )
			+    rawYieldTable_2leptons.Get("2leptons_MTtail","rare"    );
    Figure peak2l_data     = rawYieldTable_2leptons.Get("2leptons_MTinv","data"     )
			+    rawYieldTable_2leptons.Get("2leptons_MTtail","data"    );

    if (peak2l_data.value()>0) SF2lpeak = (peak2l_data - peak2l_rare) / (peak2l_1ltop + peak2l_Wjets + peak2l_ttbar_2l);

}
void backgroundEstimation::ComputeSF2ltail()
{
    Figure tail2l_1ltop    = rawYieldTable_2leptons.Get("2leptons_MTtail","1ltop"   );
    Figure tail2l_ttbar_2l = rawYieldTable_2leptons.Get("2leptons_MTtail","ttbar_2l");
    Figure tail2l_Wjets    = rawYieldTable_2leptons.Get("2leptons_MTtail","W+jets"  ); 
    Figure tail2l_rare     = rawYieldTable_2leptons.Get("2leptons_MTtail","rare"    );
    Figure tail2l_data     = rawYieldTable_2leptons.Get("2leptons_MTtail","data"    );

    if (tail2l_data.value()>0) SF2ltail = (tail2l_data - tail2l_rare - SF2lpeak* tail2l_1ltop - SF2lpeak* tail2l_Wjets) / (SF2lpeak* tail2l_ttbar_2l);

}
void backgroundEstimation::ComputeSFvetopeak()
{

    Figure peakveto_1ltop    = rawYieldTable.Get("antiveto_MTpeak","1ltop"   );
    Figure peakveto_ttbar_2l = rawYieldTable.Get("antiveto_MTpeak","ttbar_2l");
    Figure peakveto_Wjets    = rawYieldTable.Get("antiveto_MTpeak","W+jets"  ); 
    Figure peakveto_rare     = rawYieldTable.Get("antiveto_MTpeak","rare"    );
    Figure peakveto_data     = rawYieldTable.Get("antiveto_MTpeak","data"    );

    if (peakveto_data.value()>0) SFvetopeak =(peakveto_data - peakveto_rare - SFpre * peakveto_ttbar_2l) / (peakveto_1ltop + peakveto_Wjets);
/*
    Figure peakveto_1ltop    = rawYieldTable.Get("antiveto_MTinv","1ltop"   );
    Figure peakveto_ttbar_2l = rawYieldTable.Get("antiveto_MTinv","ttbar_2l");
    Figure peakveto_Wjets    = rawYieldTable.Get("antiveto_MTinv","W+jets"  ); 
    Figure peakveto_rare     = rawYieldTable.Get("antiveto_MTinv","rare"    );
    Figure peakveto_data     = rawYieldTable.Get("antiveto_MTinv","data"    );
    
    if (peakveto_data.value()>0) SFvetopeak = (peakveto_data - peakveto_rare) / (peakveto_1ltop + peakveto_Wjets + peakveto_ttbar_2l);
    // if (peakveto_data.value()>0) SFvetopeak = (peakveto_data) / (peakveto_1ltop + peakveto_Wjets + peakveto_ttbar_2l + peakveto_rare);
*/

}
void backgroundEstimation::ComputeSFvetotail()
{
    Figure tailveto_1ltop    = rawYieldTable.Get("antiveto_MTtail","1ltop"   );
    Figure tailveto_ttbar_2l = rawYieldTable.Get("antiveto_MTtail","ttbar_2l");
    Figure tailveto_Wjets    = rawYieldTable.Get("antiveto_MTtail","W+jets"  ); 
    Figure tailveto_rare     = rawYieldTable.Get("antiveto_MTtail","rare"    );
    Figure tailveto_data     = rawYieldTable.Get("antiveto_MTtail","data"    );
    
    if (tailveto_data.value()>0) SFvetotail = (tailveto_data - tailveto_rare - SFvetopeak*SFR_W_mean* tailveto_1ltop - SFvetopeak*SFR_W_mean* tailveto_Wjets) / (SFpre* tailveto_ttbar_2l);

    //if (tailveto_data.value()>0) SFvetotail = (tailveto_data - tailveto_rare - SFvetopeak* tailveto_1ltop - SFvetopeak* tailveto_Wjets) / (SFvetopeak* tailveto_ttbar_2l);
    //if (tailveto_data.value()>0) SFvetotail = (tailveto_data - SFvetopeak* tailveto_rare - SFvetopeak* tailveto_1ltop - SFvetopeak* tailveto_Wjets) / (SFvetopeak* tailveto_ttbar_2l) ;

}
void backgroundEstimation::ComputeK3K4()
{
    Figure N2_1ltop    = rawYieldTable_2leptons.Get("2leptons_N2","1ltop"   );
    Figure N2_ttbar_2l = rawYieldTable_2leptons.Get("2leptons_N2","ttbar_2l");
    Figure N2_Wjets    = rawYieldTable_2leptons.Get("2leptons_N2","W+jets"  ); 
    Figure N2_rare     = rawYieldTable_2leptons.Get("2leptons_N2","rare"    );
    Figure N2_data     = rawYieldTable_2leptons.Get("2leptons_N2","data"    );
    Figure N3_1ltop    = rawYieldTable_2leptons.Get("2leptons_N3","1ltop"   );
    Figure N3_ttbar_2l = rawYieldTable_2leptons.Get("2leptons_N3","ttbar_2l");
    Figure N3_Wjets    = rawYieldTable_2leptons.Get("2leptons_N3","W+jets"  ); 
    Figure N3_rare     = rawYieldTable_2leptons.Get("2leptons_N3","rare"    );
    Figure N3_data     = rawYieldTable_2leptons.Get("2leptons_N3","data"    );
    Figure N4_1ltop    = rawYieldTable_2leptons.Get("2leptons_N4","1ltop"   );
    Figure N4_ttbar_2l = rawYieldTable_2leptons.Get("2leptons_N4","ttbar_2l");
    Figure N4_Wjets    = rawYieldTable_2leptons.Get("2leptons_N4","W+jets"  ); 
    Figure N4_rare     = rawYieldTable_2leptons.Get("2leptons_N4","rare"    );
    Figure N4_data     = rawYieldTable_2leptons.Get("2leptons_N4","data"    );

    Figure N2 = N2_data - SF2lpeak*N2_1ltop - SF2lpeak*N2_Wjets - N2_rare;  // data yield minus non-dilepton tt MC yield for Njets =1 or 2.
    Figure N3 = N3_data - SF2lpeak*N3_1ltop - SF2lpeak*N3_Wjets - N3_rare;  // data yield minus non-dilepton tt MC yield for Njets =3.
    Figure N4 = N4_data - SF2lpeak*N4_1ltop - SF2lpeak*N4_Wjets - N4_rare;  // data yield minus non-dilepton tt MC yield for Njets >=4.

    Figure SF2 = N2/N2_ttbar_2l;
    Figure SF3 = N3/N3_ttbar_2l;
    Figure SF4 = N4/N4_ttbar_2l;
    
    K3 = SF3/SF2;
    K4 = SF4/SF3;

}

void backgroundEstimation::ComputeRandSFR()
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

    Figure noBTagPeak_1ltop          = rawYieldTable.Get("0btag_MTpeak","1ltop"   );
    Figure noBTagPeak_ttbar_2l       = rawYieldTable.Get("0btag_MTpeak","ttbar_2l");
    Figure noBTagPeak_Wjets          = rawYieldTable.Get("0btag_MTpeak","W+jets"  ) * WjetCrossSection_rescale;
    Figure noBTagPeak_rare           = rawYieldTable.Get("0btag_MTpeak","rare"    ) * rareCrossSection_rescale;
    Figure noBTagPeak_data           = rawYieldTable.Get("0btag_MTpeak","data"    );

    Figure noBTagTail_1ltop          = rawYieldTable.Get("0btag_MTtail","1ltop"   );
    Figure noBTagTail_ttbar_2l       = rawYieldTable.Get("0btag_MTtail","ttbar_2l");
    Figure noBTagTail_Wjets          = rawYieldTable.Get("0btag_MTtail","W+jets"  ) * WjetCrossSection_rescale;
    Figure noBTagTail_rare           = rawYieldTable.Get("0btag_MTtail","rare"    ) * rareCrossSection_rescale;
    Figure noBTagTail_data           = rawYieldTable.Get("0btag_MTtail","data"    );


    RW_mc    = (noBTagTail_Wjets + signalRegionTail_Wjets) / (noBTagPeak_Wjets + signalRegionPeak_Wjets);
    Rlj_mc   = (noBTagTail_1ltop + signalRegionTail_1ltop) / (noBTagPeak_1ltop + signalRegionPeak_1ltop);

    SF_0btag = (noBTagPeak_data - noBTagPeak_ttbar_2l - noBTagPeak_rare) / (noBTagPeak_1ltop + noBTagPeak_Wjets);

    SFR_all    = noBTagTail_data / ((noBTagTail_Wjets + noBTagTail_1ltop)*SF_0btag + noBTagTail_ttbar_2l + noBTagTail_rare);
    SFR_W      = (noBTagTail_data - noBTagTail_1ltop*SF_0btag - noBTagTail_ttbar_2l - noBTagTail_rare) / (noBTagTail_Wjets*SF_0btag);
    
    SFR_W_mean = Figure((SFR_all.value()+SFR_W.value())/2.0 , (SFR_all.error() + SFR_W.error())/2.0);
    if (SFR_Wjets_variation) SFR_W_mean.keepVariation(SFR_Wjets_variation);

    RW_corrected  = RW_mc  * SFR_W_mean.value();
    Rlj_corrected = Rlj_mc * SFR_W_mean.value();

    Rlj_mean = Figure((RW_corrected.value() + Rlj_corrected.value()) / 2.0, fabs(RW_corrected.value() - Rlj_corrected.value()) / 2.0);
    if (tailToPeakRatio_1lTop_variation) Rlj_mean.keepVariation(tailToPeakRatio_1lTop_variation);
}



void backgroundEstimation::FillPredictionTable()
{
    Figure N1ltop_peak = rawYieldTable.Get("signalRegion_MTpeak","1ltop"   );
    Figure Nwjets_peak = rawYieldTable.Get("signalRegion_MTpeak","W+jets"  ) * WjetCrossSection_rescale;
    Figure N2ltop_tail = rawYieldTable.Get("signalRegion_MTtail","ttbar_2l") * ttll_CR4and5_rescale * ttll_nJets_rescale * ttll_2ndlepVeto_rescale; 
    Figure Nrare_tail  = rawYieldTable.Get("signalRegion_MTtail","rare"    ) * rareCrossSection_rescale;

    // MC stat uncertainty
    if (top1lStat_variation)    N1ltop_peak.keepVariation(top1lStat_variation);
    if (WjetsStat_variation)    Nwjets_peak.keepVariation(WjetsStat_variation);     
    if (ttbar2lStat_variation)  N2ltop_tail.keepVariation(ttbar2lStat_variation);
    if (rareStat_variation)     Nrare_tail.keepVariation(rareStat_variation);       
  
    // To have cross section uncertainty in per-process prediction table
    Nwjets_peak *= Figure(1.0, 0.5);
    Nrare_tail  *= Figure(1.0, 0.5);

    // To have the (temporary and arbitrary) numbers for tt->ll systematics in the per-process prediction table
    // TODO update with the actual numbers
    N2ltop_tail *= Figure(1.0,0.10);  
    N2ltop_tail *= Figure(1.0,0.17);  
    N2ltop_tail *= Figure(1.0,0.024); 

    // Prediction
    Figure N1ltop_prediction  = N1ltop_peak  * SFpost * Rlj_mean; 
    Figure Nwjets_prediction  = Nwjets_peak  * SFpost * RW_corrected;
    Figure N2ltop_prediction  = N2ltop_tail  * SFpre;
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


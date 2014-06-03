#include "backgroundEstimation.h"

int main (int argc, char *argv[])
{
    if (argc <= 1) { WARNING_MSG << "No signal region specified" << endl; return -1; }

    string signalRegion = argv[1]; 

    backgroundEstimation(signalRegion).Run();

    return 0;
}

backgroundEstimation::backgroundEstimation(string signalRegionLabel)
{
    // ########################
    // #  Read raw MC yields  #
    // ########################

    // Defines the regions of interesets
    vector<string> regions = 
    { 
        "preveto_MTpeak",
        "preveto_MTtail",
        "0btag_MTpeak",
        "0btag_MTtail",
        /*
          "2leptons_MTpeak_"    ,
          "2leptons_MTtail_"    ,
          "1lepton+veto_MTpeak_",
          "1lepton+veto_MTtail_",
        */
        "signalRegion_MTpeak",
        "signalRegion_MTtail"
    };
    
    // Import the raw yield table from external file
    rawYieldTable = Table("rawYieldTables/"+signalRegionLabel+".tab");

    // ###################################################################
    // #  Initialize the table containing the raw mc and the prediction  #
    // ###################################################################
    
    vector<string> predictionTableColumns = { "raw_mc", "prediction" };
    vector<string> processes = 
    {
        "1ltop",
        "ttbar_2l",
        "W+jets",
        "rare",
        "total SM",
        "data",
    };
    predictionTable = Table(predictionTableColumns, processes);

    // Raw MC
    Figure N1ltop_mc  = rawYieldTable.Get("signalRegion_MTtail","1ltop"   ); 
    Figure N2ltop_mc  = rawYieldTable.Get("signalRegion_MTtail","ttbar_2l");
    Figure Nwjets_mc  = rawYieldTable.Get("signalRegion_MTtail","W+jets"  );
    Figure Nrare_mc   = rawYieldTable.Get("signalRegion_MTtail","rare"    );
    Figure NSumBkg_mc = N1ltop_mc+N2ltop_mc+Nwjets_mc+Nrare_mc;
    Figure Ndata      = rawYieldTable.Get("signalRegion_MTtail","data"    );

    predictionTable.Set("raw_mc","1ltop",    N1ltop_mc ); 
    predictionTable.Set("raw_mc","ttbar_2l", N2ltop_mc );
    predictionTable.Set("raw_mc","W+jets",   Nwjets_mc );
    predictionTable.Set("raw_mc","rare",     Nrare_mc  );
    predictionTable.Set("raw_mc","total SM", NSumBkg_mc);
    predictionTable.Set("raw_mc","data",     Ndata     );

    // ##################################################################
    // #  Initialize the table containing the systematic uncertainties  #
    // ##################################################################
    
    systematics = 
    {
        "W+jets cross section",
        "Rare cross section",
        "SFR Wjets uncertainty",
        "1-l Top tail-to-peak ratio",
        "MT peak data and MC (stat)" ,
        "tt -> 2l (stat)",
        "1l top (stat)",
        "W+jets (stat)"
    };

    vector<string> uncertainties = { "absoluteUncertainty" }; 

    systematicsUncertainties = Table(uncertainties, systematics);
    
    // Reset the systematics flags
    ResetSystematics();
}

void backgroundEstimation::Run()
{
    ComputePredictionWithSystematics();
    GetUncertaintyTable().Print();
}

void backgroundEstimation::ResetSystematics()
{
    WjetCrossSection_rescale        = 1.0;
    rareCrossSection_rescale        = 1.0;
    tailToPeakRatio_1lTop_variation = 0;
    SFR_Wjets_variation             = 0;
    WjetsStat_variation             = 0;
    top1l_variation                 = 0;
    ttbar2lStat_variation           = 0;
    MTpeakStat_variation            = 0;
}

Figure backgroundEstimation::ComputePrediction()
{
    ComputeSFpre();
    ComputeSFpost();
    ComputeRandSFR();
    FillPredictionTable();
    return predictionTable.Get("prediction","total SM");
}

void backgroundEstimation::ComputePredictionWithSystematics()
{
    // Compute prediction for nominal case
    ComputePrediction();
    PrintReport();

    // Loop over the uncertainties
    for (unsigned int s = 0 ; s < systematics.size() ; s++)
    {
        ResetSystematics();
        string systematic = systematics[s];
        float uncertainty = 0.0;

        if (systematic == "W+jets cross section") 
        {
            WjetCrossSection_rescale = 0.5; float yieldDown = ComputePrediction().value(); 
            WjetCrossSection_rescale = 1.5; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        else if (systematic == "Rare cross section") 
        {
            rareCrossSection_rescale = 0.5; float yieldDown = ComputePrediction().value();
            rareCrossSection_rescale = 1.5; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        else if (systematic == "1-l Top tail-to-peak ratio") 
        {
            tailToPeakRatio_1lTop_variation = -1.0; float yieldDown = ComputePrediction().value();
            tailToPeakRatio_1lTop_variation =  1.0; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        else if (systematic == "SFR Wjets uncertainty") 
        {
            SFR_Wjets_variation = -1.0; float yieldDown = ComputePrediction().value();
            SFR_Wjets_variation =  1.0; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        else if (systematic == "MT peak data and MC (stat)") 
        {
            MTpeakStat_variation = -1.0; float yieldDown = ComputePrediction().value();
            MTpeakStat_variation =  1.0; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        else if (systematic == "tt -> 2l (stat)")
        {
            ttbar2lStat_variation = -1.0; float yieldDown = ComputePrediction().value();
            ttbar2lStat_variation =  1.0; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        else if (systematic == "1l top (stat)")
        {
            top1l_variation = -1.0; float yieldDown = ComputePrediction().value();
            top1l_variation =  1.0; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        else if (systematic == "W+jets (stat)")
        {
            WjetsStat_variation = -1.0; float yieldDown = ComputePrediction().value();
            WjetsStat_variation =  1.0; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }

        systematicsUncertainties.Set("absoluteUncertainty",systematic,Figure(uncertainty,0.0));
    }
}

void backgroundEstimation::ComputeSFpre()
{
    Figure preveto_1ltop    = rawYieldTable.Get("preveto_MTpeak","1ltop"   );
    Figure preveto_ttbar_2l = rawYieldTable.Get("preveto_MTpeak","ttbar_2l");
    Figure preveto_Wjets    = rawYieldTable.Get("preveto_MTpeak","W+jets"  ) * WjetCrossSection_rescale;
    Figure preveto_rare     = rawYieldTable.Get("preveto_MTpeak","rare"  ) * rareCrossSection_rescale;
    Figure preveto_data     = rawYieldTable.Get("preveto_MTpeak","data"    );

    SFpre = (preveto_data - preveto_rare) / (preveto_1ltop + preveto_ttbar_2l + preveto_Wjets);
    SFpre.keepVariation(MTpeakStat_variation);

}

void backgroundEstimation::ComputeSFpost()
{
    Figure postveto_1ltop    = rawYieldTable.Get("signalRegion_MTpeak","1ltop"   );
    Figure postveto_ttbar_2l = rawYieldTable.Get("signalRegion_MTpeak","ttbar_2l");
    Figure postveto_Wjets    = rawYieldTable.Get("signalRegion_MTpeak","W+jets"  ) * WjetCrossSection_rescale;
    Figure postveto_rare     = rawYieldTable.Get("signalRegion_MTpeak","rare"  ) * rareCrossSection_rescale;
    Figure postveto_data     = rawYieldTable.Get("signalRegion_MTpeak","data"    );
    
    SFpost = (postveto_data - postveto_rare - SFpre * postveto_ttbar_2l) / (postveto_1ltop + postveto_Wjets);
    SFpre.keepVariation(MTpeakStat_variation);
}


void backgroundEstimation::ComputeRandSFR()
{
    Figure signalRegionPeak_1ltop    = rawYieldTable.Get("signalRegion_MTpeak","1ltop"   );
    Figure signalRegionPeak_ttbar_2l = rawYieldTable.Get("signalRegion_MTpeak","ttbar_2l");
    Figure signalRegionPeak_Wjets    = rawYieldTable.Get("signalRegion_MTpeak","W+jets"  ) * WjetCrossSection_rescale;
    Figure signalRegionPeak_rare     = rawYieldTable.Get("signalRegion_MTpeak","rare"  ) * rareCrossSection_rescale;
    Figure signalRegionPeak_data     = rawYieldTable.Get("signalRegion_MTpeak","data"    );

    Figure signalRegionTail_1ltop    = rawYieldTable.Get("signalRegion_MTtail","1ltop"   );
    Figure signalRegionTail_ttbar_2l = rawYieldTable.Get("signalRegion_MTtail","ttbar_2l");
    Figure signalRegionTail_Wjets    = rawYieldTable.Get("signalRegion_MTtail","W+jets"  ) * WjetCrossSection_rescale;
    Figure signalRegionTail_rare     = rawYieldTable.Get("signalRegion_MTtail","rare"  ) * rareCrossSection_rescale;
    Figure signalRegionTail_data     = rawYieldTable.Get("signalRegion_MTtail","data"    );

    Figure noBTagPeak_1ltop    = rawYieldTable.Get("0btag_MTpeak","1ltop"   );
    Figure noBTagPeak_ttbar_2l = rawYieldTable.Get("0btag_MTpeak","ttbar_2l");
    Figure noBTagPeak_Wjets    = rawYieldTable.Get("0btag_MTpeak","W+jets"  ) * WjetCrossSection_rescale;
    Figure noBTagPeak_rare     = rawYieldTable.Get("0btag_MTpeak","rare"  ) * rareCrossSection_rescale;
    Figure noBTagPeak_data     = rawYieldTable.Get("0btag_MTpeak","data"    );

    Figure noBTagTail_1ltop    = rawYieldTable.Get("0btag_MTtail","1ltop"   );
    Figure noBTagTail_ttbar_2l = rawYieldTable.Get("0btag_MTtail","ttbar_2l");
    Figure noBTagTail_Wjets    = rawYieldTable.Get("0btag_MTtail","W+jets"  ) * WjetCrossSection_rescale;
    Figure noBTagTail_rare     = rawYieldTable.Get("0btag_MTtail","rare"  ) * rareCrossSection_rescale;
    Figure noBTagTail_data     = rawYieldTable.Get("0btag_MTtail","data"    );


    RW_mc    = (noBTagTail_Wjets + signalRegionTail_Wjets) / (noBTagPeak_Wjets + signalRegionPeak_Wjets);
    Rlj_mc   = (noBTagTail_1ltop + signalRegionTail_1ltop) / (noBTagPeak_1ltop + signalRegionPeak_1ltop);

    SF_0btag = (noBTagPeak_data - noBTagPeak_ttbar_2l - noBTagPeak_rare) / (noBTagPeak_1ltop + noBTagPeak_Wjets);

    SFR_all    = noBTagTail_data / ((noBTagTail_Wjets + noBTagTail_1ltop)*SF_0btag + noBTagTail_ttbar_2l + noBTagTail_rare);
    SFR_W      = (noBTagTail_data - noBTagTail_1ltop*SF_0btag - noBTagTail_ttbar_2l - noBTagTail_rare) / (noBTagTail_Wjets*SF_0btag);
    
    SFR_W_mean = Figure((SFR_all.value()+SFR_W.value())/2.0 , (SFR_all.error() + SFR_W.error())/2.0);
    SFR_W_mean.keepVariation(SFR_Wjets_variation);

    RW_corrected  = RW_mc  * SFR_W_mean;
    Rlj_corrected = Rlj_mc * SFR_W_mean;

    Rlj_mean = Figure((RW_corrected.value() + Rlj_corrected.value()) / 2.0, fabs(RW_corrected.value() - Rlj_corrected.value()) / 2.0);
    Rlj_mean.keepVariation(tailToPeakRatio_1lTop_variation);
}

void backgroundEstimation::PrintReport()
{
    cout << "SFpre  = " << SFpre.Print(4)  << endl;
    cout << "SFpost = " << SFpost.Print(4) << endl;
    
    cout << "RW_mc    = " << RW_mc.Print(4)    << endl;
    cout << "Rlj_mc   = " << Rlj_mc.Print(4)   << endl;
        
    cout << "SFR_all    = " << SFR_all.Print(4)    << endl;
    cout << "SFR_W      = " << SFR_W.Print(4)      << endl;
    cout << "SFR_W_mean = " << SFR_W_mean.Print(4) << endl;

    cout << "RW_corrected  = " << RW_corrected.Print(4)    << endl;
    cout << "Rlj_corrected = " << Rlj_corrected.Print(4)   << endl;
    cout << "Rlj_mean      = " << Rlj_mean.Print(4) << endl;

    predictionTable.Print();
}

void backgroundEstimation::FillPredictionTable()
{
    Figure N1ltop_peak = rawYieldTable.Get("signalRegion_MTpeak","1ltop"   );
    Figure Nwjets_peak = rawYieldTable.Get("signalRegion_MTpeak","W+jets"  ) * WjetCrossSection_rescale;
    Figure N2ltop_tail = rawYieldTable.Get("signalRegion_MTtail","ttbar_2l");                           
    Figure Nrare_tail  = rawYieldTable.Get("signalRegion_MTtail","rare"    ) * rareCrossSection_rescale;
    
    N1ltop_peak.keepVariation(top1l_variation);
    Nwjets_peak.keepVariation(WjetsStat_variation);
    N2ltop_tail.keepVariation(ttbar2lStat_variation);

    // Prediction
    Figure N1ltop_prediction  = N1ltop_peak  * SFpost * Rlj_mean; 
    Figure Nwjets_prediction  = Nwjets_peak  * SFpost * RW_corrected;
    Figure N2ltop_prediction  = N2ltop_tail  * SFpre;
    Figure Nrare_prediction   = Nrare_tail;

    // Background sum
    Figure NSumBkg_prediction = N1ltop_prediction + N2ltop_prediction + Nwjets_prediction + Nrare_prediction;

    // Data
    Figure Ndata         = rawYieldTable.Get("signalRegion_MTtail","data" );

    // Fill table
    predictionTable.Set("prediction","1ltop",    N1ltop_prediction  ); 
    predictionTable.Set("prediction","ttbar_2l", N2ltop_prediction  );
    predictionTable.Set("prediction","W+jets",   Nwjets_prediction  );
    predictionTable.Set("prediction","rare",     Nrare_prediction );
    predictionTable.Set("prediction","total SM", NSumBkg_prediction );
    predictionTable.Set("prediction","data",     Ndata              );
}


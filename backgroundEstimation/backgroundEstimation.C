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
    rawYieldTable.Print();

    // ###################################################################
    // #  Initialize the table containing the raw mc and the prediction  #
    // ###################################################################
    
    vector<string> predictionTableColumns = { "raw_mc", "prediction" };
    vector<string> processes = 
    {
        "1ltop",
        "ttbar_2l",
        "W+jets",
        "others",
        "total SM",
        "data",
    };
    predictionTable = Table(predictionTableColumns, processes);

    // Raw MC
    Figure N1ltop_mc  = rawYieldTable.Get("signalRegion_MTtail","1ltop"   ); 
    Figure N2ltop_mc  = rawYieldTable.Get("signalRegion_MTtail","ttbar_2l");
    Figure Nwjets_mc  = rawYieldTable.Get("signalRegion_MTtail","W+jets"  );
    Figure Nothers_mc = rawYieldTable.Get("signalRegion_MTtail","others"  );
    Figure NSumBkg_mc = N1ltop_mc+N2ltop_mc+Nwjets_mc+Nothers_mc;
    Figure Ndata      = rawYieldTable.Get("signalRegion_MTtail","data"    );

    predictionTable.Set("raw_mc","1ltop",    N1ltop_mc ); 
    predictionTable.Set("raw_mc","ttbar_2l", N2ltop_mc );
    predictionTable.Set("raw_mc","W+jets",   Nwjets_mc );
    predictionTable.Set("raw_mc","others",   Nothers_mc);
    predictionTable.Set("raw_mc","total SM", NSumBkg_mc);
    //predictionTable.Set("raw_mc","data",     Ndata     );

    // ##################################################################
    // #  Initialize the table containing the systematic uncertainties  #
    // ##################################################################
    
    systematics = 
    {
        "W+jets cross section",
        "Rare cross section"
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
    WjetCrossSectionRescale = 1.0;
    rareCrossSectionRescale = 1.0;
}

Figure backgroundEstimation::ComputePrediction()
{
    ComputeSFpre();
    ComputeSFpost();
    ComputeRandSFR();
    FillPredictionTable();
    
    PrintReport();

    return predictionTable.Get("prediction","total SM");
}

void backgroundEstimation::ComputePredictionWithSystematics()
{
    // Compute prediction for nominal case
    ComputePrediction();

    // Loop over the uncertainties
    for (unsigned int s = 0 ; s < systematics.size() ; s++)
    {
        ResetSystematics();
        string systematic = systematics[s];
        float uncertainty = 0.0;

        if (systematic == "W+jets cross section") 
        {
            WjetCrossSectionRescale = 0.5; float yieldDown = ComputePrediction().value(); 
            WjetCrossSectionRescale = 1.5; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }
        else if (systematic == "Rare cross section") 
        {
            rareCrossSectionRescale = 0.5; float yieldDown = ComputePrediction().value();
            rareCrossSectionRescale = 1.5; float yieldUp   = ComputePrediction().value();
            uncertainty = fabs((yieldUp - yieldDown) / 2.0);
        }

        systematicsUncertainties.Set("absoluteUncertainty",systematic,Figure(uncertainty,0.0));
    }
}

void backgroundEstimation::ComputeSFpre()
{
    Figure preveto_1ltop    = rawYieldTable.Get("preveto_MTpeak","1ltop"   );
    Figure preveto_ttbar_2l = rawYieldTable.Get("preveto_MTpeak","ttbar_2l");
    Figure preveto_Wjets    = rawYieldTable.Get("preveto_MTpeak","W+jets"  ) * WjetCrossSectionRescale;
    Figure preveto_others   = rawYieldTable.Get("preveto_MTpeak","others"  ) * rareCrossSectionRescale;
    Figure preveto_data     = rawYieldTable.Get("preveto_MTpeak","data"    );

    SFpre = (preveto_data - preveto_others) / (preveto_1ltop + preveto_ttbar_2l + preveto_Wjets);

}

void backgroundEstimation::ComputeSFpost()
{
    Figure postveto_1ltop    = rawYieldTable.Get("signalRegion_MTpeak","1ltop"   );
    Figure postveto_ttbar_2l = rawYieldTable.Get("signalRegion_MTpeak","ttbar_2l");
    Figure postveto_Wjets    = rawYieldTable.Get("signalRegion_MTpeak","W+jets"  ) * WjetCrossSectionRescale;
    Figure postveto_others   = rawYieldTable.Get("signalRegion_MTpeak","others"  ) * rareCrossSectionRescale;
    Figure postveto_data     = rawYieldTable.Get("signalRegion_MTpeak","data"    );
    //Figure SFpre = Figure(SFpre.value(),0.0);
    
    SFpost = (postveto_data - postveto_others - SFpre * postveto_ttbar_2l) / (postveto_1ltop + postveto_Wjets);
}


void backgroundEstimation::ComputeRandSFR()
{
    Figure signalRegionPeak_1ltop    = rawYieldTable.Get("signalRegion_MTpeak","1ltop"   );
    Figure signalRegionPeak_ttbar_2l = rawYieldTable.Get("signalRegion_MTpeak","ttbar_2l");
    Figure signalRegionPeak_Wjets    = rawYieldTable.Get("signalRegion_MTpeak","W+jets"  ) * WjetCrossSectionRescale;
    Figure signalRegionPeak_others   = rawYieldTable.Get("signalRegion_MTpeak","others"  ) * rareCrossSectionRescale;
    Figure signalRegionPeak_data     = rawYieldTable.Get("signalRegion_MTpeak","data"    );

    Figure signalRegionTail_1ltop    = rawYieldTable.Get("signalRegion_MTtail","1ltop"   );
    Figure signalRegionTail_ttbar_2l = rawYieldTable.Get("signalRegion_MTtail","ttbar_2l");
    Figure signalRegionTail_Wjets    = rawYieldTable.Get("signalRegion_MTtail","W+jets"  ) * WjetCrossSectionRescale;
    Figure signalRegionTail_others   = rawYieldTable.Get("signalRegion_MTtail","others"  ) * rareCrossSectionRescale;
    Figure signalRegionTail_data     = rawYieldTable.Get("signalRegion_MTtail","data"    );

    Figure noBTagPeak_1ltop    = rawYieldTable.Get("0btag_MTpeak","1ltop"   );
    Figure noBTagPeak_ttbar_2l = rawYieldTable.Get("0btag_MTpeak","ttbar_2l");
    Figure noBTagPeak_Wjets    = rawYieldTable.Get("0btag_MTpeak","W+jets"  ) * WjetCrossSectionRescale;
    Figure noBTagPeak_others   = rawYieldTable.Get("0btag_MTpeak","others"  ) * rareCrossSectionRescale;
    Figure noBTagPeak_data     = rawYieldTable.Get("0btag_MTpeak","data"    );

    Figure noBTagTail_1ltop    = rawYieldTable.Get("0btag_MTtail","1ltop"   );
    Figure noBTagTail_ttbar_2l = rawYieldTable.Get("0btag_MTtail","ttbar_2l");
    Figure noBTagTail_Wjets    = rawYieldTable.Get("0btag_MTtail","W+jets"  ) * WjetCrossSectionRescale;
    Figure noBTagTail_others   = rawYieldTable.Get("0btag_MTtail","others"  ) * rareCrossSectionRescale;
    Figure noBTagTail_data     = rawYieldTable.Get("0btag_MTtail","data"    );


    RW_mc    = (noBTagTail_Wjets + signalRegionTail_Wjets) / (noBTagPeak_Wjets + signalRegionPeak_Wjets);
    Rlj_mc   = (noBTagTail_1ltop + signalRegionTail_1ltop) / (noBTagPeak_1ltop + signalRegionPeak_1ltop);
    Rlj_mean = (RW_mc + Rlj_mc) / 2.0;

    SF_0btag = (noBTagPeak_data - noBTagPeak_ttbar_2l - noBTagPeak_others) / (noBTagPeak_1ltop + noBTagPeak_Wjets);

    SFR_all    = noBTagTail_data / ((noBTagTail_Wjets + noBTagTail_1ltop)*SF_0btag + noBTagTail_ttbar_2l + noBTagTail_others);
    SFR_W      = (noBTagTail_data - noBTagTail_1ltop*SF_0btag - noBTagTail_ttbar_2l - noBTagTail_others) / (noBTagTail_Wjets*SF_0btag);
    SFR_W_mean = Figure((SFR_all.value()+SFR_W.value())/2.0 , (SFR_all.error() + SFR_W.error())/2.0);
}

void backgroundEstimation::PrintReport()
{
    cout << "SFpre  = " << SFpre.Print(4)  << endl;
    cout << "SFpost = " << SFpost.Print(4) << endl;
    
    cout << "RW_mc    = " << RW_mc.Print(4)    << endl;
    cout << "Rlj_mc   = " << Rlj_mc.Print(4)   << endl;
    cout << "Rlj_mean = " << Rlj_mean.Print(4) << endl;
    
    cout << "SFR_all    = " << SFR_all.Print(4) << endl;
    cout << "SFR_W      = " << SFR_W.Print(4) << endl;
    cout << "SFR_W_mean = " << SFR_W_mean.Print(4) << endl;

    predictionTable.Print();
}

void backgroundEstimation::FillPredictionTable()
{
    // Prediction
    Figure N1ltop_prediction  = rawYieldTable.Get("signalRegion_MTpeak","1ltop"    ) * Rlj_mean * SFR_W_mean * SFpost; 
    Figure Nwjets_prediction  = rawYieldTable.Get("signalRegion_MTpeak","W+jets"   ) * RW_mc    * SFR_W_mean * SFpost * WjetCrossSectionRescale;
    Figure N2ltop_prediction  = rawYieldTable.Get("signalRegion_MTtail","ttbar_2l" ) * SFpre;
    Figure Nothers_prediction = rawYieldTable.Get("signalRegion_MTtail","others"   ) * rareCrossSectionRescale;
    Figure NSumBkg_prediction = N1ltop_prediction + N2ltop_prediction + Nwjets_prediction + Nothers_prediction;
    Figure Ndata              = rawYieldTable.Get("signalRegion_MTtail","data" );

    predictionTable.Set("prediction","1ltop",    N1ltop_prediction  ); 
    predictionTable.Set("prediction","ttbar_2l", N2ltop_prediction  );
    predictionTable.Set("prediction","W+jets",   Nwjets_prediction  );
    predictionTable.Set("prediction","others",   Nothers_prediction );
    predictionTable.Set("prediction","total SM", NSumBkg_prediction );
    predictionTable.Set("prediction","data",     Ndata              );
}


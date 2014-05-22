#include "backgroundEstimationBox.h"

backgroundEstimationBox::backgroundEstimationBox(SonicScrewdriver* screwdriver_, string label_, string channel_)
{
    screwdriver = screwdriver_;
    label = label_;
    vector<string> regions;
    regions.push_back("preveto_MTpeak_"     +label);
    regions.push_back("preveto_MTtail_"     +label);
    regions.push_back("0btag_MTpeak_"       +label);
    regions.push_back("0btag_MTtail_"       +label);
  /*  
    regions.push_back("2leptons_MTpeak_"    +label);
    regions.push_back("2leptons_MTtail_"    +label);
    regions.push_back("1lepton+veto_MTpeak_"+label);
    regions.push_back("1lepton+veto_MTtail_"+label);
  */
    regions.push_back("signalRegion_MTpeak_"+label);
    regions.push_back("signalRegion_MTtail_"+label);

    yieldTable = TableDataMC(screwdriver,regions,channel_);

    //screwdriver_->GetProcessClassTagList  (&rawProcessesTags  );

    //string type = screwdriver->GetProcessClassType(rawProcessesTags[i]);
    ResetSystematics();
}

void backgroundEstimationBox::ResetSystematics()
{
    WjetCrossSectionRescale = 1.0;
    rareCrossSectionRescale = 1.0;
}

void backgroundEstimationBox::Compute()
{
    ComputeSFpre();
    ComputeSFpost();
    ComputeRsnSF();
    ReportNumbers();
    ComputeBkgPredictionTable();
}

void backgroundEstimationBox::ComputeWithSystematics()
{
    vector<string> systematicNames;
    vector<float>  systematicUncertainty;

    for (unsigned int systematic = NO_SYSTEMATIC ; systematic < END_SYSTEMATICS ; systematic++)
    {
        ResetSystematics();

        if (systematic == NO_SYSTEMATIC)
        { 
            Compute();
        }
        else if (systematic == WJETS_CROSS_SECTION) 
        {
            WjetCrossSectionRescale = 0.5; Compute(); float yieldDown = bkgPredTable.Get("prediction","total SM").value();
            WjetCrossSectionRescale = 1.5; Compute(); float yieldUp   = bkgPredTable.Get("prediction","total SM").value();
            
            systematicNames.push_back("W+jets cross section");
            systematicUncertainty.push_back((yieldUp - yieldDown) / 2.0);

        }
        else if (systematic == RARE_CROSS_SECTION) 
        {
            rareCrossSectionRescale = 0.5; Compute(); float yieldDown = bkgPredTable.Get("prediction","total SM").value();
            rareCrossSectionRescale = 1.5; Compute(); float yieldUp   = bkgPredTable.Get("prediction","total SM").value();
            
            systematicNames.push_back("rare cross section");
            systematicUncertainty.push_back((yieldUp - yieldDown) / 2.0);
        }
    }

    for (unsigned int i = 0 ; i < systematicNames.size() ; i++)
    {
        cout << systematicNames[i] << " : " << systematicUncertainty[i] << endl;
    }
}

/*
   Figure backgroundEstimationBox::GetBkgPrediction(){

   }
   */

void backgroundEstimationBox::ComputeSFpre()
{
    Figure preveto_1ltop    = yieldTable.Get("preveto_MTpeak_"+label,"1ltop"   );
    Figure preveto_ttbar_2l = yieldTable.Get("preveto_MTpeak_"+label,"ttbar_2l");
    Figure preveto_Wjets    = yieldTable.Get("preveto_MTpeak_"+label,"W+jets"  ) * WjetCrossSectionRescale;
    Figure preveto_others   = yieldTable.Get("preveto_MTpeak_"+label,"others"  ) * rareCrossSectionRescale;
    Figure preveto_data     = yieldTable.Get("preveto_MTpeak_"+label,"data"    );

    SFpre = (preveto_data - preveto_others) / (preveto_1ltop + preveto_ttbar_2l + preveto_Wjets);

    
}



void backgroundEstimationBox::ComputeSFpost()
{
    Figure postveto_1ltop    = yieldTable.Get("signalRegion_MTpeak_"+label,"1ltop"   );
    Figure postveto_ttbar_2l = yieldTable.Get("signalRegion_MTpeak_"+label,"ttbar_2l");
    Figure postveto_Wjets    = yieldTable.Get("signalRegion_MTpeak_"+label,"W+jets"  ) * WjetCrossSectionRescale;
    Figure postveto_others   = yieldTable.Get("signalRegion_MTpeak_"+label,"others"  ) * rareCrossSectionRescale;
    Figure postveto_data     = yieldTable.Get("signalRegion_MTpeak_"+label,"data"    );
    //Figure SFpre = Figure(SFpre.value(),0.0);
    
    SFpost = (postveto_data - postveto_others - SFpre * postveto_ttbar_2l) / (postveto_1ltop + postveto_Wjets);
}


void backgroundEstimationBox::ComputeRsnSF()
{
    Figure signalRegionPeak_1ltop    = yieldTable.Get("signalRegion_MTpeak_"+label,"1ltop"   );
    Figure signalRegionPeak_ttbar_2l = yieldTable.Get("signalRegion_MTpeak_"+label,"ttbar_2l");
    Figure signalRegionPeak_Wjets    = yieldTable.Get("signalRegion_MTpeak_"+label,"W+jets"  ) * WjetCrossSectionRescale;
    Figure signalRegionPeak_others   = yieldTable.Get("signalRegion_MTpeak_"+label,"others"  ) * rareCrossSectionRescale;
    Figure signalRegionPeak_data     = yieldTable.Get("signalRegion_MTpeak_"+label,"data"    );

    Figure signalRegionTail_1ltop    = yieldTable.Get("signalRegion_MTtail_"+label,"1ltop"   );
    Figure signalRegionTail_ttbar_2l = yieldTable.Get("signalRegion_MTtail_"+label,"ttbar_2l");
    Figure signalRegionTail_Wjets    = yieldTable.Get("signalRegion_MTtail_"+label,"W+jets"  ) * WjetCrossSectionRescale;
    Figure signalRegionTail_others   = yieldTable.Get("signalRegion_MTtail_"+label,"others"  ) * rareCrossSectionRescale;
    Figure signalRegionTail_data     = yieldTable.Get("signalRegion_MTtail_"+label,"data"    );

    Figure noBTagPeak_1ltop    = yieldTable.Get("0btag_MTpeak_"+label,"1ltop"   );
    Figure noBTagPeak_ttbar_2l = yieldTable.Get("0btag_MTpeak_"+label,"ttbar_2l");
    Figure noBTagPeak_Wjets    = yieldTable.Get("0btag_MTpeak_"+label,"W+jets"  ) * WjetCrossSectionRescale;
    Figure noBTagPeak_others   = yieldTable.Get("0btag_MTpeak_"+label,"others"  ) * rareCrossSectionRescale;
    Figure noBTagPeak_data     = yieldTable.Get("0btag_MTpeak_"+label,"data"    );

    Figure noBTagTail_1ltop    = yieldTable.Get("0btag_MTtail_"+label,"1ltop"   );
    Figure noBTagTail_ttbar_2l = yieldTable.Get("0btag_MTtail_"+label,"ttbar_2l");
    Figure noBTagTail_Wjets    = yieldTable.Get("0btag_MTtail_"+label,"W+jets"  ) * WjetCrossSectionRescale;
    Figure noBTagTail_others   = yieldTable.Get("0btag_MTtail_"+label,"others"  ) * rareCrossSectionRescale;
    Figure noBTagTail_data     = yieldTable.Get("0btag_MTtail_"+label,"data"    );


    RW_mc    = (noBTagTail_Wjets + signalRegionTail_Wjets) / (noBTagPeak_Wjets + signalRegionPeak_Wjets);
    Rlj_mc   = (noBTagTail_1ltop + signalRegionTail_1ltop) / (noBTagPeak_1ltop + signalRegionPeak_1ltop);
    Rlj_mean = (RW_mc + Rlj_mc) / 2.0;

    SF_0btag = (noBTagPeak_data - noBTagPeak_ttbar_2l - noBTagPeak_others) / (noBTagPeak_1ltop + noBTagPeak_Wjets);

    SFR_all    = noBTagTail_data / ((noBTagTail_Wjets + noBTagTail_1ltop)*SF_0btag + noBTagTail_ttbar_2l + noBTagTail_others);
    SFR_W      = (noBTagTail_data - noBTagTail_1ltop*SF_0btag - noBTagTail_ttbar_2l - noBTagTail_others) / (noBTagTail_Wjets*SF_0btag);
    SFR_W_mean = Figure((SFR_all.value()+SFR_W.value())/2.0 , (SFR_all.error() + SFR_W.value())/2.0);
}

void backgroundEstimationBox::ReportNumbers()
{
    cout << "SFpre  = " << SFpre.Print(4)  << endl;
    cout << "SFpost = " << SFpost.Print(4) << endl;
    
    cout << "RW_mc    = " << RW_mc.Print(4)    << endl;
    cout << "Rlj_mc   = " << Rlj_mc.Print(4)   << endl;
    cout << "Rlj_mean = " << Rlj_mean.Print(4) << endl;
    
    cout << "SFR_all    = " << SFR_all.Print(4) << endl;
    cout << "SFR_W      = " << SFR_W.Print(4) << endl;
    cout << "SFR_W_mean = " << SFR_W_mean.Print(4) << endl;
}

void backgroundEstimationBox::ComputeBkgPredictionTable(){

    vector<string> colTags_;
    //colTags_.push_back("process");
    colTags_.push_back("raw_mc");
    colTags_.push_back("prediction");

    vector<string> rowTags_;
    rowTags_.push_back("1ltop");
    rowTags_.push_back("ttbar_2l");
    rowTags_.push_back("W+jets");
    rowTags_.push_back("others");
    rowTags_.push_back("total SM");
    rowTags_.push_back("data");
    bkgPredTable = Table(colTags_, rowTags_);

    Figure Ndata  = yieldTable.Get("signalRegion_MTtail_"+label,"data" );

    // Raw MC
    Figure N1ltop_mc = yieldTable.Get("signalRegion_MTtail_"+label,"1ltop"    ); 
    Figure N2ltop_mc = yieldTable.Get("signalRegion_MTtail_"+label,"ttbar_2l" );
    Figure Nwjets_mc = yieldTable.Get("signalRegion_MTtail_"+label,"W+jets"   ) * WjetCrossSectionRescale;
    Figure Nothers_mc= yieldTable.Get("signalRegion_MTtail_"+label,"others"   ) * rareCrossSectionRescale;
    Figure NSumBkg_mc = N1ltop_mc+N2ltop_mc+Nwjets_mc+Nothers_mc;

    bkgPredTable.Set("raw_mc","1ltop",    N1ltop_mc  ); 
    bkgPredTable.Set("raw_mc","ttbar_2l", N2ltop_mc  );
    bkgPredTable.Set("raw_mc","W+jets",   Nwjets_mc  );
    bkgPredTable.Set("raw_mc","others",   Nothers_mc );
    bkgPredTable.Set("raw_mc","total SM", NSumBkg_mc );
    bkgPredTable.Set("raw_mc","data",     Ndata      );

    // Prediction
    Figure N1ltop_prediction  = yieldTable.Get("signalRegion_MTpeak_"+label,"1ltop"    ) * Rlj_mean * SFpost; 
    Figure N2ltop_prediction  = yieldTable.Get("signalRegion_MTtail_"+label,"ttbar_2l" ) * SFpre;
    Figure Nwjets_prediction  = yieldTable.Get("signalRegion_MTpeak_"+label,"W+jets"   ) * RW_mc * SFR_W_mean * SFpost;
    Figure Nothers_prediction = yieldTable.Get("signalRegion_MTtail_"+label,"others"   );
    Figure NSumBkg_prediction = N1ltop_prediction + N2ltop_prediction + Nwjets_prediction + Nothers_prediction;

    bkgPredTable.Set("prediction","1ltop",    N1ltop_prediction  ); 
    bkgPredTable.Set("prediction","ttbar_2l", N2ltop_prediction  );
    bkgPredTable.Set("prediction","W+jets",   Nwjets_prediction  );
    bkgPredTable.Set("prediction","others",   Nothers_prediction );
    bkgPredTable.Set("prediction","total SM", NSumBkg_prediction );
    bkgPredTable.Set("prediction","data",     Ndata              );

    bkgPredTable.PrintTable();

}


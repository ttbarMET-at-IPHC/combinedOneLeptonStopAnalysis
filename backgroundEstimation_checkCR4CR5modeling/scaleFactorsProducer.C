#include "scaleFactorsProducer.h"

int main (int argc, char *argv[])
{
    if (argc <= 1) { WARNING_MSG << "No signal region specified" << endl; return -1; }

    string signalRegion = argv[1]; 
    
    string customJetRequirement = "";
    if (argc == 3) customJetRequirement = string(argv[2]);

    scaleFactorsProducer(signalRegion,customJetRequirement).Run();

    return 0;
}

scaleFactorsProducer::scaleFactorsProducer(string signalRegionLabel_, string customJetRequirement_)
{
    signalRegionLabel = signalRegionLabel_;
    customJetRequirement = customJetRequirement_;

    // ########################
    // #  Read raw MC yields  #
    // ########################

    // Defines the regions of interesets
    vector<string> regions  = 
    { 
       "preveto_MTpeak",      
       "preveto_MTtail",      
       "signalRegion_MTpeak", 
       "signalRegion_MTtail", 
       "0btag_MTpeak",        
       "0btag_MTtail",  
       "reversedVeto_MTpeak", 
       "reversedVeto_MTtail",
       "2leptons",
       "2leptons_MTtail",
       "2leptons_MTpeak",
       "2leptons_1or2jets",   
       "2leptons_3jets",      
       "2leptons_atLeast4jets",
    };

    // Import the raw yield table from external file
    rawYieldTable = Table("rawYieldTables/"+customJetRequirement+"/"+signalRegionLabel+".tab");

    // ######################################################
    // #  Initialize the table containing the scale factors #
    // ######################################################
    
    vector<string> dummy = { "value" };
    scaleFactorTable = Table(dummy,scaleFactorsTagList);
}

void scaleFactorsProducer::Run()
{
    // Compute prediction for nominal case
    ComputePrediction();

    // Fill scale factor table
    scaleFactorTable.Set("value","SF_pre",      SFpre);
    scaleFactorTable.Set("value","SF_2l",       SF2l);
    scaleFactorTable.Set("value","SF_2ltail",   SF2ltail);
    scaleFactorTable.Set("value","SF_vetopeak", SFvetopeak);
    scaleFactorTable.Set("value","SF_vetotail", SFvetotail);
    scaleFactorTable.Print("scaleFactors/"+customJetRequirement+"/"+signalRegionLabel+".tab",4);
}

void scaleFactorsProducer::ComputePrediction()
{
    ComputeSFpre();
    ComputeSFpost();
    ComputeRandSFR();
    
    ComputeSF2l();
    ComputeSFvetopeak();
    ComputeSF2ltail();
    ComputeSFvetotail();
}

void scaleFactorsProducer::ComputeSFpre()
{
    Figure preveto_1ltop    = rawYieldTable.Get("preveto_MTpeak","1ltop"   );
    Figure preveto_ttbar_2l = rawYieldTable.Get("preveto_MTpeak","ttbar_2l");
    Figure preveto_Wjets    = rawYieldTable.Get("preveto_MTpeak","W+jets"  );
    Figure preveto_rare     = rawYieldTable.Get("preveto_MTpeak","rare"    );
    Figure preveto_data     = rawYieldTable.Get("preveto_MTpeak","data"    );

    SFpre = (preveto_data - preveto_rare) 
          / (preveto_1ltop + preveto_ttbar_2l + preveto_Wjets);
    SFpre.keepVariation(0);

}

void scaleFactorsProducer::ComputeSFpost()
{
    Figure postveto_1ltop    = rawYieldTable.Get("signalRegion_MTpeak","1ltop"   );
    Figure postveto_ttbar_2l = rawYieldTable.Get("signalRegion_MTpeak","ttbar_2l");
    Figure postveto_Wjets    = rawYieldTable.Get("signalRegion_MTpeak","W+jets"  );
    Figure postveto_rare     = rawYieldTable.Get("signalRegion_MTpeak","rare"    );
    Figure postveto_data     = rawYieldTable.Get("signalRegion_MTpeak","data"    );
    
    SFpost = (postveto_data - postveto_rare - SFpre * postveto_ttbar_2l) 
           / (postveto_1ltop + postveto_Wjets);
    SFpost.keepVariation(0);
}

void scaleFactorsProducer::ComputeSF2l()
{
    Figure yield2l_1ltop    = rawYieldTable.Get("2leptons","1ltop"    );
    Figure yield2l_ttbar_2l = rawYieldTable.Get("2leptons","ttbar_2l" );
    Figure yield2l_Wjets    = rawYieldTable.Get("2leptons","W+jets"   );
    Figure yield2l_rare     = rawYieldTable.Get("2leptons","rare"     );
    Figure yield2l_data     = rawYieldTable.Get("2leptons","data"     );

    if (yield2l_data.value() > 0) 
        SF2l = (yield2l_data - yield2l_rare) 
                 / (yield2l_1ltop + yield2l_Wjets + yield2l_ttbar_2l);

}
void scaleFactorsProducer::ComputeSF2ltail()
{
    Figure tail2l_1ltop    = rawYieldTable.Get("2leptons_MTtail","1ltop"   );
    Figure tail2l_ttbar_2l = rawYieldTable.Get("2leptons_MTtail","ttbar_2l");
    Figure tail2l_Wjets    = rawYieldTable.Get("2leptons_MTtail","W+jets"  ); 
    Figure tail2l_rare     = rawYieldTable.Get("2leptons_MTtail","rare"    );
    Figure tail2l_data     = rawYieldTable.Get("2leptons_MTtail","data"    );

    if (tail2l_data.value() > 0)
        SF2ltail = (tail2l_data - tail2l_rare - SF2l* tail2l_1ltop - SF2l* tail2l_Wjets) 
                 / (SF2l* tail2l_ttbar_2l);

}
void scaleFactorsProducer::ComputeSFvetopeak()
{

    Figure peakveto_1ltop    = rawYieldTable.Get("reversedVeto_MTpeak","1ltop"   );
    Figure peakveto_ttbar_2l = rawYieldTable.Get("reversedVeto_MTpeak","ttbar_2l");
    Figure peakveto_Wjets    = rawYieldTable.Get("reversedVeto_MTpeak","W+jets"  ); 
    Figure peakveto_rare     = rawYieldTable.Get("reversedVeto_MTpeak","rare"    );
    Figure peakveto_data     = rawYieldTable.Get("reversedVeto_MTpeak","data"    );

    if (peakveto_data.value() > 0) 
        SFvetopeak = (peakveto_data - peakveto_rare - SFpre * peakveto_ttbar_2l) 
                   / (peakveto_1ltop + peakveto_Wjets);
}

void scaleFactorsProducer::ComputeSFvetotail()
{
    Figure tailveto_1ltop    = rawYieldTable.Get("reversedVeto_MTtail","1ltop"   );
    Figure tailveto_ttbar_2l = rawYieldTable.Get("reversedVeto_MTtail","ttbar_2l");
    Figure tailveto_Wjets    = rawYieldTable.Get("reversedVeto_MTtail","W+jets"  );
    Figure tailveto_rare     = rawYieldTable.Get("reversedVeto_MTtail","rare"    );
    Figure tailveto_data     = rawYieldTable.Get("reversedVeto_MTtail","data"    );

    if (tailveto_data.value() > 0) 
    {
        // FIXME
        bool ReadSF_SFR = false;
        if (ReadSF_SFR) 
        {
            SFvetotail = (tailveto_data - tailveto_rare - SFvetopeak*SFR_lj_mean* tailveto_1ltop - SFvetopeak*SFR_W_mean* tailveto_Wjets)
                / (SFpre* tailveto_ttbar_2l);
        }
        else 
        {
            //  Using the previous analysis scale factors :
            SFvetotail = (tailveto_data - tailveto_rare - SFvetopeak*SFR_W_mean* tailveto_1ltop - SFvetopeak*SFR_W_mean* tailveto_Wjets)
                / (SFpre* tailveto_ttbar_2l);
        }
    }
}

void scaleFactorsProducer::ComputeRandSFR()
{
    Figure signalRegionPeak_1ltop    = rawYieldTable.Get("signalRegion_MTpeak","1ltop"   );
    Figure signalRegionPeak_ttbar_2l = rawYieldTable.Get("signalRegion_MTpeak","ttbar_2l");
    Figure signalRegionPeak_Wjets    = rawYieldTable.Get("signalRegion_MTpeak","W+jets"  );
    Figure signalRegionPeak_rare     = rawYieldTable.Get("signalRegion_MTpeak","rare"    );
    Figure signalRegionPeak_data     = rawYieldTable.Get("signalRegion_MTpeak","data"    );

    Figure signalRegionTail_1ltop    = rawYieldTable.Get("signalRegion_MTtail","1ltop"   );
    Figure signalRegionTail_ttbar_2l = rawYieldTable.Get("signalRegion_MTtail","ttbar_2l");
    Figure signalRegionTail_Wjets    = rawYieldTable.Get("signalRegion_MTtail","W+jets"  );
    Figure signalRegionTail_rare     = rawYieldTable.Get("signalRegion_MTtail","rare"    );
    Figure signalRegionTail_data     = rawYieldTable.Get("signalRegion_MTtail","data"    );

    Figure noBTagPeak_1ltop          = rawYieldTable.Get("0btag_MTpeak","1ltop"   );
    Figure noBTagPeak_ttbar_2l       = rawYieldTable.Get("0btag_MTpeak","ttbar_2l");
    Figure noBTagPeak_Wjets          = rawYieldTable.Get("0btag_MTpeak","W+jets"  );
    Figure noBTagPeak_rare           = rawYieldTable.Get("0btag_MTpeak","rare"    );
    Figure noBTagPeak_data           = rawYieldTable.Get("0btag_MTpeak","data"    );

    Figure noBTagTail_1ltop          = rawYieldTable.Get("0btag_MTtail","1ltop"   );
    Figure noBTagTail_ttbar_2l       = rawYieldTable.Get("0btag_MTtail","ttbar_2l");
    Figure noBTagTail_Wjets          = rawYieldTable.Get("0btag_MTtail","W+jets"  );
    Figure noBTagTail_rare           = rawYieldTable.Get("0btag_MTtail","rare"    );
    Figure noBTagTail_data           = rawYieldTable.Get("0btag_MTtail","data"    );


    RW_mc    = (noBTagTail_Wjets + signalRegionTail_Wjets) / (noBTagPeak_Wjets + signalRegionPeak_Wjets);
    Rlj_mc   = (noBTagTail_1ltop + signalRegionTail_1ltop) / (noBTagPeak_1ltop + signalRegionPeak_1ltop);

    SF_0btag = (noBTagPeak_data - noBTagPeak_ttbar_2l - noBTagPeak_rare) / (noBTagPeak_1ltop + noBTagPeak_Wjets);

    SFR_all    = noBTagTail_data / ((noBTagTail_Wjets + noBTagTail_1ltop)*SF_0btag + noBTagTail_ttbar_2l + noBTagTail_rare);
    SFR_W      = (noBTagTail_data - noBTagTail_1ltop*SF_0btag - noBTagTail_ttbar_2l - noBTagTail_rare) / (noBTagTail_Wjets*SF_0btag);

    //SFR_W_mean is take from an input file
    bool ReadSF_SFR = false; // TODO FIXME   
    if (ReadSF_SFR)
    {
        Table SFR_table;
        string srLabelInTable = signalRegionLabel;
        if (signalRegionLabel.find("BDT") != string::npos)
        {
            //trick to not search for loose or tight SR in the table
            size_t pos = signalRegionLabel.find("loose");
            if (pos != string::npos) srLabelInTable = signalRegionLabel.substr(0,pos-1);
            pos = signalRegionLabel.find("tight");
            if (pos != string::npos) srLabelInTable = signalRegionLabel.substr(0,pos-1);
            //-- end of trick
            SFR_table = Table("scaleFactors/SFR_BDT.tab");
        }
        else
        {
            SFR_table = Table("scaleFactors/SFR_CC.tab");
        }
        SFR_W_mean = SFR_table.Get("SFR_wjets",srLabelInTable);
        SFR_lj_mean = SFR_table.Get("SFR_tt1l",srLabelInTable);
    }
    else SFR_W_mean = Figure((SFR_all.value()+SFR_W.value())/2.0 , (SFR_all.error() + SFR_W.error())/2.0);

    SFR_W_mean.keepVariation(0);

    RW_corrected  = RW_mc  * SFR_W_mean.value();
    Rlj_corrected = Rlj_mc * SFR_W_mean.value();

    //Rlj_mean = Figure((RW_corrected.value() + Rlj_corrected.value()) / 2.0, fabs(RW_corrected.value() - Rlj_corrected.value()) / 2.0);
    //We now take the ration R from MC without averaging
    Rlj_mean = Rlj_corrected;
    Rlj_mean.keepVariation(0);
}


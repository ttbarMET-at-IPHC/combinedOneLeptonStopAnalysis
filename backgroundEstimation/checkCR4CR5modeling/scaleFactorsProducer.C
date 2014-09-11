#include "scaleFactorsProducer.h"

int main (int argc, char *argv[])
{
    if (argc <= 1) { WARNING_MSG << "No signal region specified" << endl; return -1; }

    string signalRegion = argv[1];

    string customBDTRequirement = "";
    if (argc == 3) customBDTRequirement = string(argv[2]);

    scaleFactorsProducer(signalRegion,customBDTRequirement).Run();

    return 0;
}

scaleFactorsProducer::scaleFactorsProducer(string signalRegionLabel_, string customBDTRequirement_)
{
    signalRegionLabel = signalRegionLabel_;
    customBDTRequirement = customBDTRequirement_;

    // ########################
    // #  Read raw MC yields  #
    // ########################

    // Import the raw yield table from external file
    rawYieldTable = Table("rawYieldTables/"+customBDTRequirement+"/"+signalRegionLabel+".tab");

    // ######################################################
    // #  Initialize the table containing the scale factors #
    // ######################################################

    vector<string> dummy = { "value" };
    scaleFactorTable = Table(dummy,scaleFactorsTagList_CR4CR5modelingChecks);
}

void scaleFactorsProducer::Run()
{
    // Compute prediction for nominal case
    ComputePrediction();

    // Fill scale factor table
    scaleFactorTable.Set("value","SF_pre",      SF_pre);
    scaleFactorTable.Set("value","SF_2l",       SF_2l);
    scaleFactorTable.Set("value","SF_2ltail",   SF_2ltail);
    scaleFactorTable.Set("value","SF_vetopeak", SF_vetopeak);
    scaleFactorTable.Set("value","SF_vetotail", SF_vetotail);

    system(("mkdir -p scaleFactors/"+customBDTRequirement).c_str());
    scaleFactorTable.Print("scaleFactors/"+customBDTRequirement+"/"+signalRegionLabel+".tab",4);
}

void scaleFactorsProducer::ComputePrediction()
{
    ComputeSFpre();

    #ifdef USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
        ImportMTTailCorrectionFromTemplateFitMethod();
    #else
        ComputeMTTailToPeakRatioCorrectionMethod();
    #endif

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

    SF_pre = (preveto_data - preveto_rare)
          / (preveto_1ltop + preveto_ttbar_2l + preveto_Wjets);
    SF_pre.keepVariation(0,"noNegativeValue");
}

void scaleFactorsProducer::ComputeSF2l()
{
    Figure yield2l_1ltop    = rawYieldTable.Get("2leptons","1ltop"    );
    Figure yield2l_ttbar_2l = rawYieldTable.Get("2leptons","ttbar_2l" );
    Figure yield2l_Wjets    = rawYieldTable.Get("2leptons","W+jets"   );
    Figure yield2l_rare     = rawYieldTable.Get("2leptons","rare"     );
    Figure yield2l_data     = rawYieldTable.Get("2leptons","data"     );

    // FIXME What should happen if the data yield is null .. ?
    if (yield2l_data.value() > 0)
        SF_2l = (yield2l_data  - yield2l_rare)
              / (yield2l_1ltop + yield2l_Wjets + yield2l_ttbar_2l);

}
void scaleFactorsProducer::ComputeSF2ltail()
{
    Figure tail2l_1ltop    = rawYieldTable.Get("2leptons_MTtail","1ltop"   );
    Figure tail2l_ttbar_2l = rawYieldTable.Get("2leptons_MTtail","ttbar_2l");
    Figure tail2l_Wjets    = rawYieldTable.Get("2leptons_MTtail","W+jets"  );
    Figure tail2l_rare     = rawYieldTable.Get("2leptons_MTtail","rare"    );
    Figure tail2l_data     = rawYieldTable.Get("2leptons_MTtail","data"    );

    // FIXME What should happen if the data yield is null .. ?
    if (tail2l_data.value() > 0)
        SF_2ltail = (tail2l_data - tail2l_rare - SF_2l* tail2l_1ltop - SF_2l* tail2l_Wjets)
                  / (SF_2l* tail2l_ttbar_2l);

}
void scaleFactorsProducer::ComputeSFvetopeak()
{

    Figure peakveto_1ltop    = rawYieldTable.Get("reversedVeto_MTpeak","1ltop"   );
    Figure peakveto_ttbar_2l = rawYieldTable.Get("reversedVeto_MTpeak","ttbar_2l");
    Figure peakveto_Wjets    = rawYieldTable.Get("reversedVeto_MTpeak","W+jets"  );
    Figure peakveto_rare     = rawYieldTable.Get("reversedVeto_MTpeak","rare"    );
    Figure peakveto_data     = rawYieldTable.Get("reversedVeto_MTpeak","data"    );

    // FIXME What should happen if the data yield is null .. ?
    if (peakveto_data.value() > 0)
        SF_vetopeak = (peakveto_data - peakveto_rare - SF_pre * peakveto_ttbar_2l)
                   / (peakveto_1ltop + peakveto_Wjets);
}

void scaleFactorsProducer::ComputeSFvetotail()
{
    Figure tailveto_1ltop    = rawYieldTable.Get("reversedVeto_MTtail","1ltop"   );
    Figure tailveto_ttbar_2l = rawYieldTable.Get("reversedVeto_MTtail","ttbar_2l");
    Figure tailveto_Wjets    = rawYieldTable.Get("reversedVeto_MTtail","W+jets"  );
    Figure tailveto_rare     = rawYieldTable.Get("reversedVeto_MTtail","rare"    );
    Figure tailveto_data     = rawYieldTable.Get("reversedVeto_MTtail","data"    );

    // FIXME What should happen if the data yield is null .. ?
    if (tailveto_data.value() > 0)
    {
    #ifdef USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
        SF_vetotail = (tailveto_data - tailveto_rare - SF_vetopeak*SF_MTtail_1ltop* tailveto_1ltop - SF_vetopeak*SF_MTtail_Wjets* tailveto_Wjets)
                   / (SF_pre* tailveto_ttbar_2l);
    #else
        SF_vetotail = (tailveto_data - tailveto_rare - SF_vetopeak*SFR_1ltop* tailveto_1ltop - SF_vetopeak*SFR_Wjets* tailveto_Wjets)
                   / (SF_pre* tailveto_ttbar_2l);
    #endif
    }

}

void scaleFactorsProducer::ComputeMTTailToPeakRatioCorrectionMethod()
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

    Figure RW_mc    = (noBTagTail_Wjets + signalRegionTail_Wjets) / (noBTagPeak_Wjets + signalRegionPeak_Wjets);
    Figure Rlj_mc   = (noBTagTail_1ltop + signalRegionTail_1ltop) / (noBTagPeak_1ltop + signalRegionPeak_1ltop);

    Figure SF_0btag = (noBTagPeak_data - noBTagPeak_ttbar_2l - noBTagPeak_rare) / (noBTagPeak_1ltop + noBTagPeak_Wjets);

    Figure SFR_all    = noBTagTail_data / ((noBTagTail_Wjets + noBTagTail_1ltop)*SF_0btag + noBTagTail_ttbar_2l + noBTagTail_rare);
    Figure SFR_Wonly  = (noBTagTail_data - noBTagTail_1ltop*SF_0btag - noBTagTail_ttbar_2l - noBTagTail_rare) / (noBTagTail_Wjets*SF_0btag);

    SFR_Wjets = Figure((SFR_all.value()+SFR_Wonly.value())/2.0 , (SFR_all.error() + SFR_Wonly.error())/2.0);
    SFR_1ltop = SFR_Wjets;

    SFR_Wjets.keepVariation(0,"noNegativeValue");
    SFR_1ltop.keepVariation(0,"noNegativeValue");
}

void scaleFactorsProducer::ImportMTTailCorrectionFromTemplateFitMethod()
{
    Table table = Table("../MTtailCorrection/results/SF_MTtail.tab");

    // Remove low/medium/highDM suffix in label for BDT's
    string signalRegionLabel_ = signalRegionLabel;
    if (findSubstring(signalRegionLabel_,"BDT"))
    {
        size_t pos;
        pos = signalRegionLabel_.find("_low");
        if (pos != string::npos) signalRegionLabel_ = signalRegionLabel_.substr(0,pos);
        pos = signalRegionLabel_.find("_medium");
        if (pos != string::npos) signalRegionLabel_ = signalRegionLabel_.substr(0,pos);
        pos = signalRegionLabel_.find("_high");
        if (pos != string::npos) signalRegionLabel_ = signalRegionLabel_.substr(0,pos);
    }
    // Remove cutsLoosenedForCR4CR5 suffix
    else
    {
        size_t pos = signalRegionLabel_.find("_cutsLoosenedForCR4CR5_");
        if (pos != string::npos) signalRegionLabel_ = signalRegionLabel.substr(0,pos);
    }

    SF_MTtail_Wjets = table.Get("SFR_Wjets",signalRegionLabel_);
    SF_MTtail_1ltop = table.Get("SFR_1ltop",signalRegionLabel_);

    SF_MTtail_Wjets.keepVariation(0,"noNegativeValue");
    SF_MTtail_1ltop.keepVariation(0,"noNegativeValue");
}

#ifndef backgroundEstimationBox_h
#define backgroundEstimationBox_h

#include "interface/tables/TableDataMC.h"
#include "interface/Figure.h"

using namespace std;
using namespace theDoctor;

enum
{
    NO_SYSTEMATIC,
    WJETS_CROSS_SECTION,
    RARE_CROSS_SECTION,
    END_SYSTEMATICS
};

//The code suppose that it exist the following regions:
// preveto_i_label, 0b_i_label, 1l_i_label, 2l_i_label, 1l1t_i_label where i = "peak" or "tail"

class backgroundEstimationBox{

    public:

        //regions might be ordered following the convention above mentioned in the enum region
        backgroundEstimationBox(SonicScrewdriver* screwdriver, string label, string channel);
        ~backgroundEstimationBox(){};

        void ResetSystematics();
        void Compute();
        void ComputeWithSystematics();

        void ComputeBkgPredictionTable();
        void ComputeBkgUncertaintyTable();
        Figure GetBkgPrediction();
        void ReportNumbers();

        Table GetBkgPredictionTable(){return bkgPredTable;};
        Table GetUncertaintyTable(){return uncertTable;};

        void ComputeSFpre();
        void ComputeSFpost();
        /*
           void ComputeRW_mc();
           void ComputeRlj_mc();
           void ComputeSF_CR1_Peak();
           void ComputeSRw();
           */
        void ComputeRsnSF();

    private:
        SonicScrewdriver* screwdriver;
        string label; // correspond to the label of the signal regions
        //vector<string> rawProcessesTags;
        TableDataMC yieldTable;

        Figure SFpre;
        Figure SFpost;
        Figure RW_mc;
        Figure Rlj_mc;
        Figure Rlj_mean;
        Figure SF_0btag;
        Figure SFR_all;
        Figure SFR_W;
        Figure SFR_W_mean;

        Table bkgPredTable;
        Table uncertTable;

        float WjetCrossSectionRescale;
        float rareCrossSectionRescale;
};

#endif

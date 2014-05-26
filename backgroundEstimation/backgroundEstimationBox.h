#ifndef backgroundEstimationBox_h
#define backgroundEstimationBox_h

#include "interface/tables/TableDataMC.h"
#include "interface/Figure.h"

using namespace std;
using namespace theDoctor;

//The code suppose that it exist the following regions:
// preveto_i_label, 0b_i_label, 1l_i_label, 2l_i_label, 1l1t_i_label where i = "peak" or "tail"

class backgroundEstimationBox{

    public:

        //regions might be ordered following the convention above mentioned in the enum region
        backgroundEstimationBox(SonicScrewdriver* screwdriver, string label, string channel);
        ~backgroundEstimationBox(){};
        void   ResetSystematics();

        void ComputeSFpre();
        void ComputeSFpost();
        void ComputeRandSFR();
        void FillPredictionTable();
        
        void PrintReport();
        
        Figure ComputePrediction();
        void   ComputePredictionWithSystematics();
        
        Table GetPredictionTable() { return predictionTable; };
        Table GetUncertaintyTable()   { return systematicsUncertainties;  };

    private:
        
        // correspond to the label of the signal regions
        string labelSR; 

        vector<string> systematics;

        // Scale factors

        Figure SFpre;
        Figure SFpost;
        Figure RW_mc;
        Figure Rlj_mc;
        Figure Rlj_mean;
        Figure SF_0btag;
        Figure SFR_all;
        Figure SFR_W;
        Figure SFR_W_mean;

        // Tables 

        TableDataMC yieldTable;
        Table       predictionTable;
        Table       systematicsUncertainties;

        // Systematic flags / value

        float WjetCrossSectionRescale;
        float rareCrossSectionRescale;
};

#endif

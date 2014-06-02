#ifndef backgroundEstimation_h
#define backgroundEstimation_h

#include "interface/tables/TableDataMC.h"
#include "interface/Figure.h"

using namespace std;
using namespace theDoctor;

class backgroundEstimation{

    public:

        //regions might be ordered following the convention above mentioned in the enum region
        backgroundEstimation(string signalRegion);
        ~backgroundEstimation(){};
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

        void Run();

    private:
        
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

        Table  rawYieldTable;
        Table  predictionTable;
        Table  systematicsUncertainties;

        // Systematic flags / value

        float WjetCrossSectionRescale;
        float rareCrossSectionRescale;
};

#endif

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
        Figure SF_0btag;
        Figure SFR_all;
        Figure SFR_W;
        Figure SFR_W_mean;
        Figure RW_corrected;
        Figure Rlj_corrected;
        Figure Rlj_mean;

        // Tables 

        Table  rawYieldTable;
        Table  predictionTable;
        Table  systematicsUncertainties;

        // Systematic flags / value

        float WjetCrossSection_rescale;
        float rareCrossSection_rescale;
        int   tailToPeakRatio_1lTop_variation;
        int   SFR_Wjets_variation;
        int   WjetsStat_variation;
        int   top1l_variation;
        int   ttbar2lStat_variation;
        int   MTpeakStat_variation;


};

#endif

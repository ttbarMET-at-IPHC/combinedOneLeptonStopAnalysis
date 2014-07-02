#ifndef backgroundEstimation_h
#define backgroundEstimation_h

#include "interface/tables/TableDataMC.h"
#include "interface/Figure.h"
#include "common.h"

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
        void ComputeSF2lpeak();
        void ComputeSFvetopeak();
        void ComputeSF2ltail();
        void ComputeSFvetotail();
        void ComputeK3K4();
        void FillPredictionTable();
        
        void PrintReport();
        
        Figure ComputePrediction();
        void   ComputePredictionWithSystematics();
        
        Table GetPredictionTable() { return predictionTable; };
        Table GetUncertaintyTable()   { return systematicsUncertainties;  };

        void Run();

    private:
       
        string signalRegionLabel;

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

        Figure SF2lpeak;
        Figure SFvetopeak;
        Figure SF2ltail;
        Figure SFvetotail;
        Figure K3;
        Figure K4;
        // Tables 

        Table  rawYieldTable;
        Table  scaleFactorTable;
        Table  predictionTable;
        Table  systematicsUncertainties;

        Table  rawYieldTable_2leptons;
        Table  scaleFactorTable_2leptons;

        // Systematic flags / value
        float ttll_CR4and5_rescale;   
        float ttll_nJets_rescale;     
        float ttll_2ndlepVeto_rescale;
        float WjetCrossSection_rescale;
        float rareCrossSection_rescale;
        int   tailToPeakRatio_1lTop_variation;
        int   SFR_Wjets_variation;
        int   MTpeakStat_variation;
        int   WjetsStat_variation;
        int   top1lStat_variation;
        int   ttbar2lStat_variation;
        int   rareStat_variation;

};

#endif

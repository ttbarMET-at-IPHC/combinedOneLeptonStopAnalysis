#ifndef backgroundEstimation_h
#define backgroundEstimation_h

#include "interface/tables/TableDataMC.h"
#include "interface/Figure.h"
#include "../common/common.h"

using namespace std;
using namespace theDoctor;

class backgroundEstimation{

    public:

        //regions might be ordered following the convention above mentioned in the enum region
        backgroundEstimation(string signalRegion);
        ~backgroundEstimation(){};

        void  ComputeSFpre();
        void  ComputeSFpost();
        void  ComputeSF0btag();
        void  ComputeSFvetopeak();
        float ComputeSecondLeptonVetoUncertainty();

        #ifdef USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
            void  ImportMTTailCorrectionFromTemplateFitMethod();
        #else
            void  ComputeMTTailToPeakRatioCorrectionMethod();
        #endif

        void   Run();
        Figure ComputePrediction();
        void   FillPredictionTable();

        void   ResetSystematics();
        void   ComputePredictionWithSystematics();


    private:

        string signalRegionLabel;

        // Scale factors

        Figure SF_pre;
        Figure SF_post;
        Figure SF_0btag;
        Figure SF_vetopeak;

        #ifdef USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
            Figure SF_MTtail_Wjets;
            Figure SF_MTtail_1ltop;
        #else
            Figure R_Wjets_mc;
            Figure R_1ltop_mc;
            Figure R_1ltop_mean;
            Figure SFR_all;
            Figure SFR_Wonly;
            Figure SFR_Wjets_mean;
            Figure SFR_1ltop_mean;
            Figure R_Wjets_corrected;
            Figure R_1ltop_corrected;
        #endif

        // Uncertainties

        float ttll_CR4CR5ModelingUncertainty;
        float ttll_nJetsModelingUncertainty;

        // Tables

            // Inputs
        Table  rawYieldTable;
        Table  secondLeptonInAcceptanceYieldTable;

            // Outputs
        Table  scaleFactorTable;
        Table  predictionTable;
        Table  systematicsUncertainties;

        // Systematic flags / value
        //
        float ttll_CR4and5_rescale;
        float ttll_nJets_rescale;
        float ttll_2ndlepVeto_rescale;
        float WjetCrossSection_rescale;
        float rareCrossSection_rescale;

        #ifdef USING_MT_TAIL_CORRECTION_FROM_TEMPLATE_FIT_METHOD
            float oneLepTopCrossSection_rescale;
            int   SF_MTtail_Wjets_variation;
            int   SF_MTtail_1ltop_variation;
        #else
            int   SFR_Wjets_variation;
            int   tailToPeakRatio_1lTop_variation;
        #endif

        int   MTpeakStat_variation;
        int   WjetsStat_variation;
        int   top1lStat_variation;
        int   ttbar2lStat_variation;
        int   rareStat_variation;

};

#endif

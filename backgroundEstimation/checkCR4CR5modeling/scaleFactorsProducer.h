#ifndef scaleFactorsProducer_h
#define scaleFactorsProducer_h

#include "interface/tables/TableDataMC.h"
#include "interface/Figure.h"
#include "../common/common.h"

using namespace std;
using namespace theDoctor;

class scaleFactorsProducer{

    public:

        //regions might be ordered following the convention above mentioned in the enum region
        scaleFactorsProducer(string signalRegion, string customBDTRequirement);
        ~scaleFactorsProducer(){};
        void   ResetSystematics();

        void ComputeSFpre();

        void ComputeMTTailToPeakRatioCorrectionMethod();
        void ImportMTTailCorrectionFromTemplateFitMethod();

        void ComputeSF2l();
        void ComputeSF2ltail();
        void ComputeSFvetopeak();
        void ComputeSFvetotail();

        void PrintReport();

        void ComputePrediction();

        void Run();

    private:

        string signalRegionLabel;
        string customBDTRequirement;

        // Scale factors

        Figure SF_pre;

        Figure SFR_Wjets;
        Figure SFR_1ltop;
        Figure SF_MTtail_Wjets;
        Figure SF_MTtail_1ltop;

        Figure SF_2l;
        Figure SF_vetopeak;
        Figure SF_2ltail;
        Figure SF_vetotail;

        // Tables

        Table  rawYieldTable;
        Table  scaleFactorTable;

};

#endif

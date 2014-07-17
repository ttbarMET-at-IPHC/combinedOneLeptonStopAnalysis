#ifndef scaleFactorsProducer_h
#define scaleFactorsProducer_h

#include "interface/tables/TableDataMC.h"
#include "interface/Figure.h"
#include "common.h"

using namespace std;
using namespace theDoctor;

class scaleFactorsProducer{

    public:

        //regions might be ordered following the convention above mentioned in the enum region
        scaleFactorsProducer(string signalRegion, string customJetRequirement);
        ~scaleFactorsProducer(){};
        void   ResetSystematics();

        void ComputeSFpre();
        void ComputeSFpost();
        void ComputeRandSFR();

        void ComputeSF2l();
        void ComputeSF2ltail();
        void ComputeSFvetopeak();
        void ComputeSFvetotail();
        
        void PrintReport();
        
        void ComputePrediction();
        
        void Run();

    private:
       
        string signalRegionLabel;
        string customJetRequirement;

        // Scale factors

        Figure SFpre;
        Figure SFpost;
        Figure RW_mc;
        Figure Rlj_mc;
        Figure SF_0btag;
        Figure SFR_all;
        Figure SFR_W;
        Figure SFR_W_mean;
        Figure SFR_lj_mean;
        Figure RW_corrected;
        Figure Rlj_corrected;
        Figure Rlj_mean;

        Figure SF2l;
        Figure SFvetopeak;
        Figure SF2ltail;
        Figure SFvetotail;


        // Tables 

        Table  rawYieldTable;
        Table  scaleFactorTable;

};

#endif

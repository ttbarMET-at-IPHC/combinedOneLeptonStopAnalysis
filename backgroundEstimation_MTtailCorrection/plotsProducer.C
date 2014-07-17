#include "common.h"

#define ttbar_sample "ttbar_powheg"
//#define ttbar_sample "ttbar_madgraph"



bool goesInPreVetoSelectionMTpeak_withSRCuts()   { return (goesInPreVetoSelectionMTpeak()   && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInPreVetoSelectionMTtail_withSRCuts()   { return (goesInPreVetoSelectionMTtail()   && SIGNAL_REGION_CUTS(enableMTCut) ); }
                                                                      
bool goesInPreselectionMTpeak_withSRCuts()       { return (goesInPreselectionMTpeak()       && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInPreselectionMTtail_withSRCuts()       { return (goesInPreselectionMTtail()       && SIGNAL_REGION_CUTS(enableMTCut) ); }
/*
bool goesInPreselectionNoMT_withSRCuts()       
{ 
    bool iok = goesInPreselection()       && SIGNAL_REGION_CUTS(disableMTCut);
    if (myEvent.MT>MT_CUT && (sampleType == "data") )  iok=0;
    return iok;
}
*/

bool goesIn0BtagControlRegionMTpeak_withSRCuts() { return (goesIn0BtagControlRegionMTpeak() && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesIn0BtagControlRegionMTtail_withSRCuts() { return (goesIn0BtagControlRegionMTtail() && SIGNAL_REGION_CUTS(enableMTCut) ); }
bool goesIn0BtagControlRegionNoMT_withSRCuts()   { return (goesIn0BtagControlRegion()       && SIGNAL_REGION_CUTS(disableMTCut) ); }

bool goesInDileptonControlRegionMTpeak_withSRCuts() { return (goesInDileptonControlRegionMTpeak() && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInDileptonControlRegionMTtail_withSRCuts() { return (goesInDileptonControlRegionMTtail() && SIGNAL_REGION_CUTS(enableMTCut) ); }
bool goesInDileptonControlRegionNoMT_withSRCuts()   { return (goesInDileptonControlRegion()       && SIGNAL_REGION_CUTS(disableMTCut)); }

bool goesInVetoControlRegionMTpeak_withSRCuts() { return (goesInVetoControlRegionMTpeak() && SIGNAL_REGION_CUTS(disableMTCut)); }
bool goesInVetoControlRegionMTtail_withSRCuts() { return (goesInVetoControlRegionMTtail() && SIGNAL_REGION_CUTS(enableMTCut) ); }
bool goesInVetoControlRegionNoMT_withSRCuts()   { return (goesInVetosControlRegion()      && SIGNAL_REGION_CUTS(disableMTCut)); }

bool goesIn0BtagControlRegionMTpeak_4j()     { return (goesIn0BtagControlRegionMTpeak()     && myEvent.nJets==4); }
bool goesIn0BtagControlRegionMTpeak_5j()     { return (goesIn0BtagControlRegionMTpeak()     && myEvent.nJets==5); }
bool goesIn0BtagControlRegionMTpeak_6j()     { return (goesIn0BtagControlRegionMTpeak()     && myEvent.nJets>=6); }
bool goesIn0BtagControlRegionMTtail_4j()     { return (goesIn0BtagControlRegionMTtail()     && myEvent.nJets==4); }
bool goesIn0BtagControlRegionMTtail_5j()     { return (goesIn0BtagControlRegionMTtail()     && myEvent.nJets==5); }
bool goesIn0BtagControlRegionMTtail_6j()     { return (goesIn0BtagControlRegionMTtail()     && myEvent.nJets>=6); }
bool goesIn0BtagControlRegionMTinverted_4j() { return (goesIn0BtagControlRegionMTinverted() && myEvent.nJets==4); }
bool goesIn0BtagControlRegionMTinverted_5j() { return (goesIn0BtagControlRegionMTinverted() && myEvent.nJets==5); }
bool goesIn0BtagControlRegionMTinverted_6j() { return (goesIn0BtagControlRegionMTinverted() && myEvent.nJets>=6); }

bool goesIn0BtagControlRegionMTtail120()     { return (goesIn0BtagControlRegion() && myEvent.MT  > 120); } 
bool goesIn0BtagControlRegionMTtail125()     { return (goesIn0BtagControlRegion() && myEvent.MT  > 125); } 
bool goesIn0BtagControlRegionMTtail130()     { return (goesIn0BtagControlRegion() && myEvent.MT  > 130); } 
bool goesIn0BtagControlRegionMTtail140()     { return (goesIn0BtagControlRegion() && myEvent.MT  > 140); } 
bool goesIn0BtagControlRegionMTtail150()     { return (goesIn0BtagControlRegion() && myEvent.MT  > 150); } 
bool goesIn0BtagControlRegionMTtail160()     { return (goesIn0BtagControlRegion() && myEvent.MT  > 160); } 
bool goesIn0BtagControlRegionMTinverted120() { return (goesIn0BtagControlRegion() && myEvent.MT <= 120); } 

bool goesInPreselNoBRequirementControlRegionMTinverted()
{
    if (myEvent.MET < 80) return false;
    if (myEvent.numberOfLepton != 1) return false;
    if (myEvent.nJets < 4)  return false; 
    if ((!myEvent.isolatedTrackVeto) || (!myEvent.tauVeto)) return false;
    if (myEvent.MT >=100)  return false; 
    return true; 
}

bool SR_BDT_T2tt_1   () { return goesInPreselection() &&   (BDToutput("T2tt_1"         )>BDTcut("T2tt_1"         )   ); }
bool SR_BDT_T2tt_2   () { return goesInPreselection() &&   (BDToutput("T2tt_2"         )>BDTcut("T2tt_2"         )   ); }
bool SR_BDT_T2tt_5   () { return goesInPreselection() &&   (BDToutput("T2tt_5_loose"   )>BDTcut("T2tt_5_loose"   )   ); }
bool SR_BDT_T2bw075_1() { return goesInPreselection() &&   (BDToutput("T2bw075_1"      )>BDTcut("T2bw075_1"      )   ); }
bool SR_BDT_T2bw075_2() { return goesInPreselection() &&   (BDToutput("T2bw075_2"      )>BDTcut("T2bw075_2"      )   ); }
bool SR_BDT_T2bw075_3() { return goesInPreselection() &&   (BDToutput("T2bw075_3"      )>BDTcut("T2bw075_3"      )   ); }
bool SR_BDT_T2bw075_5() { return goesInPreselection() &&   (BDToutput("T2bw075_5"      )>BDTcut("T2bw075_5"      )   ); }
bool SR_BDT_T2bw050_1() { return goesInPreselection() &&   (BDToutput("T2bw050_1_loose")>BDTcut("T2bw050_1_loose")   ); }
bool SR_BDT_T2bw050_3() { return goesInPreselection() &&   (BDToutput("T2bw050_3"      )>BDTcut("T2bw050_3"      )   ); }
bool SR_BDT_T2bw050_4() { return goesInPreselection() &&   (BDToutput("T2bw050_4"      )>BDTcut("T2bw050_4"      )   ); }
bool SR_BDT_T2bw050_5() { return goesInPreselection() &&   (BDToutput("T2bw050_5"      )>BDTcut("T2bw050_5"      )   ); }
bool SR_BDT_T2bw050_6() { return goesInPreselection() &&   (BDToutput("T2bw050_6"      )>BDTcut("T2bw050_6"      )   ); }
bool SR_BDT_T2bw025_1() { return goesInPreselection() &&   (BDToutput("T2bw025_1"      )>BDTcut("T2bw025_1"      )   ); }
bool SR_BDT_T2bw025_3() { return goesInPreselection() &&   (BDToutput("T2bw025_3"      )>BDTcut("T2bw025_3"      )   ); }
bool SR_BDT_T2bw025_4() { return goesInPreselection() &&   (BDToutput("T2bw025_4"      )>BDTcut("T2bw025_4"      )   ); }
bool SR_BDT_T2bw025_6() { return goesInPreselection() &&   (BDToutput("T2bw025_6"      )>BDTcut("T2bw025_6"      )   ); }

bool LowBDT_T2tt_1   () { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2tt_1"         )> -0.00  ); }
bool LowBDT_T2tt_2   () { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2tt_2"         )> 0.01   ); }
bool LowBDT_T2tt_5   () { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2tt_5_loose"   )> -0.06  ); }
bool LowBDT_T2bw075_1() { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw075_1"      )> -0.01  ); }
bool LowBDT_T2bw075_2() { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw075_2"      )> -0.04  ); }
bool LowBDT_T2bw075_3() { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw075_3"      )> -0.04  ); }
bool LowBDT_T2bw075_5() { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw075_5"      )> -0.05  ); }
bool LowBDT_T2bw050_1() { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw050_1_loose")> -0.00  ); }
bool LowBDT_T2bw050_3() { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw050_3"      )> 0.04   ); }
bool LowBDT_T2bw050_4() { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw050_4"      )> 0.01   ); }
bool LowBDT_T2bw050_5() { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw050_5"      )> 0.00   ); }
bool LowBDT_T2bw050_6() { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw050_6"      )> -0.00  ); }
bool LowBDT_T2bw025_1() { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw025_1"      )> 0.04   ); }
bool LowBDT_T2bw025_3() { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw025_3"      )> 0.05   ); }
bool LowBDT_T2bw025_4() { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw025_4"      )> 0.01   ); }
bool LowBDT_T2bw025_6() { return goesIn0BtagControlRegionMTtail() &&  (BDToutput("T2bw025_6"      )> -0.01  ); }

bool LowBDT_MTpeak_T2tt_1           () { return goesIn0BtagControlRegionMTpeak() &&  (BDToutput("T2tt_1"         )> -0.00  ); }
bool LowBDT_MTpeak_T2tt_2           () { return goesIn0BtagControlRegionMTpeak() &&  (BDToutput("T2tt_2"         )> 0.01   ); }
bool LowBDT_MTpeak_T2tt_5           () { return goesIn0BtagControlRegionMTpeak() &&  (BDToutput("T2tt_5_loose"   )> -0.06  ); }
bool LowBDT_MTpeak_T2bw075_1        () { return goesIn0BtagControlRegionMTpeak() &&  (BDToutput("T2bw075_1"      )> -0.01  ); }
bool LowBDT_MTpeak_T2bw075_2        () { return goesIn0BtagControlRegionMTpeak() &&  (BDToutput("T2bw075_2"      )> -0.04  ); }
bool LowBDT_MTpeak_T2bw075_3        () { return goesIn0BtagControlRegionMTpeak() &&  (BDToutput("T2bw075_3"      )> -0.04  ); }
bool LowBDT_MTpeak_T2bw075_5        () { return goesIn0BtagControlRegionMTpeak() &&  (BDToutput("T2bw075_5"      )> -0.05  ); }
bool LowBDT_MTpeak_T2bw050_1        () { return goesIn0BtagControlRegionMTpeak() &&  (BDToutput("T2bw050_1_loose")> -0.00  ); }
bool LowBDT_MTpeak_T2bw050_3        () { return goesIn0BtagControlRegionMTpeak() &&  (BDToutput("T2bw050_3"      )> 0.04   ); }
bool LowBDT_MTpeak_T2bw050_4        () { return goesIn0BtagControlRegionMTpeak() &&  (BDToutput("T2bw050_4"      )> 0.01   ); }
bool LowBDT_MTpeak_T2bw050_5        () { return goesIn0BtagControlRegionMTpeak() &&  (BDToutput("T2bw050_5"      )> 0.00   ); }
bool LowBDT_MTpeak_T2bw050_6        () { return goesIn0BtagControlRegionMTpeak() &&  (BDToutput("T2bw050_6"      )> -0.00  ); }
bool LowBDT_MTpeak_T2bw025_1        () { return goesIn0BtagControlRegionMTpeak() &&  (BDToutput("T2bw025_1"      )> 0.04   ); }
bool LowBDT_MTpeak_T2bw025_3        () { return goesIn0BtagControlRegionMTpeak() &&  (BDToutput("T2bw025_3"      )> 0.05   ); }
bool LowBDT_MTpeak_T2bw025_4        () { return goesIn0BtagControlRegionMTpeak() &&  (BDToutput("T2bw025_4"      )> 0.01   ); }
bool LowBDT_MTpeak_T2bw025_6        () { return goesIn0BtagControlRegionMTpeak() &&  (BDToutput("T2bw025_6"      )> -0.01  ); }

bool LowBDT_MTPeakNoBtag_T2tt_1     () { return ( goesIn0BtagControlRegionMTpeak() || goesInPreselectionMTpeak() ) &&  (BDToutput("T2tt_1"         )> -0.00  ); }
bool LowBDT_MTPeakNoBtag_T2tt_2     () { return ( goesIn0BtagControlRegionMTpeak() || goesInPreselectionMTpeak() ) &&  (BDToutput("T2tt_2"         )> 0.01   ); }
bool LowBDT_MTPeakNoBtag_T2tt_5     () { return ( goesIn0BtagControlRegionMTpeak() || goesInPreselectionMTpeak() ) &&  (BDToutput("T2tt_5_loose"   )> -0.06  ); }
bool LowBDT_MTPeakNoBtag_T2bw075_1  () { return ( goesIn0BtagControlRegionMTpeak() || goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw075_1"      )> -0.01  ); }
bool LowBDT_MTPeakNoBtag_T2bw075_2  () { return ( goesIn0BtagControlRegionMTpeak() || goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw075_2"      )> -0.04  ); }
bool LowBDT_MTPeakNoBtag_T2bw075_3  () { return ( goesIn0BtagControlRegionMTpeak() || goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw075_3"      )> -0.04  ); }
bool LowBDT_MTPeakNoBtag_T2bw075_5  () { return ( goesIn0BtagControlRegionMTpeak() || goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw075_5"      )> -0.05  ); }
bool LowBDT_MTPeakNoBtag_T2bw050_1  () { return ( goesIn0BtagControlRegionMTpeak() || goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw050_1_loose")> -0.00  ); }
bool LowBDT_MTPeakNoBtag_T2bw050_3  () { return ( goesIn0BtagControlRegionMTpeak() || goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw050_3"      )> 0.04   ); }
bool LowBDT_MTPeakNoBtag_T2bw050_4  () { return ( goesIn0BtagControlRegionMTpeak() || goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw050_4"      )> 0.01   ); }
bool LowBDT_MTPeakNoBtag_T2bw050_5  () { return ( goesIn0BtagControlRegionMTpeak() || goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw050_5"      )> 0.00   ); }
bool LowBDT_MTPeakNoBtag_T2bw050_6  () { return ( goesIn0BtagControlRegionMTpeak() || goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw050_6"      )> -0.00  ); }
bool LowBDT_MTPeakNoBtag_T2bw025_1  () { return ( goesIn0BtagControlRegionMTpeak() || goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw025_1"      )> 0.04   ); }
bool LowBDT_MTPeakNoBtag_T2bw025_3  () { return ( goesIn0BtagControlRegionMTpeak() || goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw025_3"      )> 0.05   ); }
bool LowBDT_MTPeakNoBtag_T2bw025_4  () { return ( goesIn0BtagControlRegionMTpeak() || goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw025_4"      )> 0.01   ); }
bool LowBDT_MTPeakNoBtag_T2bw025_6  () { return ( goesIn0BtagControlRegionMTpeak() || goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw025_6"      )> -0.01  ); }

bool LowBDT_MTPeakOneBtag_T2tt_1    () { return (  goesInPreselectionMTpeak() ) &&  (BDToutput("T2tt_1"         )> -0.00  ); }
bool LowBDT_MTPeakOneBtag_T2tt_2    () { return (  goesInPreselectionMTpeak() ) &&  (BDToutput("T2tt_2"         )> 0.01   ); }
bool LowBDT_MTPeakOneBtag_T2tt_5    () { return (  goesInPreselectionMTpeak() ) &&  (BDToutput("T2tt_5_loose"   )> -0.06  ); }
bool LowBDT_MTPeakOneBtag_T2bw075_1 () { return (  goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw075_1"      )> -0.01  ); }
bool LowBDT_MTPeakOneBtag_T2bw075_2 () { return (  goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw075_2"      )> -0.04  ); }
bool LowBDT_MTPeakOneBtag_T2bw075_3 () { return (  goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw075_3"      )> -0.04  ); }
bool LowBDT_MTPeakOneBtag_T2bw075_5 () { return (  goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw075_5"      )> -0.05  ); }
bool LowBDT_MTPeakOneBtag_T2bw050_1 () { return (  goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw050_1_loose")> -0.00  ); }
bool LowBDT_MTPeakOneBtag_T2bw050_3 () { return (  goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw050_3"      )> 0.04   ); }
bool LowBDT_MTPeakOneBtag_T2bw050_4 () { return (  goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw050_4"      )> 0.01   ); }
bool LowBDT_MTPeakOneBtag_T2bw050_5 () { return (  goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw050_5"      )> 0.00   ); }
bool LowBDT_MTPeakOneBtag_T2bw050_6 () { return (  goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw050_6"      )> -0.00  ); }
bool LowBDT_MTPeakOneBtag_T2bw025_1 () { return (  goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw025_1"      )> 0.04   ); }
bool LowBDT_MTPeakOneBtag_T2bw025_3 () { return (  goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw025_3"      )> 0.05   ); }
bool LowBDT_MTPeakOneBtag_T2bw025_4 () { return (  goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw025_4"      )> 0.01   ); }
bool LowBDT_MTPeakOneBtag_T2bw025_6 () { return (  goesInPreselectionMTpeak() ) &&  (BDToutput("T2bw025_6"      )> -0.01  ); }


//-- Functions for C&C  variable per variable --//
// -- MET
bool CR0btag_MTpeak_MET_200(){ return (goesIn0BtagControlRegionMTpeak() && myEvent.MET>=200); }
bool CR0btag_MTpeak_MET_250(){ return (goesIn0BtagControlRegionMTpeak() && myEvent.MET>=250); }
bool CR0btag_MTpeak_MET_300(){ return (goesIn0BtagControlRegionMTpeak() && myEvent.MET>=300); }
bool CR0btag_MTpeak_MET_320(){ return (goesIn0BtagControlRegionMTpeak() && myEvent.MET>=320); }

bool CR0btag_MTtail_MET_200(){ return (goesIn0BtagControlRegionMTtail() && myEvent.MET>=200); }
bool CR0btag_MTtail_MET_250(){ return (goesIn0BtagControlRegionMTtail() && myEvent.MET>=250); }
bool CR0btag_MTtail_MET_300(){ return (goesIn0BtagControlRegionMTtail() && myEvent.MET>=300); }
bool CR0btag_MTtail_MET_320(){ return (goesIn0BtagControlRegionMTtail() && myEvent.MET>=320); }

// -- METSig
bool CR0btag_MTpeak_METoverSqrtHT_6(){ return (goesIn0BtagControlRegionMTpeak() && myEvent.METoverSqrtHT>=6); }
bool CR0btag_MTpeak_METoverSqrtHT_7(){ return (goesIn0BtagControlRegionMTpeak() && myEvent.METoverSqrtHT>=7); }
bool CR0btag_MTpeak_METoverSqrtHT_8(){ return (goesIn0BtagControlRegionMTpeak() && myEvent.METoverSqrtHT>=8); }
bool CR0btag_MTpeak_METoverSqrtHT_9(){ return (goesIn0BtagControlRegionMTpeak() && myEvent.METoverSqrtHT>=9); }
bool CR0btag_MTpeak_METoverSqrtHT_10(){ return (goesIn0BtagControlRegionMTpeak() && myEvent.METoverSqrtHT>10); }
bool CR0btag_MTpeak_METoverSqrtHT_12(){ return (goesIn0BtagControlRegionMTpeak() && myEvent.METoverSqrtHT>=12); }

bool CR0btag_MTtail_METoverSqrtHT_6(){ return (goesIn0BtagControlRegionMTtail() && myEvent.METoverSqrtHT>=6); }
bool CR0btag_MTtail_METoverSqrtHT_7(){ return (goesIn0BtagControlRegionMTtail() && myEvent.METoverSqrtHT>=7); }
bool CR0btag_MTtail_METoverSqrtHT_8(){ return (goesIn0BtagControlRegionMTtail() && myEvent.METoverSqrtHT>=8); }
bool CR0btag_MTtail_METoverSqrtHT_9(){ return (goesIn0BtagControlRegionMTtail() && myEvent.METoverSqrtHT>=9); }
bool CR0btag_MTtail_METoverSqrtHT_10(){ return (goesIn0BtagControlRegionMTtail() && myEvent.METoverSqrtHT>=10); }
bool CR0btag_MTtail_METoverSqrtHT_12(){ return (goesIn0BtagControlRegionMTtail() && myEvent.METoverSqrtHT>=12); }

// -- BPt
bool CR0btag_MTpeak_BPt_100(){ return (goesIn0BtagControlRegionMTpeak() && myEvent.leadingBPt>=100); }
bool CR0btag_MTpeak_BPt_150(){ return (goesIn0BtagControlRegionMTpeak() && myEvent.leadingBPt>=150); }
bool CR0btag_MTpeak_BPt_180(){ return (goesIn0BtagControlRegionMTpeak() && myEvent.leadingBPt>=180); }

bool CR0btag_MTtail_BPt_100(){ return (goesIn0BtagControlRegionMTtail() && myEvent.leadingBPt>=100); }
bool CR0btag_MTtail_BPt_150(){ return (goesIn0BtagControlRegionMTtail() && myEvent.leadingBPt>=150); }
bool CR0btag_MTtail_BPt_180(){ return (goesIn0BtagControlRegionMTtail() && myEvent.leadingBPt>=180); }

// -- DPhi
bool CR0btag_MTpeak_DPhi_02(){ return (goesIn0BtagControlRegionMTpeak() && myEvent.deltaPhiMETJets<=0.2); }
bool CR0btag_MTpeak_DPhi_08(){ return (goesIn0BtagControlRegionMTpeak() && myEvent.deltaPhiMETJets<=0.8); }

bool CR0btag_MTtail_DPhi_02(){ return (goesIn0BtagControlRegionMTtail() && myEvent.deltaPhiMETJets<=0.2); }
bool CR0btag_MTtail_DPhi_08(){ return (goesIn0BtagControlRegionMTtail() && myEvent.deltaPhiMETJets<=0.8); }

// -- ISRjet
bool CR0btag_MTpeak_ISRJet(){ return (goesIn0BtagControlRegionMTpeak() && myEvent.ISRJet); }
bool CR0btag_MTtail_ISRJet(){ return (goesIn0BtagControlRegionMTtail() && myEvent.ISRJet); }

//-- Functions for C&C  Estimate of regions --//
bool CR0btag_MTpeak_MT2W_180(){ return (goesIn0BtagControlRegionMTpeak() && myEvent.MT2W>=180); }
bool CR0btag_MTpeak_MT2W_190(){ return (goesIn0BtagControlRegionMTpeak() && myEvent.MT2W>=190); }
bool CR0btag_MTpeak_MT2W_200(){ return (goesIn0BtagControlRegionMTpeak() && myEvent.MT2W>=200); }

bool CR0btag_MTtail_MT2W_180(){ return (goesIn0BtagControlRegionMTtail() && myEvent.MT2W>=180); }
bool CR0btag_MTtail_MT2W_190(){ return (goesIn0BtagControlRegionMTtail() && myEvent.MT2W>=190); }
bool CR0btag_MTtail_MT2W_200(){ return (goesIn0BtagControlRegionMTtail() && myEvent.MT2W>=200); }

bool CR0bag_MTtail_T2tt_offShellLoose() { return (goesIn0BtagControlRegionMTtail() && myEvent.MT>=120 && myEvent.METoverSqrtHT>=7 );}//125 - 8
bool CR0bag_MTtail_T2tt_offShellTight() { return (goesIn0BtagControlRegionMTtail() && myEvent.MT>=120 && myEvent.MET>=120);}//130 - 200
bool CR0bag_MTtail_T2tt_lowDeltaM    () { return (goesIn0BtagControlRegionMTtail() && myEvent.MT>=120 && myEvent.METoverSqrtHT>=7 );}//140 - 8 
bool CR0bag_MTtail_T2tt_mediumDeltaM () { return (goesIn0BtagControlRegionMTtail() && myEvent.MT>=120 && myEvent.MET>=140 );}//140 - 200
bool CR0bag_MTtail_T2tt_highDeltaM   () { return (goesIn0BtagControlRegionMTtail() && myEvent.MT>=130 && myEvent.MET>=100 );}//140 - 350 

bool CR0bag_MTpeak_T2tt_offShellLoose() { return (goesIn0BtagControlRegionMTpeak() && myEvent.METoverSqrtHT>=7 );}//8
bool CR0bag_MTpeak_T2tt_offShellTight() { return (goesIn0BtagControlRegionMTpeak() && myEvent.MET>=120);}//200
bool CR0bag_MTpeak_T2tt_lowDeltaM    () { return (goesIn0BtagControlRegionMTpeak() && myEvent.METoverSqrtHT>=7 );}//8
bool CR0bag_MTpeak_T2tt_mediumDeltaM () { return (goesIn0BtagControlRegionMTpeak() && myEvent.MET>=140 );}//200
bool CR0bag_MTpeak_T2tt_highDeltaM   () { return (goesIn0BtagControlRegionMTpeak() && myEvent.MET>=100 );}//350

bool CR0bag_MTtail_T2bw025_veryOffShell_loose() { return (goesIn0BtagControlRegionMTtail() && myEvent.MT>=120 && myEvent.METoverSqrtHT>=7 );}//9
bool CR0bag_MTtail_T2bw025_offShell_loose()     { return (goesIn0BtagControlRegionMTtail() && myEvent.MT>=120 && myEvent.METoverSqrtHT>=7 );}
bool CR0bag_MTtail_T2bw025_lowDeltaM_tight()    { return (goesIn0BtagControlRegionMTtail() && myEvent.MT>=120 && myEvent.METoverSqrtHT>=6 );}
bool CR0bag_MTtail_T2bw025_highDeltaM()         { return (goesIn0BtagControlRegionMTtail() && myEvent.MT>=120 && myEvent.METoverSqrtHT>=7 );}//10

bool CR0bag_MTpeak_T2bw025_veryOffShell_loose() { return (goesIn0BtagControlRegionMTpeak() && myEvent.METoverSqrtHT>=7 );}//9
bool CR0bag_MTpeak_T2bw025_offShell_loose()     { return (goesIn0BtagControlRegionMTpeak() && myEvent.METoverSqrtHT>=7 );}
bool CR0bag_MTpeak_T2bw025_lowDeltaM_tight()    { return (goesIn0BtagControlRegionMTpeak() && myEvent.METoverSqrtHT>=6 );}
bool CR0bag_MTpeak_T2bw025_highDeltaM()         { return (goesIn0BtagControlRegionMTpeak() && myEvent.METoverSqrtHT>=7 );}//10

bool CR0bag_MTtail_T2bw050_offShell_loose()     { return (goesIn0BtagControlRegionMTtail() && myEvent.MT>=120 && myEvent.METoverSqrtHT>=7 );}//9
bool CR0bag_MTtail_T2bw050_lowMass()            { return (goesIn0BtagControlRegionMTtail() && myEvent.MT>=120 && myEvent.METoverSqrtHT>=6 );}
bool CR0bag_MTtail_T2bw050_mediumDeltaM_loose() { return (goesIn0BtagControlRegionMTtail() && myEvent.MT>=130 && myEvent.METoverSqrtHT>=5 );}//150-7
bool CR0bag_MTtail_T2bw050_highDeltaM()         { return (goesIn0BtagControlRegionMTtail() && myEvent.MT>=120 && myEvent.METoverSqrtHT>=7 );}//160 - 10

bool CR0bag_MTpeak_T2bw050_offShell_loose()     { return (goesIn0BtagControlRegionMTpeak() && myEvent.METoverSqrtHT>=7 );}//9
bool CR0bag_MTpeak_T2bw050_lowMass()            { return (goesIn0BtagControlRegionMTpeak() && myEvent.METoverSqrtHT>=6 );}
bool CR0bag_MTpeak_T2bw050_mediumDeltaM_loose() { return (goesIn0BtagControlRegionMTpeak() && myEvent.METoverSqrtHT>=5 );}//7
bool CR0bag_MTpeak_T2bw050_highDeltaM()         { return (goesIn0BtagControlRegionMTpeak() && myEvent.METoverSqrtHT>=7 );}//10

bool CR0bag_MTtail_T2bw075_lowDeltaM_tight()    { return (goesIn0BtagControlRegionMTtail() && myEvent.MT>=120 && myEvent.METoverSqrtHT>=7 );}//120 - 12
bool CR0bag_MTtail_T2bw075_mediumDeltaM()       { return (goesIn0BtagControlRegionMTtail() && myEvent.MT>=120 && myEvent.METoverSqrtHT>=7 );}//140 - 10
bool CR0bag_MTtail_T2bw075_highDeltaM()         { return (goesIn0BtagControlRegionMTtail() && myEvent.MT>=120 && myEvent.MET>=120 );}//MT>=160 - MET>=320

bool CR0bag_MTpeak_T2bw075_lowDeltaM_tight()    { return (goesIn0BtagControlRegionMTpeak() && myEvent.METoverSqrtHT>=7);}//12
bool CR0bag_MTpeak_T2bw075_mediumDeltaM()       { return (goesIn0BtagControlRegionMTpeak() && myEvent.METoverSqrtHT>=7 );}//10
bool CR0bag_MTpeak_T2bw075_highDeltaM()         { return (goesIn0BtagControlRegionMTpeak() && myEvent.MET>=120 );}//MET>=320

//#endif
// #########################################################################
//                              Main function
// #########################################################################

int main (int argc, char *argv[])
{

    printBoxedMessage("Starting plot generation");

    // ####################
    // ##   Init tools   ##
    // ####################

    // Create a sonic Screwdriver
    SonicScrewdriver screwdriver;

    // ##########################
    // ##   Create Variables   ##
    // ##########################

    float Mlb_customBinning[3]={0,150,600};
    screwdriver.AddVariable("Mlb_small",     "Mlb",          "GeV",    2, Mlb_customBinning, &(myEvent.Mlb),    "");
    screwdriver.AddVariable("M3b",           "M3b",          "GeV",    40,0,1000,            &(myEvent.M3b),    "");
    screwdriver.AddVariable("Mlb",           "Mlb",          "GeV",    40,0,600,             &(myEvent.Mlb),    "");

    // To perform systematic study
    // (Note : JESup/down are not in skimmed babyTuples as for now)
    /*
    screwdriver.AddVariable("M3b_JESup",     "M3b_JESup",    "GeV",    40,0,1000,       &(myEvent.M3b_JESup),      "");
    screwdriver.AddVariable("M3b_JESdown",   "M3b_JESdown",  "GeV",    40,0,1000,       &(myEvent.M3b_JESdown),    "");
    screwdriver.AddVariable("Mlb_JESup",     "Mlb_JESup",    "GeV",    40,0,1000,       &(myEvent.Mlb_JESup),      "");
    screwdriver.AddVariable("Mlb_JESdown",   "Mlb_JESdown",  "GeV",    40,0,1000,       &(myEvent.Mlb_JESdown),    "");
    */

    int nJets;
    screwdriver.AddVariable("nJets", "Number of selected jets", "",  4, 4, 7,        &(nJets),                "");

    // #########################################################
    // ##   Create ProcessClasses (and associated datasets)   ##
    // #########################################################

    screwdriver.AddProcessClass("1ltop", "1l top",                             "background",kRed-7);
        #ifdef USING_TTBAR_POWHEG
            screwdriver.AddDataset("ttbar_powheg",                "1ltop",  0, 0);
        #endif
        #ifdef USING_TTBAR_MADGRAPH
            screwdriver.AddDataset("ttbar_madgraph_1l",             "1ltop",  0, 0);
        #endif
    //screwdriver.AddDataset("ttbar_madgraph_scaledown",    "1ltop",  0, 0);
    //screwdriver.AddDataset("ttbar_madgraph_scaleup",      "1ltop",  0, 0);
    //screwdriver.AddDataset("ttbar_madgraph_matchingdown", "1ltop",  0, 0);
    //screwdriver.AddDataset("ttbar_madgraph_matchingup",   "1ltop",  0, 0);
    //screwdriver.AddDataset("ttbar_madgraph_mass166-5",    "1ltop",  0, 0);
    //screwdriver.AddDataset("ttbar_madgraph_mass178-5",    "1ltop",  0, 0);

    screwdriver.AddDataset("singleTop_st",                  "1ltop",  0, 0);


    screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",kCyan-3);
        #ifdef USING_TTBAR_MADGRAPH
            screwdriver.AddDataset("ttbar_madgraph_2l",   "ttbar_2l",  0, 0);
        #endif

    screwdriver.AddProcessClass("W+jets",   "W+jets",                          "background",kOrange-2);
        screwdriver.AddDataset("W+jets",    "W+jets", 0, 0);

    screwdriver.AddProcessClass("rare",   "rare",                              "background",kMagenta-5);
        screwdriver.AddDataset("rare",   "rare", 0, 0);

    screwdriver.AddProcessClass("data",   "data",                              "data",COLORPLOT_BLACK);
        screwdriver.AddDataset("SingleElec",   "data", 0, 0);
        screwdriver.AddDataset("SingleMuon",   "data", 0, 0);

    // ##########################
    // ##    Create Regions    ##
    // ##########################


    screwdriver.AddRegion("MTpeak",              "Control Region (MT peak) - No b-tag cut", &goesInPreselNoBRequirementControlRegionMTinverted);
    screwdriver.AddRegion("presel_MTinverted",   "Preselection (MT < 100 GeV)",             &goesInPreselectionMTinverted);
    screwdriver.AddRegion("presel_MTpeak",       "Preselection (MT peak)",                  &goesInPreselectionMTpeak);
    screwdriver.AddRegion("0btag",               "0 b-tag Control Region",                  &goesIn0BtagControlRegion);
    screwdriver.AddRegion("0btag_MTpeak",        "0 b-tag (MT peak)",                       &goesIn0BtagControlRegionMTpeak);
    screwdriver.AddRegion("0btag_MTinverted",    "0 b-tag (MT < 100 GeV)",                  &goesIn0BtagControlRegionMTinverted);
    screwdriver.AddRegion("0btag_MTtail",        "0 b-tag (MT tail)",                       &goesIn0BtagControlRegionMTtail);
    screwdriver.AddRegion("0btag_MTpeak_4j",     "0 b-tag (MT peak); Njets = 4",            &goesIn0BtagControlRegionMTpeak_4j);
    screwdriver.AddRegion("0btag_MTinverted_4j", "0 b-tag (MT < 100 GeV); Njets = 4",       &goesIn0BtagControlRegionMTinverted_4j);
    screwdriver.AddRegion("0btag_MTtail_4j",     "0 b-tag (MT tail); Njets = 4",            &goesIn0BtagControlRegionMTtail_4j);
    screwdriver.AddRegion("0btag_MTpeak_5j",     "0 b-tag (MT peak); Njets >= 5",           &goesIn0BtagControlRegionMTpeak_5j);
    screwdriver.AddRegion("0btag_MTinverted_5j", "0 b-tag (MT < 100 GeV); Njets >= 5",      &goesIn0BtagControlRegionMTinverted_5j);
    screwdriver.AddRegion("0btag_MTtail_5j",     "0 b-tag (MT tail); Njets >= 5",           &goesIn0BtagControlRegionMTtail_5j);
    screwdriver.AddRegion("0btag_MTpeak_6j",     "0 b-tag (MT peak); Njets >= 6",           &goesIn0BtagControlRegionMTpeak_6j);
    screwdriver.AddRegion("0btag_MTinverted_6j", "0 b-tag (MT < 100 GeV); Njets >= 6",      &goesIn0BtagControlRegionMTinverted_6j);
    screwdriver.AddRegion("0btag_MTtail_6j",     "0 b-tag (MT tail); Njets >= 6",           &goesIn0BtagControlRegionMTtail_6j);
    screwdriver.AddRegion("0btag_MTinverted120", "0 b-tag (MT < 120 GeV)",                  &goesIn0BtagControlRegionMTinverted120);
    screwdriver.AddRegion("0btag_MTtail120",     "0 b-tag (MT > 120 GeV)",                  &goesIn0BtagControlRegionMTtail120);

    string rebinning;
    rebinning="rebin=2";
    screwdriver.AddRegion("LowBDT_T2tt_1",    "", &LowBDT_T2tt_1,    rebinning);
    screwdriver.AddRegion("LowBDT_T2tt_2",    "", &LowBDT_T2tt_2,    rebinning);
    screwdriver.AddRegion("LowBDT_T2tt_5",    "", &LowBDT_T2tt_5,    rebinning);
    screwdriver.AddRegion("LowBDT_T2bw075_1", "", &LowBDT_T2bw075_1, rebinning);
    screwdriver.AddRegion("LowBDT_T2bw075_2", "", &LowBDT_T2bw075_2, rebinning);
    screwdriver.AddRegion("LowBDT_T2bw075_3", "", &LowBDT_T2bw075_3, rebinning);
    screwdriver.AddRegion("LowBDT_T2bw075_5", "", &LowBDT_T2bw075_5, rebinning);
    screwdriver.AddRegion("LowBDT_T2bw050_1", "", &LowBDT_T2bw050_1, rebinning);
    screwdriver.AddRegion("LowBDT_T2bw050_3", "", &LowBDT_T2bw050_3, rebinning);
    screwdriver.AddRegion("LowBDT_T2bw050_4", "", &LowBDT_T2bw050_4, rebinning);
    screwdriver.AddRegion("LowBDT_T2bw050_5", "", &LowBDT_T2bw050_5, rebinning);
    screwdriver.AddRegion("LowBDT_T2bw050_6", "", &LowBDT_T2bw050_6, rebinning);
    screwdriver.AddRegion("LowBDT_T2bw025_1", "", &LowBDT_T2bw025_1, rebinning);
    screwdriver.AddRegion("LowBDT_T2bw025_3", "", &LowBDT_T2bw025_3, rebinning);
    screwdriver.AddRegion("LowBDT_T2bw025_4", "", &LowBDT_T2bw025_4, rebinning);
    screwdriver.AddRegion("LowBDT_T2bw025_6", "", &LowBDT_T2bw025_6, rebinning);

    rebinning="rebin=4";
    screwdriver.AddRegion("LowBDT_MTpeak_T2tt_1",    "",  &LowBDT_MTpeak_T2tt_1   , rebinning);
    screwdriver.AddRegion("LowBDT_MTpeak_T2tt_2",    "",  &LowBDT_MTpeak_T2tt_2   , rebinning);
    screwdriver.AddRegion("LowBDT_MTpeak_T2tt_5",    "",  &LowBDT_MTpeak_T2tt_5   , rebinning);
    screwdriver.AddRegion("LowBDT_MTpeak_T2bw075_1", "",  &LowBDT_MTpeak_T2bw075_1, rebinning);
    screwdriver.AddRegion("LowBDT_MTpeak_T2bw075_2", "",  &LowBDT_MTpeak_T2bw075_2, rebinning);
    screwdriver.AddRegion("LowBDT_MTpeak_T2bw075_3", "",  &LowBDT_MTpeak_T2bw075_3, rebinning);
    screwdriver.AddRegion("LowBDT_MTpeak_T2bw075_5", "",  &LowBDT_MTpeak_T2bw075_5, rebinning);
    screwdriver.AddRegion("LowBDT_MTpeak_T2bw050_1", "",  &LowBDT_MTpeak_T2bw050_1, rebinning);
    screwdriver.AddRegion("LowBDT_MTpeak_T2bw050_3", "",  &LowBDT_MTpeak_T2bw050_3, rebinning);
    screwdriver.AddRegion("LowBDT_MTpeak_T2bw050_4", "",  &LowBDT_MTpeak_T2bw050_4, rebinning);
    screwdriver.AddRegion("LowBDT_MTpeak_T2bw050_5", "",  &LowBDT_MTpeak_T2bw050_5, rebinning);
    screwdriver.AddRegion("LowBDT_MTpeak_T2bw050_6", "",  &LowBDT_MTpeak_T2bw050_6, rebinning);
    screwdriver.AddRegion("LowBDT_MTpeak_T2bw025_1", "",  &LowBDT_MTpeak_T2bw025_1, rebinning);
    screwdriver.AddRegion("LowBDT_MTpeak_T2bw025_3", "",  &LowBDT_MTpeak_T2bw025_3, rebinning);
    screwdriver.AddRegion("LowBDT_MTpeak_T2bw025_4", "",  &LowBDT_MTpeak_T2bw025_4, rebinning);
    screwdriver.AddRegion("LowBDT_MTpeak_T2bw025_6", "",  &LowBDT_MTpeak_T2bw025_6, rebinning);

    screwdriver.AddRegion("LowBDT_MTPeakNoBtag_T2tt_1",     "",  &LowBDT_MTPeakNoBtag_T2tt_1   , rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakNoBtag_T2tt_2",     "",  &LowBDT_MTPeakNoBtag_T2tt_2   , rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakNoBtag_T2tt_5",     "",  &LowBDT_MTPeakNoBtag_T2tt_5   , rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakNoBtag_T2bw075_1",  "",  &LowBDT_MTPeakNoBtag_T2bw075_1, rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakNoBtag_T2bw075_2",  "",  &LowBDT_MTPeakNoBtag_T2bw075_2, rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakNoBtag_T2bw075_3",  "",  &LowBDT_MTPeakNoBtag_T2bw075_3, rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakNoBtag_T2bw075_5",  "",  &LowBDT_MTPeakNoBtag_T2bw075_5, rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakNoBtag_T2bw050_1",  "",  &LowBDT_MTPeakNoBtag_T2bw050_1, rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakNoBtag_T2bw050_3",  "",  &LowBDT_MTPeakNoBtag_T2bw050_3, rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakNoBtag_T2bw050_4",  "",  &LowBDT_MTPeakNoBtag_T2bw050_4, rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakNoBtag_T2bw050_5",  "",  &LowBDT_MTPeakNoBtag_T2bw050_5, rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakNoBtag_T2bw050_6",  "",  &LowBDT_MTPeakNoBtag_T2bw050_6, rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakNoBtag_T2bw025_1",  "",  &LowBDT_MTPeakNoBtag_T2bw025_1, rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakNoBtag_T2bw025_3",  "",  &LowBDT_MTPeakNoBtag_T2bw025_3, rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakNoBtag_T2bw025_4",  "",  &LowBDT_MTPeakNoBtag_T2bw025_4, rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakNoBtag_T2bw025_6",  "",  &LowBDT_MTPeakNoBtag_T2bw025_6, rebinning);

    screwdriver.AddRegion("LowBDT_MTPeakOneBtag_T2tt_1",    "",  &LowBDT_MTPeakOneBtag_T2tt_1     , rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakOneBtag_T2tt_2",    "",  &LowBDT_MTPeakOneBtag_T2tt_2     , rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakOneBtag_T2tt_5",    "",  &LowBDT_MTPeakOneBtag_T2tt_5     , rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakOneBtag_T2bw075_1", "",  &LowBDT_MTPeakOneBtag_T2bw075_1  , rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakOneBtag_T2bw075_2", "",  &LowBDT_MTPeakOneBtag_T2bw075_2  , rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakOneBtag_T2bw075_3", "",  &LowBDT_MTPeakOneBtag_T2bw075_3  , rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakOneBtag_T2bw075_5", "",  &LowBDT_MTPeakOneBtag_T2bw075_5  , rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakOneBtag_T2bw050_1", "",  &LowBDT_MTPeakOneBtag_T2bw050_1  , rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakOneBtag_T2bw050_3", "",  &LowBDT_MTPeakOneBtag_T2bw050_3  , rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakOneBtag_T2bw050_4", "",  &LowBDT_MTPeakOneBtag_T2bw050_4  , rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakOneBtag_T2bw050_5", "",  &LowBDT_MTPeakOneBtag_T2bw050_5  , rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakOneBtag_T2bw050_6", "",  &LowBDT_MTPeakOneBtag_T2bw050_6  , rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakOneBtag_T2bw025_1", "",  &LowBDT_MTPeakOneBtag_T2bw025_1  , rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakOneBtag_T2bw025_3", "",  &LowBDT_MTPeakOneBtag_T2bw025_3  , rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakOneBtag_T2bw025_4", "",  &LowBDT_MTPeakOneBtag_T2bw025_4  , rebinning);
    screwdriver.AddRegion("LowBDT_MTPeakOneBtag_T2bw025_6", "",  &LowBDT_MTPeakOneBtag_T2bw025_6  , rebinning);

    screwdriver.AddRegion("SR_BDT_T2tt_1",    "",  &SR_BDT_T2tt_1   );
    screwdriver.AddRegion("SR_BDT_T2tt_2",    "",  &SR_BDT_T2tt_2   );
    screwdriver.AddRegion("SR_BDT_T2tt_5",    "",  &SR_BDT_T2tt_5   );
    screwdriver.AddRegion("SR_BDT_T2bw075_1", "",  &SR_BDT_T2bw075_1);
    screwdriver.AddRegion("SR_BDT_T2bw075_2", "",  &SR_BDT_T2bw075_2);
    screwdriver.AddRegion("SR_BDT_T2bw075_3", "",  &SR_BDT_T2bw075_3);
    screwdriver.AddRegion("SR_BDT_T2bw075_5", "",  &SR_BDT_T2bw075_5);
    screwdriver.AddRegion("SR_BDT_T2bw050_1", "",  &SR_BDT_T2bw050_1);
    screwdriver.AddRegion("SR_BDT_T2bw050_3", "",  &SR_BDT_T2bw050_3);
    screwdriver.AddRegion("SR_BDT_T2bw050_4", "",  &SR_BDT_T2bw050_4);
    screwdriver.AddRegion("SR_BDT_T2bw050_5", "",  &SR_BDT_T2bw050_5);
    screwdriver.AddRegion("SR_BDT_T2bw050_6", "",  &SR_BDT_T2bw050_6);
    screwdriver.AddRegion("SR_BDT_T2bw025_1", "",  &SR_BDT_T2bw025_1);
    screwdriver.AddRegion("SR_BDT_T2bw025_3", "",  &SR_BDT_T2bw025_3);
    screwdriver.AddRegion("SR_BDT_T2bw025_4", "",  &SR_BDT_T2bw025_4);
    screwdriver.AddRegion("SR_BDT_T2bw025_6", "",  &SR_BDT_T2bw025_6);

    screwdriver.AddRegion("CR0btag_MTtail_MT_120",           "",  &goesIn0BtagControlRegionMTtail120);
    screwdriver.AddRegion("CR0btag_MTtail_MT_125",           "",  &goesIn0BtagControlRegionMTtail125);
    screwdriver.AddRegion("CR0btag_MTtail_MT_130",           "",  &goesIn0BtagControlRegionMTtail130);
    screwdriver.AddRegion("CR0btag_MTtail_MT_140",           "",  &goesIn0BtagControlRegionMTtail140);
    screwdriver.AddRegion("CR0btag_MTtail_MT_150",           "",  &goesIn0BtagControlRegionMTtail150);
    screwdriver.AddRegion("CR0btag_MTtail_MT_160",           "",  &goesIn0BtagControlRegionMTtail160);

    screwdriver.AddRegion("CR0btag_MTpeak_MET_200",          "",  &CR0btag_MTpeak_MET_200           ); 
    screwdriver.AddRegion("CR0btag_MTpeak_MET_250",          "",  &CR0btag_MTpeak_MET_250           );
    screwdriver.AddRegion("CR0btag_MTpeak_MET_300",          "",  &CR0btag_MTpeak_MET_300           );
    screwdriver.AddRegion("CR0btag_MTpeak_MET_320",          "",  &CR0btag_MTpeak_MET_320           );
    screwdriver.AddRegion("CR0btag_MTtail_MET_200",          "",  &CR0btag_MTtail_MET_200           );
    screwdriver.AddRegion("CR0btag_MTtail_MET_250",          "",  &CR0btag_MTtail_MET_250           );
    screwdriver.AddRegion("CR0btag_MTtail_MET_300",          "",  &CR0btag_MTtail_MET_300           );
    screwdriver.AddRegion("CR0btag_MTtail_MET_320",          "",  &CR0btag_MTtail_MET_320           );
    screwdriver.AddRegion("CR0btag_MTpeak_METoverSqrtHT_6" , "",  &CR0btag_MTpeak_METoverSqrtHT_6   );
    screwdriver.AddRegion("CR0btag_MTpeak_METoverSqrtHT_7" , "",  &CR0btag_MTpeak_METoverSqrtHT_7   );
    screwdriver.AddRegion("CR0btag_MTpeak_METoverSqrtHT_8" , "",  &CR0btag_MTpeak_METoverSqrtHT_8   );
    screwdriver.AddRegion("CR0btag_MTpeak_METoverSqrtHT_9" , "",  &CR0btag_MTpeak_METoverSqrtHT_9   );
    screwdriver.AddRegion("CR0btag_MTpeak_METoverSqrtHT_10", "",  &CR0btag_MTpeak_METoverSqrtHT_10  );
    screwdriver.AddRegion("CR0btag_MTpeak_METoverSqrtHT_12", "",  &CR0btag_MTpeak_METoverSqrtHT_12  );
    screwdriver.AddRegion("CR0btag_MTtail_METoverSqrtHT_6" , "",  &CR0btag_MTtail_METoverSqrtHT_6   );
    screwdriver.AddRegion("CR0btag_MTtail_METoverSqrtHT_7" , "",  &CR0btag_MTtail_METoverSqrtHT_7   );
    screwdriver.AddRegion("CR0btag_MTtail_METoverSqrtHT_8" , "",  &CR0btag_MTtail_METoverSqrtHT_8   ); 
    screwdriver.AddRegion("CR0btag_MTtail_METoverSqrtHT_9" , "",  &CR0btag_MTtail_METoverSqrtHT_9   );
    screwdriver.AddRegion("CR0btag_MTtail_METoverSqrtHT_10", "",  &CR0btag_MTtail_METoverSqrtHT_10  );
    screwdriver.AddRegion("CR0btag_MTtail_METoverSqrtHT_12", "",  &CR0btag_MTtail_METoverSqrtHT_12  );
    screwdriver.AddRegion("CR0btag_MTpeak_BPt_100" ,         "",  &CR0btag_MTpeak_BPt_100           );
    screwdriver.AddRegion("CR0btag_MTpeak_BPt_150" ,         "",  &CR0btag_MTpeak_BPt_150           );
    screwdriver.AddRegion("CR0btag_MTpeak_BPt_180" ,         "",  &CR0btag_MTpeak_BPt_180           );
    screwdriver.AddRegion("CR0btag_MTtail_BPt_100" ,         "",  &CR0btag_MTtail_BPt_100           );
    screwdriver.AddRegion("CR0btag_MTtail_BPt_150" ,         "",  &CR0btag_MTtail_BPt_150           );
    screwdriver.AddRegion("CR0btag_MTtail_BPt_180" ,         "",  &CR0btag_MTtail_BPt_180           );
    screwdriver.AddRegion("CR0btag_MTpeak_DPhi_02" ,         "",  &CR0btag_MTpeak_DPhi_02           );
    screwdriver.AddRegion("CR0btag_MTpeak_DPhi_08" ,         "",  &CR0btag_MTpeak_DPhi_08           );
    screwdriver.AddRegion("CR0btag_MTtail_DPhi_02" ,         "",  &CR0btag_MTtail_DPhi_02           );
    screwdriver.AddRegion("CR0btag_MTtail_DPhi_08" ,         "",  &CR0btag_MTtail_DPhi_08           );
    screwdriver.AddRegion("CR0btag_MTpeak_ISRJet"  ,         "",  &CR0btag_MTpeak_ISRJet            );
    screwdriver.AddRegion("CR0btag_MTtail_ISRJet"  ,         "",  &CR0btag_MTtail_ISRJet            );
    screwdriver.AddRegion("CR0btag_MTpeak_MT2W_180",         "",  &CR0btag_MTpeak_MT2W_180          ); 
    screwdriver.AddRegion("CR0btag_MTpeak_MT2W_190",         "",  &CR0btag_MTpeak_MT2W_190          );
    screwdriver.AddRegion("CR0btag_MTpeak_MT2W_200",         "",  &CR0btag_MTpeak_MT2W_200          );
    screwdriver.AddRegion("CR0btag_MTtail_MT2W_180",         "",  &CR0btag_MTtail_MT2W_180          );
    screwdriver.AddRegion("CR0btag_MTtail_MT2W_190",         "",  &CR0btag_MTtail_MT2W_190          );
    //screwdriver.AddRegion("CR0btag_MTtail_MT2W_200"      ,      "CR0btag_MTtail_MT2W_200"         ,  &CR0btag_MTtail_MT2W_200         );

    rebinning="rebin=8";
    screwdriver.AddRegion("CR0bag_MTtail_T2tt_offShellLoose",         "", &CR0bag_MTtail_T2tt_offShellLoose         ,rebinning);
    screwdriver.AddRegion("CR0bag_MTtail_T2tt_offShellTight",         "", &CR0bag_MTtail_T2tt_offShellTight         ,rebinning);
    screwdriver.AddRegion("CR0bag_MTtail_T2tt_lowDeltaM",             "", &CR0bag_MTtail_T2tt_lowDeltaM             ,rebinning);
    screwdriver.AddRegion("CR0bag_MTtail_T2tt_mediumDeltaM",          "", &CR0bag_MTtail_T2tt_mediumDeltaM          ,rebinning);
    screwdriver.AddRegion("CR0bag_MTtail_T2tt_highDeltaM",            "", &CR0bag_MTtail_T2tt_highDeltaM            ,rebinning);
    screwdriver.AddRegion("CR0bag_MTpeak_T2tt_offShellLoose",         "", &CR0bag_MTpeak_T2tt_offShellLoose         ,rebinning);
    screwdriver.AddRegion("CR0bag_MTpeak_T2tt_offShellTight",         "", &CR0bag_MTpeak_T2tt_offShellTight         ,rebinning);
    screwdriver.AddRegion("CR0bag_MTpeak_T2tt_lowDeltaM",             "", &CR0bag_MTpeak_T2tt_lowDeltaM             ,rebinning);
    screwdriver.AddRegion("CR0bag_MTpeak_T2tt_mediumDeltaM",          "", &CR0bag_MTpeak_T2tt_mediumDeltaM          ,rebinning);
    screwdriver.AddRegion("CR0bag_MTpeak_T2tt_highDeltaM",            "", &CR0bag_MTpeak_T2tt_highDeltaM            ,rebinning);
    screwdriver.AddRegion("CR0bag_MTtail_T2bw025_veryOffShell_loose", "", &CR0bag_MTtail_T2bw025_veryOffShell_loose ,rebinning);
    screwdriver.AddRegion("CR0bag_MTtail_T2bw025_offShell_loose",     "", &CR0bag_MTtail_T2bw025_offShell_loose     ,rebinning);
    screwdriver.AddRegion("CR0bag_MTtail_T2bw025_lowDeltaM_tight",    "", &CR0bag_MTtail_T2bw025_lowDeltaM_tight    ,rebinning);
    screwdriver.AddRegion("CR0bag_MTtail_T2bw025_highDeltaM",         "", &CR0bag_MTtail_T2bw025_highDeltaM         ,rebinning);
    screwdriver.AddRegion("CR0bag_MTpeak_T2bw025_veryOffShell_loose", "", &CR0bag_MTpeak_T2bw025_veryOffShell_loose ,rebinning);
    screwdriver.AddRegion("CR0bag_MTpeak_T2bw025_offShell_loose",     "", &CR0bag_MTpeak_T2bw025_offShell_loose     ,rebinning);
    screwdriver.AddRegion("CR0bag_MTpeak_T2bw025_lowDeltaM_tight",    "", &CR0bag_MTpeak_T2bw025_lowDeltaM_tight    ,rebinning);
    screwdriver.AddRegion("CR0bag_MTpeak_T2bw025_highDeltaM",         "", &CR0bag_MTpeak_T2bw025_highDeltaM         ,rebinning);
    screwdriver.AddRegion("CR0bag_MTtail_T2bw050_offShell_loose",     "", &CR0bag_MTtail_T2bw050_offShell_loose     ,rebinning);
    screwdriver.AddRegion("CR0bag_MTtail_T2bw050_lowMass",            "", &CR0bag_MTtail_T2bw050_lowMass            ,rebinning);
    screwdriver.AddRegion("CR0bag_MTtail_T2bw050_mediumDeltaM_loose", "", &CR0bag_MTtail_T2bw050_mediumDeltaM_loose ,rebinning);
    screwdriver.AddRegion("CR0bag_MTtail_T2bw050_highDeltaM",         "", &CR0bag_MTtail_T2bw050_highDeltaM         ,rebinning);
    screwdriver.AddRegion("CR0bag_MTpeak_T2bw050_offShell_loose",     "", &CR0bag_MTpeak_T2bw050_offShell_loose     ,rebinning);
    screwdriver.AddRegion("CR0bag_MTpeak_T2bw050_lowMass",            "", &CR0bag_MTpeak_T2bw050_lowMass            ,rebinning);
    screwdriver.AddRegion("CR0bag_MTpeak_T2bw050_mediumDeltaM_loose", "", &CR0bag_MTpeak_T2bw050_mediumDeltaM_loose ,rebinning);
    screwdriver.AddRegion("CR0bag_MTpeak_T2bw050_highDeltaM",         "", &CR0bag_MTpeak_T2bw050_highDeltaM         ,rebinning);
    screwdriver.AddRegion("CR0bag_MTtail_T2bw075_lowDeltaM_tight",    "", &CR0bag_MTtail_T2bw075_lowDeltaM_tight    ,rebinning);
    screwdriver.AddRegion("CR0bag_MTtail_T2bw075_mediumDeltaM",       "", &CR0bag_MTtail_T2bw075_mediumDeltaM       ,rebinning);
    screwdriver.AddRegion("CR0bag_MTtail_T2bw075_highDeltaM",         "", &CR0bag_MTtail_T2bw075_highDeltaM         ,rebinning);
    screwdriver.AddRegion("CR0bag_MTpeak_T2bw075_lowDeltaM_tight",    "", &CR0bag_MTpeak_T2bw075_lowDeltaM_tight    ,rebinning);
    screwdriver.AddRegion("CR0bag_MTpeak_T2bw075_mediumDeltaM",       "", &CR0bag_MTpeak_T2bw075_mediumDeltaM       ,rebinning);
    screwdriver.AddRegion("CR0bag_MTpeak_T2bw075_highDeltaM",         "", &CR0bag_MTpeak_T2bw075_highDeltaM         );

    // ##########################                                                                                                  
    // ##   Create Channels    ##                                                                                                  
    // ##########################                                                                                                  

    screwdriver.AddChannel("singleLepton", "e/#mu-channels",   &goesInSingleLeptonChannel);                                        

    // ########################################                                                                                       
    // ##       Create histograms and        ##                                                                                       
    // ##  schedule type of plots to produce ##                                                                                       
    // ########################################                                                                                       

    // Create histograms                                                                                                           
    screwdriver.Create1DHistos();

    // Schedule plots
    screwdriver.SchedulePlots("1DDataMCComparison");

    // Config plots

    screwdriver.SetGlobalStringOption("Plot", "infoTopRight", "CMS Internal");
    screwdriver.SetGlobalStringOption("Plot", "infoTopLeft",  "#sqrt{s} = 8 TeV, L = 19.5 fb^{-1}");

    screwdriver.SetGlobalBoolOption("Plot", "exportPdf", true);
    screwdriver.SetGlobalBoolOption("Plot", "exportEps", false);
    screwdriver.SetGlobalBoolOption("Plot", "exportPng", false);

    // ########################################
    // ##       Run over the datasets        ##
    // ########################################

    vector<string> datasetsList;
    screwdriver.GetDatasetList(&datasetsList);

    cout << "   > Reading datasets... " << endl;
    cout << endl;

    for (unsigned int d = 0 ; d < datasetsList.size() ; d++)
    {
        string currentDataset = datasetsList[d];
        string currentProcessClass = screwdriver.GetProcessClass(currentDataset);

        sampleName = currentDataset;
        sampleType = screwdriver.GetProcessClassType(currentProcessClass);

        // Open the tree
        TFile f((string(FOLDER_BABYTUPLES)+currentDataset+".root").c_str());
        TTree* theTree = (TTree*) f.Get("babyTuple"); 

        intermediatePointers pointers;
        InitializeBranchesForReading(theTree,&myEvent,&pointers);

        // ########################################
        // ##        Run over the events         ##
        // ########################################

        bool ttbarDatasetToBeSplitted = false;
        if (findSubstring(currentDataset,"ttbar")
                && (currentDataset != "ttbar_madgraph_1l") 
                && (currentDataset != "ttbar_madgraph_2l"))
            ttbarDatasetToBeSplitted = true;

        int nEntries = theTree->GetEntries();
        for (int i = 0 ; i < nEntries ; i++)
        {
            if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

            // Get the i-th entry
            ReadEvent(theTree,i,&pointers,&myEvent);

            float weight = getWeight();

            // Split 1-lepton ttbar and 2-lepton ttbar
            string currentProcessClass_ = currentProcessClass;
            if (ttbarDatasetToBeSplitted && (myEvent.numberOfGenLepton == 2))
                currentProcessClass_ = "ttbar_2l";

            // FIXME Why managing overflow this way ? 
            // It's already done on the plot automatically...
            nJets = myEvent.nJets;
            if(myEvent.nJets >6 ) nJets = 6;

            screwdriver.AutoFillProcessClass(currentProcessClass_,weight);

        } 
        printProgressBar(nEntries,nEntries,currentDataset);
        cout << endl;
        f.Close();

    }

    // ###################################
    // ##   Make plots and write them   ##
    // ###################################

    cout << endl;
    cout << "   > Making plots..." << endl;
    screwdriver.MakePlots();
    cout << "   > Saving plots..." << endl;
    // TODO : if we need to redo tests for different ttbar sample, 
    // move the plots to a separate folder with the name of the alternative sample 
    screwdriver.WritePlots(string("./inputPlots/"));

    printBoxedMessage("Plot generation completed");

    // #############################
    // ##   Post-plotting tests   ##
    // #############################

    printBoxedMessage("Program done.");
    return (0);
}


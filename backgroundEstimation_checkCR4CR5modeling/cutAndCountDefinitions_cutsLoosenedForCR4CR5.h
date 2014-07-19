#ifndef _CUTANDCOUNTDEFINITIONS_CUTSLOOSENEDFORCR4CR5_H_
#define _CUTANDCOUNTDEFINITIONS_CUTSLOOSENEDFORCR4CR5_H_

bool cutAndCount_T2tt_offShellLoose_cutsLoosenedForCR4CR5_1   (bool applyMTCut) { return cutAndCount_T2tt( -1,   8, 125 * applyMTCut, -1,   -1, 999999, false, true); }
bool cutAndCount_T2tt_offShellTight_cutsLoosenedForCR4CR5_1   (bool applyMTCut) { return cutAndCount_T2tt( 300, -1, 130 * applyMTCut, -1,   -1, 999999, false, true); }
bool cutAndCount_T2tt_lowDeltaM_cutsLoosenedForCR4CR5_1       (bool applyMTCut) { return cutAndCount_T2tt( -1,   8, 140 * applyMTCut, -1,   -1, 999999, false);       }
bool cutAndCount_T2tt_mediumDeltaM_cutsLoosenedForCR4CR5_1    (bool applyMTCut) { return cutAndCount_T2tt( 200, -1, 140 * applyMTCut, 180,  -1, 999999, false);       }
bool cutAndCount_T2tt_highDeltaM_cutsLoosenedForCR4CR5_1      (bool applyMTCut) { return cutAndCount_T2tt( 270, -1, 130 * applyMTCut, 190,  -1, 999999, false);       }
bool cutAndCount_T2tt_highDeltaM_cutsLoosenedForCR4CR5_2      (bool applyMTCut) { return cutAndCount_T2tt( 350, -1, 130 * applyMTCut, 130,  -1, 999999, false);       }

bool cutAndCount_T2bw025_offShell_cutsLoosenedForCR4CR5_1     (bool applyMTCut) { return cutAndCount_T2bw(-1,    9, 120 * applyMTCut, -1,   -1,     -1, false, true); }
bool cutAndCount_T2bw025_lowMasses_cutsLoosenedForCR4CR5_1    (bool applyMTCut) { return cutAndCount_T2bw(-1,    6, 120 * applyMTCut, 200, 180,     -1, false);       }
bool cutAndCount_T2bw025_highMasses_cutsLoosenedForCR4CR5_1   (bool applyMTCut) { return cutAndCount_T2bw(220,  -1, 120 * applyMTCut, 200, 180,     -1, false);       }
bool cutAndCount_T2bw025_highMasses_cutsLoosenedForCR4CR5_2   (bool applyMTCut) { return cutAndCount_T2bw(275,  -1, 120 * applyMTCut, 140, 180,     -1, false);       }
bool cutAndCount_T2bw025_highMasses_cutsLoosenedForCR4CR5_3   (bool applyMTCut) { return cutAndCount_T2bw(275,  -1, 120 * applyMTCut, 180,  65,     -1, false);       }

bool cutAndCount_T2bw050_offShell_cutsLoosenedForCR4CR5_1     (bool applyMTCut) { return cutAndCount_T2bw(-1,    9, 120 * applyMTCut, -1,   -1,     -1, false, true); }
bool cutAndCount_T2bw050_lowMasses_cutsLoosenedForCR4CR5_1    (bool applyMTCut) { return cutAndCount_T2bw(-1,    6, 135 * applyMTCut, 180,  -1,     -1, false);       }
bool cutAndCount_T2bw050_mediumDeltaM_cutsLoosenedForCR4CR5_1 (bool applyMTCut) { return cutAndCount_T2bw(-1,    7, 140 * applyMTCut, 190, 100,     -1, false);       }
bool cutAndCount_T2bw050_highDeltaM_cutsLoosenedForCR4CR5_1   (bool applyMTCut) { return cutAndCount_T2bw(245,  -1, 120 * applyMTCut, 200, 100,     -1, false);       }
bool cutAndCount_T2bw050_highDeltaM_cutsLoosenedForCR4CR5_2   (bool applyMTCut) { return cutAndCount_T2bw(300,  -1, 120 * applyMTCut, 150, 100,     -1, false);       }
bool cutAndCount_T2bw050_highDeltaM_cutsLoosenedForCR4CR5_3   (bool applyMTCut) { return cutAndCount_T2bw(275,  -1, 120 * applyMTCut, 180,  65,     -1, false);       }

bool cutAndCount_T2bw075_lowDeltaM_cutsLoosenedForCR4CR5_1    (bool applyMTCut) { return cutAndCount_T2bw(-1,   12, 120 * applyMTCut, -1,   -1,     -1, false, true); }
bool cutAndCount_T2bw075_mediumDeltaM_cutsLoosenedForCR4CR5_1 (bool applyMTCut) { return cutAndCount_T2bw(-1,   10, 130 * applyMTCut, 180,  -1,     -1, false);       }
bool cutAndCount_T2bw075_highDeltaM_cutsLoosenedForCR4CR5_1   (bool applyMTCut) { return cutAndCount_T2bw(265,  -1, 140 * applyMTCut, 200,  -1,     -1, false);       }
bool cutAndCount_T2bw075_highDeltaM_cutsLoosenedForCR4CR5_2   (bool applyMTCut) { return cutAndCount_T2bw(300,  -1, 140 * applyMTCut, 180,  -1,     -1, false);       }


// Merp, that's dirty. FIXME.
map<string,int> cutAndCount_numbersOfAlternativeLoosenedRegions;

void init_cutsLoosenedForCR4CR5_numbersOfRegions()
{
    cutAndCount_numbersOfAlternativeLoosenedRegions["preselection"                    ] = 1;
    cutAndCount_numbersOfAlternativeLoosenedRegions["cutAndCount_T2tt_offShellLoose"  ] = 1;
    cutAndCount_numbersOfAlternativeLoosenedRegions["cutAndCount_T2tt_offShellTight"  ] = 1;
    cutAndCount_numbersOfAlternativeLoosenedRegions["cutAndCount_T2tt_lowDeltaM"      ] = 1;
    cutAndCount_numbersOfAlternativeLoosenedRegions["cutAndCount_T2tt_mediumDeltaM"   ] = 1;
    cutAndCount_numbersOfAlternativeLoosenedRegions["cutAndCount_T2tt_highDeltaM"     ] = 2;
    cutAndCount_numbersOfAlternativeLoosenedRegions["cutAndCount_T2bw025_offShell"    ] = 1;
    cutAndCount_numbersOfAlternativeLoosenedRegions["cutAndCount_T2bw025_lowMasses"   ] = 1;
    cutAndCount_numbersOfAlternativeLoosenedRegions["cutAndCount_T2bw025_highMasses"  ] = 3;
    cutAndCount_numbersOfAlternativeLoosenedRegions["cutAndCount_T2bw050_offShell"    ] = 1;
    cutAndCount_numbersOfAlternativeLoosenedRegions["cutAndCount_T2bw050_lowMasses"   ] = 1;
    cutAndCount_numbersOfAlternativeLoosenedRegions["cutAndCount_T2bw050_mediumDeltaM"] = 1;
    cutAndCount_numbersOfAlternativeLoosenedRegions["cutAndCount_T2bw050_highDeltaM"  ] = 3;
    cutAndCount_numbersOfAlternativeLoosenedRegions["cutAndCount_T2bw075_lowDeltaM"   ] = 1;
    cutAndCount_numbersOfAlternativeLoosenedRegions["cutAndCount_T2bw075_mediumDeltaM"] = 1;
    cutAndCount_numbersOfAlternativeLoosenedRegions["cutAndCount_T2bw075_highDeltaM"  ] = 2;
}

#endif

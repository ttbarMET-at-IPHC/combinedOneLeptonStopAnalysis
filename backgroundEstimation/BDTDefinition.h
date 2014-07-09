

int signalregion(TString decaymode, int stopmass, int lspmass)
{

  int signalregion = 0;

  if (decaymode == "T2bw075") {


	  if (lspmass > stopmass - 200)
	    signalregion = 1;
	  if (lspmass <= stopmass - 200 && lspmass > stopmass - 325)
	    signalregion = 2;
	  if (lspmass <= stopmass - 325 && lspmass > stopmass - 475)
	    signalregion = 3;
	  if (lspmass <= stopmass - 475)
	    signalregion = 5;

  }



  else if (decaymode == "T2bw050") {

	  if (lspmass > stopmass - 250)
	    signalregion = 1;
	  if (lspmass <= stopmass - 250 && lspmass > stopmass - 400)
	    signalregion = 3;
	  if (lspmass <= stopmass - 400 && lspmass > stopmass - 450)
	    signalregion = 4;
	  if (lspmass <= stopmass - 450 && lspmass > stopmass - 625)
	    signalregion = 5;
	  if (lspmass <= stopmass - 625)
	    signalregion = 6;

  }	

  else if (decaymode == "T2bw025") {


	  if (lspmass <= 125)
	    {
	      if (lspmass > stopmass - 275)
		signalregion = 1;

	    }
	  if (lspmass > 125)
	    {
	      if (lspmass > stopmass - 225)
		signalregion = 1;
	      if (lspmass <= stopmass - 225 && lspmass > stopmass - 275)
		signalregion = 3;

	    }

	  if (lspmass <= stopmass - 275 && lspmass > stopmass - 425)
	    signalregion = 3;
	  if (lspmass <= stopmass - 425 && lspmass > stopmass - 600)
	    signalregion = 4;
	  if (lspmass <= stopmass - 600)
	    signalregion = 6;

  }


  else if (decaymode == "T2tt") {


	  if (lspmass > stopmass - 225)
	    signalregion = 1;
	  if (lspmass <= stopmass - 225 && lspmass > stopmass - 275)
	    signalregion = 2;
	  if (lspmass <= stopmass - 275)
	    signalregion = 5;

  } 

  return signalregion;

}



// This is conflicting with the previous function
//TString signalregion(TString decaymode, int stopmass, int lspmass)
TString signalregionName(TString decaymode, int stopmass, int lspmass)
{

  TString SR = "nan";


   if (decaymode == "T2bw075") {


          if (lspmass > stopmass - 200)
            SR = "T2bw075_1";
          if (lspmass <= stopmass - 200 && lspmass > stopmass - 325)
            SR = "T2bw075_2";
          if (lspmass <= stopmass - 325 && lspmass > stopmass - 475)
            SR = "T2bw075_3";
          if (lspmass <= stopmass - 475)
            SR = "T2bw075_5";

  }



  if (decaymode == "T2bw050") {


          if (lspmass > stopmass - 250) {
		SR = "T2bw050_1_tight" ; 
		    if (lspmass < 150) SR = "T2bw050_1_loose";
		}

          if (lspmass <= stopmass - 250 && lspmass > stopmass - 400)
            SR = "T2bw050_3";
          if (lspmass <= stopmass - 400 && lspmass > stopmass - 450)
            SR = "T2bw050_4";
          if (lspmass <= stopmass - 450 && lspmass > stopmass - 625)
            SR = "T2bw050_5";
          if (lspmass <= stopmass - 625)
            SR = "T2bw050_6";


  }	

  if (decaymode == "T2bw025") {


          if (lspmass <= 125)
            {
              if (lspmass > stopmass - 275)
                SR = "T2bw025_1";

            }
          if (lspmass > 125)
            {
              if (lspmass > stopmass - 225)
                SR = "T2bw025_1";
              if (lspmass <= stopmass - 225 && lspmass > stopmass - 275)
                SR = "T2bw025_3";

            }

          if (lspmass <= stopmass - 275 && lspmass > stopmass - 425)
            SR = "T2bw025_3";
          if (lspmass <= stopmass - 425 && lspmass > stopmass - 600)
            SR = "T2bw025_4";
          if (lspmass <= stopmass - 600)
            SR = "T2bw025_6";



  }


  if (decaymode == "T2tt") {


	  if (lspmass > stopmass - 225)
	    SR = "T2tt_1";
	  if (lspmass <= stopmass - 225 && lspmass > stopmass - 275)
	    SR = "T2tt_2";
		  if (lspmass <= stopmass - 275) {

			SR="T2tt_5_loose"; if (lspmass <= stopmass - 400) SR = "T2tt_5_tight";
		   }


 	}

  return SR;
}






double signalcut(TString decaymode, int stopmass, int lspmass)
{

  double cutvalue = 0.;


   if (decaymode == "T2bw075") {


          if (lspmass > stopmass - 200)
            cutvalue = 0.225;
          if (lspmass <= stopmass - 200 && lspmass > stopmass - 325)
            cutvalue = 0.275;
          if (lspmass <= stopmass - 325 && lspmass > stopmass - 475)
            cutvalue = 0.350;
          if (lspmass <= stopmass - 475)
            cutvalue = 0.2;

  }



  if (decaymode == "T2bw050") {


          if (lspmass > stopmass - 250) {
		cutvalue = 0.25 ; 
		    if (lspmass < 150) cutvalue = 0.175;
		}

          if (lspmass <= stopmass - 250 && lspmass > stopmass - 400)
            cutvalue = 0.3;
          if (lspmass <= stopmass - 400 && lspmass > stopmass - 450)
            cutvalue = 0.250;
          if (lspmass <= stopmass - 450 && lspmass > stopmass - 625)
            cutvalue = 0.225;
          if (lspmass <= stopmass - 625)
            cutvalue = 0.2;


  }	

  if (decaymode == "T2bw025") {


          if (lspmass <= 125)
            {
              if (lspmass > stopmass - 275)
                cutvalue = 0.2;

            }
          if (lspmass > 125)
            {
              if (lspmass > stopmass - 225)
                cutvalue = 0.2;
              if (lspmass <= stopmass - 225 && lspmass > stopmass - 275)
                cutvalue = 0.3;

            }

          if (lspmass <= stopmass - 275 && lspmass > stopmass - 425)
            cutvalue = 0.3;
          if (lspmass <= stopmass - 425 && lspmass > stopmass - 600)
            cutvalue = 0.2;
          if (lspmass <= stopmass - 600)
            cutvalue = 0.175;



  }


  if (decaymode == "T2tt") {


	  if (lspmass > stopmass - 225)
	    cutvalue = 0.325;
	  if (lspmass <= stopmass - 225 && lspmass > stopmass - 275)
	    cutvalue = 0.350;
		  if (lspmass <= stopmass - 275) {

			cutvalue=0.250; if (lspmass <= stopmass - 400) cutvalue = 0.325;
		   }


 	}


  return cutvalue;
}





float BDToutput(string BDTregion)
{

         if (BDTregion == "T2tt_1"      )    return myEvent.BDT_output_t2tt_R1;
    else if (BDTregion == "T2tt_2"      )    return myEvent.BDT_output_t2tt_R2;
    else if (BDTregion == "T2tt_5_loose")    return myEvent.BDT_output_t2tt_R5;
    else if (BDTregion == "T2tt_5_tight")    return myEvent.BDT_output_t2tt_R5;
    
    else if (BDTregion == "T2bw075_1"   )    return myEvent.BDT_output_t2bw075_R1;
    else if (BDTregion == "T2bw075_2"   )    return myEvent.BDT_output_t2bw075_R2;
    else if (BDTregion == "T2bw075_3"   )    return myEvent.BDT_output_t2bw075_R3;
    else if (BDTregion == "T2bw075_5"   )    return myEvent.BDT_output_t2bw075_R5;

    else if (BDTregion == "T2bw050_1_loose") return myEvent.BDT_output_t2bw050_R1;
    else if (BDTregion == "T2bw050_1_tight") return myEvent.BDT_output_t2bw050_R1;
    else if (BDTregion == "T2bw050_3"   )    return myEvent.BDT_output_t2bw050_R3;
    else if (BDTregion == "T2bw050_4"   )    return myEvent.BDT_output_t2bw050_R4;
    else if (BDTregion == "T2bw050_5"   )    return myEvent.BDT_output_t2bw050_R5;
    else if (BDTregion == "T2bw050_6"   )    return myEvent.BDT_output_t2bw050_R6;

    else if (BDTregion == "T2bw025_1"   )    return myEvent.BDT_output_t2bw025_R1;
    else if (BDTregion == "T2bw025_3"   )    return myEvent.BDT_output_t2bw025_R3;
    else if (BDTregion == "T2bw025_4"   )    return myEvent.BDT_output_t2bw025_R4;
    else if (BDTregion == "T2bw025_6"   )    return myEvent.BDT_output_t2bw025_R6;

    else return -10.0;
}


float BDTcut(string BDTregion)
{
         if (BDTregion == "T2tt_1"      )    return 0.325;
    else if (BDTregion == "T2tt_2"      )    return 0.35 ;
    else if (BDTregion == "T2tt_5_loose")    return 0.25 ;
    else if (BDTregion == "T2tt_5_tight")    return 0.325;
    
    else if (BDTregion == "T2bw075_1"   )    return 0.225;
    else if (BDTregion == "T2bw075_2"   )    return 0.275;
    else if (BDTregion == "T2bw075_3"   )    return 0.350;
    else if (BDTregion == "T2bw075_5"   )    return 0.2  ;

    else if (BDTregion == "T2bw050_1_loose") return 0.175;
    else if (BDTregion == "T2bw050_1_tight") return 0.25 ;
    else if (BDTregion == "T2bw050_3"   )    return 0.3  ;
    else if (BDTregion == "T2bw050_4"   )    return 0.250;
    else if (BDTregion == "T2bw050_5"   )    return 0.225;
    else if (BDTregion == "T2bw050_6"   )    return 0.2  ;

    else if (BDTregion == "T2bw025_1"   )    return 0.2  ;
    else if (BDTregion == "T2bw025_3"   )    return 0.3  ;
    else if (BDTregion == "T2bw025_4"   )    return 0.2  ;
    else if (BDTregion == "T2bw025_6"   )    return 0.175;
    
    else return -1.0;
}

bool goesInBDTRegion(string BDTregion) { return (BDToutput(BDTregion) > BDTcut(BDTregion)); }

bool BDT_T2tt_1         (bool applyMTCut) { return goesInBDTRegion("T2tt_1"         ); }
bool BDT_T2tt_2         (bool applyMTCut) { return goesInBDTRegion("T2tt_2"         ); }
bool BDT_T2tt_5_loose   (bool applyMTCut) { return goesInBDTRegion("T2tt_5_loose"   ); }
bool BDT_T2tt_5_tight   (bool applyMTCut) { return goesInBDTRegion("T2tt_5_tight"   ); }
bool BDT_T2bw075_1      (bool applyMTCut) { return goesInBDTRegion("T2bw075_1"      ); }
bool BDT_T2bw075_2      (bool applyMTCut) { return goesInBDTRegion("T2bw075_2"      ); }
bool BDT_T2bw075_3      (bool applyMTCut) { return goesInBDTRegion("T2bw075_3"      ); }
bool BDT_T2bw075_5      (bool applyMTCut) { return goesInBDTRegion("T2bw075_5"      ); }
bool BDT_T2bw050_1_loose(bool applyMTCut) { return goesInBDTRegion("T2bw050_1_loose"); }
bool BDT_T2bw050_1_tight(bool applyMTCut) { return goesInBDTRegion("T2bw050_1_tight"); }
bool BDT_T2bw050_3      (bool applyMTCut) { return goesInBDTRegion("T2bw050_3"      ); }
bool BDT_T2bw050_4      (bool applyMTCut) { return goesInBDTRegion("T2bw050_4"      ); }
bool BDT_T2bw050_5      (bool applyMTCut) { return goesInBDTRegion("T2bw050_5"      ); }
bool BDT_T2bw050_6      (bool applyMTCut) { return goesInBDTRegion("T2bw050_6"      ); }
bool BDT_T2bw025_1      (bool applyMTCut) { return goesInBDTRegion("T2bw025_1"      ); }
bool BDT_T2bw025_3      (bool applyMTCut) { return goesInBDTRegion("T2bw025_3"      ); }
bool BDT_T2bw025_4      (bool applyMTCut) { return goesInBDTRegion("T2bw025_4"      ); }
bool BDT_T2bw025_6      (bool applyMTCut) { return goesInBDTRegion("T2bw025_6"      ); }

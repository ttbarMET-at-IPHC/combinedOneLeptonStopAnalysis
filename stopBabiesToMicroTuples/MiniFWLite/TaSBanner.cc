#include "Rtypes.h"
#include "Riostream.h"
#include "TSystem.h"

const char* VTAG="02-YY-ZZ";

Int_t doBanner() {

  cout << endl;
  cout << "---------------------" << endl;
  cout << "Loading libMiniFWLite" << endl;
  cout << "---------------------" << endl;
  cout << endl;

  return 0 ;
}

static Int_t dummy = doBanner();

import ctypes
import os
from ROOT import TLorentzVector
from rootpy import log

log["/ROOT.Minuit2"].setLevel(log.ERROR)
log["/ROOT.Minuit"].setLevel(log.ERROR)

lib = os.path.dirname(os.path.realpath(__file__))+'/libTopness.so'
Topnesswrap = ctypes.CDLL(lib)
Topnesscompute_ = Topnesswrap.topnessMinimization

# Arguments types
#Topnesscompute_.argtypes = (TLorentzVector, TLorentzVector, TLorentzVector, TLorentzVector)
Topnesscompute_.argtypes = (\
  ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double,\
  ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double,\
  ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_double,\
  ctypes.c_double, ctypes.c_double)

# Return type
Topnesscompute_.restype = ctypes.c_float


def computeTopness ( 
    lepton_pt, lepton_eta, lepton_phi, lepton_E,
    bjet1_pt, bjet1_eta, bjet1_phi, bjet1_E,
    bjet2_pt, bjet2_eta, bjet2_phi, bjet2_E,
    met, met_phi):

    result = Topnesscompute_( \
    		lepton_pt, lepton_eta, lepton_phi, lepton_E,\
    		bjet1_pt, bjet1_eta, bjet1_phi, bjet1_E,\
   		bjet2_pt, bjet2_eta, bjet2_phi, bjet2_E,\
   	 	met, met_phi)
    return float(result)

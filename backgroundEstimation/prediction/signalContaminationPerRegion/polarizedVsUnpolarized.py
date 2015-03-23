#/usr/bin/env python
from rootpy.io import root_open
import ROOT as rt
from array import *

# define the palette for z axis
NRGBs = 5
NCont = 500
stops = array("d",[0.00, 0.34, 0.61, 0.84, 1.00  ])
red   = array("d",[0.25, 0.25, 0.95, 1.00, 1.00  ])
green = array("d",[0.25, 0.90, 1.00, 0.40, 0.25  ])
blue  = array("d",[0.75, 1.00, 0.30, 0.20, 0.25  ])


signals = [ "T2tt" ]
#signals = [ "T2bw-025", "T2bw-050", "T2bw-075" ]

for signal in signals :

    rootFilePolarized = root_open('globalSFmap_'+signal+'.root')
    rootFileNominal   = root_open('../SCC_nominal/globalSFmap_'+signal+'.root')

    histoPolarizer = rootFilePolarized.get(";1").GetListOfPrimitives()[1]
    histoNominal   = rootFileNominal  .get(";1").GetListOfPrimitives()[1]

    histoRatio = histoPolarizer.Clone()
    histoRatio.Divide(histoNominal)

    rt.TColor.CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont)
    rt.gStyle.SetNumberContours(NCont)
    rt.gStyle.SetPaintTextFormat("4.2f");

    c = rt.TCanvas("","",800,600);
    histoRatio.SetMinimum(0.85);
    histoRatio.SetMaximum(1.15);
    histoRatio.SetStats(0);
    histoRatio.SetTitle("B_{Polarized-SCC}/B_{Unpolarized-SCC} for "+signal+" signal type");
    histoRatio.Draw("colz text");
    c.SaveAs("polarizedVsUnpolarized_"+signal+".pdf")

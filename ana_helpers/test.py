import ROOT
from ROOT import TH1D, TF1, TCanvas
from model import model
import numpy as np


hist = TH1D("hist", "; M [Gev/c^{2}]; Yiled", 30, 1.0, 6.0)

signal = TF1("signal", "gaus", 1.0, 6.0)
background = TF1("background", "gaus", 1.0, 6.0)
signal.SetParameters(5.0, 3.096, 0.2)
background.SetParameters(0.02, 3.096, 2.0)

total = TF1("total", "signal+background", 1.0, 6.0)

hist.FillRandom("background", 50000)

P = model()
P.set_histo(hist)
P.set_sideband([1.5, 2.5, 4.0, 5.0])
P.set_kernel(500., (1e-10, 1e10), 1, (1e-10, 1e10))
P.predict(30, "back")
P.show("test")

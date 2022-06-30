"""
#
# simple class to implement GPR
# 06-30-2022
#
"""
import ROOT
from ROOT import TH1D, TCanvas

import numpy as np
from sklearn.gaussian_process import GaussianProcessRegressor
from sklearn.gaussian_process.kernels import RBF, Matern, RationalQuadratic, ExpSineSquared, DotProduct, WhiteKernel, ConstantKernel as C
from sklearn.preprocessing import StandardScaler, Normalizer

class model:
  def __init__(self):
    self
  def set_histo(self, hist):
    self.hist = hist

  def set_sideband(self, sb):
    self.sb = sb

  # use RBF kernel(only for now)
  # lim1 && lim2 are tuples
  def set_kernel(self, val1, lim1, val2, lim2):
    self.kernel = C(val1, lim1)* RBF(val2, lim2)
    
  def predict(self, niter, hist_name):
    X = []
    
    hist = self.hist
    sideband = self.sb
    kernel = self.kernel
    
    bins = hist.GetNbinsX()
    bin_min = hist.GetXaxis().GetBinLowEdge(1)
    last_bin_low = hist.GetXaxis().GetBinLowEdge(bins)
    last_bin_width = hist.GetXaxis().GetBinWidth(bins)
    bin_max = last_bin_low + last_bin_width
    
    for x in range(bins):
      X.append([hist.GetBinCenter(x+1), hist.GetBinContent(x+1), hist.GetBinError(x+1)])
      
    data = np.array(X)
    mask = (((sideband[0] <= data[:, 0]) & (data[:, 0] <= sideband[1])) | ((sideband[2] <= data[:, 0]) & (data[:, 0] <= sideband[3])))
    
    X_train = data[mask]
    
    M = GaussianProcessRegressor(kernel=kernel, alpha=X_train[:, 2]**2, n_restarts_optimizer=niter)
    M.fit(np.atleast_2d(X_train[:, 0]).T, X_train[:, 1])
    
    print("kernel : ", M.kernel_)
    print("score = ", M.score(np.atleast_2d(X_train[:, 0]).T, X_train[:, 1]))
    
    y_pred, sigma = M.predict(np.atleast_2d(data[:, 0]).T, return_std=True)
    
    self.H = TH1D(hist_name, "; mass(GeV/c^{2}); Yield", bins, bin_min, bin_max)
    
    for x in range(bins):
      self.H.SetBinContent(x+1, y_pred[x])
      self.H.SetBinError(x+1, 2.0*sigma[x])
      
  def show(self, cname):
    self.hist.SetMarkerColor(8)
    self.hist.SetMarkerStyle(8)
    
    self.H.SetFillColor(2)
    self.H.SetFillStyle(3001)
    
    can = TCanvas(cname, "", 500, 500)
    self.hist.Draw("E1 P0")
    self.H.Draw("E4 SAME")
    can.Draw()
    can.Print(cname+".png")

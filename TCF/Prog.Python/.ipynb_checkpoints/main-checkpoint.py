import ROOT
import numpy as np
import math

ROOT.gROOT.SetBatch(False)
data_file = "data.txt"

h2_sup = ROOT.TH2D("h2_sup", "", 100, 0, 100, 100, 0, 100)
h2_inf = ROOT.TH2D("h2_inf", "", 100, 0, 100, 100, 0, 100)
h2_T   = ROOT.TH2D("h2_T", "T = I/I0; x; y", 100, 0, 100, 100, 0, 100)

try:
    with open(data_file, "r") as f:
        for line in f:
            parts = list(map(float, line.split()))
            if len(parts) == 4:
                x_cell, y_cell, raggi_sup, raggi_inf = parts
                h2_sup.Fill(x_cell, y_cell, raggi_sup)
                h2_inf.Fill(x_cell, y_cell, raggi_inf)
                
                if raggi_sup != 0:
                    T = raggi_inf / raggi_sup
                    h2_T.Fill(x_cell, y_cell, T)
except FileNotFoundError:
    print(f"Errore: file {data_file} non trovato.")


izero = h2_sup.GetMean(3) 
fit = ROOT.TF2("fit", "[0] * exp(-0.3 * [1] * (x - 30)/50)", 30, 80, 30, 70)
fit.SetParNames("izero", "coeff")
fit.SetParameters(izero, 1000.0)
fit.SetLineWidth(3)
fit.SetLineColor(ROOT.kRed)

h2_inf.Fit("fit", "S R")
coeff = fit.GetParameter(1)
coeff_err = fit.GetParError(1)

fitT = ROOT.TF2("fitT", "exp(-0.3 * [0] * (x - 30)/50)", 30, 80, 30, 70)
fitT.SetParNames("k=rho*dmax")
fitT.SetParameter(0, 2.0)
fitT.SetLineWidth(3)
fitT.SetLineColor(ROOT.kRed)

h2_T.Fit("fitT", "SR")
kT = fitT.GetParameter(0)
kT_err = fitT.GetParError(0)

print("\n===== RISULTATI FIT =====")
print(f"Fit Intensità I:   k = rho*dmax = {coeff:.4f} ± {coeff_err:.4f}  [g/cm^2]")
print(f"Fit Trasmissione I/I_0:   k = rho*dmax = {kT:.4f} ± {kT_err:.4f}  [g/cm^2]")

nome = ["Alluminio", "Rame", "Acciaio al carbonio", "Ghisa", "Titanio", "Acciaio inox (304)", 
        "Ottone", "Piombo", "Tungsteno", "Nichel", "Oro", "Argento", "Legno (quercia)", 
        "Calcestruzzo", "Vetro", "Polietilene", "Legno (pino)", "Fibra di carbonio", 
        "Fibra di vetro", "Gomma", "Silicio", "Berillio"]

rho = [2.70, 8.96, 7.85, 6.90, 4.43, 7.93, 8.50, 11.34, 19.25, 8.90, 19.32, 10.49, 
       0.80, 2.30, 2.60, 0.935, 0.575, 1.80, 2.60, 1.35, 2.33, 1.85]

rho_err = [0.0]*12 + [0.05, 0.0, 0.20, 0.025, 0.075, 0.20, 0.10, 0.25, 0.0, 0.0]

nmat = len(nome)
dmax = np.zeros(nmat)
dmax_err = np.zeros(nmat)

for i in range(nmat):
    dmax[i] = coeff / rho[i]
    term1 = (coeff_err / coeff)**2
    term2 = (rho_err[i] / rho[i])**2 if rho[i] != 0 else 0
    dmax_err[i] = dmax[i] * math.sqrt(term1 + term2)


ge_dmax = ROOT.TGraphErrors(nmat, np.array(rho), dmax, np.array(rho_err), dmax_err)
ge_dmax.SetMarkerStyle(20)
ge_dmax.SetMarkerSize(1.0)
ge_dmax.SetMarkerColor(ROOT.kBlue)

rhodmax = ROOT.TF1("rhodmax", "[0]/x", 0.1, 25)
rhodmax.SetParameter(0, coeff)
rhodmax.SetLineWidth(2)
rhodmax.SetLineColor(ROOT.kRed)

c1 = ROOT.TCanvas("c1", "Analisi Materiali - Istogrammi", 1200, 900)
c1.Divide(2, 2)

c1.cd(1); h2_inf.SetTitle("Intensita' I; x; y"); h2_inf.Draw("LEGO")
c1.cd(2); h2_inf.Draw("COLZ")
c1.cd(3); h2_T.SetTitle("Trasmissione T = I/I_{0}; x; y"); h2_T.Draw("LEGO")
c1.cd(4); h2_T.Draw("COLZ")

c2 = ROOT.TCanvas("c2", "Analisi Materiali - Grafici", 1200, 900)
c2.Divide(1, 1)

c2.cd(1)
ge_dmax.SetTitle("Andamento spessore massimo in funzione della densita'; rho [g/cm^{3}]; d_{max} [cm]")
ge_dmax.Draw("AP")
rhodmax.Draw("SAME")


c1.Update()
c2.Update()

print(f"{'Materiale':<25} | {'rho (g/cm^3)':<18} | {'dmax (cm)':<20}")
print(f"{'-'*25} | {'-'*18} | {'-'*20}")

for i in range(nmat):
    rho_str = f"{rho[i]:.2f}"
    dmax_str = f"{dmax[i]:.3f} ± {dmax_err[i]:.3f}"
    print(f"{nome[i]:<25} | {rho_str:<18} | {dmax_str:<20}")


input()
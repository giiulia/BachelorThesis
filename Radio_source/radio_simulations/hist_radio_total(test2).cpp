#include <cstdlib>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>

#include "TLegend.h"
#include "TH1D.h"
#include "TPad.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TApplication.h"
#include "TCanvas.h"


using namespace std;
int main (int argc, char ** argv){

    TApplication theApp("theApp", &argc, argv);

    double binSize = 5.24;
    double eMin = -1.619689;
    double eMax = 10725.14 + 0.5*binSize;
    int nBins = (eMax-eMin)/binSize;

    double binSizesim = 1.200048;
    double eMinsim = 0.3996262;
    double eMaxsim = 10007.10 + 0.5*binSizesim;    
    int nBinssim = (eMaxsim-eMinsim)/binSizesim;

    TH1D* h200_exp = new TH1D("hist_radio_exp_200", "", nBinssim, eMinsim, eMaxsim);
    TH1D* h250_exp = new TH1D("hist_radio_exp_250", "", nBinssim, eMinsim, eMaxsim);
    TH1D* h300_exp = new TH1D("hist_radio_exp_300", "", nBinssim, eMinsim, eMaxsim);
    TH1D* h350_exp = new TH1D("hist_radio_exp_350", "", nBinssim, eMinsim, eMaxsim);
    TH1D* hreal = new TH1D("hist_radio_real", "", nBins, eMin, eMax);

    double energyexp200, countsexp200;
    double fexp200 = 3.5;
    ifstream misureexp200("Data/exponential_200mum.xy", ios::app);
    if(!misureexp200.is_open()){
        cout<<"file isn't open"<<endl;
    }
    while (misureexp200 >> energyexp200 >> countsexp200)
    {
        h200_exp->Fill(energyexp200, countsexp200);        
        
    }

    h200_exp->Rebin(5.24/1.2);
    h200_exp->Scale(fexp200);

    double energyexp250, countsexp250;
    double fexp250 = 3.5;
    ifstream misureexp250("Data/exponential_250mum.xy", ios::app);
    if(!misureexp250.is_open()){
        cout<<"file isn't open"<<endl;
    }
    while (misureexp250 >> energyexp250 >> countsexp250)
    {
        h250_exp->Fill(energyexp250, countsexp250);        
        
    }

    h250_exp->Rebin(5.24/1.2);
    h250_exp->Scale(fexp250);

    double energyexp300, countsexp300;
    double fexp300 = 3.5;
    ifstream misureexp300("Data/exponential_300mum.xy", ios::app);
    if(!misureexp300.is_open()){
        cout<<"file isn't open"<<endl;
    }
    while (misureexp300 >> energyexp300 >> countsexp300)
    {
        h300_exp->Fill(energyexp300, countsexp300);        
        
    }

    h300_exp->Rebin(5.24/1.2);
    h300_exp->Scale(fexp300);

    double energyexp350, countsexp350;
    double fexp350 = 3.5;
    ifstream misureexp350("Data/exponential_350mum.xy", ios::app);
    if(!misureexp350.is_open()){
        cout<<"file isn't open"<<endl;
    }
    while (misureexp350 >> energyexp350 >> countsexp350)
    {
        h350_exp->Fill(energyexp350, countsexp350);        
        
    }

    h350_exp->Rebin(5.24/1.2);
    h350_exp->Scale(fexp350);

    double energy, counts;
    ifstream misure("Data/radio_measures.xy", ios::app);
    if(!misure.is_open()){
        cout<<"file isn't open"<<endl;
    }
    while (misure >> energy >> counts)
    {
        hreal->Fill(energy, counts);        
    }

    gStyle->SetOptStat(kFALSE);
    TLegend* leg = new TLegend(0.91, 0.7, 0.58, 0.88);

    TCanvas c1;
    c1.SetLeftMargin(.20);
    c1.SetBottomMargin(.20);


    h200_exp->SetStats(kFALSE);
    h200_exp->SetTitle(" ; energia[keV]; conteggi"); 
    h200_exp->GetXaxis()->SetRangeUser(8500,9500);
    h200_exp->SetMaximum(25000);
    h200_exp->GetXaxis()->SetTitleSize(0.07);
    h200_exp->GetYaxis()->SetTitleSize(0.07);
    h200_exp->GetXaxis()->SetLabelSize(0.05);
    h200_exp->GetYaxis()->SetLabelSize(0.05);
    
    h200_exp->SetLineColor (kBlack);
    h200_exp->Draw("hist");
    leg->AddEntry(h200_exp, "sorgente exp 200 #mu m", "l");

    h250_exp->SetLineColor (kBlue);
    h250_exp->Draw("hist same");
    leg->AddEntry(h250_exp, "sorgente exp 250 #mu m", "l");

    h300_exp->SetLineColor (kGreen+1);
    h300_exp->Draw("hist same");
    leg->AddEntry(h300_exp, "sorgente exp 300 #mu m", "l");

    h350_exp->SetLineColor (kPink);
    h350_exp->Draw("hist same");
    leg->AddEntry(h350_exp, "sorgente exp 350 #mu m", "l");

    hreal->SetLineColor(kRed);
    hreal->Draw("hist same");
    leg->AddEntry(hreal, "misura reale", "l");

    leg->Draw("same");

    c1.Print ("Graphs/exponential.pdf", "pdf") ;

    theApp.Run();

    return 0;
  }
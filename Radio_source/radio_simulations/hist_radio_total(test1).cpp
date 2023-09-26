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

    TH1D* h300 = new TH1D("hist_radio_300mum_6882", "", nBinssim, eMinsim, eMaxsim);
    TH1D* hreal = new TH1D("hist_radio_real_6882", "", nBins, eMin, eMax);

    double energy300, counts300;
    double f300 = 25;
    ifstream misure300("Data/source_radio_300mum_calibrated_at_6288keV.xy", ios::app);
    if(!misure300.is_open()){
        cout<<"file isn't open"<<endl;
    }
    while (misure300 >> energy300 >> counts300)
    {
        h300->Fill(energy300, counts300);        
        
    }

    h300->Rebin(5.24/1.2);
    h300->Scale(f300);

    double energy, counts;
    ifstream misure("Data/radio_measures_calibrated_at_6288keV.xy", ios::app);
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


    h300->SetStats(kFALSE);
    h300->SetTitle(" ; energia[keV]; conteggi"); 
    h300->GetXaxis()->SetRangeUser(6200,6400);
    h300->SetMaximum(50000);
    h300->GetXaxis()->SetTitleSize(0.07);
    h300->GetYaxis()->SetTitleSize(0.07);
    h300->GetXaxis()->SetLabelSize(0.05);
    h300->GetYaxis()->SetLabelSize(0.05);
    
    h300->SetLineColor (kGreen);
    h300->Draw("hist same");
    leg->AddEntry(h300, "300 #mu m", "l");

    hreal->SetLineColor(kRed);
    hreal->Draw("hist same");
    leg->AddEntry(hreal, "misura reale", "l");

    leg->Draw("same");

    c1.Print ("Graphs/peak_at_6288keV.pdf", "pdf") ;

    theApp.Run();

    return 0;
  }
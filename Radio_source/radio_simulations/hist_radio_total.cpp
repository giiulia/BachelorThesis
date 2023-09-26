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

    TH1D* h250 = new TH1D("hist_radio_250mum", "", nBinssim, eMinsim, eMaxsim);
    TH1D* h300 = new TH1D("hist_radio_300mum", "", nBinssim, eMinsim, eMaxsim);
    TH1D* h350 = new TH1D("hist_radio_350mum", "", nBinssim, eMinsim, eMaxsim);
    TH1D* hreal = new TH1D("hist_radio_real", "", nBins, eMin, eMax);

    double energy250, counts250;
    double f250 = 2;
    ifstream misure250("Data/sim_250mum.xy", ios::app);
    if(!misure250.is_open()){
        cout<<"file isn't open"<<endl;
    }
    while (misure250 >> energy250 >> counts250)
    {
        h250->Fill(energy250, counts250);        
        
    }

    h250->Rebin(5.24/1.2);
    h250->Scale(f250);

    double energy300, counts300;
    double f300 = 1.9;
    ifstream misure300("Data/sim_300mum.xy", ios::app);
    if(!misure300.is_open()){
        cout<<"file isn't open"<<endl;
    }
    while (misure300 >> energy300 >> counts300)
    {
        h300->Fill(energy300, counts300);        
        
    }

    h300->Rebin(5.24/1.2);
    h300->Scale(f300);


    double energy350, counts350;
    double f350 = 1.9;
    ifstream misure350("Data/sim_350mum.xy", ios::app);
    if(!misure350.is_open()){
        cout<<"file isn't open"<<endl;
    }
    while (misure350 >> energy350 >> counts350)
    {
        h350->Fill(energy350, counts350);        
        
    }

    h350->Rebin(5.24/1.2);
    h350->Scale(f350);

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


    h250->SetStats(kFALSE);
    h250->SetTitle(" ; energia[keV]; conteggi"); 
    h250->GetXaxis()->SetRangeUser(8500,9500);
    h250->SetMaximum(25000);
    h250->GetXaxis()->SetTitleSize(0.07);
    h250->GetYaxis()->SetTitleSize(0.07);
    h250->GetXaxis()->SetLabelSize(0.05);
    h250->GetYaxis()->SetLabelSize(0.05);


    h250->SetLineColor (kBlack);
    h250->Draw("hist");
    leg->AddEntry(h250, "250 #mu m", "l");

    h300->SetLineColor(kBlue);
    h300->Draw("hist same");
    leg->AddEntry(h300, "300 #mu m", "l");   

    h350->SetLineColor (kGreen+1);
    h350->Draw("hist same");
    leg->AddEntry(h350, "350 #mu m", "l");
  
    hreal->SetLineColor(kRed);
    hreal->Draw("hist same");
    leg->AddEntry(hreal, "misura reale", "l");

    leg->Draw("same");

    c1.Print ("Graphs/radio_total_spectrum.pdf", "pdf") ;

    theApp.Run();
    
    return 0;
  }
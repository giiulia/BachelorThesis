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

double exponential(double x){ //esponenziale che parte da 8840 all'altezza di 20 conteggi con 0.00241335 
    return pow(10, 3.31*exp((x-8733)*0.0018));
}

double rand_range(double min, double max){
    return min + (max - min)*rand()/static_cast<float> (RAND_MAX);
}

double IsBelow(double f(double), double xMin, double xMax, double yMin, double yMax){ //DA 7430 A 8840
    double x = rand_range(xMin, xMax);
    double y = rand_range(yMin, yMax);

    if(y < f(x)){
        return x;
    }
    else return 0;
}

using namespace std;
int main (int argc, char ** argv){

    TApplication theApp("theApp", &argc, argv);

    double binSize = 5.24;
    double eMin = 0;
    double eMax = 10725.14 + 0.5*binSize;
    int nBins = (eMax-eMin)/binSize;

    double binSizesim = 1.200048;
    double eMinsim = 0;
    double eMaxsim = 9998.199 + 0.5*binSizesim;    
    int nBinssim = (eMaxsim-eMinsim)/binSizesim;

    TH1D* h150 = new TH1D("hist_radio_150mum", "", nBinssim, eMinsim, eMaxsim);
    TH1D* h200 = new TH1D("hist_radio_200mum", "", nBinssim, eMinsim, eMaxsim);  
    TH1D* h250 = new TH1D("hist_radio_250mum", "", nBinssim, eMinsim, eMaxsim);
    TH1D* h300_MC = new TH1D("hist_radio_300mum", "", nBinssim, eMinsim, eMaxsim);
    TH1D* hreal = new TH1D("hist_radio_real", "", nBins, eMin, eMax);

    TH1D* hexpo = new TH1D("hist_radio_expo", "", nBins, eMin, eMax);

    double nHit = 0;
    double x = 0;
    while(nHit != 100000){
        x = IsBelow(exponential, 7430, 8733, 0, 100);
        if(x != 0) {
            hexpo->Fill(x);
            nHit++;
        }
    }

    hexpo->Scale(181225./100000.);

    
    double energy150, counts150;
    Double_t f150 = 29;
    ifstream misure150("sorgradio150mum.xy", ios::app);
    if(!misure150.is_open()){
        cout<<"il file non si è aperto"<<endl;
    }
    while (misure150 >> energy150 >> counts150)
    {
        h150->Fill(energy150, counts150);        
        
    }

    h150->Scale(f150);

    double energy200, counts200;
    double f200 = 6.10;
    ifstream misure200("sorgradio200mum.xy", ios::app);
    if(!misure200.is_open()){
        cout<<"il file non si è aperto"<<endl;
    }
    while (misure200 >> energy200 >> counts200)
    {
        h200->Fill(energy200, counts200*f200);        
        
    }

    double energy250, counts250;
    double f250 = 6.19;
    ifstream misure250("sorgradio250mum.xy", ios::app);
    if(!misure250.is_open()){
        cout<<"il file non si è aperto"<<endl;
    }
    while (misure250 >> energy250 >> counts250)
    {
        h250->Fill(energy250, counts250*f250);        
        
    }

    double energy300, counts300;
    double f300 = 6.01;
    ifstream misure300("sorgradio300mum.xy", ios::app);
    if(!misure300.is_open()){
        cout<<"il file non si è aperto"<<endl;
    }
    while (misure300 >> energy300 >> counts300)
    {
        h300_MC->Fill(energy300, counts300);        
        
    }

    h300_MC->Scale(f300);
    TH1D h300 = *h300_MC + *hexpo;

    double a300 = h300.Integral(7167, 7355);
    cout<<"INTEGRALE 300: "<<a300<<endl;

    double energy, counts;
    ifstream misure("misure_radio.xy", ios::app);
    if(!misure.is_open()){
        cout<<"il file non si è aperto"<<endl;
    }
    while (misure >> energy >> counts)
    {
        hreal->Fill(energy, counts);        
    }

    double areal = hreal->Integral(1642, 1685);
    cout<<"INTEGRALE REAL: "<<areal<<endl;

    gStyle->SetOptStat(kFALSE);
    TLegend* leg = new TLegend(0.91, 0.7, 0.58, 0.88);

    TCanvas *c1 = new TCanvas("c1","c1",600,400);
    c1->SetLeftMargin(.20);
    c1->SetBottomMargin(.20);

    h300.SetTitle(" ; energia[keV]; conteggi"); 
    h300.GetXaxis()->SetRangeUser(8500,9500);
    h300.SetMaximum(25000);
    h300.GetXaxis()->SetTitleSize(0.07);
    h300.GetYaxis()->SetTitleSize(0.07);
    h300.GetXaxis()->SetLabelSize(0.05);
    h300.GetYaxis()->SetLabelSize(0.05);

    //h150->SetLineColor (kBlue);
    //h150->Draw("hist");
    //leg->AddEntry(h150, "150 #mu m", "l");

    //h200->SetFillColor (kBlack);
    //h200->Draw("hist same");
    //leg->AddEntry(h200, "200 #mu m", "l");

    h300.SetLineColor(kOrange);
    h300.Draw("hist same");
    leg->AddEntry(&h300, "300 #mu m", "l");   
    
    hreal->SetLineColor(kRed);
    hreal->Draw("hist same");
    leg->AddEntry(hreal, "misura reale", "l");

    leg->Draw("same");

    c1->Print ("spettro_radio_total.pdf", "pdf") ;
/*
    TCanvas *c2 = new TCanvas("c1","c1",600,400);
    c2->SetLeftMargin(.20);
    c2->SetBottomMargin(.20);

    h150->SetTitle(" ; energia[keV]; conteggi"); 
    h150->GetXaxis()->SetRangeUser(0,1000);
    h150->SetMaximum(140000);
    h150->GetXaxis()->SetTitleSize(0.07);
    h150->GetYaxis()->SetTitleSize(0.07);
    h150->GetXaxis()->SetLabelSize(0.05);
    h150->GetYaxis()->SetLabelSize(0.05);


    h150->SetLineColor (kBlue);
    h150->Draw("hist");

    //h200->SetFillColor (kBlack);
    //h200->Draw("hist same");

    h300->SetLineColor(kOrange);
    h300->Draw("hist same");
    
    hreal->SetLineColor(kRed);
    hreal->Draw("hist same");

    leg->Draw("same");

    c2->Print ("spettro_radio_total_lowEnergy.pdf", "pdf") ;
*/
    theApp.Run();
    


    return 0;
  }
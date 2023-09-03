#include <cstdlib>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>

#include "THStack.h"
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
    double eMin = 0;
    double eMax = 10725.14 + 0.5*binSize;
    int nBins = (eMax-eMin)/binSize;

    double binSizesim = 1.200048;
    double eMinsim = 0;
    double eMaxsim = 9998.199 + 0.5*binSizesim;    
    int nBinssim = (eMaxsim-eMinsim)/binSizesim;

    //TH1D* h150 = new TH1D("hist_radio_150mum", "", nBinssim, eMinsim, eMaxsim);
    //TH1D* h250 = new TH1D("hist_radio_250mum", "", nBinssim, eMinsim, eMaxsim);
    TH1D* h300 = new TH1D("hist_radio_300mum", "", nBinssim, eMinsim, eMaxsim);
    TH1D* hreal = new TH1D("hist_radio_real", "", nBins, eMin, eMax);
    /*
    double energy150, counts150;
    double f150 = 5.14;
    ifstream misure150("sorgradio150mum.xy", ios::app);
    if(!misure150.is_open()){
        cout<<"il file non si è aperto"<<endl;
    }
    while (misure150 >> energy150 >> counts150)
    {
        h150->Fill(energy150, counts150*f150);        
        
    }

    double energy250, counts250;
    double f250 = 5.05;
    ifstream misure250("sorgradio250mum.xy", ios::app);
    if(!misure250.is_open()){
        cout<<"il file non si è aperto"<<endl;
    }
    while (misure250 >> energy250 >> counts250)
    {
        h250->Fill(energy250, counts250*f250);        
        
    }
*/
    double energy300, counts300;
    double f300 = 23.70;
    ifstream misure300("sorgradio300mum.xy", ios::app);
    if(!misure300.is_open()){
        cout<<"il file non si è aperto"<<endl;
    }
    while (misure300 >> energy300 >> counts300)
    {
        h300->Fill(energy300, counts300*f300);        
        
    }

    double energy, counts;
    ifstream misure("misure_radio.xy", ios::app);
    if(!misure.is_open()){
        cout<<"il file non si è aperto"<<endl;
    }
    while (misure >> energy >> counts)
    {
        hreal->Fill(energy, counts);        
    }


    
    THStack *hs = new THStack("hs", "comparison of histograms");

    //h150->SetFillColor (kBlue);
    //hs -> Add(h150);
    //h250->SetFillColor (kGreen);
    //hs -> Add(h250);
    h300->SetFillColor (kOrange);
    hs -> Add(h300);
    hreal->SetFillColor (kRed);
    hs -> Add(hreal);
    

    //gStyle->SetPalette(1);

    TCanvas c1;
    c1.SetLeftMargin(.15);
    c1.SetBottomMargin(.15);
    hs->SetTitle(" ; energia[keV]; conteggi");

    hs->Draw ("hist");
    hs->GetXaxis()->SetRangeUser(8000,10000);
    hs->SetMaximum(25000);
    //c1.SetLogy();
    c1.Print ("spettro_radio_total.pdf", "pdf") ;
   
    TCanvas c2;
    c2.SetLeftMargin(.15);
    c2.SetBottomMargin(.15);
    hs->SetTitle(" ; energia[keV]; conteggi");

    hs->Draw ("hist");
    hs->GetXaxis()->SetRangeUser(0,2000);
    hs->SetMaximum(140000);
    //c1.SetLogy();
    c2.Print ("spettro_radio_total_lowEnergy.pdf", "pdf") ;   

    theApp.Run();    


    return 0;
  }
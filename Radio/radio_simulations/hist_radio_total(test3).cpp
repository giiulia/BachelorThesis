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

   
    TH1D* hSourceOnDet200 = new TH1D("hist_radio_sod200", "", nBinssim, eMinsim, eMaxsim);
    TH1D* hSourceOnDet250 = new TH1D("hist_radio_sod250", "", nBinssim, eMinsim, eMaxsim);
    TH1D* hSourceOnDet300 = new TH1D("hist_radio_sod300", "", nBinssim, eMinsim, eMaxsim);

    TH1D* hreal = new TH1D("hist_radio_real", "", nBins, eMin, eMax);


    double energyssr200, countsssr200;
    double fssr200 = 1.5;
    ifstream fileSourceOnDet200("Data/source_on_detector_200mum.xy", ios::app);
    if(!fileSourceOnDet200.is_open()){
        cout<<"file isn't open"<<endl;
    }
    while (fileSourceOnDet200 >> energyssr200 >> countsssr200)
    {
        hSourceOnDet200->Fill(energyssr200, countsssr200);        
        
    }

    hSourceOnDet200->Rebin(5.24/1.2);
    hSourceOnDet200->Scale(fssr200);

    double energyssr250, countsssr250;
    double fssr250 = 1.5;
    ifstream fileSourceOnDet250("Data/source_on_detector_250mum.xy", ios::app);
    if(!fileSourceOnDet250.is_open()){
        cout<<"file isn't open"<<endl;
    }
    while (fileSourceOnDet250 >> energyssr250 >> countsssr250)
    {
        hSourceOnDet250->Fill(energyssr250, countsssr250);        
        
    }

    hSourceOnDet250->Rebin(5.24/1.2);
    hSourceOnDet250->Scale(fssr250);

    double energyssr300, countsssr300;
    double fssr300 = 1.5;
    ifstream fileSourceOnDet300("Data/source_on_detector_300mum.xy", ios::app);
    if(!fileSourceOnDet300.is_open()){
        cout<<"file isn't open"<<endl;
    }
    while (fileSourceOnDet300 >> energyssr300 >> countsssr300)
    {
        hSourceOnDet300->Fill(energyssr300, countsssr300);        
        
    }

    hSourceOnDet300->Rebin(5.24/1.2);
    hSourceOnDet300->Scale(fssr300);

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


    hSourceOnDet200->SetStats(kFALSE);
    hSourceOnDet200->SetTitle(" ; energia[keV]; conteggi"); 
    hSourceOnDet200->GetXaxis()->SetRangeUser(8500,9500);
    hSourceOnDet200->SetMaximum(25000);
    hSourceOnDet200->GetXaxis()->SetTitleSize(0.07);
    hSourceOnDet200->GetYaxis()->SetTitleSize(0.07);
    hSourceOnDet200->GetXaxis()->SetLabelSize(0.05);
    hSourceOnDet200->GetYaxis()->SetLabelSize(0.05);
   
    hSourceOnDet200->SetLineColor (kBlue);
    hSourceOnDet200->Draw("hist same");
    leg->AddEntry(hSourceOnDet200, "spessore attivo 200#mu m", "l");
 
    hSourceOnDet250->SetLineColor (kOrange+1);
    hSourceOnDet250->Draw("hist same");
    leg->AddEntry(hSourceOnDet250, "spessore attivo 250#mu m", "l");

    hSourceOnDet300->SetLineColor (kBlack);
    hSourceOnDet300->Draw("hist same");
    leg->AddEntry(hSourceOnDet300, "spessore attivo 300#mu m", "l");

    hreal->SetLineColor(kRed);
    hreal->Draw("hist same");
    leg->AddEntry(hreal, "misura reale", "l");

    leg->Draw("same");

    c1.Print ("Graphs/source_on_detector_20+30.pdf", "pdf") ;

    theApp.Run();

    return 0;
  }
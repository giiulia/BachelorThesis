#include <cstdlib>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>

#include "TF1.h"
#include "TF1Convolution.h"
#include "TFitResult.h"
#include "TH1D.h"
#include "TPad.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TApplication.h"
#include "TCanvas.h"

double GausEspC(double * x, double * par){
    double a,b;

    a= par[1]*exp(par[2]*(x[0]-8910.));
    b= par[0]*exp( -(x[0]-8785.)*(x[0]-8785.)/pow(par[1], 2) ) / (par[1]*sqrt(2*3.14));


    return a + b;

}

double exponential(double * x, double * par)
  {
    return par[1]*exp((x[0]-8900)*par[0]);
  }

double gaussiana(double * x, double *par)
  {
    return par[0]*exp((x[0]-par[1])/pow(par[2], 2))/(sqrt(2*M_PI)*par[2]);
  }


using namespace std;
int main (int argc, char ** argv){

    TApplication theApp("theApp", &argc, argv);
    int sum = 0;
    double binSize = 5.24;
    double eMin = 3.620546;
    double eMax = 10725.14 + 0.5*binSize;    // 10725.14 last energy in file
    int nBins = (eMax-eMin)/binSize;
    TH1D* h1 = new TH1D("hist_radio_zoom", "", nBins, eMin, eMax);
    TH1D* h2 = new TH1D("hist_radio_LowEnergy", "", nBins, eMin, eMax);

    double energy, counts;
    ifstream misure("Data/radio_measures.xy", ios::app);
    if(!misure.is_open()){
        cout<<"file isn't open"<<endl;
    }
    while (misure >> energy >> counts)
    {
        h1->Fill(energy, counts);
        sum = sum + counts;

        h2->Fill(energy, counts);
    }
    
    gStyle->SetOptStat(kFALSE);


    TCanvas c1;
    c1.SetLeftMargin(.20);
    c1.SetBottomMargin(.20);

    h1->SetStats(kFALSE);
    h1->GetXaxis()->SetTitle(" energia [keV]");
    h1->GetYaxis()->SetTitle(" conteggi ");
    //h1->GetXaxis()->SetRangeUser(8500,9500);
    //h1->SetMaximum(25000);
    h1->GetXaxis()->SetTitleSize(0.07);
    h1->GetYaxis()->SetTitleSize(0.07);
    h1->GetXaxis()->SetLabelSize(0.05);
    h1->GetYaxis()->SetLabelSize(0.05);
    h1->GetYaxis()->SetNdivisions(505, kTRUE);

    double a = h1->Integral(7272.2, 7377); //7322.2
    cout<<"INTEGRAL peak at 8785 keV: "<<a<<endl;
    h1->SetLineColor(kRed);

    h1->Draw ("hist");
    c1.SetLogy();
   // c1.Print ("Graphs/radio_real_spectrum.pdf", "pdf") ;
   
    cout<<"entries: "<<sum<<endl;

//-----------------------------------------------------
 

    TCanvas c2;
    c2.SetLeftMargin(.20);
    c2.SetBottomMargin(.20);
    h2->SetTitle(" ; energia[keV]; conteggi");
    h2->GetXaxis()->SetTitleSize(0.07);
    h2->GetYaxis()->SetTitleSize(0.07);
    h2->GetXaxis()->SetLabelSize(0.05);
    h2->GetYaxis()->SetLabelSize(0.05);
    h2->GetYaxis()->SetNdivisions(505, kTRUE);

    h2->Draw ("hist");
    h2->GetXaxis()->SetRangeUser(0,2000);
    h2->SetMaximum(140000);
    c2.Print ("Graphs/radio_lowEnergy_spectrum.pdf", "pdf") ;  
  
    theApp.Run();  

  
    return 0;
  }
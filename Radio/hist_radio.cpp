#include <cstdlib>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>

#include "TF1.h"
#include "TF1Convolution.h"
#include "TH1D.h"
#include "TPad.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TApplication.h"
#include "TCanvas.h"


double exponential(double * x, double * par)
  {
    return 2.534*exp((x[0]-8700)*par[0]);
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
    double eMin = 0;
    double eMax = 10725.14 + 0.5*binSize;    // 10725.14 è l'ultima energia nel file
    int nBins = (eMax-eMin)/binSize;
    TH1D* h = new TH1D("hist_radio", "", nBins, eMin, eMax);

    double energy, counts;
    ifstream misure("misure_radio.xy", ios::app);
    if(!misure.is_open()){
        cout<<"il file non si è aperto"<<endl;
    }
    while (misure >> energy >> counts)
    {
        if(counts>1){
            h->Fill(energy, log10(counts));
        }
        else h->Fill(energy, 0);
        sum = sum + counts;
    }
    

    //gStyle->SetPalette(1);

    TCanvas c1;
    c1.SetLeftMargin(.20);
    c1.SetBottomMargin(.20);

    h->SetStats(kFALSE);
    h->GetXaxis()->SetTitle(" energia [keV]");
    h->GetYaxis()->SetTitle(" conteggi ");
    h->GetXaxis()->SetTitleSize(0.07);
    h->GetYaxis()->SetTitleSize(0.07);
    h->GetXaxis()->SetLabelSize(0.05);
    h->GetYaxis()->SetLabelSize(0.05);
    h->SetFillColor (kRed) ;
    h->GetYaxis()->SetNdivisions(505, kTRUE);

    double a = h->Integral(7272.2, 7377); //7322.2
    cout<<"INTEGRALE picco a 8785 keV: "<<a<<endl;

    h->Draw ("hist");
    //c1.SetLogy();
    c1.Print ("spettro_radio_reale.pdf", "pdf") ;
   
    cout<<"entries: "<<sum<<endl;

    TCanvas c2;
    c2.SetLeftMargin(.20);
    c2.SetBottomMargin(.20);

    TF1 expon("expo", exponential, 7430., 8700., 1);
    expon.SetParameter(0, 0.0005);

    TFitResultPtr fit_result = h->Fit("expo", "S");

    h->Draw ("hist");
    h->GetXaxis()->SetRangeUser(7000,9000);
    //c2.SetLogy();
    c2.Print ("spettro_radio_reale_zoom.pdf", "pdf") ; 

    TCanvas c3;
    c3.SetLeftMargin(.20);
    c3.SetBottomMargin(.20);
    h->SetTitle(" ; energia[keV]; conteggi");

    h->Draw ("hist");
    h->GetXaxis()->SetRangeUser(0,2000);
    h->SetMaximum(140000);
    //c3.SetLogy();
    c3.Print ("spettro_radio_lowEnergy.pdf", "pdf") ;  
    theApp.Run();  

  
    return 0;
  }
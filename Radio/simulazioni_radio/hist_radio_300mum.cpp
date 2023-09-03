#include <cstdlib>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>

#include "TH1D.h"
#include "TPad.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TApplication.h"
#include "TCanvas.h"

double exponential(double x){ //esponenziale che parte da 8840 all'altezza di 20 conteggi con 0.00241335 
    return 20*exp(exp((x-8840)*0.000508222));
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

    srand(time(NULL));

    TApplication theApp("theApp", &argc, argv);
    double f300 = 6.19;
    double binSize = 1.200048;
    double eMin = 0;
    double eMax = 9998.199 + 0.5*binSize;
    int nBins = (eMax-eMin)/binSize;
    TH1D* h = new TH1D("hist_radio", "", nBins, eMin, eMax);

    double energy, counts;
    ifstream misure("sorgradio300mum.xy", ios::app);
    if(!misure.is_open()){
        cout<<"il file non si è aperto"<<endl;
    }
    while (misure >> energy >> counts)
    {
        if(counts>1){
            h->Fill(energy, log10(counts));
        }
        else h->Fill(energy, 0); 
    }

    double nHit = 0;
    double x = 0;
    while(nHit != 680){
        x = IsBelow(exponential, 7430, 8840, 0, 100);
        if(x != 0) {
            h->Fill(x);
            nHit++;
        }
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
    h->SetFillColor (kOrange) ;

    double a = h->Integral(7317, 7327); //7322.2
    cout<<"INTEGRALE picco a 8785 keV: "<<a<<endl;

    h->Draw ("hist");
    //c1.SetLogy();
    c1.Print ("spettro_radio_sim300mum.pdf", "pdf") ;
   
    TCanvas c2;
    c2.SetLeftMargin(.20);
    c2.SetBottomMargin(.20);

    h->Draw ("hist");
    h->GetXaxis()->SetRangeUser(8000,10000);
    //c2.SetLogy();
    c2.Print ("spettro_radio_300mum_zoom.pdf", "pdf") ; 
    
    theApp.Run();   


  
    return 0;
  }
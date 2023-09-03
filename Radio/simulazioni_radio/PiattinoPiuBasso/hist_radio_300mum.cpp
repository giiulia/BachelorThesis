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

double exponential(double x){ //esponenziale che parte da 8840 all'altezza di 20 conteggi con 0.00241335 
    return 3.31*exp((x-8733)*0.0018);
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
    double f300 = 5.42;
        double binSize = 1.2;
    double eMin = 0.6000240;
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
            h->Fill(energy, f300*log10(counts));
        }
        else h->Fill(energy, 0); 
    }

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

    hexpo->Scale(1662.71/100000.);

   // THStack *hs = new THStack("hs", "stack of histograms");

    //hs->Add(hexpo);
    //hs->Add(h);
    TH1D h3 = *h + *hexpo;

    TCanvas c1;
    c1.SetLeftMargin(.20);
    c1.SetBottomMargin(.20);

    h3.SetStats(kFALSE);
    h3.GetXaxis()->SetTitle(" energia [keV]");
    h3.GetYaxis()->SetTitle(" conteggi ");
    h3.GetXaxis()->SetTitleSize(0.07);
    h3.GetYaxis()->SetTitleSize(0.07);
    h3.GetXaxis()->SetLabelSize(0.05);
    h3.GetYaxis()->SetLabelSize(0.05);
    h3.SetFillColor (kOrange) ;
  //  hexpo->SetFillColor (kOrange) ;

    double a = h3.Integral(7317, 7327); //7322.2
    cout<<"INTEGRALE picco a 8785 keV: "<<a<<endl;

    h3.Draw ("hist");
    //c1.SetLogy();
    c1.Print ("spettro_radio_sim300mum.pdf", "pdf") ;
  
    TCanvas c2;
    c2.SetLeftMargin(.20);
    c2.SetBottomMargin(.20);

    h3.Draw ("hist");
    h3.GetXaxis()->SetRangeUser(7000,9000);
    //c2.SetLogy();
    c2.Print ("spettro_radio_300mum_zoom.pdf", "pdf") ; 

    theApp.Run();   


  
    return 0;
  }
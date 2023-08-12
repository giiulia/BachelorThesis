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



using namespace std;
int main (int argc, char ** argv){

    TApplication theApp("theApp", &argc, argv);
    
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
        h->Fill(energy, counts); 
    }
    

    //gStyle->SetPalette(1);

    TCanvas c1;
    c1.SetLeftMargin(.15);
    c1.SetBottomMargin(.15);

    h->SetStats(kFALSE);
    h->GetXaxis()->SetTitle(" energia [keV]");
    h->GetYaxis()->SetTitle(" conteggi ");
    h->GetXaxis()->SetTitleSize(0.07);
    h->GetYaxis()->SetTitleSize(0.07);
    h->GetXaxis()->SetLabelSize(0.05);
    h->GetYaxis()->SetLabelSize(0.05);
    h->SetFillColor (kOrange + 1) ;
    h->Draw ();
    //c1.SetLogy();
    c1.Print ("spettro_radio_reale", "pdf") ;
   
    cout<<"entries: "<<h->GetEntries()<<endl;

    theApp.Run();    

  
    return 0;
  }
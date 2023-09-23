#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>

#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFitResultPtr.h"
#include "TApplication.h"
#include "TAxis.h"
#include "TAttFill.h"
#include "TMultiGraph.h"
#include "TStyle.h"

double funzioneFWHM (double * x, double * par){

    return sqrt((par[0]/x[0] ) + par[1]*x[0]);

}

using namespace std;
int main(int argc, char* argv[]){
	TApplication theApp("theApp", &argc, argv);
 	gStyle->SetOptFit(1112);
//lettura dati
    ifstream dati;
 	dati.open("dati_elaborati.txt", ios::in);

	vector<double> v_sh_time, v_sorg, v_sorg_err, v_imp, v_imp_err;
	double Shtime, sorg, sorg_err, imp, imp_err;

	dati.ignore(300, '\n');

  	while (!dati.eof()) {

		dati >> Shtime;
      	dati >> sorg; //in keV
      	dati >> sorg_err;
      	dati >> imp;
      	dati >> imp_err;
      	v_sh_time.push_back(Shtime);
      	v_sorg.push_back(sorg);
      	v_sorg_err.push_back(sorg_err);
      	v_imp.push_back(imp);
      	v_imp_err.push_back(imp_err);

 	}
  	
  	dati.close();


//analisi errori sistematici
  	vector <double> v_sorg_err_completo, v_imp_err_completo;
  	vector <double> v_sh_time_err;

  	for(int i = 0; i < v_sh_time.size(); i++){

  		v_sorg_err_completo.push_back( v_sorg_err.at(i) + 1 ); //errore sistematico medio per riv1

  		v_imp_err_completo.push_back( v_imp_err.at(i) + 1 );

  		//v_sh_time_err.push_back( 0.1 );


  	}

//interpolazione
	TF1 modelloFWHM ("funzioneFWHM", funzioneFWHM, 0.5, 10, 2);
	modelloFWHM.SetParName(0, "C^{2} #alpha e_{n}^{2}"); 
    modelloFWHM.SetParName(1, "#beta i_{n}^{2}");
    //modelloFWHM.SetParameter (0, 280); 
	modelloFWHM.SetParameter (1, 10);

//grafico
  	TCanvas c;
	c.SetLeftMargin(0.20);
    c.SetBottomMargin(0.20);
 	
 	TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle(" ; shaping time [#mus]; FWHM [keV]");

    TGraphErrors * g_sorg_sh_time = new TGraphErrors( v_sh_time.size(), &v_sh_time[0], &v_sorg[0], 0, &v_sorg_err_completo[0] );
  	TGraphErrors * g_imp_sh_time = new TGraphErrors( v_sh_time.size(), &v_sh_time[0], &v_imp[0], 0, &v_imp_err_completo[0] );

	g_sorg_sh_time->SetTitle(" sorgente ");
  	g_sorg_sh_time->SetMarkerColor(6);
  	g_sorg_sh_time->SetMarkerSize(1);
  	g_sorg_sh_time->SetMarkerStyle(20);
    TFitResultPtr fit_result = g_sorg_sh_time->Fit (&modelloFWHM, "SQ+") ;

  	
  	g_imp_sh_time->SetTitle(" impulsatore ");
  	g_imp_sh_time->SetMarkerColor(4);
  	g_imp_sh_time->SetMarkerSize(1);
  	g_imp_sh_time->SetMarkerStyle(20);
    TFitResultPtr fit_result2 = g_imp_sh_time->Fit (&modelloFWHM, "SQ+") ;


  	mg->Add(g_sorg_sh_time);
    mg->Add(g_imp_sh_time);

    mg->GetXaxis()->SetLabelSize(0.05);
    mg->GetYaxis()->SetLabelSize(0.05);
    mg->GetXaxis()->SetTitleSize(0.07);
    mg->GetYaxis()->SetTitleSize(0.07);

    mg->Draw("AP");
    
    c.BuildLegend() ; 
   // c.Print("Grafici/fwhm_sh_time.pdf", "pdf");

//differenze quadratiche
    vector<double> v_diff, v_diff_err;
    for(int i = 0; i<v_sh_time.size(); i++){

    	v_diff.push_back( sqrt(pow(v_sorg.at(i), 2) - pow(v_imp.at(i), 2)) );
    	v_diff_err.push_back(  sqrt(pow(v_sorg.at(i)*v_sorg_err_completo.at(i), 2) + pow(v_imp.at(i)*v_imp_err_completo.at(i), 2))/sqrt(pow(v_sorg.at(i), 2) - pow(v_imp.at(i), 2))  );

    } 

//grafico
    TCanvas c1;
	c1.SetLeftMargin(0.15);
    c1.SetBottomMargin(0.15);

    TGraphErrors * g_diff = new TGraphErrors( v_sh_time.size(), &v_sh_time[0], &v_diff[0], 0, &v_diff_err[0] );
    g_diff->SetTitle(" ; shaping time [#mus]; #sqrt{#Delta sq} [keV]");

  	g_diff->SetMarkerColor(6);
  	g_diff->SetMarkerSize(1);
  	g_diff->SetMarkerStyle(20);

  	g_diff->Draw("AP");

    theApp.Run();

	return 0;
} 
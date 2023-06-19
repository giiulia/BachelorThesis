#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>

#include "TH1F.h"
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

    return sqrt(par[0]/sqrt(x[0]) + x[0]*par[1]);

}

using namespace std;
int main(int argc, char* argv[]){
	TApplication theApp("theApp", &argc, argv);
 	gStyle->SetOptFit(1112);
//lettura dati
    ifstream dati;
 	dati.open("dati_elaborati.txt", ios::in);

	vector<double> v_Vbias, v_sorg, v_sorg_err, v_imp, v_imp_err;
	double Vbias, sorg, sorg_err, imp, imp_err;

	dati.ignore(300, '\n');

  	while (!dati.eof()) {

		dati >> Vbias;
      	dati >> sorg; //in keV
      	dati >> sorg_err;
      	dati >> imp;
      	dati >> imp_err;
      	v_Vbias.push_back(Vbias);
      	v_sorg.push_back(sorg);
      	v_sorg_err.push_back(sorg_err);
      	v_imp.push_back(imp);
      	v_imp_err.push_back(imp_err);

 	}
  	
  	dati.close();

//analisi errori sistematici
  	vector <double> v_sorg_err_completo, v_imp_err_completo;
  	vector <double> v_Vbias_err;

  	for(int i = 0; i < v_Vbias.size(); i++){

  		v_sorg_err_completo.push_back( v_sorg_err.at(i) + 0.6 );

  		v_imp_err_completo.push_back( v_imp_err.at(i) + 0.6 );

  		v_Vbias_err.push_back( 0.1 );

  		cout<<"Vbias: "<<v_Vbias.at(i) << endl;
  		cout<<"  sorg: "<<v_sorg.at(i) << " +- "<<v_sorg_err_completo.at(i)<< endl;
  	  	cout<<"  imp: "<<v_imp.at(i) << " +- "<<v_imp_err_completo.at(i)<<"\n"<< endl;


  	}

//interpolazione
	TF1 modelloFWHM ("funzioneFWHM", funzioneFWHM, 5, 70, 2);
	modelloFWHM.SetParName(0, "k_C"); 
	modelloFWHM.SetParName(1, "k_I");
	modelloFWHM.SetParameter (0, 10000); 
	modelloFWHM.SetParameter (1, 20);

//grafico
  	TCanvas c;
	c.SetLeftMargin(0.15);
    c.SetBottomMargin(0.15);
 	
 	TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle(" ; V bias [V]; FWHM [keV]");

    cout<<"v_Vbias.size(): "<<v_Vbias.size()<<endl;
    TGraphErrors * g_sorg_Vbias = new TGraphErrors( v_Vbias.size(), &v_Vbias[0], &v_sorg[0], &v_Vbias_err[0], &v_sorg_err_completo[0] );
  	TGraphErrors * g_imp_Vbias = new TGraphErrors( v_Vbias.size(), &v_Vbias[0], &v_imp[0], &v_Vbias_err[0], &v_imp_err_completo[0] );

	g_sorg_Vbias->SetTitle(" sorgente ");
  	g_sorg_Vbias->SetMarkerColor(6);
  	g_sorg_Vbias->SetMarkerSize(1);
  	g_sorg_Vbias->SetMarkerStyle(20);
    TFitResultPtr fit_result = g_sorg_Vbias->Fit (&modelloFWHM, "SQ+") ;

	
  	g_imp_Vbias->SetTitle(" impulsatore ");
  	g_imp_Vbias->SetMarkerColor(4);
  	g_imp_Vbias->SetMarkerSize(1);
  	g_imp_Vbias->SetMarkerStyle(20);
    TFitResultPtr fit_result2 = g_imp_Vbias->Fit (&modelloFWHM, "SQ+") ;


  	mg->Add(g_sorg_Vbias);
    mg->Add(g_imp_Vbias);
    mg->Draw("AP");
    
    c.BuildLegend() ; 
  //  c.Print("Grafici/fwhm_Vbias.pdf", "pdf");

    theApp.Run();

	return 0;
} 
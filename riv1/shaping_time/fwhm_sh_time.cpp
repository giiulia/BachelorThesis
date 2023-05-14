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

using namespace std;

double min_max( double t1, double t2, double t3 ){ //restituisce l'errore sistematico
	double a[] = {t1, t2, t3};
	double sist;
	double min = 300;
	double max = 10;
	

	for(int i = 0; i < 3; i++){
		if(a[i] < min) 
			min = a[i];
		if(a[i] > max){
			max = a[i];
		}
	}
	sist = (max - min) /2.;

	cout<<"errore sisetmatico: " << sist << endl;
	return sist;

}

double calc_media_pesata( double values[], double errors[] ){
	double media = 0;
	double err_sum = 0;
	for (int i = 0; i < 3; i++){
		media += values[i]/pow(errors[i], 2);
		err_sum += 1/pow(errors[i], 2);
	}
	media = media / err_sum;
	return media;
}

double calc_err_media_pesata( double errors[] ){
	double err;
	double err_sum = 0;
	for (int i = 0; i < 3; i++){
		err_sum += 1/pow(errors[i], 2);
	}
	err = sqrt(1 / err_sum);
	cout<<"errore casuale: " << err << endl;

	return err;
}

int main(int argc, char* argv[]){
	TApplication theApp("theApp", &argc, argv);
 	gStyle->SetOptFit(1112);
//lettura dati
    ifstream dati;
 	dati.open("tentativo1.txt", ios::in);

	vector<double> v_sh_time, v_sorg1, v_sorg_err1, v_imp1, v_imp_err1;
	double Shtime, sorg, sorg_err, imp, imp_err;

	dati.ignore(300, '\n');

  	while (!dati.eof()) {

		dati >> Shtime;
      	dati >> sorg; //in keV
      	dati >> sorg_err;
      	dati >> imp;
      	dati >> imp_err;
      	v_sh_time.push_back(Shtime);
      	v_sorg1.push_back(sorg);
      	v_sorg_err1.push_back(sorg_err);
      	v_imp1.push_back(imp);
      	v_imp_err1.push_back(imp_err);

 	}
  	
  	dati.close();

  	dati.open("tentativo2.txt", ios::in);

  	vector<double> v_sorg2, v_sorg_err2, v_imp2, v_imp_err2;

	dati.ignore(300, '\n');

  	while (!dati.eof()) {

		dati >> Shtime;
      	dati >> sorg; //in keV
      	dati >> sorg_err;
      	dati >> imp;
      	dati >> imp_err;
      	v_sorg2.push_back(sorg);
      	v_sorg_err2.push_back(sorg_err);
      	v_imp2.push_back(imp);
      	v_imp_err2.push_back(imp_err);

 	}
  	
  	dati.close();

	dati.open("tentativo3.txt", ios::in);

  	vector<double> v_sorg3, v_sorg_err3, v_imp3, v_imp_err3;

	dati.ignore(300, '\n');

  	while (!dati.eof()) {
		
		dati >> Shtime;
      	dati >> sorg; //in keV
      	dati >> sorg_err;
      	dati >> imp;
      	dati >> imp_err;
      	v_sorg3.push_back(sorg);
      	v_sorg_err3.push_back(sorg_err);
      	v_imp3.push_back(imp);
      	v_imp_err3.push_back(imp_err);

 	}
  	
  	dati.close();


//media pesata tra i tentativi
	vector <double> v_sorg, v_sorg_err, v_imp, v_imp_err;

  	for(int i = 0; i < v_sh_time.size(); i++){
  		
  		double values[] = { v_sorg1.at(i), v_sorg2.at(i), v_sorg3.at(i) };
  		double errors[] = { v_sorg_err1.at(i), v_sorg_err2.at(i), v_sorg_err3.at(i) };


  		v_sorg.push_back( calc_media_pesata(  values, errors  ) );
  		v_sorg_err.push_back( calc_err_media_pesata(  errors  ) );
  		v_imp.push_back( calc_media_pesata(  values, errors  ) );
  		v_imp_err.push_back( calc_err_media_pesata(  errors  ) );

  	}

//analisi errori sistematici
  	vector <double> v_sistematici_sorg, v_sistematici_imp;
  	vector <double> v_sorg_err_completo, v_imp_err_completo;
  	vector <double> v_sh_time_err;

  	for(int i = 0; i < v_sh_time.size(); i++){
  		
  		v_sistematici_sorg.push_back( min_max(  v_sorg1.at(i),  v_sorg2.at(i),  v_sorg3.at(i)  ) );
  		v_sistematici_imp.push_back( min_max(  v_imp1.at(i),  v_imp2.at(i),  v_imp3.at(i)  ) );

  	}

  	for(int i = 0; i < v_sh_time.size(); i++){

  		v_sorg_err_completo.push_back( v_sorg_err.at(i) + v_sistematici_sorg.at(i) );

  		v_imp_err_completo.push_back( v_imp_err.at(i) + v_sistematici_imp.at(i) );

  		//v_sh_time_err.push_bach( 0.1 );


  	}

//grafico
  	TCanvas c;
	c.SetLeftMargin(0.15);
    c.SetBottomMargin(0.15);
 	
  	TGraphErrors * g_sorg_sh_time = new TGraphErrors( v_sh_time.size(), &v_sh_time[0], &v_sorg[0], 0, &v_sorg_err[0] );
  	TGraphErrors * g_imp_sh_time = new TGraphErrors( v_sh_time.size(), &v_sh_time[0], &v_imp[0], 0, &v_imp_err[0] );

  	g_sorg_sh_time->SetTitle(" rivelatore ");
  	g_sorg_sh_time->SetMarkerColor(6);
  	g_sorg_sh_time->SetMarkerSize(1);
  	g_sorg_sh_time->SetMarkerStyle(20);
	g_sorg_sh_time->GetXaxis()->SetTitle("shaping time [#mus]");
  	g_sorg_sh_time->GetYaxis()->SetTitle("FWHM [keV]");
	
  	g_imp_sh_time->SetTitle(" impulsatore ");
  	g_imp_sh_time->SetMarkerColor(4);
  	g_imp_sh_time->SetMarkerSize(1);
  	g_imp_sh_time->SetMarkerStyle(20);

  	g_sorg_sh_time->Draw("AP");
 	g_imp_sh_time->Draw("same") ;
    
    c.BuildLegend() ; 
    c.Print("Grafici/fwhm_sh_time.pdf", "pdf");

    theApp.Run();

	return 0;
} 
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

double functionFWHM (double * x, double * par){

    return sqrt(par[0]/x[0] + par[1]*x[0]);

}

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

	cout<<"sisetmatic error: " << sist << endl;
	return sist;

}

double calc_weighted_mean( double values[], double errors[] ){
	double media = 0;
	double err_sum = 0;
	for (int i = 0; i < 3; i++){
		media += values[i]/pow(errors[i], 2);
		err_sum += 1/pow(errors[i], 2);
	}
	media = media / err_sum;
	return media;
}

double calc_err_weighted_mean( double errors[] ){
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

//reading data
    ifstream data;
 	data.open("try1.txt", ios::in);

	vector<double> v_sh_time, v_source1, v_source_err1, v_puls1, v_puls_err1;
	double Shtime, source, source_err, puls, puls_err;

	data.ignore(300, '\n');

  	while (!data.eof()) {

		data >> Shtime;
      	data >> source; //in keV
      	data >> source_err;
      	data >> puls;
      	data >> puls_err;
      	v_sh_time.push_back(Shtime);
      	v_source1.push_back(source);
      	v_source_err1.push_back(source_err);
      	v_puls1.push_back(puls);
      	v_puls_err1.push_back(puls_err);

 	}
  	
  	data.close();

  	data.open("try2.txt", ios::in);

  	vector<double> v_source2, v_source_err2, v_puls2, v_puls_err2;

	data.ignore(300, '\n');

  	while (!data.eof()) {

		data >> Shtime;
      	data >> source; //in keV
      	data >> source_err;
      	data >> puls;
      	data >> puls_err;
      	v_source2.push_back(source);
      	v_source_err2.push_back(source_err);
      	v_puls2.push_back(puls);
      	v_puls_err2.push_back(puls_err);

 	}
  	
  	data.close();

	data.open("try3.txt", ios::in);

  	vector<double> v_source3, v_source_err3, v_puls3, v_puls_err3;

	data.ignore(300, '\n');

  	while (!data.eof()) {
		
		data >> Shtime;
      	data >> source; //in keV
      	data >> source_err;
      	data >> puls;
      	data >> puls_err;
      	v_source3.push_back(source);
      	v_source_err3.push_back(source_err);
      	v_puls3.push_back(puls);
      	v_puls_err3.push_back(puls_err);

 	}
  	
  	data.close();


//weighted mean between tries
	vector <double> v_source, v_source_err, v_puls, v_puls_err;

  	for(int i = 0; i < v_sh_time.size(); i++){
  		
  		double values_source[] = { v_source1.at(i), v_source2.at(i), v_source3.at(i) };
  		double errors_source[] = { v_source_err1.at(i), v_source_err2.at(i), v_source_err3.at(i) };

  		double values_puls[] = { v_puls1.at(i), v_puls2.at(i), v_puls3.at(i) };
  		double errors_puls[] = { v_puls_err1.at(i), v_puls_err2.at(i), v_puls_err3.at(i) };

  		v_source.push_back( calc_weighted_mean(  values_source, errors_source  ) );
  		v_source_err.push_back( calc_err_weighted_mean(  errors_source  ) );
  		v_puls.push_back( calc_weighted_mean(  values_puls, errors_puls  ) );
  		v_puls_err.push_back( calc_err_weighted_mean(  errors_puls  ) );

  	}

//analysis sistematic errors
  	vector <double> v_sistematics_source, v_sistematics_puls;
  	vector <double> v_source_err_complete, v_puls_err_complete;
  	vector <double> v_sh_time_err;

  	for(int i = 0; i < v_sh_time.size(); i++){
  		
  		v_sistematics_source.push_back( min_max(  v_source1.at(i),  v_source2.at(i),  v_source3.at(i)  ) );
  		v_sistematics_puls.push_back( min_max(  v_puls1.at(i),  v_puls2.at(i),  v_puls3.at(i)  ) );

  	}

  	for(int i = 0; i < v_sh_time.size(); i++){

  		v_source_err_complete.push_back( v_source_err.at(i) + v_sistematics_source.at(i) );

  		v_puls_err_complete.push_back( v_puls_err.at(i) + v_sistematics_puls.at(i) );

  		//v_sh_time_err.push_back( 0.1 );


  	}

//interpolation
	TF1 modelFWHM ("functionFWHM", functionFWHM, 0.5, 10, 2);
	modelFWHM.SetParName(0, "k_C"); 
	modelFWHM.SetParName(1, "k_I");
	//modelFWHM.SetParameter (0, 280); 
	modelFWHM.SetParameter (1, 3000000);


//graph
  	TCanvas c;
	c.SetLeftMargin(0.15);
    c.SetBottomMargin(0.15);
 	
 	TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle(" ; shaping time [#mus]; FWHM [keV]");

    TGraphErrors * g_source_sh_time = new TGraphErrors( v_sh_time.size(), &v_sh_time[0], &v_source[0], 0, &v_source_err_complete[0] );
  	TGraphErrors * g_puls_sh_time = new TGraphErrors( v_sh_time.size(), &v_sh_time[0], &v_puls[0], 0, &v_puls_err_complete[0] );

	g_source_sh_time->SetTitle(" rivelatore ");
  	g_source_sh_time->SetMarkerColor(6);
  	g_source_sh_time->SetMarkerSize(1);
  	g_source_sh_time->SetMarkerStyle(20);
    TFitResultPtr fit_result = g_source_sh_time->Fit (&modelFWHM, "SQ+") ;

  	
  	g_puls_sh_time->SetTitle(" impulsatore ");
  	g_puls_sh_time->SetMarkerColor(4);
  	g_puls_sh_time->SetMarkerSize(1);
  	g_puls_sh_time->SetMarkerStyle(20);
    TFitResultPtr fit_result2 = g_puls_sh_time->Fit (&modelFWHM, "SQ+") ;


  	mg->Add(g_source_sh_time);
    mg->Add(g_puls_sh_time);
    mg->Draw("AP");
    
    c.BuildLegend() ; 
   // c.Print("Graphs/fwhm_sh_time.pdf", "pdf");

    theApp.Run();

	return 0;
} 
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

    return sqrt((par[0]/x[0] + par[1]) + par[2]*x[0]);

}

using namespace std;
int main(int argc, char* argv[]){
	TApplication theApp("theApp", &argc, argv);
 	gStyle->SetOptFit(1112);

//reading data
    ifstream data;
 	data.open("elaborated_data.txt", ios::in);

	vector<double> v_sh_time, v_source, v_source_err, v_puls, v_puls_err;
	double Shtime, source, source_err, puls, puls_err;

	data.ignore(300, '\n');

  	while (!data.eof()) {

		data >> Shtime;
      	data >> source; //in keV
      	data >> source_err;
      	data >> puls;
      	data >> puls_err;
      	v_sh_time.push_back(Shtime);
      	v_source.push_back(source);
      	v_source_err.push_back(source_err);
      	v_puls.push_back(puls);
      	v_puls_err.push_back(puls_err);

 	}
  	
  	data.close();


//analysis sistematic errors
  	vector <double> v_source_err_complete, v_puls_err_complete;
  	vector <double> v_sh_time_err;

  	for(int i = 0; i < v_sh_time.size(); i++){

  		v_source_err_complete.push_back( v_source_err.at(i) + 1 ); 

  		v_puls_err_complete.push_back( v_puls_err.at(i) + 1 );

  	}


//interpolation
    TCanvas c1;
    c1.SetLeftMargin(0.20);
    c1.SetBottomMargin(0.20);
 	
 	TMultiGraph *mg1 = new TMultiGraph();
    mg1->SetTitle(" ; shaping time [#mus]; FWHM [keV]");

    TGraphErrors * g_source_sh_time1 = new TGraphErrors( v_sh_time.size(), &v_sh_time[0], &v_source[0], 0, &v_source_err_complete[0] );
  	TGraphErrors * g_puls_sh_time1 = new TGraphErrors( v_sh_time.size(), &v_sh_time[0], &v_puls[0], 0, &v_puls_err_complete[0] );

    TF1 modelFWHM ("functionFWHM", functionFWHM, 0.5, 10, 3);
    modelFWHM.SetParName(0, "C^{2} #alpha e_{n}^{2}"); 
    modelFWHM.SetParName(1, "C^{2} #gamma Af ");
    modelFWHM.SetParName(2, "#beta i_{n}^{2}");

    modelFWHM.SetParameter (0, 1750); 
    modelFWHM.SetParameter (1, 40);
    modelFWHM.SetParameter (2, 40);



	g_source_sh_time1->SetTitle(" sorgente ");
  	g_source_sh_time1->SetMarkerColor(6);
  	g_source_sh_time1->SetMarkerSize(1);
  	g_source_sh_time1->SetMarkerStyle(20);
    TFitResultPtr fit_result = g_source_sh_time1->Fit (&modelFWHM, "SQ+") ;  	
  	

    g_puls_sh_time1->SetTitle(" impulsatore ");
  	g_puls_sh_time1->SetMarkerColor(4);
  	g_puls_sh_time1->SetMarkerSize(1);
  	g_puls_sh_time1->SetMarkerStyle(20);
    TFitResultPtr fit_result2 = g_puls_sh_time1->Fit (&modelFWHM, "SQ+") ;


    mg1->Add(g_source_sh_time1);
    mg1->Add(g_puls_sh_time1);
    
    mg1->GetXaxis()->SetLabelSize(0.05);
    mg1->GetYaxis()->SetLabelSize(0.05);
    mg1->GetXaxis()->SetTitleSize(0.07);
    mg1->GetYaxis()->SetTitleSize(0.07);

    mg1->Draw("AP");

    
    c1.BuildLegend() ; 
    
//quadratic differences
    vector<double> v_diff, v_diff_err;
    for(int i = 0; i<v_sh_time.size(); i++){

    	v_diff.push_back( sqrt(pow(v_source.at(i), 2) - pow(v_puls.at(i), 2)) );
    	v_diff_err.push_back(  sqrt(pow(v_source.at(i)*v_source_err_complete.at(i), 2) + pow(v_puls.at(i)*v_puls_err_complete.at(i), 2))/sqrt(pow(v_source.at(i), 2) - pow(v_puls.at(i), 2))  );

    } 

//graph
    TCanvas c2;
	c2.SetLeftMargin(0.15);
    c2.SetBottomMargin(0.15);

    TGraphErrors * g_diff = new TGraphErrors( v_sh_time.size(), &v_sh_time[0], &v_diff[0], 0, &v_diff_err[0] );
    g_diff->SetTitle(" ; shaping time [#mus]; #sqrt{#Delta sq} [keV]");

  	g_diff->SetMarkerColor(6);
  	g_diff->SetMarkerSize(1);
  	g_diff->SetMarkerStyle(20);

  	g_diff->Draw("AP");

    theApp.Run();

	return 0;
} 
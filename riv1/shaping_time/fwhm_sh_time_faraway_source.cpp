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

    return sqrt(par[0]*(1/x[0]) + par[1]*x[0]);

}

using namespace std;

int main(int argc, char* argv[]){
	TApplication theApp("theApp", &argc, argv);
 	gStyle->SetOptFit(1112);

//reading data
    ifstream data;
 	data.open("elaborated_data_noDigit.txt", ios::in);

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

  		v_source_err_complete.push_back( v_source_err.at(i) + 1);

  		v_puls_err_complete.push_back( v_puls_err.at(i) + 1 );

  		//v_sh_time_err.push_back( 0.1 );


  	}

//interpolation
	TF1 modelFWHM ("functionFWHM", functionFWHM, 0.5, 10, 2);
	modelFWHM.SetParName(0, "C^{2} #alpha e_{n}^{2}"); 
	modelFWHM.SetParName(1, "#beta i_{n}^{2}");

	//modelFWHM.SetParameter (0, 280); 
	modelFWHM.SetParameter (1, 3000000);


//graph
  	TCanvas c;
	c.SetLeftMargin(0.20);
    c.SetBottomMargin(0.20);
 	
 	TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle(" ; shaping time [#mus]; FWHM [keV]");

    TGraphErrors * g_source_sh_time = new TGraphErrors( v_sh_time.size(), &v_sh_time[0], &v_source[0], 0, &v_source_err_complete[0] );
  	TGraphErrors * g_puls_sh_time = new TGraphErrors( v_sh_time.size(), &v_sh_time[0], &v_puls[0], 0, &v_puls_err_complete[0] );

	g_source_sh_time->SetTitle(" sorgente ");
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

    mg->GetXaxis()->SetLabelSize(0.05);
    mg->GetYaxis()->SetLabelSize(0.05);
    mg->GetXaxis()->SetTitleSize(0.07);
    mg->GetYaxis()->SetTitleSize(0.07);

    mg->Draw("AP");
    
    c.BuildLegend() ; 
   // c.Print("Graphs/fwhm_sh_time.pdf", "pdf");

//quadratic differences
    vector<double> v_diff, v_diff_err;
    for(int i = 0; i<v_sh_time.size(); i++){

    	v_diff.push_back( sqrt(pow(v_source.at(i), 2) - pow(v_puls.at(i), 2)) );
    	v_diff_err.push_back(  sqrt(pow(v_source.at(i)*v_source_err_complete.at(i), 2) + pow(v_puls.at(i)*v_puls_err_complete.at(i), 2))/sqrt(pow(v_source.at(i), 2) - pow(v_puls.at(i), 2))  );

    } 

//graph
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
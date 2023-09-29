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

double functionFWHM (double * x, double * par){

    return sqrt(par[0]/x[0] + par[1]);

}

using namespace std;

int main(int argc, char* argv[]){
	TApplication theApp("theApp", &argc, argv);
 	gStyle->SetOptFit(1112);

//reading data
    ifstream data;
 	data.open("elaborated_data_noDigit.txt", ios::in);

	vector<double> v_Vbias, v_source, v_source_err, v_puls, v_puls_err;
	double Vbias, source, source_err, puls, puls_err;

	data.ignore(300, '\n');

  	while (!data.eof()) {

		data >> Vbias;
      	data >> source; //in keV
      	data >> source_err;
      	data >> puls;
      	data >> puls_err;
      	v_Vbias.push_back(Vbias);
      	v_source.push_back(source);
      	v_source_err.push_back(source_err);
      	v_puls.push_back(puls);
      	v_puls_err.push_back(puls_err);

 	}
  	
  	data.close();


//analysis sistematic errors
  	vector <double> v_source_err_complete, v_puls_err_complete;
  	vector <double> v_Vbias_err;

  	for(int i = 0; i < v_Vbias.size(); i++){

  		v_source_err_complete.push_back( v_source_err.at(i) + 1 );

  		v_puls_err_complete.push_back( v_puls_err.at(i) + 1 );

  		cout<<"Vbias: "<<v_Vbias.at(i) << endl;
  		cout<<"  source: "<<v_source.at(i) << " +- "<<v_source_err_complete.at(i)<< endl;
  	  	cout<<"  pulser: "<<v_puls.at(i) << " +- "<<v_puls_err_complete.at(i)<<"\n"<< endl;


  	}

//interpolation
	TF1 modelFWHM ("functionFWHM", functionFWHM, 5, 70, 2);
	modelFWHM.SetParName(0, "#alpha'"); 
	modelFWHM.SetParName(1, "#gamma'");
	modelFWHM.SetParameter (0, 10000); 
	modelFWHM.SetParameter (1, 20);

//graph
  	TCanvas c;
	c.SetLeftMargin(0.20);
    c.SetBottomMargin(0.20);
 	
 	TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle(" ; V bias [V]; FWHM [keV]");

    cout<<"v_Vbias.size(): "<<v_Vbias.size()<<endl;
    TGraphErrors * g_source_Vbias = new TGraphErrors( v_Vbias.size(), &v_Vbias[0], &v_source[0], &v_Vbias_err[0], &v_source_err_complete[0] );
  	TGraphErrors * g_puls_Vbias = new TGraphErrors( v_Vbias.size(), &v_Vbias[0], &v_puls[0], &v_Vbias_err[0], &v_puls_err_complete[0] );

	g_source_Vbias->SetTitle(" sorgente ");
  	g_source_Vbias->SetMarkerColor(6);
  	g_source_Vbias->SetMarkerSize(1);
  	g_source_Vbias->SetMarkerStyle(20);
    TFitResultPtr fit_result = g_source_Vbias->Fit (&modelFWHM, "SQ+") ;

	
  	g_puls_Vbias->SetTitle(" impulsatore ");
  	g_puls_Vbias->SetMarkerColor(4);
  	g_puls_Vbias->SetMarkerSize(1);
  	g_puls_Vbias->SetMarkerStyle(20);
    TFitResultPtr fit_result2 = g_puls_Vbias->Fit (&modelFWHM, "SQ+") ;


    mg->Add(g_source_Vbias);
    mg->Add(g_puls_Vbias);

    mg->GetXaxis()->SetLabelSize(0.05);
    mg->GetYaxis()->SetLabelSize(0.05);
    mg->GetXaxis()->SetTitleSize(0.07);
    mg->GetYaxis()->SetTitleSize(0.07);


    mg->Draw("AP");
    
    c.BuildLegend() ; 
  //  c.Print("Graphs/fwhm_Vbias.pdf", "pdf");

 //quadratic differences
    vector<double> v_diff, v_diff_err;
    for(int i = 0; i<v_Vbias.size(); i++){

    	v_diff.push_back( sqrt(pow(v_source.at(i), 2) - pow(v_puls.at(i), 2)) );
    	v_diff_err.push_back(  sqrt(pow(v_source.at(i)*v_source_err_complete.at(i), 2) + pow(v_puls.at(i)*v_puls_err_complete.at(i), 2))/sqrt(pow(v_source.at(i), 2) - pow(v_puls.at(i), 2))  );

    } 

//graph
    TCanvas c1;
	c1.SetLeftMargin(0.15);
    c1.SetBottomMargin(0.15);

    TGraphErrors * g_diff = new TGraphErrors( v_Vbias.size(), &v_Vbias[0], &v_diff[0], 0, &v_diff_err[0] );
    g_diff->SetTitle(" ; Vbias [V]; #sqrt{#Delta sq} [keV]");

  	g_diff->SetMarkerColor(6);
  	g_diff->SetMarkerSize(1);
  	g_diff->SetMarkerStyle(20);

  	g_diff->Draw("AP");

    theApp.Run();
	
	return 0;
} 
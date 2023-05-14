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
int main(int argc, char* argv[]){
	TApplication theApp("theApp", &argc, argv);
 	gStyle->SetOptFit(1112);
//lettura dati
    ifstream dati;
 	dati.open("Dati/dati_Ceq.txt", ios::in);

	vector<double> v_alim, v_ceq;
    double Valim, Ceq;
	
	dati.ignore(50, '\n');
  	while (!dati.eof()) {
 
     	dati >> Valim; // in V
     	cout<<"Valim"<<Valim<<endl;
      	dati >> Ceq; // in pF
      	v_alim.push_back(Valim);
      	v_ceq.push_back(Ceq);

 	}
  	
  	dati.close();

//manipolazione dati ed errori
  	double Ccavo_Cs [] = {127.88, 3}; // in pF
	vector<double> v_criv;
	vector<double> v_criv_err, v_alim_err;

  	for(int i = 0; i < v_alim.size(); i++){
  		v_criv.push_back( pow(10, -12)*(v_ceq.at(i) - Ccavo_Cs[0]));

      	v_criv_err.push_back( pow(10, -12)*sqrt(pow(3, 2) + pow(Ccavo_Cs[1], 2)) );
        cout<<"v_criv_err.at(i): "<<pow(10, -12)*sqrt(pow(1, 2) + pow(Ccavo_Cs[1], 2)) << "F" <<endl;
      	if(i < 8){
      		v_alim_err.push_back( sqrt(pow(0.01, 2)) );
      	}
      	else{
      		v_alim_err.push_back( sqrt(pow(0.1, 2)) );
      	}

  	}

//grafico
  	TCanvas c;
	c.SetLeftMargin(0.15);
    c.SetBottomMargin(0.15);
 	
  	TGraphErrors g_Ceq(v_alim.size(), &v_alim[0], &v_criv[0], &v_alim_err[0], &v_criv_err[0]);

  	g_Ceq.SetTitle(" ");
  	g_Ceq.SetMarkerStyle(20);
  	g_Ceq.SetMarkerSize(0.3);
	g_Ceq.GetXaxis()->SetTitle("V al rivelatore [V]");
  	g_Ceq.GetYaxis()->SetTitle("C del rivelatore [F]");


 	g_Ceq.Draw("AP") ;
    
    c.Print("Grafici/grafico_Ceq.pdf", "pdf") ; 

    theApp.Run();

	return 0;
}

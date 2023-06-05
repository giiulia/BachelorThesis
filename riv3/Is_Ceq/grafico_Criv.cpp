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

double funzioneC(double * x, double * par){
	return par[1]+sqrt(par[0]/abs(x[0]+par[2]));
}

double funzioned(double * x, double * par){
	return pow(10, 15)*8.85*pow(10, -12)*12*0.0009/(par[1]+sqrt(par[0]/abs(x[0]+par[2])));
}

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
  	double Ccavo_Cs [] = {128, 5}; // in pF 
  	double Cs[] = {4380000, 200000}; // in pF 
  	double Ccavo[2];

	vector<double> v_criv;
	vector<double> v_criv_err, v_alim_err;


	double valore_Ccavo = Cs[0]*Ccavo_Cs[0]/(Cs[0] - Ccavo_Cs[0]);
	double sigma_Ccavo = sqrt(  pow(Cs[1]*Ccavo_Cs[0]*Ccavo_Cs[0]/pow(Ccavo_Cs[0] - Cs[0], 2), 2) + pow(Ccavo_Cs[1]*Cs[0]*Cs[0]/pow(Ccavo_Cs[0] - Cs[0], 2), 2)  );
	Ccavo[0] = valore_Ccavo;
	Ccavo[1] = sigma_Ccavo;
	cout<<"Ccavo: "<<valore_Ccavo<< " +- "<<sigma_Ccavo<<" pF"<<endl;
  	for(int i = 0; i < v_alim.size(); i++){
  		v_criv.push_back(  pow(10, -3)*( (Ccavo[0]*(Cs[0]-v_ceq.at(i))-(Cs[0]*v_ceq.at(i)))/(v_ceq.at(i) - Cs[0]) )  ); // il 10^-3 è per ottenere i nF nel grafico
  		cout<<"v_criv.at(i): "<<v_criv.at(i)<<endl;

      	v_criv_err.push_back( pow(10, -3)*sqrt(  pow(5*Cs[0]*Cs[0]/pow(v_ceq.at(i)-Cs[0], 2), 2) + pow(Ccavo[1], 2) + pow(Cs[1]*v_ceq.at(i)*v_ceq.at(i)/pow(v_ceq.at(i)-Cs[0], 2), 2)  ));
        cout<<"v_criv_err.at(i): "<<pow(10, -3)*sqrt(  pow(5*Cs[0]*Cs[0]/pow(v_ceq.at(i)-Cs[0], 2), 2) + pow(Ccavo[1], 2) + pow(Cs[1]*v_ceq.at(i)*v_ceq.at(i)/pow(v_ceq.at(i)-Cs[0], 2), 2)  ) << "nF" <<endl;
      	if(i < 15){
      		v_alim_err.push_back( sqrt(pow(0.01, 2)) );
      	}
      	else{
      		v_alim_err.push_back( sqrt(pow(0.1, 2)) );
      	}

  	}

//interpolazione
	TF1 modelloC ("funzioneC", funzioneC, 0, 90, 3);
  	modelloC.SetParName(0, "k");
	modelloC.SetParameter (0, 11.5); 
	modelloC.SetParName(1, "offset");
	modelloC.SetParameter (1, 0.05);
	modelloC.SetParName(2, "drop");
	modelloC.SetParameter (2, -0.00001);

//grafico
  	TCanvas c;
	c.SetLeftMargin(0.15);
    c.SetBottomMargin(0.15);
 	
  	TGraphErrors g_Ceq(v_alim.size(), &v_alim[0], &v_criv[0], &v_alim_err[0], &v_criv_err[0]);

  	g_Ceq.SetTitle(" ");
  	g_Ceq.SetMarkerStyle(20);
  	g_Ceq.SetMarkerSize(0.3);
  	g_Ceq.GetXaxis()->SetTitleSize(0.05);
    g_Ceq.GetYaxis()->SetTitleSize(0.05);
    g_Ceq.GetXaxis()->SetLabelSize(0.05);
    g_Ceq.GetYaxis()->SetLabelSize(0.05);
	g_Ceq.GetXaxis()->SetTitle("V al rivelatore [V]");
  	g_Ceq.GetYaxis()->SetTitle("C del rivelatore [nF]");

    TFitResultPtr fit_result = g_Ceq.Fit (&modelloC, "SQ+") ;

 	g_Ceq.Draw("AP") ;
    
 //   c.Print("Grafici/grafico_Ceq.pdf", "pdf") ; 

//regione di svuotamento
	vector<double> v_d, v_d_err;
	double A = 0.0009; //900 mm^2 in m^2

	for (int i = 0; i < v_alim.size(); i++){
		v_d.push_back(  8.85*pow(10, -6)*12*A/(v_criv.at(i)*pow(10, -9))  ); //il 10^6 serve per ottenere dei micrometri
		v_d_err.push_back(  pow(10, -3)*v_criv_err.at(i)*8.85*pow(10, -12)*12*A/pow((v_criv.at(i)*pow(10, -9)), 2)  );

	}

//interpolazione regione di svuotamento
	TF1 modellod ("funzioned", funzioned, 0, 90, 3);
  	modellod.SetParName(0, "k");
	modellod.SetParameter (0, 11.5); 
	modellod.SetParName(1, "offset");
	modellod.SetParameter (1, 0.05);
	modellod.SetParName(2, "drop");
	modellod.SetParameter (2, -0.00001);
	

//grafico regione di svuotamento
  	TCanvas c1;
	c1.SetLeftMargin(0.15);
    c1.SetBottomMargin(0.15);
 	
  	TGraphErrors g_d(v_alim.size(), &v_alim[0], &v_d[0], &v_alim_err[0], &v_d_err[0]);

  	g_d.SetTitle(" ");
  	g_d.SetMarkerStyle(20);
  	g_d.SetMarkerSize(0.3);
  	g_d.GetXaxis()->SetTitleSize(0.05);
    g_d.GetYaxis()->SetTitleSize(0.05);
    g_d.GetXaxis()->SetLabelSize(0.05);
    g_d.GetYaxis()->SetLabelSize(0.05);
	g_d.GetXaxis()->SetTitle("V al rivelatore [V]");
  	g_d.GetYaxis()->SetTitle("regione di svuotamento [#mum]");
  
    TFitResultPtr fit_result2 = g_d.Fit (&modellod, "SQ+") ;

 	g_d.Draw("AP") ;
    
   // c1.Print("Grafici/grafico_d.pdf", "pdf") ; 


    theApp.Run();

	return 0;
}

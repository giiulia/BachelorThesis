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
 	dati.open("Dati/dati_Is.txt", ios::in);

	vector<double> v_alim, v_fuga;
    double Valim, Vfuga;
	
	dati.ignore(50, '\n');
  	while (!dati.eof()) {

     	dati >> Valim;
      	dati >> Vfuga;
      	v_alim.push_back(-Valim);
      	v_fuga.push_back(-Vfuga);

 	}
  	
  	dati.close();

//manipolazione dati ed errori
  	double R[] = {100.4, 0.1}; //kOhm

	vector<double> v_riv, v_corrente; // la tensione di alimentazione che effettivamente arriva al rivelatore è meno di v Alim
	vector<double> v_riv_err, v_corrente_err;

  	for(int i = 0; i < v_alim.size(); i++){
  		v_riv.push_back(v_alim.at(i) - v_fuga.at(i));

      	v_corrente.push_back(v_fuga.at(i)*pow(10, -3)/R[0]);
        v_corrente_err.push_back( sqrt(pow(0.001*pow(10, -3)/R[0], 2) + pow(v_fuga.at(i)*R[1]*pow(10, -3)/(R[0]*R[0]), 2)) );
        cout<<"v_corrente_err.at(i): "<<sqrt(pow(0.001*pow(10, -3)/R[0], 2) + pow(v_fuga.at(i)*R[1]*pow(10, -3)/(R[0]*R[0]), 2)) <<endl;
      	if(i < 8){
      		v_riv_err.push_back( sqrt(pow(0.01, 2) + pow(0.001, 2)) );
      	}
      	else{
      		v_riv_err.push_back( sqrt(pow(0.1, 2) + pow(0.001, 2)) );
      	}

  	}

//grafico
  	TCanvas c;
	c.SetLeftMargin(0.15);
    c.SetBottomMargin(0.15);
 	
  	TGraphErrors g_Is(v_alim.size(), &v_riv[0], &v_corrente[0], &v_riv_err[0], &v_corrente_err[0]);

  	g_Is.SetTitle(" ");
  	g_Is.SetMarkerStyle(20);
	g_Is.GetXaxis()->SetTitle("V al rivelatore [V]");
  	g_Is.GetYaxis()->SetTitle("I fuga [A]");


 	g_Is.Draw("AP") ;
    
    c.Print("Grafici/grafico_Is.pdf", "pdf") ; 


//analisi
    double Rvoltmetro = 10000; //10 MOhm in kOhm
    double Req = Rvoltmetro*R[0]/( Rvoltmetro + R[0] ); // in kOhm
  	cout<<"I(1)/I(tot)%: "<<Req*100/R[0]<<endl;

	for(int i = 0; i < v_fuga.size(); i++){
  		cout<<"I(1): "<<v_fuga.at(i)*pow(10, -3)/R[0]<<" A;"<<"		I(2): "<<v_fuga.at(i)*pow(10, -3)*(1/Req - 1/R[0])<<" A"<<endl;
  	}

    theApp.Run();

	return 0;
}


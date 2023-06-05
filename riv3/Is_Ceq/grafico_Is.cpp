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

double funzioneIs (double * x, double * par){

    return par[0]+par[1]*(exp(x[0]/par[2]) - 1) ;

}

using namespace std;
int main(int argc, char* argv[]){
	TApplication theApp("theApp", &argc, argv);
 	gStyle->SetOptFit(1112);
//lettura dati
    ifstream dati;
 	dati.open("Dati/dati_Is_29.5_21gradi.txt", ios::in);

	vector<double> v_alim, v_fuga;
    double Valim, Vfuga;
	
	dati.ignore(50, '\n');
  	while (!dati.eof()) {

     	dati >> Valim;
      	dati >> Vfuga;
      	v_alim.push_back(-Valim); //in V
      	v_fuga.push_back(-Vfuga); //in mV

 	}
  	
  	dati.close();

//analisi
  	//double R[] = {0.1004, 0.0001}; //MOhm
  	double R[] = {0.987, 0.001}; //MOhm

    double Rvoltmetro[] = {10, 0.01}; //MOhm 
    double Req[] = {Rvoltmetro[0]*R[0]/( Rvoltmetro[0] + R[0] ),  sqrt( pow(  (R[0]*R[0]*Rvoltmetro[1]/pow(R[0]+Rvoltmetro[0], 2)  ), 2) + pow(  (Rvoltmetro[0]*Rvoltmetro[0]*R[1]/pow(Rvoltmetro[0]*R[0], 2)  ), 2) )}; // in MOhm
  	cout<<"Req[0]: "<<Req[0]<<"MOhm; Req[1]: "<<Req[1]<<"MOhm"<<endl;
  	cout<<"I(1)/I(tot)%: "<<Req[0]*100/R[0]<<endl;

	for(int i = 0; i < v_fuga.size(); i++){
  		cout<<"I(1): "<<v_fuga.at(i)/R[0]<<" nA;"<<"		I(2): "<<v_fuga.at(i)*(1/Req[0] - 1/R[0])<<" nA"<<endl; //non trascurabile!!
  	}

//manipolazione dati ed errori

	vector<double> v_riv, v_corrente; // la tensione di alimentazione che effettivamente arriva al rivelatore è meno di v Alim
	vector<double> v_riv_err, v_corrente_err;

  	for(int i = 0; i < v_alim.size(); i++){
  		v_riv.push_back(v_alim.at(i) - v_fuga.at(i)*pow(10, -3));

      	v_corrente.push_back(v_fuga.at(i)/R[0] + v_fuga.at(i)*(1/Req[0] - 1/R[0])); //correzione, la corrente è di più di quella che passa su Rc. (Espressa in nA)
        v_corrente_err.push_back( sqrt(2*pow(0.1/R[0], 2) + 2*pow(v_fuga.at(i)*R[1]/(R[0]*R[0]), 2) + pow(0.1/Req[0], 2) + pow(Req[1]*v_fuga.at(i)/(Req[0]*Req[0]), 2) ) );
        cout<<"corrente ERRORE: errore dovuto alla tesione= "<<0.1/R[0] <<" errore dovuto alla resistenza= "<< v_fuga.at(i)*R[1]/(R[0]*R[0])<<" nA "<<"errore dovuto alla correzione= "<<sqrt(pow(0.1/Req[0], 2) + pow(Req[1]*v_fuga.at(i)/(Req[0]*Req[0]), 2) + pow(0.1/R[0], 2) + pow(v_fuga.at(i)*R[1]/(R[0]*R[0]), 2) ) <<endl;
      	
      	v_riv_err.push_back( sqrt(pow(0.01, 2) + pow(0.0001, 2)) );

  	}

//interpolazione
	TF1 modelloIs ("funzioneIs", funzioneIs, -90, 0, 3);
	modelloIs.SetParName(0, "offset"); //come se ci fosse una corrente non ideale in più
	modelloIs.SetParName(1, "Is");
  	modelloIs.SetParName(2, "n V_{th}");
	//modelloIs.SetParameter (0, 260); 
	modelloIs.SetParameter (1, 60); 
	modelloIs.SetParameter (2, 52000);

//grafico
  	TCanvas c;
	c.SetLeftMargin(0.15);
    c.SetBottomMargin(0.15);
 	
  	TGraphErrors g_Is(v_alim.size(), &v_riv[0], &v_corrente[0], &v_riv_err[0], &v_corrente_err[0]);

  	g_Is.SetTitle(" ");
  	g_Is.SetMarkerSize(0.5);
  	g_Is.SetMarkerStyle(20);
  	g_Is.GetXaxis()->SetTitleSize(0.05);
    g_Is.GetYaxis()->SetTitleSize(0.05);
    g_Is.GetXaxis()->SetLabelSize(0.05);
    g_Is.GetYaxis()->SetLabelSize(0.05);
	g_Is.GetXaxis()->SetTitle("V al rivelatore [V]");
  	g_Is.GetYaxis()->SetTitle("I fuga [nA]");

    TFitResultPtr fit_result = g_Is.Fit (&modelloIs, "SQ+") ;

 	g_Is.Draw("AP") ;
    
    //c.Print("Grafici/grafico_Is.pdf", "pdf") ; 

  //  c.Print("Grafici/grafico_Is.pdf", "pdf") ; 

    theApp.Run();

	return 0;
}


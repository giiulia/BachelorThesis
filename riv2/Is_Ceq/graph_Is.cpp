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

double functionIs (double * x, double * par){

    return par[0]+par[1]*(exp(-x[0]/par[2]) - 1) ;

}

using namespace std;
int main(int argc, char* argv[]){
	TApplication theApp("theApp", &argc, argv);
 	gStyle->SetOptFit(1112);

//reading data
    ifstream data;
 	data.open("Data/data_Is_12.5_21degrees.txt", ios::in);

	vector<double> v_alim, v_leak;
    double Valim, Vleak;
	
	data.ignore(50, '\n');
  	while (!data.eof()) {

     	data >> Valim;
      	data >> Vleak;
      	v_alim.push_back(Valim); //in V
      	v_leak.push_back(-Vleak); //in mV

 	}
  	
  	data.close();

//analysis
  	//double R[] = {0.1004, 0.0001}; //MOhm
  	double R[] = {0.987, 0.001}; //MOhm

    double Rmetrometer[] = {10, 0.01}; //MOhm 
    double Req[] = {Rmetrometer[0]*R[0]/( Rmetrometer[0] + R[0] ),  sqrt( pow(  (R[0]*R[0]*Rmetrometer[1]/pow(R[0]+Rmetrometer[0], 2)  ), 2) + pow(  (Rmetrometer[0]*Rmetrometer[0]*R[1]/pow(Rmetrometer[0]*R[0], 2)  ), 2) )}; // in MOhm
  	cout<<"Req[0]: "<<Req[0]<<"MOhm; Req[1]: "<<Req[1]<<"MOhm"<<endl;
  	cout<<"I(1)/I(tot)%: "<<Req[0]*100/R[0]<<endl;

	for(int i = 0; i < v_leak.size(); i++){
  		cout<<"I(1): "<<v_leak.at(i)/R[0]<<" nA;"<<"		I(2): "<<v_leak.at(i)*(1/Req[0] - 1/R[0])<<" nA"<<endl; //not negligeble!!
  	}

//data manipulation ed errors

	vector<double> v_det, v_current; //the tension of alimetation that arrives at the detector is less than v Alim
	vector<double> v_det_err, v_current_err;

  	for(int i = 0; i < v_alim.size(); i++){
  		v_det.push_back(v_alim.at(i) + v_leak.at(i)*pow(10, -3));

      	v_current.push_back(v_leak.at(i)/R[0] + v_leak.at(i)*(1/Req[0] - 1/R[0])); //correction, the current is more than the one on Rload. (Expressed in nA)
        v_current_err.push_back( sqrt(2*pow(0.1/R[0], 2) + 2*pow(v_leak.at(i)*R[1]/(R[0]*R[0]), 2) + pow(0.1/Req[0], 2) + pow(Req[1]*v_leak.at(i)/(Req[0]*Req[0]), 2) ) );
        cout<<"current ERROR: error due to tesion = "<<0.1/R[0] <<" error due to resistance = "<< v_leak.at(i)*R[1]/(R[0]*R[0])<<" nA "<<"error due to correction = "<<sqrt(pow(0.1/Req[0], 2) + pow(Req[1]*v_leak.at(i)/(Req[0]*Req[0]), 2) + pow(0.1/R[0], 2) + pow(v_leak.at(i)*R[1]/(R[0]*R[0]), 2) ) <<endl;
      	
      	v_det_err.push_back( sqrt(pow(0.01, 2) + pow(0.0001, 2)) );

  	}

//interpolation
	TF1 modelIs ("functionIs", functionIs, -90, 0, 3);
	modelIs.SetParName(0, "offset"); //ideal current added
	modelIs.SetParName(1, "Is");
  	modelIs.SetParName(2, "n V_{th}");
	//modelIs.SetParameter (0, 260); 
	modelIs.SetParameter (1, 60); 
	modelIs.SetParameter (2, 52000);

//graph
  	TCanvas c;
	c.SetLeftMargin(0.20);
    c.SetBottomMargin(0.20);
 	
  	TGraphErrors g_Is(v_alim.size(), &v_det[0], &v_current[0], &v_det_err[0], &v_current_err[0]);

  	g_Is.SetTitle(" ");
  	g_Is.SetMarkerSize(0.5);
  	g_Is.SetMarkerStyle(20);
  	g_Is.GetXaxis()->SetTitleSize(0.07);
    g_Is.GetYaxis()->SetTitleSize(0.07);
    g_Is.GetXaxis()->SetLabelSize(0.05);
    g_Is.GetYaxis()->SetLabelSize(0.05);
	g_Is.GetXaxis()->SetTitle("V al rivelatore [V]");
  	g_Is.GetYaxis()->SetTitle("I fuga [nA]");

    TFitResultPtr fit_result = g_Is.Fit (&modelIs, "SQ+") ;

 	g_Is.Draw("AP") ;
    
  //  c.Print("Graphs/graph_Is.pdf", "pdf") ; 

    theApp.Run();

	return 0;
}


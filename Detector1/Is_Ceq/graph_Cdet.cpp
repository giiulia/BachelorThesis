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

double functionC(double * x, double * par){
	return par[1]+sqrt(par[0]/abs(x[0]+par[2]));
}

double functiond(double * x, double * par){
	return pow(10, 15)*8.85*pow(10, -12)*12*0.0009/(par[1]+sqrt(par[0]/abs(x[0]+par[2])));
}

using namespace std;
int main(int argc, char* argv[]){
	TApplication theApp("theApp", &argc, argv);
 	gStyle->SetOptFit(1112);

//reading data
    ifstream data;
 	data.open("Data/data_Ceq.txt", ios::in);

	vector<double> v_alim, v_ceq;
    double Valim, Ceq;
	
	data.ignore(50, '\n');
  	while (!dati.eof()) {
 
     	data >> Valim; // in V
      	data >> Ceq; // in pF
      	v_alim.push_back(Valim);
      	v_ceq.push_back(Ceq);

 	}
  	
  	data.close();

//manipulation data ed errors
  	double Ccable_Cs [] = {128, 5}; // in pF 	ERROR=2%  
  	double Cs[] = {4380000, 200000}; // in pF 
  	double Ccable[2];

	vector<double> v_cdet;
	vector<double> v_cdet_err, v_alim_err;


	double value_Ccable = Cs[0]*Ccable_Cs[0]/(Cs[0] - Ccable_Cs[0]);
	double sigma_Ccable = sqrt(  pow(Cs[1]*Ccable_Cs[0]*Ccable_Cs[0]/pow(Ccable_Cs[0] - Cs[0], 2), 2) + pow(Ccable_Cs[1]*Cs[0]*Cs[0]/pow(Ccable_Cs[0] - Cs[0], 2), 2)  );
	Ccable[0] = value_Ccable;
	Ccable[1] = sigma_Ccable;
	cout<<"Ccable: "<<value_Ccable<< " +- "<<sigma_Ccable<<" pF"<<endl;
  	for(int i = 0; i < v_alim.size(); i++){
  		v_cdet.push_back(  pow(10, -3)*( (Ccable[0]*(Cs[0]-v_ceq.at(i))-(Cs[0]*v_ceq.at(i)))/(v_ceq.at(i) - Cs[0]) )  ); //in nF
  		cout<<"v_cdet.at(i): "<<v_cdet.at(i)<<endl;
      	v_cdet_err.push_back( pow(10, -3)*sqrt(  pow(5*Cs[0]*Cs[0]/pow(v_ceq.at(i)-Cs[0], 2), 2) + pow(Ccable[1], 2) + pow(Cs[1]*v_ceq.at(i)*v_ceq.at(i)/pow(v_ceq.at(i)-Cs[0], 2), 2)  ));
        cout<<"v_cdet_err.at(i): "<<pow(10, -3)*sqrt(  pow(5*Cs[0]*Cs[0]/pow(v_ceq.at(i)-Cs[0], 2), 2) + pow(Ccable[1], 2) + pow(Cs[1]*v_ceq.at(i)*v_ceq.at(i)/pow(v_ceq.at(i)-Cs[0], 2), 2)  ) << "nF" <<endl;
      	if(i < 8){
      		v_alim_err.push_back( sqrt(pow(0.01, 2)) );
      	}
      	else{
      		v_alim_err.push_back( sqrt(pow(0.1, 2)) );
      	}

  	}

//interpolation
	TF1 modelC ("functionC", functionC, 0, 90, 3);
  	modelC.SetParName(0, "k");
	modelC.SetParameter (0, 11.5); 
	modelC.SetParName(1, "offset");
	modelC.SetParameter (1, 0.05);
	modelC.SetParName(2, "drop");
	modelC.SetParameter (2, -0.00001);
	
//graph
  	TCanvas c;
	c.SetLeftMargin(0.20);
    c.SetBottomMargin(0.20);
 	
  	TGraphErrors g_Ceq(v_alim.size(), &v_alim[0], &v_cdet[0], &v_alim_err[0], &v_cdet_err[0]);

  	g_Ceq.SetTitle(" ");
  	g_Ceq.SetMarkerStyle(20);
  	g_Ceq.SetMarkerSize(0.3);
	g_Ceq.GetXaxis()->SetTitleSize(0.07);
    g_Ceq.GetYaxis()->SetTitleSize(0.07);
    g_Ceq.GetXaxis()->SetLabelSize(0.05);
    g_Ceq.GetYaxis()->SetLabelSize(0.05);
	g_Ceq.GetXaxis()->SetTitle("V al rivelatore [V]");
  	g_Ceq.GetYaxis()->SetTitle("C del rivelatore [nF]");

    TFitResultPtr fit_result = g_Ceq.Fit (&modelC, "SQ+") ;

 	g_Ceq.Draw("AP") ;
    
    //c.Print("Graphs/graph_Cdet.pdf", "pdf") ; 

//depletion region
	vector<double> v_d, v_d_err;
	double A = 0.0009; //900 mm^2 in m^2

	for (int i = 0; i < v_alim.size(); i++){
		v_d.push_back(  8.85*pow(10, -6)*12*A/(v_cdet.at(i)*pow(10, -9))  ); //in micrometers
		cout<<"d: "<<v_d.at(i)<<endl;
		v_d_err.push_back(  pow(10, -3)*v_cdet_err.at(i)*8.85*pow(10, -12)*12*A/pow((v_cdet.at(i)*pow(10, -9)), 2)  );
		cout<<"d_err: "<<v_d_err.at(i)<<endl;


	}

//interpolation depletion region
	TF1 modeld ("functiond", functiond, 0, 90, 3);
  	modeld.SetParName(0, "k");
	modeld.SetParameter (0, 11.5); 
	modeld.SetParName(1, "offset");
	modeld.SetParameter (1, 0.05);
	modeld.SetParName(2, "drop");
	modeld.SetParameter (2, -0.00001);

//graph depletion region
  	TCanvas c1;
	c1.SetLeftMargin(0.15);
    c1.SetBottomMargin(0.15);
 	
  	TGraphErrors g_d(v_alim.size(), &v_alim[0], &v_d[0], &v_alim_err[0], &v_d_err[0]);

  	g_d.SetTitle(" ");
  	g_d.SetMarkerStyle(20);
  	g_d.SetMarkerSize(0.3);
    g_d.GetXaxis()->SetTitleSize(0.07);
    g_d.GetYaxis()->SetTitleSize(0.07);
    g_d.GetXaxis()->SetLabelSize(0.05);
    g_d.GetYaxis()->SetLabelSize(0.05);
	g_d.GetXaxis()->SetTitle("V al rivelatore [V]");
  	g_d.GetYaxis()->SetTitle("regione di svuotamento [#mum]");
  
    TFitResultPtr fit_result2 = g_d.Fit (&modeld, "SQ+") ;

 	g_d.Draw("AP") ;
    
   // c1.Print("Graphs/graph_d.pdf", "pdf") ; 


    theApp.Run();

	return 0;
}

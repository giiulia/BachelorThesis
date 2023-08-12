
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>

#include "TGraph2D.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TPad.h"
#include "TStyle.h"
#include "TAxis.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "controlli.h"
#include "alpha_class.h"


#define N 1e8 //tentativi
#define diametroRivelatore 3.2 //in cm
#define spessoreMorto 0.000005 //in cm (50nm) 
#define diametroSorgente 0.7 //in cm - da misura con calibro, controllare?


int main (int argc, char ** argv){
    double L = 3; //distanza tra sorgente e base superiore strato attivo in cm

    TApplication theApp("theApp", &argc, argv);
    srand(time(NULL)); 
    double path_lenght;

    TH1F h_path("3cm", " ", sqrt(N), 0.015, spessoreMorto*15);

    double r;  
    double rho;
    double x0;
    double y0;

    int j=0;
    int conteggi = 0;
    double max_path = 0;
    double max_lenght_theta = 0;
    

    while (j < N){
      alpha alpha(rand_range(-M_PI/2, 0), rand_phi(0, 2*M_PI, 2*M_PI));
      r = rand_range(0, diametroSorgente/2.);
      rho = rand_phi(0, 2*M_PI, 2*M_PI);
      x0 = r*cos(rho);
      y0 = r*sin(rho);
      //std::cout<<"x0: "<<x0<<"\t"<<"y0: "<<y0<<std::endl;

      if (is_coincidence(L, alpha.coeff_x(), alpha.coeff_y(), x0, y0) == true){
        path_lenght = get_path(x0, y0, alpha.get_theta(), alpha.get_phi(), spessoreMorto, alpha.coeff_x(), alpha.coeff_y(), x_coinc_coord(L, alpha.coeff_x(), x0), y_coinc_coord(L, alpha.coeff_y(), y0));
        if(path_lenght>max_path){
          max_path = path_lenght;
          max_lenght_theta = alpha.get_theta();
        }
        h_path.Fill(path_lenght);
        conteggi++;

      }
      if (j % 10000000 == 0){
      	std::cout << "Svolte/Totale: \t" << j << "/" << N << "\n";
      }
      j++;
    }
    

    std::cout.precision(3);
    std::cout << "il " << conteggi*100./pow(10, 8) << " % " << " di " << N << " alpha sono passati sul rilevatore" << "\n" ;

    std::ofstream outfile_media("percorsi.txt", std::ios::app);
    if (outfile_media.is_open())
    {
      outfile_media << L << "\t\t"<< max_path <<"\t\t"<< max_lenght_theta*360/(2*M_PI) <<"\t\t\t\t"<< h_path.GetStdDev(1) << std::endl;
      outfile_media.close();
    }

    gStyle->SetPalette(1);

    TCanvas c1;
    c1.SetLeftMargin(.15);
    c1.SetBottomMargin(.15);


    h_path.GetXaxis()->SetTitle(" distanza percorsa [cm]");
    h_path.GetYaxis()->SetTitle(" conteggi ");
    h_path.GetXaxis()->SetTitleSize(0.07);
    h_path.GetYaxis()->SetTitleSize(0.07);
    h_path.GetXaxis()->SetLabelSize(0.05);
    h_path.GetYaxis()->SetLabelSize(0.05);
    h_path.SetFillColor (kOrange + 1) ;
    h_path.Draw ();
    std::string file = "hist_";
    std::string distanza = std::to_string(L);
    std::string estensione = ".pdf";
    c1.SetLogy();
    c1.Print ((file+distanza+estensione).c_str(), "pdf") ;
   


    theApp.Run();    

  
    return 0;
  }
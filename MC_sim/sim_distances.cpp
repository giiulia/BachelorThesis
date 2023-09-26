
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


#define N 1e8 //tries
#define DetectorDiameter 3.4 //in cm
#define DeadLayer 0.000005 //in cm (50nm) 
#define SourceDiameter 0.7 //in cm - measure with caliber


int main (int argc, char ** argv){
  double L = 5.7; //distance between source and active layer in cm

  TApplication theApp("theApp", &argc, argv);
  srand(time(NULL)); 
  double path_lenght;

  TH1F h_path("5.7cm", " ", sqrt(N), 0.015, DeadLayer*1000);

  double r;  
  double rho;
  double x0;
  double y0;

  int j=0;
  int counts = 0;
  double max_path = 0;
  double max_lenght_theta = 0;
  

  while (j < N){
    alpha alpha(rand_range(-M_PI/2, 0), rand_phi(0, 2*M_PI, 2*M_PI));
    r = rand_range(0, SourceDiameter/2.);
    rho = rand_phi(0, 2*M_PI, 2*M_PI);
    x0 = r*cos(rho);
    y0 = r*sin(rho);
    //std::cout<<"x0: "<<x0<<"\t"<<"y0: "<<y0<<std::endl;

    if (is_coincidence(L, alpha.coeff_x(), alpha.coeff_y(), x0, y0) == true){
      path_lenght = get_path(x0, y0, alpha.get_theta(), alpha.get_phi(), DeadLayer, alpha.coeff_x(), alpha.coeff_y(), x_coinc_coord(L, alpha.coeff_x(), x0), y_coinc_coord(L, alpha.coeff_y(), y0));
      if(path_lenght>max_path){
        max_path = path_lenght;
        max_lenght_theta = alpha.get_theta();
      }
      h_path.Fill(path_lenght);
      counts++;

    }
    if (j % 10000000 == 0){
    	std::cout << "done/Total: \t" << j << "/" << N << "\n";
    }
    j++;
  }
  

  std::cout.precision(3);
  std::cout << "il " << counts*100./pow(10, 8) << " % " << " di " << N << " alpha detected" << "\n" ;

  std::ofstream outfile_media("paths.txt", std::ios::app);
  if (outfile_media.is_open())
  {
    outfile_media << L << "\t\t"<< max_path <<"\t\t"<< max_lenght_theta*360/(2*M_PI) <<"\t\t\t\t"<< h_path.GetStdDev(1) << std::endl;
    outfile_media.close();
  }
  
  theApp.Run();    

  
    return 0;
  }
#include "controlli.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <ctime>

#define diametroRivelatore 3 //in cm


bool is_coincidence(double L, double K_x, double K_y, double x0, double y0){ //L = distanza tra gli scintillatori
    double delta_x = x0 + L*K_x; //K_x = tan(theta)*cos(phi)
    double delta_y = y0 + L*K_y; //K_y = tan(theta)*sin(phi)
    if(pow(delta_x, 2) + pow(delta_y, 2) < pow(diametroRivelatore/2, 2) ){
        return true;
    }
    return false;
}

double x_coinc_coord(double L, double k_x, double x0){
    return x0 + L*k_x; //K_x = tan(theta)*cos(phi)
}

double y_coinc_coord(double L, double k_y, double y0){
    return y0 + L*k_y; //K_y = tan(theta)*sin(phi)
}

double rand_phi(double min, double max, double extreme){
    double k=0;
    do{
        k=min + (max - min) * rand() /static_cast<double> (RAND_MAX);
    }while(k==extreme);

    return k;
}

double rand_range(double min, double max){
    
    return min + (max - min) *rand() /static_cast<double> (RAND_MAX);
}

double get_path (double x0, double y0, double theta, double phi, double S, double K_x, double K_y, double x_coinc, double y_coinc){

    //std::cout<<"ang_theta: "<<theta<<"\t"<<"ang_phi: "<<phi<<std::endl;
    //std::cout<<"S*K_x: "<<S*K_x<<"\t"<<"S*K_y: "<<S*K_y<<std::endl;
    double delta_x = x_coinc + S*K_x;
    double delta_y = y_coinc + S*K_y; 
    //std::cout<<"y_coinc: "<<y_coinc<<"\t"<<"delta_y: "<<delta_y<<std::endl;
    //std::cout<<"x_coinc: "<<x_coinc<<"\t"<<"delta_x: "<<delta_x<<std::endl;
    double complete = sqrt(pow(S*K_x, 2) + pow(S*K_y, 2) + pow(S, 2));
    double r = diametroRivelatore/2.;
    
    if(sqrt(pow(delta_x, 2) + pow(delta_y, 2)) > r){
        //std::cout<<"QUIIII"<<std::endl;
        double h_out = (-sqrt(pow(K_x*r, 2) - pow(K_x*y_coinc, 2) + 2*K_x*K_y*x_coinc*y_coinc + pow(K_y*r, 2) - pow(K_y*x_coinc, 2)) - x_coinc*K_x - K_y*y_coinc)/(pow(K_x, 2) + pow(K_y, 2));
        std::cout<<"x0: "<<x0<<"\t"<<"y0: "<<y0<<std::endl;
        std::cout<<"ang_theta: "<<theta<<"\t"<<"ang_phi: "<<phi<<std::endl;
        std::cout<<"S*K_x: "<<S*K_x<<"\t"<<"S*K_y: "<<S*K_y<<std::endl;
        std::cout<<"y_coinc: "<<y_coinc<<"\t"<<"delta_y: "<<delta_y<<std::endl;
        std::cout<<"x_coinc: "<<x_coinc<<"\t"<<"delta_x: "<<delta_x<<std::endl;
        std::cout<<"S: "<<S<<"\t"<<"h_out: "<<h_out<<std::endl;
        double delta_out = - S - h_out;
        double cut = complete - (  pow(sqrt(pow(delta_x, 2) + pow(delta_y, 2)) - r, 2) + pow(delta_out, 2) );
        std::cout<<"complete: "<<complete<<"\t"<<"cut: "<<cut<<std::endl;
        return cut;
    
    }
    
    return complete;
} 
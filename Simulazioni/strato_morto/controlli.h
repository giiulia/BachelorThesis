#ifndef controlli_h
#define controlli_h



bool is_coincidence(double L, double K_x, double K_y, double x0, double y0);
double rand_phi(double min, double max, double extreme);
double rand_range(double min, double max);
double get_path (double x0, double y0, double theta, double phi, double S, double K_x, double K_y, double x_coinc, double y_coinc);
double x_coinc_coord(double L, double k_x, double x0);
double y_coinc_coord(double L, double k_y, double y0);


#endif
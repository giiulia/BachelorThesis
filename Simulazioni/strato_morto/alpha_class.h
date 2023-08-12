#include <vector>

class alpha{
public:
  
  alpha(double theta, double phi);
  
  ~alpha ();

  double coeff_x();
  double coeff_y();

  double get_theta();
  double get_phi();

private:
  double ang_theta;
  double ang_phi;

};
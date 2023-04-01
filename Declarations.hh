#ifndef DECLARATIONS_HH
#define DECLARATIONS_HH

#include <array>
#include "Parameters.hh"


double poly1(const double &a,
             const double &b,
             const double &x);

double poly2(const double &a,
             const double &b,
             const double &c,
             const double &x);

//inline double minmod(const double &x,
//                     const double &y);

std::array<double, 2> minmod_rc(const std::array<double, 3> &cell_avgs);

std::array<double, 2> ppm_rc(const std::array<double, 7> &cell_avgs,
		             const std::array<double, 5> &vel_avgs);


#endif  // DECLARATIONS_HH

/* *********************
 * Function declarations
 * ********************* */

#include <array>
#include "Parameters.hh"


double poly1(const double &a,
             const double &b,
             const double &x);

double poly2(const double &a,
             const double &b,
             const double &c,
             const double &x);

// TODO: add other reconstruction schemes
std::array<double, 2> ppm(const std::array<double, 7> &cell_avgs,
		          const std::array<double, 5> &vel_avgs);

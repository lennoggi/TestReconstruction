#include <cmath>
#include "Declarations.hh"

using namespace std;


inline double minmod(const double &x,
                     const double &y) {
    if (signbit(x) != signbit(y)) {
        return 0.;
    }

    if (fabs(x) < fabs(y)) {
        return x;
    }

    else {
        return y;
    }
}


array<double, 2> minmod_rc(const array<double, 3> &cell_avgs) {
    // Unpack all cell averages
    const double a_Im = cell_avgs.at(0);
    const double a_I  = cell_avgs.at(1);
    const double a_Ip = cell_avgs.at(2);

    // Build the minmod factor
    const double minmod_factor = 0.5*minmod(a_I - a_Im, a_Ip - a_I);

    // Return the reconstructed states at the two faces of cell I
    return {a_I - minmod_factor, a_I + minmod_factor};
}

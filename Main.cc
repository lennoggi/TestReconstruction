/* *************
 * Main function
 * ************* */

#include <iostream>
#include <iomanip>
#include <array>

#include "Declarations.hh"
#include "Parameters.hh"

using namespace std;


int main() {
    /* The number of cells must be odd (one central cells having the same number
     * of neighbors on each side)                                               */
    static_assert(ncells & 1);

    /* Set up the grid (really the cell centers) and the true value of the test
     * function at the faces of the central cell                                */
    const double dx_half  = 0.5*DX;
    const int ncells_half = ncells/2;
    array<double, ncells> cell_avgs;

    #if (TEST_FUNCTION == 1)
    const string test_function = "Order-1 polynomial";

    for (int i = 0; i < ncells; ++i) {
        const int index = i - ncells_half;
        cell_avgs.at(i) = poly1(POLY1_A, POLY1_B, XC + index*DX);
    };

    const double left_face_true  = poly1(POLY1_A, POLY1_B, XC - dx_half);
    const double right_face_true = poly1(POLY1_A, POLY1_B, XC + dx_half);


    #elif (TEST_FUNCTION == 2)
    const string test_function = "Order-2 polynomial";

    for (int i = 0; i < ncells; ++i) {
        const int index = i - ncells_half;
        cell_avgs.at(i) = poly2(POLY2_A, POLY2_B, POLY2_C, XC + index*DX);
    };

    const double left_face_true  = poly2(POLY2_A, POLY2_B, POLY2_C, XC - dx_half);
    const double right_face_true = poly2(POLY2_A, POLY2_B, POLY2_C, XC + dx_half);

    #else
    static_assert(0);  // This should have been caught already
    #endif


    // Reconstruct
    #if (RC == 0)  // Minmod
    // TODO
    static_assert(0);  // FIXME: remove when minmod is implemented

    #elif (RC == 1)  // PPM
    static_assert(ncells == 7);
    const string rc_method = "PPM";
    array<double, 5> vel_avgs;

    #if (VELOCITY_PROFILE == 0)  // Zero velocity
    for (int i = 0; i < 5; ++i) {
        vel_avgs.at(i) = 0.;
    }

    #elif (VELOCITY_PROFILE == 1)  // (Linearly) increasing
    const double one_over_2dx = 0.5/DX;
    for (int i = 0; i < 5; ++i) {
        const int index = i - 2;
        vel_avgs.at(i)  = one_over_2dx*index;  // vel_avgs lies in [-1, 1]
    }

    #elif (VELOCITY_PROFILE == 2)  // (Linearly) decreasing
    const double one_over_2dx = 0.5/DX;
    for (int i = 0; i < 5; ++i) {
        const int index = i - 2;
        vel_avgs.at(i)  = -one_over_2dx*index;  // vel_avgs lies in [-1, 1]
    }

    #else
    static_assert(0);  // This should have been caught already
    #endif  // VELOCITY_PROFILE

    const array<double, 2> rc = ppm(cell_avgs, vel_avgs);


    #else
    static_assert(0);  // This should have been caught already
    #endif  // RC



    // Compare the reconstructed result to the true result at both interfaces
    cout << endl
         << "Setup" << endl
         << "-----" << endl
         << "Test function:         " << test_function << endl
         << "Reconstruction method: " << rc_method     << endl
         << endl
         << "Left face" << endl
         << "---------" << endl
         << "True value:          " << setprecision(16) << left_face_true << endl
         << "Reconstructed value: " << setprecision(16) << rc.at(0)       << endl
         << endl
         << "Right face" << endl
         << "----------" << endl
         << "True value:          " << setprecision(16) << right_face_true << endl
         << "Reconstructed value: " << setprecision(16) << rc.at(1)        << endl
         << endl;

    return 0;
}

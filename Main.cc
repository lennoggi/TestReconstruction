/* *************
 * Main function
 * ************* */

#include <iostream>
#include <array>

#include "Declarations.hh"
#include "Parameters.hh"

using namespace std;


int main() {
    /* Set up the grid (really the cell centers) and the true value of the test
     * function at the faces of the central cell                                */
    const int i_central_cell = ncells/2;
    array<double, ncells> cell_avgs;

    #if (TEST_FUNCTION == 1)
    const string test_function = "Order-1 polynomial";

    for (int i = 0; i < ncells; ++i) {
        cell_avgs.at(i) = poly1(POLY1_A, POLY1_B, XMIN + i*DX);
    };

    const double left_face_true  = poly1(POLY1_A, POLY1_B,
		                         XMIN + (i_central_cell - 0.5)*DX);
    const double right_face_true = poly1(POLY1_A, POLY1_B,
		                         XMIN + (i_central_cell + 0.5)*DX);

    #elif (TEST_FUNCTION == 2)
    const string test_function = "Order-2 polynomial";
    for (int i = 0; i < ncells; ++i) {
        cell_avgs.at(i) = poly2(POLY2_A, POLY2_B, POLY2_C, XMIN + i*DX);
    };

    const double left_face_true  = poly2(POLY2_A, POLY2_B, POLY2_C,
		                         XMIN + (i_central_cell - 0.5)*DX);
    const double right_face_true = poly2(POLY2_A, POLY2_B, POLY2_C,
		                         XMIN + (i_central_cell + 0.5)*DX);

    #else
    static_assert(0);  // This should have been caught already
    #endif


    // Reconstruct
    #if (RC == 0)  // Minmod
    // TODO
    static_assert(0);  // FIXME: remove when minmod is implemented
    #elif (RC == 1)  // PPM
    const string rc_method = "PPM";
    const array<double, 2> rc = ppm(cell_avgs);
    #else
    static_assert(0);  // This should have been caught already
    #endif


    // Compare the reconstructed result to the true result at both interfaces
    cout << endl
         << "Setup" << endl
         << "-----" << endl
         << "Test function:         " << test_function << endl
         << "Reconstruction method: " << rc_method     << endl
         << endl
         << "Left face" << endl
         << "---------" << endl
         << "True value:          " << left_face_true << endl
         << "Reconstructed value: " << rc.at(0)       << endl
         << endl
         << "Right face" << endl
         << "----------" << endl
         << "True value:          " << right_face_true << endl
         << "Reconstructed value: " << rc.at(1)        << endl
         << endl;

    return 0;
}


#ifndef PARAMETERS_HH
#define PARAMETERS_HH

/* Reconstruction scheme:
 *   - 0 for minmod
 *   - 1 for PPM
 *   - TODO: mp5, mp9, WENO, WENOZ, ...
 */
#define RC 1
static_assert(RC == 0 or RC == 1);

#if (RC == 1)
#define PPM_SHOCK_DETECTION true
#define PPM_POLY_K          0.01
#define PPM_POLY_GAMMA      (5./3.)
#define PPM_ETA1            20.
#define PPM_ETA2            0.05
#define PPM_EPS             0.33
#define PPM_OMEGA1          0.75
#define PPM_OMEGA2          10.
#endif


/* Test function to be reconstructed:
 *   - 1 for first-order  polynomial
 *   - 2 for second-order polynomial
 *   - TODO: others
 */
#define TEST_FUNCTION 1
static_assert(TEST_FUNCTION == 0 or TEST_FUNCTION == 1);

#if (TEST_FUNCTION == 1)
#define POLY1_A 1.
#define POLY1_B 1.
#elif (TEST_FUNCTION == 2)
#define POLY2_A 1.
#define POLY2_B 1.
#define POLY2_C 1.
#else
static_assert(0);  // This should have been caught already
#endif


// Grid setup
#define XMIN 0.
#define DX   0.1


/* ##########################################
 * #  DON'T TOUCH ANYTHING BELOW THIS LINE  #
 * ########################################## */

#if (RC == 0)
constexpr inline int ncells = 3;
#elif (RC == 1)
// FIXME: use ncells=5 for PPM
constexpr inline int ncells = 7;
static_assert(PPM_SHOCK_DETECTION or not PPM_SHOCK_DETECTION);
static_assert(PPM_POLY_K     > 0.);
static_assert(PPM_POLY_GAMMA > 0.);
static_assert(PPM_EPS        > 0.);
static_assert(PPM_ETA1       > 0.);
static_assert(PPM_ETA2       > 0.);
static_assert(PPM_OMEGA1     > 0.);
static_assert(PPM_OMEGA2     > 0.);
#else
static_assert(0);  // This should have been caught already
#endif

/* The number of cells must be odd (one central cells having the same number of
 * neighbors on each side)                                                      */
static_assert(ncells & 1);

#endif  // PARAMETERS_HH

#ifndef PARAMETERS_HH
#define PARAMETERS_HH

/* Reconstruction scheme:
 *   - 1 for minmod
 *   - 2 for PPM
 *   - TODO: mp5, mp9, WENO, WENOZ, ...
 */
#define RC 1


// PPM parameters
#define PPM_SHOCK_DETECTION false
#define PPM_ZONE_FLATTENING false
#define PPM_MONOTONIZATION  true

#define PPM_POLY_K          0.01
#define PPM_POLY_GAMMA      (5./3.)
#define PPM_ETA1            20.
#define PPM_ETA2            0.05
#define PPM_EPS             0.33
#define PPM_OMEGA1          0.75
#define PPM_OMEGA2          10.

/* PPM velocity profile:
 *   - 1 for constant (equal to 0)
 *   - 2 for linearly increasing
 *   - 3 for linearly decreasing
 * We only care whether v[i] > v[i+1] for some index i, so the shape of the
 * profile doesn't matter so long as it it constant or monotonically
 * increasing/decreasing
 * TODO: implement non-monotonic profiles
 */
#define VELOCITY_PROFILE 2


/* Test function to be reconstructed:
 *   - 1 for first-order  polynomial
 *   - 2 for second-order polynomial
 *   - TODO: others
 */
#define TEST_FUNCTION 2

#if (TEST_FUNCTION == 1)
#define POLY1_A -12.88
#define POLY1_B -17.35
#elif (TEST_FUNCTION == 2)
#define POLY2_A 1.
#define POLY2_B 2.
#define POLY2_C 0.
#endif


// Grid setup
#define XC 0.
#define DX 0.1

// Verbosity
#define VERBOSE true



/* ##########################################
 * #  DON'T TOUCH ANYTHING BELOW THIS LINE  #
 * ########################################## */
static_assert(RC == 1 or RC == 2);

#if (RC == 1)
constexpr inline int ncells = 3;
#elif (RC == 2)
constexpr inline int ncells = 7;  // FIXME: use ncells=5
#else
static_assert(0);  // This should have been caught already
#endif

static_assert(PPM_SHOCK_DETECTION or not PPM_SHOCK_DETECTION);
static_assert(PPM_ZONE_FLATTENING or not PPM_ZONE_FLATTENING);
static_assert(PPM_MONOTONIZATION  or not PPM_MONOTONIZATION);

static_assert(PPM_POLY_K     > 0.);
static_assert(PPM_POLY_GAMMA > 0.);
static_assert(PPM_ETA1       > 0.);
static_assert(PPM_ETA2       > 0.);
static_assert(PPM_EPS        > 0.);
static_assert(PPM_OMEGA1     > 0.);
static_assert(PPM_OMEGA2     > 0.);

static_assert(VELOCITY_PROFILE == 1 or
              VELOCITY_PROFILE == 2 or
              VELOCITY_PROFILE == 3);

static_assert(TEST_FUNCTION == 1 or TEST_FUNCTION == 2);
static_assert(DX > 0.);
static_assert(VERBOSE or not VERBOSE);


#endif  // PARAMETERS_HH

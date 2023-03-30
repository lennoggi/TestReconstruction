#ifndef PARAMETERS_HH
#define PARAMETERS_HH

/* Reconstruction scheme:
 *   - 0 for minmod
 *   - 1 for PPM
 *   - TODO: mp5, mp9, WENO, WENOZ, ...
 */
#define RC 1
static_assert(RC == 0 or RC == 1);

#if (RC == 0)
// TODO
constexpr inline int ncells = 3;
static_assert(0);


#elif (RC == 1)
#define PPM_SHOCK_DETECTION false
#define PPM_ZONE_FLATTENING false
#define PPM_MONOTONIZATION  true
static_assert(PPM_SHOCK_DETECTION or not PPM_SHOCK_DETECTION);
static_assert(PPM_ZONE_FLATTENING or not PPM_ZONE_FLATTENING);
static_assert(PPM_MONOTONIZATION  or not PPM_MONOTONIZATION);

#define PPM_POLY_K          0.01
#define PPM_POLY_GAMMA      (5./3.)
#define PPM_ETA1            20.
#define PPM_ETA2            0.05
#define PPM_EPS             0.33
#define PPM_OMEGA1          0.75
#define PPM_OMEGA2          10.
static_assert(PPM_POLY_K     > 0.);
static_assert(PPM_POLY_GAMMA > 0.);
static_assert(PPM_ETA1       > 0.);
static_assert(PPM_ETA2       > 0.);
static_assert(PPM_EPS    > 0.);
static_assert(PPM_OMEGA1 > 0.);
static_assert(PPM_OMEGA2 > 0.);

/* PPM velocity profile:
 *   - 0 for constant (equal to 0)
 *   - 1 for linearly increasing
 *   - 2 for linearly decreasing
 * We only care whether v[i] > v[i+1] for some index i, so the shape of the
 * profile doesn't matter so long as it it constant or monotonically
 * increasing/decreasing
 * TODO: implement non-monotonic profiles
 */
#define VELOCITY_PROFILE 1
static_assert(VELOCITY_PROFILE == 0 or
              VELOCITY_PROFILE == 1 or
              VELOCITY_PROFILE == 2);

// FIXME: use ncells=5 for PPM
constexpr inline int ncells = 7;
#endif  // RC == 1


/* Test function to be reconstructed:
 *   - 1 for first-order  polynomial
 *   - 2 for second-order polynomial
 *   - TODO: others
 */
#define TEST_FUNCTION 2
static_assert(TEST_FUNCTION == 1 or TEST_FUNCTION == 2);

#if (TEST_FUNCTION == 1)
#define POLY1_A -12.88
#define POLY1_B -17.35
#elif (TEST_FUNCTION == 2)
#define POLY2_A 1.
#define POLY2_B 2.
#define POLY2_C 0.
#else
static_assert(0);  // This should have been caught already
#endif


// Grid setup
#define XC 0.
#define DX 0.1
static_assert(DX > 0.);


// Verbosity
#define VERBOSE true
static_assert(VERBOSE or not VERBOSE);


#endif  // PARAMETERS_HH

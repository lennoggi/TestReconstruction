/********************************************************************
 * PPM reconstruction routine. See Colella & Woodward (1984) (e.g. at
 * https://crd.lbl.gov/assets/pubs_presos/AMCS/ANAG/A141984.pdf)
 ******************************************************************** */

#include <cmath>
#include "Parameters.hh"
#include "Declarations.hh"
#include "Helpers.hh"

using namespace std;


array<double, 2> ppm(const array<double, 7> &cell_avgs,
                     const array<double, 5> &vel_avgs) {
    // Unpack all cell averages
    const double a_Immm = cell_avgs.at(0);
    const double a_Imm  = cell_avgs.at(1);
    const double a_Im   = cell_avgs.at(2);
    const double a_I    = cell_avgs.at(3);
    const double a_Ip   = cell_avgs.at(4);
    const double a_Ipp  = cell_avgs.at(5);
    const double a_Ippp = cell_avgs.at(6);

    // Unpack the velocity averages
    const double vel_Imm  = vel_avgs.at(0);
    const double vel_Im   = vel_avgs.at(1);
    const double vel_I    = vel_avgs.at(2);
    const double vel_Ip   = vel_avgs.at(3);
    const double vel_Ipp  = vel_avgs.at(4);

    // Helpers
    const double diff_Im = a_I   - a_Imm;
    const double diff_I  = a_Ip  - a_Im;
    const double diff_Ip = a_Ipp - a_I;

    const double delta_Im = 0.5*diff_Im;
    const double delta_I  = 0.5*diff_I;
    const double delta_Ip = 0.5*diff_Ip;

    const double _2fabs_Im_Imm = 2.*fabs(a_Im  - a_Imm);
    const double _2fabs_I_Im   = 2.*fabs(a_I   - a_Im);
    const double _2fabs_Ip_I   = 2.*fabs(a_Ip  - a_I);
    const double _2fabs_Ipp_Ip = 2.*fabs(a_Ipp - a_Ip);

    const double deltamod_Im = sgn(delta_Im)*min(fabs(delta_Im), min(_2fabs_Im_Imm, _2fabs_I_Im));
    const double deltamod_I  = sgn(delta_I) *min(fabs(delta_I),  min(_2fabs_I_Im,   _2fabs_Ip_I));
    const double deltamod_Ip = sgn(delta_Ip)*min(fabs(delta_Ip), min(_2fabs_Ip_I,   _2fabs_Ipp_Ip));


    /* Initial reconstructed states at the interfaces between cells Im/I and I/Ip
     * NOTE: not const because they may change later                            */
    const double a_Im_I = 0.5*(a_Im + a_I)  + (1./6.)*(deltamod_I  - deltamod_Im);
    const double a_I_Ip = 0.5*(a_I  + a_Ip) + (1./6.)*(deltamod_Ip - deltamod_I);

    double rc_low = a_Im_I;
    double rc_up  = a_I_Ip;


    // Build the pressure from a polytropic EOS
    const double press_Immm = PPM_POLY_K*pow(a_Immm, PPM_POLY_GAMMA);
    const double press_Imm  = PPM_POLY_K*pow(a_Imm,  PPM_POLY_GAMMA);
    const double press_Im   = PPM_POLY_K*pow(a_Im,   PPM_POLY_GAMMA);
    const double press_I    = PPM_POLY_K*pow(a_I,    PPM_POLY_GAMMA);
    const double press_Ip   = PPM_POLY_K*pow(a_Ip,   PPM_POLY_GAMMA);
    const double press_Ipp  = PPM_POLY_K*pow(a_Ipp,  PPM_POLY_GAMMA);
    const double press_Ippp = PPM_POLY_K*pow(a_Ippp, PPM_POLY_GAMMA);

    const double diff_press_I = press_Ip - press_Im;
    const double min_press_I  = min(press_Im, press_Ip);


    /* Shock detection (eqs. 1.15, 1.16, 1.17 with uniform grid spacing).
     * This is only applied to rho and only if the shock is marked as a contact
     * discontinuity (see eq. 3.2)                                                */
    // FIXME: contact discontinuity check only valid for polytropic/ideal-fluid EOS
    #if (SHOCK_DETECTION)
    const bool contact_disc = (PPM_POLY_K*PPM_POLY_GAMMA*fabs(diff_I)*min_press_I >=
                               fabs(diff_press_I)*min(a_Ip, a_Im));
    if (contact_disc) {
      const double d2a_Im      = a_I   - 2*a_Im + a_Imm;
      const double d2a_Ip      = a_Ipp - 2*a_Ip + a_I;
      const double d2_prod     = d2a_Im*d2a_Ip;
      const double eta_tilde_I = (d2_prod < 0) ? (-1./6.)*d2_prod/diff_I : 0;
      const double eta_I       = max(0., min(PPM_ETA1*(eta_tilde_I - PPM_ETA2), 1.));

      rc_low = (1. - eta_I)*rc_low + eta_I*(a_Im + 0.5*deltamod_Im);
      rc_up  = (1. - eta_I)*rc_up  + eta_I*(a_Ip - 0.5*deltamod_Ip);
    }
    #endif


    // Zone flattening to reduce post-shock oscillations (see eq. 4.1 + appendix)
    const double w_I      = (diff_press_I  > PPM_EPS*min_press_I and
                             vel_Im > vel_Ip) ? 1. : 0.;
    const double ftilde_I = 1. - max(0., w_I*PPM_OMEGA2*(diff_press_I/(press_Ipp - press_Imm) - PPM_OMEGA1));

    if (diff_press_I < 0) {
        const double diff_press_Ip = press_Ipp - press_I;
        const double w_Ip          = (diff_press_Ip > PPM_EPS*min(press_I, press_Ipp) and
                                      vel_I  > vel_Ipp) ? 1. : 0.;
        const double ftilde_Ip     = 1. - max(0., w_Ip*PPM_OMEGA2*(diff_press_Ip/(press_Ippp - press_Im) - PPM_OMEGA1));
        const double f_I           = max(ftilde_I, ftilde_Ip);
        const double one_minus_fI  = 1. - f_I;
        const double fI_gfI        = f_I*a_I;
        rc_low = fI_gfI + one_minus_fI*rc_low;
        rc_up  = fI_gfI + one_minus_fI*rc_up;
    }

    else {
        const double diff_press_Im = press_I - press_Imm;
        const double w_Im          = (diff_press_Im  > PPM_EPS*min(press_Imm, press_I) and
                                      vel_Imm > vel_I) ? 1. : 0.;
        const double ftilde_Im     = 1. - max(0., w_Im*PPM_OMEGA2*(diff_press_Im/(press_Ip - press_Immm) - PPM_OMEGA1));
        const double f_I           = max(ftilde_I, ftilde_Im);
        const double one_minus_fI  = 1. - f_I;
        const double fI_gfI        = f_I*a_I;
        rc_low = fI_gfI + one_minus_fI*rc_low;
        rc_up  = fI_gfI + one_minus_fI*rc_up;
    }


    // Monotonization (see eq. 1.10)
    if ((rc_up - a_I)*(a_I - rc_low) <= 0) {
        rc_low = a_I;
        rc_up  = a_I;
    }

    else {
        const double diff_rc    = rc_up - rc_low;
        const double diff_rc_sq = diff_rc*diff_rc;
        const double gf6_I      = 6.*diff_rc*(a_I - 0.5*(rc_low + rc_up));

        if (gf6_I > diff_rc_sq) {
            rc_low = 3.*a_I - 2.*rc_up;
        }

        else if (gf6_I < -diff_rc_sq) {
            rc_up = 3.*a_I - 2.*rc_low;
        }
    }


    // Return the lower and upper reconstructed states in cell I
    const array<double, 2> rc = {rc_low, rc_up};
    return rc;
}

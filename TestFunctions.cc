/* **************************************
 * Some test functions for reconstruction
 * ************************************** */

// Linear function
double poly1(const double &a,
             const double &b,
             const double &x) {
    return a*x + b;
}

// Parabola
double poly2(const double &a,
             const double &b,
             const double &c,
             const double &x) {
    return a*x*x + b*x + c;
}

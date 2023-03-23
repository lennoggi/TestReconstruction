/* *********************
 * Some helper functions
 * ********************* */

template<typename T>
inline int sgn(const T &x) {
    return (x >= T(0)) - (x < T(0));
}

using P = complex<double>;
const double EPS = 1e-6;
const double PI = cos(-1);

// inner product
// Checks if two vectors are orthogonal.
double dot(const P& p1, const P& p2) {
  return real(p1) * real(p2) + imag(p1) * imag(p2);
}

// outer product
// Checks if two vectors are parallel.
double det(const P& p1, const P& p2) {
  return real(p1) * imag(p2) - imag(p1) * real(p2);
}

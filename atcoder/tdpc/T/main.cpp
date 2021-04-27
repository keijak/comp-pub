#include <bits/stdc++.h>

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

// Computes the n-th term of a linear recurrence sequence.
template <typename T = Mint>
struct Kitamasa {
  const int k;

  // Initial k elements (a[1] .. a[k]).
  const std::vector<T> a_;

  // Coefficients of the recurrence.
  //   a[k+1] = coeff[1]*a[1] + coeff[2]*a[2] + ... + coeff[k]*a[k].
  const std::vector<T> coeff_;

  explicit Kitamasa(std::vector<T> a, std::vector<T> c)
      : k(a.size()), a_(a), coeff_(std::move(c)) {}

  T nth(long long n) const {
    auto x = nth_coeff(n);
    T res = 0;
    for (int i = 0; i < k; ++i) {
      res += x[i] * a_[i];
    }
    return res;
  }

 private:
  // n: 1-indexed.
  std::vector<T> nth_coeff(long long n) const {
    if (n <= k) {
      std::vector<T> res(k, 0);
      res[n - 1] = a_[n - 1];
      return res;
    }
    if (n & 1) {
      return next(nth_coeff(n - 1));
    } else {
      const int h = n / 2;
      std::vector<T> s = nth_coeff(h);
      std::vector<T> t = s;
      std::vector<T> res(k, 0);
      for (int i = 0; i < k; ++i) {
        t = next(std::move(t));
        for (int j = 0; j < k; ++j) {
          res[j] += t[j] * s[i];
        }
      }
      return res;
    }
  }

  std::vector<T> next(std::vector<T> x) const {
    std::vector<T> res(k, 0);
    for (int i = 1; i < k; ++i) {
      res[i] = x[i - 1];
    }
    for (int i = 0; i < k; ++i) {
      res[i] += x[k - 1] * coeff_[i];
    }
    return res;
  }
};

using namespace std;

int main() {
  int k, n;
  cin >> k >> n;
  Kitamasa<Mint> kitamasa(vector<Mint>(k, 1), vector<Mint>(k, 1));
  cout << kitamasa.nth(n) << endl;
}

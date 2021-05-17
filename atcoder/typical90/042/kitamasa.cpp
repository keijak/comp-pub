// #pragma GCC target("avx2")
// #pragma GCC optimize("Ofast")
// #pragma GCC optimize("unroll-loops")

#include <atcoder/modint>
#include <cctype>
#include <cstdio>
#include <type_traits>
#include <vector>

struct Input {
  inline operator int() { return read_int<int>(); }
  inline operator long long() { return read_int<long long>(); }
  inline operator unsigned() { return read_int<unsigned>(); }

 private:
  template <typename T>
  static T read_int() {
    T ret = 0, sgn = 1;
    int ch = getchar_unlocked();
    while (isspace(ch)) {
      ch = getchar_unlocked();
    }
    if constexpr (!std::is_unsigned<T>::value) {
      if (ch == '-') {
        sgn = -1;
        ch = getchar_unlocked();
      }
    }
    for (; ('0' <= ch) & (ch <= '9'); ch = getchar_unlocked()) {
      ret = (ret * 10) + (ch - '0');
    }
    ungetc(ch, stdin);
    if constexpr (std::is_unsigned<T>::value) {
      return ret;
    } else {
      return ret * sgn;
    }
  }
} input;

// Computes the n-th term of a linear recurrence sequence.
template <typename T>
struct Kitamasa {
  const int k;

  // Initial k elements (a[1] .. a[k]).
  const std::vector<T> a_;

  // Coefficients of the recurrence.
  //   a[n] = coeff[1]*a[n-1] + coeff[2]*a[n-2] + ... + coeff[k]*a[n-k].
  const std::vector<T> coeff_;

  explicit Kitamasa(std::vector<T> a, std::vector<T> c)
      : k(a.size()), a_(a), coeff_(std::move(c)) {}

  // n: 1-indexed.
  T nth(long long n) const {
    auto x = nth_coeff(n);
    T res = 0;
    for (int i = 0; i < k; ++i) {
      res += x[i] * a_[i];
    }
    return res;
  }

 private:
  std::vector<T> nth_coeff(long long n) const {
    if (n <= k) {
      std::vector<T> res(k, 0);
      res[n - 1] = 1;
      return res;
    }
    // Doubling.
    if (n & 1) {
      // f(n-1) => f(n)
      return next(nth_coeff(n - 1));
    } else {
      // f(n/2) => f(n)
      std::vector<T> s = nth_coeff(n / 2);
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
      res[i] += x[k - 1] * coeff_[k - 1 - i];
    }
    return res;
  }
};

using namespace std;
using Mint = atcoder::modint1000000007;

int main() {
  const unsigned K = input;
  if (K % 9 != 0) {
    puts("0");
  } else {
    vector<Mint> a(9, 0), c(9, 1);
    a[8] = 1;
    Kitamasa<Mint> solver(move(a), move(c));
    unsigned ans = solver.nth(K + 9).val();
    printf("%u\n", ans);
  }
}

#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

#include <atcoder/modint>
using Mint = atcoder::modint998244353;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;

struct Void {};

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &seq,
                        const char *sep = " ",
                        const char *ends = "\n",
                        std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

// N-dimensional array.
template<typename T, int dim>
struct NdArray {
  std::vector<T> data_;
  std::array<int, dim> shape_;
  std::array<int, dim> base_;

  NdArray(const std::array<int, dim> &shape)
      : data_(std::accumulate(shape.begin(), shape.end(), size_t(1),
                              [](size_t x, size_t y) { return x * y; })),
        shape_(shape) {
    size_t b = 1;
    for (int j = dim - 1; j >= 0; --j) {
      base_[j] = b;
      b *= shape_[j];
    }
  }

  T &operator[](const std::array<int, dim> &index) {
    size_t pos = 0;
    for (int j = dim - 1; j >= 0; --j) {
      pos += index[j] * base_[j];
    }
    return data_[pos];
  }

  // Fills one row with the specified value.
  inline void fill(int i0, T val) {
    std::fill(data_.begin() + i0 * base_[0],
              data_.begin() + (i0 + 1) * base_[0], val);
  }
};

template<typename T>
inline bool has_bit(const T &x, int i) { return (x >> i) & 1; }

#include <boost/multiprecision/cpp_int.hpp>
using BigInt = boost::multiprecision::cpp_int;
namespace multip = boost::multiprecision;

auto solve() {
  string N = in;
  //int n = ssize(N);
  int m = in;
  vector<int> C = in(m);
  vector<int> rc(10, -1);
  REP(j, m) rc[C[j]] = j;
  const int kFull = (1 << m) - 1;

  BigInt NB;
  stringstream ss;
  ss << N;
  ss >> NB;

  vector<map<BigInt, Mint>> memo(1 << m);
  auto f = [&](auto &f, const BigInt &x, int state) -> Mint {
    if (x <= 0) return 0;
    if (auto it = memo[state].find(x); it != memo[state].end()) {
      return it->second;
    }
    const int curd = int(x % 10);
    Mint ret = 0;
    REP(d, 10) {
      int j = rc[d];
      if (j != -1 and not has_bit(state, j)) continue;

      if (d <= curd) {
        BigInt y = (x - d) / 10;
        if (y == 0 and d == 0) continue;
        ret += f(f, y, state);
        if (j != -1) {
          ret += f(f, y, state & ~(1 << j));
        }
      } else {
        BigInt y = (x - d);
        if (y < 0) continue;
        y /= 10;
        ret += f(f, y, state);
        if (j != -1) {
          ret += f(f, y, state & ~(1 << j));
        }
      }
      if (d != 0 and state == 0) {
        ret += 1;
      }
    }
    //DUMP(x, state, ret);
    memo[state][x] = ret;
    return ret;
  };
  Mint ans = f(f, NB, kFull);
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}

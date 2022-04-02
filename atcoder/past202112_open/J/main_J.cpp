#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
template<typename T>
constexpr T kBigVal = std::numeric_limits<T>::max() / 2;
void exit() { std::cout.flush(), std::cerr.flush(), std::_Exit(0); }

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) std::cin >> x;
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

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

template<typename T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<typename T>
struct Array2d {
  std::vector<T> data_;
  size_t h_, w_;
  bool transposed_ = false;
  int rotate_ = 0;

  Array2d(size_t h, size_t w) : data_(h * w, T{0}), h_(h), w_(w) {}

  inline T &get(size_t i, size_t j) {
    int h = h_, w = w_;
    REP(k, rotate_) {
      size_t tmp = i;
      i = j;
      j = h - 1 - tmp;
      swap(h, w);
    }
    if (transposed_) {
      swap(i, j);
      swap(h, w);
    }
    return data_[i * w + j];
  }

  // Fills one row with the specified value.
  inline void fill(size_t i0, T val) {
    std::fill(data_.begin() + i0 * w_, data_.begin() + (i0 + 1) * w_, val);
  }

  void rotate90(int k = 1) {
    rotate_ = (rotate_ + k + 4) % 4;
  }

  void transpose() {
    transposed_ = !transposed_;
    rotate_ = (4 - rotate_) % 4;
  }

  void flip_v() {
    transpose();
    rotate90(1);
  }

  void flip_h() {
    transpose();
    rotate90(-1);
  }
};

auto solve() {
  int n = in, Q = in;
  Array2d<int> a(n, n);
  REP(qi, Q) {
    int qt = in;
    if (qt == 1) {
      int x = in, y = in;
      --x, --y;
      a.get(x, y) = a.get(x, y) ^ 1;
    } else if (qt == 2) {
      char c = in;
      if (c == 'A') {
        a.rotate90(-1);
      } else {
        a.rotate90();
      }
    } else {
      char c = in;
      if (c == 'A') {
        a.flip_v();
      } else {
        a.flip_h();
      }
    }

//    cerr << "# Query " << qi << ": " << endl;
//    cerr << "# rot: " << a.rotate_ << endl;
//    cerr << "# tra: " << a.transposed_ << endl;
  }
  REP(i, n) {
    REP(j, n) {
      cout << a.get(i, j);
    }
    cout << '\n';
  }
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { (solve()); }
  exit();
}

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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

int query(vector<int> ix) {
  assert(ssize(ix) == 3);
  cout << "? " << ix[0] << " " << ix[1] << " " << ix[2] << endl;
  int res;
  cin >> res;
  return res;
}

void answer(vector<int> ix) {
  assert(ssize(ix) == 2);
  cout << "! " << ix[0] << " " << ix[1] << endl;
}

void solve() {
  int n = in;
  vector<int> ix = {1, 2, 3, 4};
  array<int, 4> buf = {};

  auto ask4 = [&]() {
    REP(i, 4) {
      int x = ix[i];
      vector<int> q;
      for (auto y: ix) {
        if (y != x) q.push_back(y);
      }
      buf[i] = query(q);
    }
  };

  auto drop_big = [&](int k) {
    array<int, 4> si = {0, 1, 2, 3};
    sort(ALL(si), [&](int i, int j) { return buf[i] < buf[j]; });
    if (k == 1) {
      ix.erase(ix.begin() + si[3]);
    } else if (k == 2) {
      int i1 = si[2];
      int i2 = si[3];
      if (i1 > i2) swap(i1, i2);
      ix.erase(ix.begin() + i2);
      ix.erase(ix.begin() + i1);
    }
  };

  ask4();
  int pos = 5;
  while (pos <= n) {
    if (pos == n) {
      drop_big(1);
      ix.push_back(pos++);
    } else {
      drop_big(2);
      ix.push_back(pos++);
      ix.push_back(pos++);
    }
    ask4();
  }
  drop_big(2);
  answer(ix);
}

int main() {
  const int T = in;
  REP(t, T) {
    solve();
  }
}

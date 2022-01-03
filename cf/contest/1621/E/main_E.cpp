#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) {
  return (int) a.size();
}

struct Void {};

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same<T, Void>::value) {
    return std::cout;  // print nothing
  } else if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
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

using Rat = pair<Int, Int>;

struct LessRat {
  bool operator()(const Rat &z1, const Rat &z2) const {
    const auto&[x1, y1] = z1;
    const auto&[x2, y2] = z2;
    return __int128(x1) * y2 < __int128(y1) * x2;
  }
};

auto solve() {
  int n = in, m = in;
  multiset<Rat, LessRat> a;
  REP(i, n) {
    Int x = in;
    a.insert(Rat{x, 1});
  }

  vector<vector<Int>> b(m); // age of students
  vector<Rat> avg(m);
  REP(i, m) {
    Int k = in;
    b[i] = (vector<Int>) in(k);
    Int s = accumulate(ALL(b[i]), 0LL);
    Int g = gcd(s, k);
    avg[i] = Rat{s / g, k / g};
  }
  DUMP(avg);
  vector<int> ix(m);
  REP(i, m) ix[i] = i;
  sort(ALL(ix), [&](int i, int j) { return LessRat()(avg[i], avg[j]); });

  DUMP(ix);
  multiset<Rat, LessRat> avail = a;
  set<int> impossibles;
  vector<Rat> saved(m);

  vector<string> ans(m);

  REP(i, m - 1) {
    Rat r = avg[ix[i]];
    auto it = avail.lower_bound(r);
    if (it == avail.end()) {
      impossibles.insert(i);
    } else {
      saved[i] = *it;
      avail.erase(it);
    }
  }

  for (int i = m - 1; i >= 0; --i) {
    int j = ix[i];
    DUMP(j);

    int kj = ssize(b[j]) - 1;
    Int sumj = accumulate(ALL(b[j]), 0LL);
    ans[j].resize(ssize(b[j]));
    REP(s, ssize(b[j])) {
      if (not impossibles.empty() or avail.empty()) {
        ans[j][s] = '0';
      } else {
        const Rat tmax = *avail.rbegin();
        Int sumr = sumj - b[j][s];
        Int g = std::gcd(sumr, kj);
        if (LessRat()(tmax, Rat{sumr / g, kj / g})) {
          ans[j][s] = '0';
        } else {
          ans[j][s] = '1';
        }
      }
    }

    if (i > 0) {
      impossibles.erase(i - 1);
      avail.insert(saved[i - 1]);
    }

    Rat r = avg[ix[i]];
    auto it = avail.lower_bound(r);
    if (it == avail.end()) {
      impossibles.insert(i);
    } else {
      avail.erase(it);
    }
  }

  string res;
  REP(i, m) {
    res += ans[i];
  }
  return res;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = in;
  REP(t, T) {
    auto ans = solve();
    print(ans);
  }
}
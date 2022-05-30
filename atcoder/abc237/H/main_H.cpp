#include <bits/stdc++.h>
#include <atcoder/maxflow>

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

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    assert(std::cin >> x);
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) assert(std::cin >> x);
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

void exit_() {
#ifdef MY_DEBUG
  std::string unused;
  assert(not(std::cin >> unused));
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)cerr
#endif

using namespace std;

bool contains(const string &s, const string &t) {
  if (t.size() > s.size()) return false;
  return s.find(t) != string::npos;
}

auto solve() {
  const string S = in;
  const int n = ssize(S);

  auto is_pal = [&](int l, int r) -> bool {
    int i = l, j = r - 1;
    while (i < j) {
      if (S[i] != S[j]) return false;
      ++i, --j;
    }
    return true;
  };

  set < string > pals;
  for (int l = 0; l < n; ++l) {
    for (int r = l + 1; r <= n; ++r) {
      if (is_pal(l, r)) {
        pals.insert(S.substr(l, r - l));
      }
    }
  }
  DUMP(pals);
  vector<string> palv(ALL(pals));
  const int P = ssize(palv);
  atcoder::mf_graph<int> g(P * 2 + 2);
  const int kSource = P * 2;
  const int kSink = kSource + 1;
  REP(v, P) {
    g.add_edge(kSource, v, 1);
    g.add_edge(P + v, kSink, 1);
  }
  REP(i, P) {
    REP(j, P) {
      if (i == j) continue;
      if (contains(palv[i], palv[j])) {
        g.add_edge(i, P + j, 1);
      }
    }
  }
  int max_match = g.flow(kSource, kSink);
  print(P - max_match);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) { (solve()); }
  exit_();
}

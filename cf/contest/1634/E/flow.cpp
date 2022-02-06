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

auto solve() -> optional<vector<string>> {
  int m = in;
  vector<vector<int>> as(m);
  vector<map<int, int>> afreq(m);
  map<int, int> freq;
  REP(i, m) {
    int n = in;
    as[i].resize(n);
    REP(j, n) {
      int x = in;
      as[i][j] = x;
      afreq[i][x]++;
      freq[x]++;
    }
  }
  vector<int> xs;
  Int weight_sum = 0;
  for (const auto&[k, v]: freq) {
    if (v % 2 == 1) {
      return nullopt;
    }
    xs.push_back(k);
    weight_sum += v;
  }

  int kSource = m + ssize(xs);
  int kSink = kSource + 1;
  atcoder::mf_graph<int> g(kSink + 1);
  REP(i, m) {
    int k = ssize(as[i]) / 2;
    g.add_edge(kSource, i, k);
  }
  REP(i, ssize(xs)) {
    auto x = xs[i];
    g.add_edge(m + i, kSink, freq[x] / 2);
  }
  REP(i, m) {
    for (auto[x, cnt]: afreq[i]) {
      int j = lower_bound(ALL(xs), x) - xs.begin();
      g.add_edge(i, m + j, cnt);
    }
  }
  auto flow = g.flow(kSource, kSink);
  DUMP(flow);
  DUMP(weight_sum);
  if (flow != weight_sum / 2) {
    return nullopt;
  }
  DUMP("OK");

  vector<string> ans(m);
  int offset = m + ssize(xs);
  REP(i, m) {
    ans[i].resize(ssize(as[i]));
    map<int, int> ls;
    for (auto[x, cnt]: afreq[i]) {
      auto edge = g.get_edge(offset++);
      ls[x] = edge.flow;
    }
    REP(j, ssize(as[i])) {
      if (ls[as[i][j]]) {
        ans[i][j] = 'L';
        --ls[as[i][j]];
      } else {
        ans[i][j] = 'R';
      }
    }
  }
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = 1;//in;
  REP(t, T) {
    auto ans = solve();
    if (not ans) {
      print("NO");
    } else {
      print("YES");
      for (const auto &line: *ans) {
        print(line);
      }
    }
  }
}

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

struct Edge {
  int to;
  int blue;  // 0:red 1:blue
};

struct Line {
  Int a;
  Int b;
};
bool operator<(const Line &l1, const Line &l2) {
  return tie(l1.a, l1.b) < tie(l2.a, l2.b);
}

template<class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

auto solve() {
  int n = in, m = in, Q = in;
  Int L = in;
  vector<vector<Edge>> g(n);
  REP(i, m) {
    int a = in, b = in, c = in;
    --a, --b, --c;
    g[a].push_back({b, c});
    g[b].push_back({a, c});
  }

  const Int kBig = L + 100;
  const int blimit = 30;

  vector mincost(n, vector(blimit + 1, kBig));
  mincost[0][0] = 1;
  vector lines(n, vector(blimit + 1, Line{}));
  lines[0][0] = {1, 0};
  MinHeap<tuple<int, Int, int>> heap;
  heap.emplace(0, 1, 0);

  while (not heap.empty()) {
    const auto[bcount, cost, v] = heap.top();
    heap.pop();
    if (cost != mincost[v][bcount]) continue;
    if (cost == kBig) continue;
    const Line &vline = lines[v][bcount];
    for (const auto &e: g[v]) {
      Line uline{kBig, kBig};
      int bcount2 = bcount;
      if (e.blue) {
        if (bcount < blimit) {
          uline = {min(2 * vline.a, kBig), vline.b};
          ++bcount2;
        }
      } else {
        uline = {vline.a, min(vline.a + vline.b, kBig)};
      }
      Int new_cost = min(uline.a + uline.b, kBig);
      if (mincost[e.to][bcount2] > new_cost) {
        mincost[e.to][bcount2] = new_cost;
        lines[e.to][bcount2] = uline;
        heap.emplace(bcount2, new_cost, e.to);
      }
    }
  }

  auto table = vector(n, kBig);
  REP(v, n) {
    REP(i, blimit + 1) {
      chmin(table[v], mincost[v][i]);
    }
  }

  REP(i, Q) {
    const int t = int(in) - 1;
    if (table[t] > L) {
      print("Large");
    } else {
      print(table[t]);
    }
  }

  return Void();
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

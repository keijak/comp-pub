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

struct UnionFind {
  int n_;
  std::vector<int> parent_;  // negative: size

  explicit UnionFind(int n) : n_(n), parent_(n, -1) {}

  bool unite(int x, int y) {
    x = find(x), y = find(y);
    if (x == y) return false;
    if (-parent_[x] < -parent_[y]) std::swap(x, y);
    parent_[x] += parent_[y];
    parent_[y] = x;
    return true;
  }

  int find(int v) {
    if (parent_[v] < 0) return v;
    return parent_[v] = find(parent_[v]);
  }

  int size(int v) { return -parent_[find(v)]; }

  bool same(int x, int y) { return find(x) == find(y); }
};

struct Seg {
  int c;
  Int l;
  Int r;
};

auto solve() {
  const int n = in;
  UnionFind uf(n);
  vector<Seg> segs(n);
  vector<tuple<Int, bool, int>> events;
  REP(i, n) {
    int c = in;
    Int l = in, r = in;
    segs[i] = Seg{c, l, r};
    events.emplace_back(l, false, i);
    events.emplace_back(r, true, i);
  }
  sort(ALL(events));
  set<int> rs, bs;
  optional<int> rt, bt;

  for (auto [x, ending, i]: events) {
    const auto &si = segs[i];
    if (ending) {
      if (si.c == 1) {
        if (bt.has_value() and bt.value() == i) {
          rs.insert(rt.value());
          bt = nullopt;
          rt = nullopt;
        } else {
          bs.erase(i);
        }
      } else {
        if (rt.has_value() and rt.value() == i) {
          bs.insert(bt.value());
          bt = nullopt;
          rt = nullopt;
        } else {
          rs.erase(i);
        }
      }

    } else {
      if (si.c == 1) {
        if (bt.has_value() and rt.has_value()) {
          uf.unite(i, *rt);
          if (segs[*bt].r < si.r) {
            bt = i;
          }
        } else if (not rs.empty()) {
          int v = -1;
          Int rmax = -1;
          for (auto j: rs) {
            auto [cj, lj, rj] = segs[j];
            uf.unite(i, j);
            if (chmax(rmax, rj)) {
              v = j;
            }
          }
          rs.clear();
          rt = v;
          bt = i;
        } else {
          bs.insert(i);
        }
      } else {
        if (bt.has_value() and rt.has_value()) {
          uf.unite(i, *bt);
          if (segs[*rt].r < si.r) {
            rt = i;
          }
        } else if (not bs.empty()) {
          int v = -1;
          Int rmax = -1;
          for (auto j: bs) {
            auto [cj, lj, rj] = segs[j];
            uf.unite(i, j);
            if (chmax(rmax, rj)) {
              v = j;
            }
          }
          bs.clear();
          bt = v;
          rt = i;
        } else {
          rs.insert(i);
        }
      }
    }
  }
  int cnt = 0;
  REP(i, n) {
    if (uf.find(i) == i) {
      cnt++;
    }
  }
  print(cnt);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int T = in;
  REP(t, T) { (solve()); }
  exit_();
}

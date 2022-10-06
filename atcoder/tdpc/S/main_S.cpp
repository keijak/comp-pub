// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
constexpr T kBig = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    assert(bool(std::cin));
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        std::cin >> x;
        assert(bool(std::cin));
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

template<typename Container>
std::ostream &out_seq(const Container &seq, const char *sep = " ",
                      const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &out_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &out(const T &x) {
  return out_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &out(const T &head, Ts... tail) {
  return out_one(head, ' '), out(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

using namespace std;

template<typename T>
inline bool has_bit(const T &x, int i) { return (x >> i) & 1; }

template<typename T>
std::vector<T> pow_seq(int n, T base = 2) {
  assert(n >= 0);
  std::vector<T> p(n + 1);
  p[0] = 1;
  for (int i = 0; i < n; ++i) {
    p[i + 1] = p[i] * base;
  }
  return p;
}

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

auto solve() {
  const int H = in, W = in;
  auto pow4 = pow_seq<int>(H, 4);

  auto decode = [&](int state) {
    vector<int> ret(H);
    REP(i, H) {
      ret[i] = state % 4;
      state /= 4;
    }
    return ret;
  };

  auto dp = vector(pow4[H], Mint(0));
  REP(bits, 1 << H) {
    if (not(bits & 1)) continue;
    int state = 1;
    int connect = 1;
    REP(i, 1, H) {
      if (not has_bit(bits, i)) {
        continue;
      }
      if (not has_bit(bits, i - 1)) {
        connect += 1;
      }
      state += pow4[i] * connect;
    }
    dp[state] = 1;
  }
  REP(i, 1, W) {
    auto ndp = vector(pow4[H], Mint(0));
    REP(state, pow4[H]) {
      if (dp[state].val() == 0) continue;
      auto pv = decode(state);
      DUMP(i, pv, dp[state]);
      REP(bits, 1 << H) {
        UnionFind uf(H + 3);
        vector<int> v(H, 0);
        REP(j, H) {
          if (has_bit(bits, j)) {
            if (pv[j] != 0) {
              uf.unite(j, H - 1 + pv[j]);
            }
          }
        }
        if (uf.size(H) == 1) continue;
        REP(j, 1, H) {
          if (has_bit(bits, j) and has_bit(bits, j - 1)) {
            uf.unite(j, j - 1);
          }
        }
        REP(trial, 3) {
          REP(j, H) {
            if (has_bit(bits, j)) {
              if (uf.same(j, H)) {
                v[j] = 1;
              } else if (uf.same(j, H + 1)) {
                v[j] = 2;
              } else if (uf.same(j, H + 2)) {
                v[j] = 3;
              } else {
                for (int z = 2; z <= 3; ++z) {
                  if (uf.size(H - 1 + z) == 1) {
                    uf.unite(j, H - 1 + z);
                    v[j] = z;
                    break;
                  }
                }
              }
            }
          }
        }
        if (uf.same(H + 2, H + 1)) {
          REP(j, H) if (v[j] == 3) v[j] = 2;
        }
        if (uf.same(H + 2, H)) {
          REP(j, H) if (v[j] == 3) v[j] = 1;
        }
        if (uf.same(H + 1, H)) {
          REP(j, H) if (v[j] == 2) v[j] = 1;
        }
        int j2 = H, j3 = H;
        REP(j, H) {
          if (v[j] == 2) chmin(j2, j);
          if (v[j] == 3) chmin(j3, j);
        }
        if (j2 > j3) {
          REP(j, H) {
            if (v[j] == 2) v[j] = 3;
            else if (v[j] == 3) v[j] = 2;
          }
        }
        int next_state = 0;
        REP(j, H) {
          next_state += pow4[j] * v[j];
        }
        DUMP(i, decode(state), bits, decode(next_state));
        ndp[next_state] += dp[state];
      }
    }
    swap(ndp, dp);
  }

  Mint ans = 0;
  REP(state, pow4[H]) {
    auto v = decode(state);
    if (v[H - 1] != 1) continue;
    if (dp[state].val() == 0) continue;
    DUMP(state, v, dp[state]);
//    if (state / pow3[H - 1] != 1) continue;
    ans += dp[state];
  }
  out(ans);
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    (solve());
  }
  exit_();
}

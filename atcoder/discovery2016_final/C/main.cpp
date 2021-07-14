#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

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
  return (int) std::size(a);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template<typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}
template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value &&
        !std::is_same<T, std::string_view>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

void read_from_cin() {}
template<typename T, typename... Ts>
void read_from_cin(T &value, Ts &...args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define INPUT(type, ...) \
  type __VA_ARGS__;      \
  read_from_cin(__VA_ARGS__)

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

auto solve() {
  INPUT(string, S);
  INPUT(int, K);
  const int slen = ssize(S);
  assert(slen % 2 == 0);
  const int n = slen / 2 + 1;
  vector<int> stk;
  stk.push_back(0);
  vector<int> parent(n, -1);
  vector<vector<int>> child(n);
  int v_counter = 1;
  REP(i, slen) {
    if (S[i] == '(') {
      int v = v_counter++;
      int p = stk.back();
      parent[v] = p;
      child[p].push_back(v);
      stk.push_back(v);
    } else {
      stk.pop_back();
    }
  }
  assert(v_counter == n);
  DUMP(parent);
  REP(v, n) {
    DUMP(v, child[v]);
  }

  // dp[v][k] := number of patterns where subtree of v contains k reds.
  auto dp = vector(n, vector(n * 2 + 1, Mint(0)));
  auto sizes = vector(n, 0);
  auto tmp = vector(n * 2 + 1, Mint(0));

  auto dfs = [&](auto &rec, int v) -> void {
    int v_size = 1;
    int v_rmax = 0;
    dp[v][0] = 1;
    sizes[v] = 1;
    for (auto u : child[v]) {
      rec(rec, u);
      sizes[v] += sizes[u];
    }
    for (auto u : child[v]) {
      int u_size = sizes[u];
      int u_rmax = (u_size - 1) * 2;
      int merged_size = v_size + u_size;
      int merged_rmax = v_rmax + u_rmax + 2;
      REP(k, sizes[v] * 2 + 1) {
        tmp[k] = 0;
      }
      REP(i, v_rmax + 1) {
        REP(j, u_rmax + 1) {
          auto x = dp[v][i] * dp[u][j];
          int bu = u_rmax - j;
          // BB
          if (abs(j - (bu + 2)) <= K) {
            tmp[i + j] += x;
          }
          // BR RB
          if (abs((j + 1) - (bu + 1)) <= K) {
            tmp[i + j + 1] += x * 2;
          }
          // RR
          if (abs((j + 2) - bu) <= K) {
            tmp[i + j + 2] += x;
          }
        }
      }
      swap(dp[v], tmp);
      v_size = merged_size;
      v_rmax = merged_rmax;
    }
    assert(v_size == sizes[v]);
  };
  dfs(dfs, 0);
  DUMP(dp[0]);
  return accumulate(ALL(dp[0]), Mint(0));
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}

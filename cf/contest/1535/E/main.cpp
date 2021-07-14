#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}
template <typename Container>
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
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;
static const int K = 21;  // max upper lookup (2^K)

int main() {
  int Q, a0, c0;
  scanf("%d %d %d", &Q, &a0, &c0);
  int n = 1;

  vector<vector<optional<int>>> upper(1, vector<optional<int>>(K));
  vector<int> depth(1, 0);
  vector<i64> gold(1, a0), cost(1, c0);
  upper.reserve(Q);
  depth.reserve(Q);
  gold.reserve(Q);
  cost.reserve(Q);

  auto ancestor = [&](int v, int i) -> optional<int> {
    for (int k = K - 1; k >= 0; --k) {
      if (i & (1 << k)) {
        if (not upper[v][k]) return nullopt;
        v = upper[v][k].value();
      }
    }
    return v;
  };

  auto min_steps = [&](int start, std::function<bool(int)> pred) -> i64 {
    long long max_false = 0;
    int i = start;
    for (int d = K - 1; d >= 0; --d) {
      auto j = upper[i][d];
      if (not j.has_value()) continue;
      if (pred(j.value())) continue;
      max_false += 1LL << d;
      i = j.value();
    }
    return max_false + 1;
  };

  vector<int> idmap(Q, -1);
  idmap[0] = 0;

  REP(q, Q) {
    int qtype;
    cin >> qtype;
    if (qtype == 1) {
      // Add a node.
      int i = n++;
      idmap[q + 1] = i;
      int p, a, c;
      std::scanf("%d %d %d", &p, &a, &c);
      p = idmap[p];
      assert(p >= 0);
      gold.push_back(a);
      cost.push_back(c);
      upper.push_back(vector<optional<int>>(K));
      depth.push_back(depth[p] + 1);
      upper[i][0] = p;
      for (int k = 0; k + 1 < K; ++k) {
        if (upper[i][k].has_value()) {
          upper[i][k + 1] = upper[upper[i][k].value()][k];
        }
      }
    } else {
      assert(qtype == 2);
      int v_, w;
      std::scanf("%d %d", &v_, &w);
      int v = idmap[v_];

      i64 bought = 0, spent = 0;
      if (v >= 0 and gold[v] > 0) {
        i64 fv = min_steps(v, [&](int x) -> bool { return gold[x] == 0; });
        for (int jump = fv - 1; w > 0 and jump >= 0; --jump) {
          auto x = ancestor(v, jump).value();
          i64 b = min<i64>(w, gold[x]);
          bought += b;
          spent += cost[x] * b;
          gold[x] -= b;
          w -= b;
        }
      }
      std::printf("%lld %lld\n", bought, spent);
      std::fflush(stdout);
    }
  }
}

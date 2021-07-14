#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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

template<class T>
T ceil_div(T x, T y) {
  assert(y != 0);
  return x / y + (((x ^ y) >= 0) and (x % y));
}

using namespace std;

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  INPUT(int, n, m);
  vector<unordered_set<int>> g(n);
  REP(i, m) {
    INPUT(int, u, v);
    --u, --v;
    g[u].insert(v);
    g[v].insert(u);
  }
  INPUT(int, Q);
  vector<pair<int, int>> queries(Q);
  REP(i, Q) {
    INPUT(int, x, color);
    --x;
    queries[i] = {x, color};
  }

  const int bsize = sqrt(Q) + 1;
  const int bcount = ceil_div(Q, bsize);
  DUMP(bsize, bcount);

  vector<pair<int, int>> colors(n, {-1, 1});
  vector<int> ans(Q, -1);
  REP(i, bcount) {
    const int l = bsize * i;
    const int r = min(bsize * (i + 1), Q);

    // Answer queries.
    for (int j = l; j < r; ++j) {
      auto[x, _] = queries[j];
      ans[j] = colors[x].second;
      for (int p = j - 1; p >= l; --p) {
        auto[y, cy] = queries[p];
        if (y == x or g[x].count(y)) {
          ans[j] = cy;
          break;
        }
      }
    }

    // Update colors.
    vector<char> updated(n, false);
    for (int j = r - 1; j >= l; --j) {
      auto[v, paint] = queries[j];
      if (updated[v]) continue;
      updated[v] = true;
      if (colors[v].first < j) {
        colors[v].first = j;
        colors[v].second = paint;
      }
      for (auto u : g[v]) {
        if (colors[u].first < j) {
          colors[u].first = j;
          colors[u].second = paint;
        }
      }
    }
  }

  REP(i, Q) {
    print(ans[i]);
  }
}

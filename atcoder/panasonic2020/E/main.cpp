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
  return (int) a.size();
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  if constexpr (std::is_same_v<T, bool>) {
    std::cout << (x ? "Yes" : "No") << "\n";
  } else {
    std::cout << x << "\n";
  }
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
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
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct SizedInput {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  SizedInput operator()(std::size_t n) const { return {n}; }
} const in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

auto solve() {
  const string a = in, b = in, c = in;
  const int na = a.size(), nb = b.size(), nc = c.size();

  auto check = [&](const string &s, const string &t, int i) -> bool {
    if (i + ssize(s) - 1 < 0) return true;
    if (i >= ssize(t)) return true;
    for (int j = 0; j < ssize(t); ++j) {
      if (j - i < 0) continue;
      if (j - i >= ssize(s)) break;
      if (s[j - i] == '?') continue;
      if (t[j] == '?') continue;
      if (s[j - i] != t[j]) return false;
    }
    return true;
  };
  vector<int> ax, bx;
  unordered_set<int> dx;
  for (int i = -na - nb; i <= nc + nb; ++i) {
    if (check(a, c, i)) ax.push_back(i);
  }
  for (int i = -na - nb; i <= nc + na; ++i) {
    if (check(b, c, i)) bx.push_back(i);
  }
  for (int i = -na - nc; i <= nc + nb; ++i) {
    if (check(a, b, i)) dx.insert(i);
  }
  int ans = na + nb + nc;
  for (auto i: ax) {
    for (auto j: bx) {
      const int k = i - j;
      array<pair<int, int>, 3> iv = {pair{0, nc}, pair{i, i + na}, pair{j, j + nb}};
      sort(ALL(iv));
      int lb = iv[0].first;
      int ub = iv[0].second;
      if (ub < iv[1].first) continue;
      chmax(ub, iv[1].second);
      if (ub < iv[2].first) continue;
      chmax(ub, iv[2].second);
      if (ub - lb >= ans) continue;
      if (not dx.count(k)) continue;
      chmin(ans, ub - lb);
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  auto ans = solve();
  print(ans);
}

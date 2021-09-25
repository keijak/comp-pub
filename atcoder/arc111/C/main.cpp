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

auto solve() -> optional<vector<pair<int, int>>> {
  const int n = in;
  vector<i64> a = in(n), b = in(n), p = in(n);
  REP(i, n) --p[i];
  vector<int> done(n, false);
  REP(i, n) {
    if (p[i] == i) {
      done[i] = true;
      continue;
    }
    if (a[i] <= b[p[i]]) return nullopt;
  }
  vector<int> idx(n);
  REP(i, n) idx[i] = i;
  sort(ALL(idx), [&](int i, int j) { return a[i] < a[j]; });

  vector<pair<int, int>> ans;
  auto rev = vector(n, -1);
  REP(i, n) {
    rev[p[i]] = i;
  }
  DUMP(idx);
  DUMP(rev);
  for (auto j: idx) {
    if (done[j]) continue;
    if (rev[j] == j) continue;
    if (p[j] == j) continue;
    assert(a[j] > b[p[j]]);
    if (rev[j] == -1) return nullopt;
    const int i = rev[j];
    assert(not done[i]);
    ans.emplace_back(j, i);
    rev[j] = j;
    rev[p[j]] = i;
    swap(p[j], p[i]);
    assert(b[p[j]] == b[j]);
    done[j] = true;
    if (p[i] == i) {
      done[i] = true;
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    if (not ans) {
      print(-1);
      continue;
    }
    print(ans.value().size());
    for (auto[i, j]: ans.value()) {
      print(i + 1, j + 1);
    }
  }
}

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
  const int n = in, m = in;
  vector<int> S = in(m);
  DUMP(S);
  vector<multiset<int>> P(n);
  REP(i, n) {
    REP(j, m) {
      int p = in;
      P[i].insert(p);
    }
    DUMP(i, P[i]);
  }

  priority_queue<tuple<int, int>> heap;
  REP(j, m) {
    heap.emplace(0, j);
  }
  i64 ans = 0;
  REP(round, n) {
    priority_queue<tuple<int, int>> heap2;
    vector<tuple<int, int>> to_change;
    while (not heap.empty()) {
      auto[changed, mo] = heap.top();
      heap.pop();
      auto it = P[round].find(S[mo]);
      if (it != P[round].end()) {
        P[round].erase(it);
        heap2.emplace(changed, mo);
      } else {
        to_change.emplace_back(changed, mo);
      }
    }
    for (auto[changed, mo]: to_change) {
      auto it = P[round].begin();
      S[mo] = *it;
      P[round].erase(it);
      if (changed >= 1) ++ans;
      heap2.emplace(changed + 1, mo);
    }
    heap.swap(heap2);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = in;
  REP(test_case, t) {
    cout << "Case #" << (test_case + 1) << ": ";
    auto ans = solve();
    print(ans);
  }
}

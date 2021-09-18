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

template<typename T = long long>
struct PWLConvexCurve {
  static constexpr T kInf = std::numeric_limits<T>::max() / 2;

  T fmin;
  std::priority_queue<T, std::vector<T>, std::less<>> lq;
  std::priority_queue<T, std::vector<T>, std::greater<>> rq;
  PWLConvexCurve() : fmin{0} {
    lq.push(-kInf);
    rq.push(kInf);
  }

  void add_constant(T c) { fmin += c; }

  // Add g(x) = max(x-a, 0)
  void add_x_minus_a(T a) {
    fmin += max<T>(lq.top() - a, 0);
    lq.push(a);
    rq.push(lq.top());
    lq.pop();
  }

  // Add g(x) = max(a-x, 0)
  void add_a_minus_x(T a) {
    fmin += max<T>(a - rq.top(), 0);
    rq.push(a);
    lq.push(rq.top());
    rq.pop();
  }

  // Add g(x) = abs(x - a)
  void add_abs(T a) {
    add_x_minus_a(a);
    add_a_minus_x(a);
  }

  // Flatten the left slope.
  void cum_max() {
    while (not lq.empty()) lq.pop();
  }

  // Flatten the right slope.
  void cum_min() {
    while (not rq.empty()) rq.pop();
  }

  T get_min() const { return fmin; }
};
template<typename T>
std::ostream &operator<<(std::ostream &os, const PWLConvexCurve<T> &curve) {
  return os << "fmin=" << curve.fmin << " l0=" << curve.lq.top()
            << " r0=" << curve.rq.top();
}

auto solve() {
  const int n = in, L = in;
  PWLConvexCurve curve;
  map<int, vector<int>> qs;
  REP(i, n) {
    int t = in, p = in;
    qs[t].push_back(p);
  }
  for (const auto&[t, ps]: qs) {
    curve.cum_min();
    for (const auto &p: ps) {
      curve.add_abs(p);
    }
  }
  return curve.get_min();
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    print(ans);
  }
}

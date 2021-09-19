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

template<typename T = long long>
struct PWLConvexCurve {
  static constexpr T kInf = std::numeric_limits<T>::max() / 2;

 private:
  std::priority_queue<T, std::vector<T>, std::less<>> lq_;
  std::priority_queue<T, std::vector<T>, std::greater<>> rq_;
  T min_;
  T l_offset_;
  T r_offset_;

 public:
  PWLConvexCurve() : min_{0}, l_offset_{0}, r_offset_{0} {}

  T min() const { return min_; }

  // Adds g(x) = c
  void add_constant(T c) { min_ += c; }

  // Adds g(x) = max(x-a, 0)
  void add_x_minus_a(T a) {
    min_ += std::max<T>(top_l() - a, 0);
    push_l(a);
    push_r(top_l());
    lq_.pop();
  }

  // Adds g(x) = max(a-x, 0)
  void add_a_minus_x(T a) {
    min_ += std::max<T>(a - top_r(), 0);
    push_r(a);
    push_l(top_r());
    rq_.pop();
  }

  // Adds g(x) = abs(x - a)
  void add_abs(T a) {
    add_x_minus_a(a);
    add_a_minus_x(a);
  }

  // Flattens the left slope.
  void cum_max() {
    while (not lq_.empty()) lq_.pop();
  }

  // Flattens the right slope.
  void cum_min() {
    while (not rq_.empty()) rq_.pop();
  }

  // Shifts the left points toward right by a.
  // Shifts the right points toward right by b.
  // Can widen the flat interval.
  // \_/ => \___/
  void shift(T a, T b) {
    assert(a <= b);
    l_offset_ += a;
    r_offset_ += b;
  }

  // Shifts all points toward right by a.
  void shift(T a) {
    shift(a, a);
  }

  friend std::ostream &operator<<(std::ostream &os, const PWLConvexCurve &curve) {
    return os << "min=" << curve.min() << " l0=" << curve.top_l()
              << " r0=" << curve.top_r();
  }
 private:
  void push_l(T a) {
    lq_.push(a - l_offset_);
  }
  void push_r(T a) {
    rq_.push(a - r_offset_);
  }
  T top_l() const {
    if (lq_.empty()) return -kInf;
    return lq_.top() + l_offset_;
  }
  T top_r() const {
    if (rq_.empty()) return kInf;
    return rq_.top() + r_offset_;
  }
};

auto solve() {
  const int n = in;
  PWLConvexCurve curve;
  REP(i, n) {
    curve.add_abs(0);
  }
  i64 pt = 0;
  REP(i, n) {
    const i64 t = in, d = in, a = in;
    i64 dt = t - pt;
    curve.shift(-dt, dt);
    if (d == 0) {
      curve.add_a_minus_x(a);
    } else {
      curve.add_x_minus_a(a);
    }
    pt = t;
  }
  return curve.min();
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

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
#include <variant>

auto solve() -> std::variant<int, char> {
  i64 R = in, G = in, B = in;
  vector<pair<i64, char>> s;
  s.emplace_back(R, 'R');
  s.emplace_back(G, 'G');
  s.emplace_back(B, 'B');
  sort(ALL(s));
  if (s[1].first - s[0].first > 1) {
    i64 d = (s[1].first - s[0].first) / 2;
    s[0].first += d;
    s[1].first -= d;
    s[2].first -= d;
  }
  DUMP(s);
  if (s[2].first - s[1].first > 1) {
    i64 d = (s[2].first - s[1].first) / 2;
    s[2].first -= d * 2;
  }
  assert(s[0].first <= s[1].first and s[1].first <= s[2].first);
  assert(s[0].first + 1 >= s[1].first and s[1].first + 1 >= s[2].first);
  DUMP(s);
  if (s[2].first - s[0].first == 2) {
    s[0].first += 1;
    s[1].first -= 1;
    s[2].first -= 1;
  }
  sort(ALL(s));
  DUMP(s);
  if (s[0].first >= 2) {
    i64 x = (s[0].first - 1);
    s[0].first -= x;
    s[1].first -= x;
    s[2].first -= x;
  }
  DUMP(s);
  if (s[0].first == 1 and s[1].first == 1 and s[2].first == 1) return 2;
  if (s[0].first == 1 and s[1].first == 1 and s[2].first == 2) return s[2].second;
  if (s[0].first == 1 and s[1].first == 2 and s[2].first == 2) return s[0].second;
  assert(false);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(18);
  const int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    if (holds_alternative<int>(ans)) {
      print(get<int>(ans));
    } else {
      print(get<char>(ans));
    }
  }
}
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

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
} const in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

template<int sign = 1>
struct Infinity {
  template<typename T>
  constexpr operator T() const {
    static_assert(sign == 1 or not std::is_unsigned_v<T>,
                  "must be positive in an unsigned type");
    if constexpr (std::numeric_limits<T>::has_infinity) {
      return T(sign) * std::numeric_limits<T>::infinity();
    } else {
      return T(sign) * (std::numeric_limits<T>::max() / T(2));
    }
  }
  constexpr Infinity<sign * -1> operator-() const { return {}; }
  template<typename T>
  friend constexpr bool operator==(const T &x, const Infinity &y) {
    return x == T(y);
  }
  template<typename T>
  friend constexpr bool operator!=(const T &x, const Infinity &y) {
    return x != T(y);
  }
};
constexpr Infinity<> big;

auto solve() -> optional<vector<int>> {
  int n = in;
  vector<int> a(n);
  cin >> a;

  for (int i = 0; i < n; i += 2) {
    if (a[i] % 2 == 0) return nullopt;
  }
  for (int i = 1; i < n; i += 2) {
    if (a[i] % 2 == 1) return nullopt;
  }
  a.push_back(big);

  int limit = 5 * n / 2;

  vector<int> ans;

  for (int x = n; x >= 3; x -= 2) {
    if (std::is_sorted(ALL(a))) break;

    int pos = find(ALL(a), x) - a.begin();
    DUMP(x, pos, a, ans);

    if (pos > 0) {
      ans.push_back(pos + 1);
      reverse(a.begin(), a.begin() + (pos + 1));
    }

    pos = find(ALL(a), x - 1) - a.begin();
    if (pos > 1) {
      ans.push_back(pos);
      reverse(a.begin(), a.begin() + pos);
    }

    pos = find(ALL(a), x) - a.begin();
    if (pos > 0) {
      ans.push_back(x);
      reverse(a.begin(), a.begin() + x);
    }

    pos = find(ALL(a), x) - a.begin();
    if (pos > 0) {
      ans.push_back(pos + 1);
      reverse(a.begin(), a.begin() + (pos + 1));
    }

    assert(a[0] == x);
    assert(a[1] == x - 1);
    ans.push_back(x);
    reverse(a.begin(), a.begin() + x);
  }
  DUMP(ssize(ans), limit);

  assert(ssize(ans) <= limit);
  assert (std::is_sorted(ALL(a)));
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  const int t = in;
  REP(test_case, t) {
    DUMP(test_case);
    auto ans = solve();
    if (not ans) {
      print(-1);
    } else {
      int m = ssize(*ans);
      print(m);
      if (m) print_seq(*ans);
    }
  }
}

#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

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
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef MY_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &...args) {
  pdebug(value);
  std::cerr << ", ";
  pdebug(args...);
}
#define DEBUG(...)                                   \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ") "; \
    std::cerr << #__VA_ARGS__ << ":\033[0m ";        \
    pdebug(__VA_ARGS__);                             \
    std::cerr << std::endl;                          \
  } while (0)
#else
#define pdebug(...)
#define DEBUG(...)
#endif

using namespace std;

// Returns all divisors of n. O(sqrt(n)) + sorting.
std::vector<i64> divisors(i64 n) {
  std::vector<i64> res;
  for (i64 k = 1; k * k <= n; ++k) {
    if (n % k != 0) continue;
    res.push_back(k);
    i64 q = n / k;
    if (q != k) res.push_back(q);
  }
  std::sort(res.begin(), res.end());
  return res;
}

optional<array<pair<u64, u64>, 3>> solve() {
  u64 n, m, k;
  cin >> n >> m >> k;
  if ((2 * n * m) % k != 0) return nullopt;

  //   u64 g = gcd(n, k);
  //   u64 x = n / g;
  //   u64 z = k / g;
  //   u64 y = m / gcd(m, z);
  //   if (x * 2 <= n) {
  //     return array{pair{0ull, 0ull}, pair{x * 2, 0ull}, pair{0ull, y}};
  //   }
  //   if (y * 2 <= m) {
  //     return array{pair{0ull, 0ull}, pair{x, 0ull}, pair{0ull, y * 2}};
  //   }

  auto tri = [&](u64 x, u64 y) -> array<pair<u64, u64>, 3> {
    return array{pair{0ull, 0ull}, pair{x, 0ull}, pair{0ull, y}};
  };

  for (u64 a = 1; a * a <= k; ++a) {
    if (k % a != 0) continue;
    u64 b = k / a;

    if (m % a == 0 and (2 * n) % b == 0) {
      u64 y = m / a, x = (2 * n) / b;
      if (y <= m and x <= n) return tri(x, y);
      if (x <= m and y <= n) return tri(y, x);
    }

    if (m % b == 0 and (2 * n) % a == 0) {
      u64 y = m / b, x = (2 * n) / a;
      if (y <= m and x <= n) return tri(x, y);
      if (x <= m and y <= n) return tri(y, x);
    }

    if ((2 * m) % a == 0 and n % b == 0) {
      u64 y = (2 * m) / a, x = n / b;
      if (y <= m and x <= n) return tri(x, y);
      if (x <= m and y <= n) return tri(y, x);
    }

    if ((2 * m) % b == 0 and n % a == 0) {
      u64 y = (2 * m) / b, x = n / a;
      if (y <= m and x <= n) return tri(x, y);
      if (x <= m and y <= n) return tri(y, x);
    }

    // if (a <= m and b <= n) {
    //   return array{pair{0ull, 0ull}, pair{b, 0ull}, pair{0ull, a}};
    // }
    // if (a <= n and b <= m) {
    //   return array{pair{0ull, 0ull}, pair{a, 0ull}, pair{0ull, b}};
    // }
  }
  return nullopt;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  auto res = solve();
  if (!res) {
    cout << "NO\n";
  } else {
    auto [p1, p2, p3] = res.value();
    cout << "YES\n";
    cout << p1.first << " " << p1.second << "\n";
    cout << p2.first << " " << p2.second << "\n";
    cout << p3.first << " " << p3.second << "\n";
  }
}

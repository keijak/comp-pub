#include <bits/stdc++.h>
using i64 = std::int64_t;
using u64 = std::uint64_t;
#define REP(i, n) for (int i = 0; i < (i64)(n); ++i)
#define ALL(x) std::begin(x), std::end(x)
#define SIZE(a) (int)((a).size())

template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}

template <typename T>
using V = std::vector<T>;
template <typename T>
std::vector<T> make_vec(size_t n, T a) {
  return std::vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return std::vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
}
template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &pprint(const Container &a, std::string_view sep = " ",
                     std::string_view ends = "\n", std::ostream *os = nullptr) {
  if (os == nullptr) os = &std::cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
}
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T,
          typename = std::enable_if_t<is_iterable<T>::value &&
                                      !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
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
void pdebug(const T &value, const Ts &... args) {
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

int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);

  i64 K, X, Y;
  cin >> K >> X >> Y;
  const i64 ax = abs(X), ay = abs(Y);

  i64 min_steps = (ax + ay + K - 1) / K;
  i64 excess = (min_steps * K - (ax + ay)) % K;

  if (excess % 2 == 1 and K % 2 == 0) {
    cout << -1 << endl;
    return 0;
  }
  if (excess % 2 == 1) {
    excess += K;
    min_steps++;
  }
  assert(excess % 2 == 0);
  assert(excess / 2 < K);
  assert(ax + ay + excess == min_steps * K);

  V<pair<i64, i64>> path;
  auto make_path = [&]() {
    V<i64> strides;
    i64 d = ax + ay;
    if (excess == 0) {
      REP(i, min_steps) strides.push_back(K);
    } else if (d < K and d % 2 == 0) {
      strides.push_back(d / 2);
      strides.push_back(d / 2);
    } else if (d <= K - excess / 2) {
      strides.push_back(K);
      strides.push_back(excess / 2);
      strides.push_back(excess / 2);
    } else {
      strides.push_back(K - excess / 2);
      strides.push_back(K - excess / 2);
      assert(min_steps >= 2);
      REP(i, min_steps - 2) strides.push_back(K);
    }

    i64 x = 0, y = 0;
    for (int i = 0; i < SIZE(strides);) {
      if (strides[i] == K) {
        x += K;
        if (x > ax) {
          y += (x - ax) * (y > ay ? -1 : 1);
          x = ax;
        }
        path.emplace_back(x, y);
        ++i;
      } else {
        assert(strides[i] == strides[i + 1]);
        i64 x1 = x + strides[i];
        i64 y1 = y;
        if (x1 > ax) {
          y1 += (x1 - ax) * (y1 > ay ? -1 : 1);
          x1 = ax + (K - strides[i]);
        } else {
          y1 -= (K - strides[i]);
        }
        path.emplace_back(x1, y1);

        x += 2 * strides[i];
        if (x > ax) {
          y += (x - ax) * (y > ay ? -1 : 1);
          x = ax;
        }
        path.emplace_back(x, y);
        i += 2;
      }
    }
    assert(x == ax and y == ay);
  };
  make_path();
  DEBUG(path);
  cout << path.size() << '\n';
  for (auto [x, y] : path) {
    if (X < 0) x *= -1;
    if (Y < 0) y *= -1;
    cout << x << ' ' << y << '\n';
  }
}

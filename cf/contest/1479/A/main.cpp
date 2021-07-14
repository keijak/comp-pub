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
                        std::ostream *os = nullptr) {
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

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", &(os << "{")) << "}";
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

int sign(int x) {
  if (x < 0) return -1;
  if (x > 0) return +1;
  return 0;
}

const int INF = 1e6;

int main() {
  ios_base::sync_with_stdio(false);
  int n;
  cin >> n;
  vector<int> delta(n + 1, 0);
  vector<optional<int>> a(n + 2);
  a[0] = INF;
  a[n + 1] = INF;
  delta[0] = -1;
  delta[n] = +1;
  int lo = 1, hi = n;
  int q = 100;

  auto update = [&](int i, int val) -> void {
    a[i] = val;
    if (a[i - 1].has_value()) {
      delta[i - 1] = sign(val - *a[i - 1]);
    }
    if (a[i + 1].has_value()) {
      delta[i] = sign(*a[i + 1] - val);
    }
    if (i - 2 >= 0 and delta[i - 2] < 0 and delta[i - 1] > 0) {
      cout << "! " << i - 1 << endl;
      exit(0);
    }
    if (i - 1 >= 0 and delta[i - 1] < 0 and delta[i] > 0) {
      cout << "! " << i << endl;
      exit(0);
    }
    if (i + 1 <= n and delta[i] < 0 and delta[i + 1] > 0) {
      cout << "! " << i + 1 << endl;
      exit(0);
    }
  };

  auto ask_all = [&]() -> int {
    for (int i = lo; i <= hi; ++i) {
      if (not a[i].has_value()) {
        cout << "? " << i << endl;
        int res;
        cin >> res;
        update(i, res);
      }
    }
    for (int i = lo; i <= hi; ++i) {
      if (delta[i - 1] < 0 and delta[i] > 0) {
        return i;
      }
    }
    assert(false);
  };

  while (q > 0) {
    DEBUG(q, hi, lo);
    if (q >= hi - lo + 1) {
      int ans = ask_all();
      cout << "! " << ans << endl;
      exit(0);
    }
    int ml = (hi + lo) / 2;
    int mh = ml + 1;
    if (not a[ml]) {
      assert(q-- > 0);
      cout << "? " << ml << endl;
      int res;
      cin >> res;
      update(ml, res);
    }
    if (not a[mh]) {
      assert(q-- > 0);
      cout << "? " << mh << endl;
      int res;
      cin >> res;
      update(mh, res);
    }
    assert(abs(delta[ml]) == 1);
    if (delta[ml] < 0) {
      lo = mh;
    } else {
      hi = ml;
    }
  }
}

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
const i64 BIG = (1LL << 40) - 1;

i64 solve() {
  int n;
  cin >> n;
  vector<i64> a(n + 2);
  REP(i, n) {
    cin >> a[i + 1];
    assert(BIG > a[i + 1]);
  }
  a[0] = a[n + 1] = BIG;

  vector<pair<i64, int>> sa(n);
  REP(i, 1, n + 1) { sa[i - 1] = {a[i], i}; }
  sort(ALL(sa), greater<>());

  vector<int> next_left(n + 2), next_right(n + 2);
  vector<int> max_left(40, 0), min_right(40, n + 1);
  REP(i, 1, n + 1) {
    int l = 0;
    REP(j, 40) {
      i64 bj = 1LL << j;
      if (a[i] & bj) {
        max_left[j] = i;
      } else {
        chmax(l, max_left[j]);
      }
    }
    next_left[i] = l;
  }
  for (int i = n; i >= 1; --i) {
    int r = n + 1;
    REP(j, 40) {
      i64 bj = 1LL << j;
      if (a[i] & bj) {
        min_right[j] = i;
      } else {
        chmin(r, min_right[j]);
      }
    }
    next_right[i] = r;
  }

  map<int, i64> placed;
  placed[0] = placed[n + 1] = BIG;
  i64 ans = 0;
  for (auto [h, idx] : sa) {
    assert(a[idx] == h);
    auto pit = placed.upper_bound(idx);
    assert(pit != placed.end());
    assert(pit != placed.begin());
    i64 r_big = pit->first;
    --pit;
    i64 l_big = pit->first;
    i64 r_c = next_right[idx];
    i64 l_c = next_left[idx];
    if (l_big <= l_c and r_c <= r_big) {
      ans += (r_big - r_c) * (l_c - l_big);  // including both ends
    }
    if (r_c <= r_big) {
      ans += (r_big - r_c) * (idx - max(l_c, l_big));  // including only r_c
    }
    if (l_c >= l_big) {
      ans += (min(r_c, r_big) - idx) * (l_c - l_big);  // including only l_c
    }
    placed[idx] = h;
  }

  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}

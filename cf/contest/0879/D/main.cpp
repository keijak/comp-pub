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

using Int = __int128_t;

u64 solve() {
  u64 n, k, m;
  cin >> n >> k >> m;
  vector<i64> a(n);
  cin >> a;

  vector<pair<i64, u64>> r;  // run length
  REP(i, n) {
    if (r.empty() or r.back().first != a[i]) {
      r.emplace_back(a[i], 1);
    } else {
      if (++(r.back().second) == k) {
        r.pop_back();
      }
    }
  }
  DEBUG(r);

  u64 rsum = 0;
  for (auto [x, cnt] : r) {
    assert(cnt > 0);
    assert(cnt < k);
    rsum += cnt;
  }

  Int rsz = ssize(r);
  if (rsz == 0) return 0;
  if (rsz == 1) return (r[0].second * m) % k;
  if (r.front().first != r.back().first) return rsum * m;
  if (m == 1) return rsum;

  int q = 0, p = rsz - 1;
  while (p >= 0) {
    if (r[p].first != r[q].first) break;
    Int c = r[p].second + r[q].second;
    if (c % k == 0) {
      --p, ++q;
      continue;
    }
    break;
  }

  if (p < 0) {
    if (m % 2 == 0) return 0;
    return rsum;
  }
  u64 ans = 0;
  REP(i, q) { ans += r[i].second; }
  REP(i, p + 1, rsz) { ans += r[i].second; }
  if (p == q) {
    DEBUG(p, q);
    u64 med = (r[p].second * m) % k;
    if (med == 0) {
      vector<pair<i64, u64>> r2;
      REP(i, q) r2.push_back(r[i]);
      REP(i, p + 1, rsz) {
        auto [x, cnt] = r[i];
        if (r2.empty()) {
          r2.emplace_back(x, cnt);
          continue;
        }
        auto [x2, cnt2] = r2.back();
        if (x != x2) {
          r2.emplace_back(x, cnt);
          continue;
        }
        u64 csum = cnt + cnt2;
        if (csum % k == 0) {
          r2.pop_back();
        } else {
          r2.back().second = csum % k;
        }
      }
      ans = 0;
      for (auto [x, cnt] : r2) ans += cnt;
      return ans;
    }
    ans += med;
  } else {
    u64 med = 0;
    for (int i = q; i <= p; ++i) {
      med += r[i].second % k;
    }
    u64 del = 0;
    if (r[p].first == r[q].first) {
      u64 c = r[p].second + r[q].second;
      del = c - c % k;
    }
    ans += med + (med - del) * (m - 1);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}

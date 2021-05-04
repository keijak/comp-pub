#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;

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

#ifdef ENABLE_DEBUG
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

const i64 BIG = i64(1e9) + 10;
const u32 FULL = (1 << 5) - 1;

i64 solve() {
  int n;
  cin >> n;
  vector<array<i64, 5>> a(n);
  REP(i, n) { cin >> a[i][0] >> a[i][1] >> a[i][2] >> a[i][3] >> a[i][4]; }
  i64 tv = 0, fv = BIG;
  while (fv - tv > 1) {
    i64 mid = (tv + fv) / 2;
    set<u32> masks;
    REP(i, n) {
      u32 mask = 0;
      REP(j, 5) {
        if (a[i][j] >= mid) {
          mask |= (1 << j);
        }
      }
      masks.insert(mask);
    }
    vector<u32> mv(ALL(masks));
    const int m = ssize(mv);
    bool ok = false;
    REP(i, m) {
      if (mv[i] == FULL) {
        ok = true;
        goto loopend;
      }
      REP(j, i) {
        u32 xij = mv[i] | mv[j];
        if (xij == FULL) {
          ok = true;
          goto loopend;
        }
        REP(k, j) {
          u32 x = mv[i] | mv[j] | mv[k];
          if (x == FULL) {
            ok = true;
            goto loopend;
          }
        }
      }
    }
  loopend:
    if (ok) {
      tv = mid;
    } else {
      fv = mid;
    }
  }
  return tv;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}
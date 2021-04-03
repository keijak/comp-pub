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

// Most Significant Set Bit (Highest One Bit)
int mssb_pos(unsigned x) {
  static const int CLZ_WIDTH = __builtin_clz(1);
  assert(x != 0);
  return CLZ_WIDTH - __builtin_clz(x);
}
template <typename U>
inline U mssb(U x) {
  return U(1) << mssb_pos(x);
}
template <typename U>
inline U bit_floor(U x) {
  return U(1) << mssb_pos(x);
}
template <typename U>
U bit_ceil(U x) {
  auto ret = bit_floor(x);
  return (ret == x) ? ret : (ret << 1);
}

#include <ext/pb_ds/assoc_container.hpp>
template <class K, class V>
using gp_hash_table = __gnu_pbds::gp_hash_table<
    K, V, std::hash<K>, std::equal_to<K>,
    __gnu_pbds::direct_mask_range_hashing<>, __gnu_pbds::linear_probe_fn<>,
    __gnu_pbds::hash_standard_resize_policy<
        __gnu_pbds::hash_exponential_size_policy<>,
        __gnu_pbds::hash_load_check_resize_trigger<>, true>>;

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int n, m, q;
  cin >> n >> m >> q;
  vector<string> c(n);
  REP(i, n) {
    cin >> c[i];
    assert(ssize(c[i]) == m);
  }

  vector cum(n + 1, vector(m + 1, 0LL));
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= m; ++j) {
      i64 s = cum[i - 1][j] + cum[i][j - 1] - cum[i - 1][j - 1];
      s += (c[i - 1][j - 1] == '1') ? 1 : 0;
      cum[i][j] = s;
    }
  }

  gp_hash_table<u64, i64> memo;
  memo.resize(1 << 22);

  auto f = [&](auto &f, i64 x, i64 y) -> i64 {
    if (x <= n and y <= m) {
      return cum[x][y];
    }
    u64 mkey = (u64(x) << 32ULL) | y;
    if (auto it = memo.find(mkey); it != memo.end()) {
      return it->second;
    }
    if (x <= n) {
      i64 by = m;
      while (by * 2 < y) by *= 2;
      i64 o1 = f(f, x, by);
      i64 z1 = x * by - o1;
      if (by * 2 == y) {
        o1 += z1;
      } else {
        i64 o2 = f(f, x, y - by);
        i64 z2 = x * (y - by) - o2;
        o1 += z2;
      }
      // memo[mkey] = o1;
      return o1;
    }
    if (y <= m) {
      i64 bx = n;
      while (bx * 2 < x) bx *= 2;
      i64 o1 = f(f, bx, y);
      i64 z1 = bx * y - o1;
      if (bx * 2 == x) {
        o1 += z1;
      } else {
        i64 o2 = f(f, x - bx, y);
        i64 z2 = (x - bx) * y - o2;
        o1 += z2;
      }
      // memo[mkey] = o1;
      return o1;
    }
    i64 bx = n;
    while (bx * 2 < x) bx *= 2;
    i64 by = m;
    while (by * 2 < y) by *= 2;
    i64 o1 = f(f, bx, by);
    i64 z1 = bx * by - o1;
    i64 o2 = 0;
    i64 o3 = 0;
    if (bx * 2 == x) {
      o2 = z1;
    } else {
      i64 z2 = f(f, x - bx, by);
      o2 = (x - bx) * by - z2;
    }
    if (by * 2 == y) {
      o3 = z1;
    } else {
      i64 z3 = f(f, bx, y - by);
      o3 = bx * (y - by) - z3;
    }
    i64 o4 = 0;
    bool do_memo = false;
    if (bx * 2 == x and by * 2 == y) {
      o4 = o1;
      do_memo = true;
    } else if (y - by > 0 and x - bx > 0) {
      o4 = f(f, x - bx, y - by);
    }
    o1 = o1 + o2 + o3 + o4;
    if (do_memo) memo[mkey] = o1;
    return o1;
  };

  REP(qi, q) {
    i64 x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    // memo.clear();
    auto o1 = f(f, x1 - 1, y1 - 1);
    auto o2 = f(f, x2, y1 - 1);
    auto o3 = f(f, x1 - 1, y2);
    auto o4 = f(f, x2, y2);
    DEBUG(o4, o3, o2, o1);
    i64 ans = o4 - o3 - o2 + o1;
    cout << ans << "\n";
  }
}

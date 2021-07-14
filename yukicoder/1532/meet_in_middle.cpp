#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

#include <boost/multiprecision/cpp_int.hpp>
using Int = boost::multiprecision::checked_int128_t;
// using i256 = boost::multiprecision::int256_t;
// using i512 = boost::multiprecision::int512_t;
// using i1024 = boost::multiprecision::int1024_t;
// using Int = boost::multiprecision::cpp_int;

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
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

#include <ext/pb_ds/assoc_container.hpp>

template <class K, class V>
using gp_hash_table = __gnu_pbds::gp_hash_table<
    K, V, std::hash<K>, std::equal_to<K>,
    __gnu_pbds::direct_mask_range_hashing<K>, __gnu_pbds::linear_probe_fn<>,
    __gnu_pbds::hash_standard_resize_policy<
        __gnu_pbds::hash_exponential_size_policy<>,
        __gnu_pbds::hash_load_check_resize_trigger<true>, true>>;

std::mt19937_64 &PRNG() {
  static std::random_device seed_gen;
  static std::mt19937_64 engine(seed_gen());  // non-deterministic
  return engine;
}

auto solve() {
  i64 n, K;
  cin >> n >> K;
  vector<int> va, vb;
  for (int i = 2; i <= n; ++i) {
    if (i & 1) {
      va.push_back(i);
    } else {
      vb.push_back(i);
    }
  }
  // for (int i = 2; i <= n; ++i) va.push_back(i);
  // shuffle(ALL(va), PRNG());
  // REP(i, n / 2) {
  //   vb.push_back(va.back());
  //   va.pop_back();
  // }
  sort(ALL(va), greater{});
  sort(ALL(vb), greater{});
  DUMP(va, vb);

  // auto f = [&](auto &f, const vector<int> &v, int i, i64 acc,
  //              gp_hash_table<i64, i64> &mp) -> void {
  //   const int m = ssize(v);
  //   if (i == m) {
  //     return;
  //   }

  //   i64 nex = acc * v[i];
  //   if (nex > K) return;
  //   f(f, v, i + 1, acc, mp);
  //   ++mp[nex];
  //   f(f, v, i + 1, nex, mp);
  // };

  using Map = unordered_map<i64, i64>;

  auto f = [&](const vector<int> &v, Map &mp) -> void {
    vector<pair<i64, int>> q;
    q.reserve(10000000);
    q.push_back({1, 0});
    while (not q.empty()) {
      auto [prod, last] = move(q.back());
      q.pop_back();
      ++mp[prod];
      // DUMP(prod, count);
      for (i64 i = last; i < (int)v.size(); ++i) {
        i64 np = prod * v[i];
        if (np > K) continue;
        q.push_back({np, i + 1});
      }
    }
  };

  Map ma, mb;
  ma.reserve(1 << 20);
  ma.max_load_factor(0.25);
  mb.reserve(1 << 20);
  mb.max_load_factor(0.25);

  // ma.resize(1 << 20);
  // mb.resize(1 << 20);
  // ma[1] = 1;
  // mb[1] = 1;
  f(va, ma);
  cerr << "ma:" << ssize(ma) << endl;
  f(vb, mb);
  DUMP(ma);
  DUMP(mb);
  cerr << "mb:" << ssize(mb) << endl;
  vector<pair<i64, i64>> ca(ssize(ma) + 1), cb(ssize(mb));
  {
    int i = 0;
    for (auto [k, v] : ma) {
      ++i;
      ca[i] = {k, v};
      // ca[i].second += ca[i - 1].second;
    }
    sort(ALL(ca));
    REP(j, 1, ssize(ca)) { ca[j].second += ca[j - 1].second; }
    i = 0;
    for (auto [k, v] : mb) {
      cb[i] = {k, v};
      ++i;
      // cb[i].second += cb[i - 1].second;
    }
    sort(ALL(cb), greater{});
  }
  ca.emplace_back(K + 1, 0);
  DUMP(ca);
  DUMP(cb);
  i64 ans = 0;
  {
    int i = 1;
    for (const auto &[x2, k2] : cb) {
      while (i < ssize(ca) and ca[i].first * x2 <= K) {
        ++i;
      }
      ans += k2 * ca[i - 1].second;
      DUMP(x2, k2, ca[i - 1].second, ans);
    }
  }
  ans *= 2;
  return ans - 1;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}

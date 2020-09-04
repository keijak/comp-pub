#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>
using __gnu_pbds::gp_hash_table;

using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

template <typename T>
using V = std::vector<T>;
template <typename T>
vector<T> make_vec(size_t n, T a) {
  return vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
}
template <typename T>
ostream &operator<<(ostream &os, const vector<T> &v) {
  os << '[';
  for (auto &&e : v) os << e << ' ';
  return os << ']';
}
template <typename T>
ostream &operator<<(ostream &os, const set<T> &v) {
  os << '[';
  for (auto &&e : v) os << e << ' ';
  return os << ']';
}

#ifdef ENABLE_DEBUG
template <typename T>
void debug(T value) {
  cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                              \
  do {                                          \
    cerr << " \033[33m (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ":\033[0m ";        \
    debug(__VA_ARGS__);                         \
    cerr << endl;                               \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

struct RollingHash {
  using u64 = unsigned long long;
  using u128 = __uint128_t;
  static const u64 mod = (1ULL << 61) - 1;
  vector<u64> hashed, power;

  RollingHash(string_view s) : hashed(s.size() + 1), power(s.size() + 1) {
    int n = s.size();
    power[0] = 1;
    for (int i = 0; i < n; i++) {
      power[i + 1] = mul(power[i], base());
      hashed[i + 1] = add(mul(hashed[i], base()), s[i]);
    }
  }

  // Returns the hash value for the [l,r) interval.
  u64 get(int l, int r) {
    return add(hashed[r], mod - mul(hashed[l], power[r - l]));
  }

 private:
  static u64 base() {
    static u64 val = []() -> u64 {
      random_device seed_gen;
      mt19937_64 engine(seed_gen());
      uniform_int_distribution<u64> rand(1, mod - 1);
      return rand(engine);
    }();
    return val;
  }

  static u64 add(u64 a, u64 b) {
    a += b;
    if (a >= mod) a -= mod;
    return a;
  }

  static u64 mul(u64 a, u64 b) {
    u128 t = (u128)a * b;
    u64 na = t >> 61;
    u64 nb = t & mod;
    na += nb;
    if (na >= mod) na -= mod;
    return na;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  string s;
  cin >> s;
  RollingHash rh(s);

  auto check = [&](int k) -> bool {
    gp_hash_table<u64, int> end_index;
    for (int i = 0; i + k <= n; ++i) {
      auto hashv = rh.get(i, i + k);
      auto it = end_index.find(hashv);
      if (it == end_index.end()) {
        end_index[hashv] = i + k;
      } else if (it->second <= i) {
        return true;
      }
    }
    return false;
  };

  int tv = 0, fv = n + 1;
  while (fv - tv > 1) {
    int mid = (tv + fv) / 2;
    if (check(mid)) {
      tv = mid;
    } else {
      fv = mid;
    }
  }
  cout << tv << endl;
}

#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>
template <class Key, class T>
using gp_hash_table = __gnu_pbds::gp_hash_table<Key, T>;

using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

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
  const u64 base;
  vector<u64> hashed, power;

  RollingHash(const string& s, u64 base) : base(base) {
    int n = s.size();
    hashed.assign(n + 1, 0);
    power.assign(n + 1, 0);
    power[0] = 1;
    for (int i = 0; i < n; i++) {
      power[i + 1] = mul(power[i], base);
      hashed[i + 1] = add(mul(hashed[i], base), s[i]);
    }
  }

  static u64 gen_base() {
    random_device seed_gen;
    mt19937_64 engine(seed_gen());
    uniform_int_distribution<u64> rand(1, mod - 1);
    return rand(engine);
  }

  // Returns the hash value for the [l,r) interval.
  u64 get(int l, int r) {
    return add(hashed[r], mod - mul(hashed[l], power[r - l]));
  }

 private:
  u64 add(u64 a, u64 b) {
    if ((a += b) >= mod) {
      a -= mod;
    }
    return a;
  }

  u64 mul(u64 a, u64 b) {
    u128 t = (u128)a * b;
    u64 na = t >> 61;
    u64 nb = t & mod;
    if ((na += nb) >= mod) {
      na -= mod;
    }
    return na;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  V<pair<size_t, string>> S(N);
  REP(i, N) {
    string s;
    cin >> s;
    S[i] = {s.size(), s};
  }
  sort(S.begin(), S.end());
  gp_hash_table<u64, V<int>> mp;
  const u64 B = RollingHash::gen_base();
  i64 ans = 0;
  REP(i, N) {
    int len = S[i].first;
    const string& ss = S[i].second;
    V<int> char_cnt(26);
    REP(j, len) {
      int ch = ss[j] - 'a';
      char_cnt[ch]++;
    }
    RollingHash rh(ss, B);
    for (int j = len - 1; j >= 1; --j) {
      REP(c, 26) {
        if (char_cnt[c] > 0) {
          auto it = mp.find(rh.get(j + 1, len));
          if (it == mp.end()) continue;
          ans += it->second[c];
        }
      }
      --char_cnt[ss[j] - 'a'];
    }
    int prefix = ss[0] - 'a';
    u64 shash = rh.get(1, len);
    auto ait = mp.find(shash);
    if (ait == mp.end()) {
      mp[shash].assign(26, 0);
    }
    mp[shash][prefix]++;
  }
  cout << ans << endl;
}

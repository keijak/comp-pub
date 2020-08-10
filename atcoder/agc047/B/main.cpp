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

const u64 B = 100'000'007ULL;

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
  i64 ans = 0;
  REP(i, N) {
    int len = S[i].first;
    const string& ss = S[i].second;
    V<int> char_cnt(26);
    REP(j, len) {
      int ch = ss[j] - 'a';
      char_cnt[ch]++;
    }
    u64 shash = 0;
    u64 base = B;
    if (len > 1) {
      for (int j = len - 1; j >= 1; --j) {
        REP(c, 26) {
          if (char_cnt[c] > 0) {
            auto it = mp.find(shash);
            if (it == mp.end()) continue;
            if (c < it->second.size()) ans += it->second[c];
          }
        }
        shash += base * ss[j];
        base *= B;
        --char_cnt[ss[j] - 'a'];
      }
    }
    int prefix = ss[0] - 'a';
    auto& v = mp[shash];
    if (v.empty()) v.resize(26);
    v[prefix]++;
  }
  cout << ans << endl;
}

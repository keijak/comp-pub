#include <bits/stdc++.h>
using i64 = long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}

using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;
  vector<i64> w(n);
  cin >> w;
  map<i64, i64> vlmax;
  i64 lmax = 0;
  i64 vmin = 1e9;
  REP(i, m) {
    i64 l, v;
    cin >> l >> v;
    chmax(lmax, l);
    chmin(vmin, v);
    {
      auto it = vlmax.upper_bound(v);
      if (it != vlmax.begin() and prev(it)->second >= l) {
        continue;
      }
    }
    auto it = vlmax.lower_bound(v);
    while (it != vlmax.end() and it->second <= l) {
      it = vlmax.erase(it);
    }
    chmax(vlmax[v], l);
  }
  if (vmin < *max_element(ALL(w))) {
    cout << -1 << endl;
    exit(0);
  }

  auto get_len = [&](i64 w) -> i64 {
    auto it = vlmax.lower_bound(w);
    if (it == vlmax.begin()) return 0LL;
    --it;
    return it->second;
  };

  vector<int> camels(n);
  REP(i, n) { camels[i] = i; }

  i64 ans = (n - 1) * lmax;
  do {
    auto dp = vector(n, vector(n, optional<pair<i64, i64>>()));
    auto mindist = [&](auto self, int l, int r) -> pair<i64, i64> {
      assert(l < r);
      if (r - l == 1) {
        i64 weight = w[camels[l]] + w[camels[r]];
        i64 len = get_len(weight);
        return {weight, len};
      }
      if (dp[l][r].has_value()) return dp[l][r].value();
      i64 weight = -1, len = 0;  // weight and length of [l,r]
      for (int k = l + 1; k < r; ++k) {
        auto [w1, len1] = self(self, l, k);
        auto [w2, len2] = self(self, k, r);
        i64 ww = w1 + w2 - w[camels[k]];
        if (weight == -1) {
          weight = ww;
          len = get_len(weight);
        } else {
          assert(weight == ww);
        }
        chmax(len, len1 + len2);
      }
      auto res = make_pair(weight, len);
      dp[l][r] = res;
      return res;
    };
    auto [weight, len] = mindist(mindist, 0, n - 1);
    chmin(ans, len);
  } while (next_permutation(ALL(camels)));

  cout << ans << endl;
}

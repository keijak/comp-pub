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
  const int L = 100'000'000;

  int n, m;
  cin >> n >> m;
  vector<i64> w(n);
  cin >> w;
  vector<i64> vlmax(L + 1);
  i64 lmax = 0;
  i64 vmin = 1e9;
  REP(i, m) {
    i64 l, v;
    cin >> l >> v;
    chmax(vlmax[v], l);
    chmax(lmax, l);
    chmin(vmin, v);
  }
  if (vmin < *max_element(ALL(w))) {
    cout << -1 << endl;
    exit(0);
  }
  REP(i, L) { vlmax[i + 1] = max(vlmax[i], vlmax[i + 1]); }

  auto get_len = [&](i64 w) {
    if (w > L) return lmax;
    return vlmax[w - 1];
  };

  vector<int> camels(n);
  REP(i, n) { camels[i] = i; }

  i64 ans = (n - 1) * lmax;
  do {
    vector<i64> pos(n);
    pos[0] = 0;
    for (int i = 1; i < n; ++i) {
      i64 w_sub = w[camels[i]];
      for (int j = i - 1; j >= 0; --j) {
        w_sub += w[camels[j]];
        i64 len = get_len(w_sub);
        chmax(pos[i], pos[j] + len);
      }
    }
    chmin(ans, pos[n - 1]);
  } while (next_permutation(ALL(camels)));

  cout << ans << endl;
}
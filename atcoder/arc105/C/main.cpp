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
  vector<int> w(n);
  cin >> w;
  map<int, int> vlmax;
  int lmax = 0;
  int vmin = 1e9;
  REP(i, m) {
    int l, v;
    cin >> l >> v;
    chmax(lmax, l);
    chmin(vmin, v);

    auto it = vlmax.lower_bound(v);
    if (it != vlmax.begin() and prev(it)->second >= l) {
      continue;
    }
    while (it != vlmax.end() and it->second <= l) {
      it = vlmax.erase(it);
    }
    chmax(vlmax[v], l);
  }
  if (vmin < *max_element(ALL(w))) {
    cout << -1 << endl;
    exit(0);
  }

  auto get_len = [&](int w) -> int {
    auto it = vlmax.lower_bound(w);
    if (it == vlmax.begin()) return 0LL;
    --it;
    return it->second;
  };

  vector<int> camels(n);
  REP(i, n) { camels[i] = i; }

  int ans = (n - 1) * lmax;
  do {
    vector<int> pos(n);
    for (int i = 1; i < n; ++i) {
      int w_sub = w[camels[i]];
      for (int j = i - 1; j >= 0; --j) {
        w_sub += w[camels[j]];
        int len = get_len(w_sub);
        chmax(pos[i], pos[j] + len);
      }
    }
    chmin(ans, pos[n - 1]);
  } while (next_permutation(ALL(camels)));

  cout << ans << endl;
}

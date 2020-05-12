#include <iostream>
#include <vector>
using namespace std;
using i64 = long long;

int main() {
  int x, y, N;
  cin >> x >> y >> N;
  vector<pair<int, int>> tops(N);
  for (auto&& p : tops) {
    cin >> p.first >> p.second;
  }
  int K = min(N + 1, x + 1);
  int T = x + y + 1;
  vector<i64> tp(K * T, 0);
  for (i64 z = tops[0].first; z < T; ++z) {
    tp[T + z] = tops[0].second;
  }
  for (int i = 1; i < N; ++i) {
    vector<i64> ti = tp;
    int nticket = tops[i].first;
    int happiness = tops[i].second;
    for (int k = 1; k < K; ++k) {
      for (int z = nticket; z < T; ++z) {
        i64 h = tp.at((k - 1) * T + z - nticket) + happiness;
        if (ti[k * T + z] < h) {
          ti[k * T + z] = h;
        }
      }
    }
    tp.swap(ti);
  }
  i64 ans = 0;
  for (int k = 1; k < K; ++k) {
    i64 h = tp.at(k * T + x + y);
    if (ans < h) {
      ans = h;
    }
  }
  cout << ans << endl;
}

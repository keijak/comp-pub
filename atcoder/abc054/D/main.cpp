#include <iostream>
#include <tuple>
#include <vector>
using namespace std;
#define REP(i, n) for (int i = 0; i < static_cast<int>(n); ++i)
const int INF = 1 << 30;

int main() {
  int N, ma, mb;
  cin >> N >> ma >> mb;
  int mincost = INF, xmax = 0;
  vector<tuple<int, int>> x_minus, x_plus;
  REP(i, N) {
    int a, b, c;
    cin >> a >> b >> c;
    int x = a * mb - ma * b;
    if (x == 0) {
      mincost = min(mincost, c);
    } else if (x < 0) {
      x_minus.emplace_back(c, -x);
    } else {
      x_plus.emplace_back(c, x);
    }
    xmax = max(xmax, abs(x));
  }
  vector<int> dp_plus(xmax + 1, INF), dp_minus(xmax + 1, INF);
  dp_plus[0] = dp_minus[0] = 0;
  for (auto& cx : x_plus) {
    int ci = get<0>(cx), xi = get<1>(cx);
    for (int w = xmax - xi; w >= 0; --w) {
      dp_plus[w + xi] = min(dp_plus[w + xi], dp_plus[w] + ci);
    }
  }
  for (auto& cx : x_minus) {
    int ci = get<0>(cx), xi = get<1>(cx);
    for (int w = xmax - xi; w >= 0; --w) {
      dp_minus[w + xi] = min(dp_minus[w + xi], dp_minus[w] + ci);
    }
  }
  for (int w = 1; w <= xmax; ++w) {
    int wp = dp_plus[w], wm = dp_minus[w];
    if (wp != INF && wm != INF && mincost > wp + wm) {
      mincost = wp + wm;
    }
  }
  cout << (mincost == INF ? -1 : mincost) << '\n';
}
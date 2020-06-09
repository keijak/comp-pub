//#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define DEBUG(...)
#else
template <typename T>
void _debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void _debug(T value, Ts... args) {
  std::cerr << value << ", ";
  _debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    _debug(__VA_ARGS__);               \
    cerr << endl;                      \
  } while (0)
#endif

const int INF = 1e7;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  vector<pair<int, int>> boxes(N);
  REP(i, N) {
    cin >> boxes[i].first >> boxes[i].second;
    boxes[i].second *= -1;
  }
  sort(boxes.begin(), boxes.end());

  // LIS
  vector<int> lis(N, INF);
  REP(i, N) {
    int h = -boxes[i].second;
    int k = lower_bound(lis.begin(), lis.end(), h) - lis.begin();
    lis[k] = h;
    DEBUG(k, h);
  }
  int i = N - 1;
  for (; i >= 0; --i) {
    if (lis[i] < INF) break;
  }
  cout << (i + 1) << endl;
}

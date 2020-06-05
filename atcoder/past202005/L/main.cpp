#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define DEBUG(...)
#else
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    debug(__VA_ARGS__);                \
    cerr << endl;                      \
  } while (0)
#endif

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  vector<vector<i64>> times(N);
  REP(i, N) {
    int K;
    cin >> K;
    times[i].resize(K);
    for (auto &x : times[i]) cin >> x;
  }
  int M;
  cin >> M;

  vector<int> head1(N), head2(N, 1);
  priority_queue<tuple<i64, int, int>> pq1, pq2;
  REP(i, N) {
    pq1.emplace(times[i][0], 0, i);
    pq2.emplace(times[i][0], 0, i);
    if (times[i].size() >= 2) {
      pq2.emplace(times[i][1], 1, i);
    }
  }
  REP(customer, M) {
    int ncheck;
    cin >> ncheck;
    if (ncheck == 1) {
      int i, j;
      i64 t;
      do {
        auto best = pq1.top();
        i = get<2>(best);
        j = get<1>(best);
        t = get<0>(best);
        pq1.pop();
      } while (times[i][j] < 0);
      cout << t << '\n';
      times[i][j] = -1;
      head1[i] = head2[i];
      head2[i]++;
      if (size_t k = head1[i]; k < times[i].size()) {
        pq1.emplace(times[i][k], k, i);
      }
      if (size_t k = head2[i]; k < times[i].size()) {
        pq2.emplace(times[i][k], k, i);
      }
    } else {
      assert(ncheck == 2);
      int i, j;
      i64 t;
      do {
        auto best = pq2.top();
        i = get<2>(best);
        j = get<1>(best);
        t = get<0>(best);
        pq2.pop();
      } while (times[i][j] < 0);
      cout << t << '\n';
      times[i][j] = -1;
      if (j == head1[i]) {
        head1[i] = head2[i];
        if (size_t k = head1[i]; k < times[i].size()) {
          pq1.emplace(times[i][k], k, i);
        }
      } else {
        assert(j == head2[i]);
      }
      head2[i]++;
      if (size_t k = head2[i]; k < times[i].size()) {
        pq2.emplace(times[i][k], k, i);
      }
    }
  }
}

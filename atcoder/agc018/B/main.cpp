#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M;
  cin >> N >> M;
  vector<vector<int>> A(N, vector<int>(M, -1));
  vector<set<int>> sports(M);
  REP(i, N) REP(j, M) {
    cin >> A[i][j];
    A[i][j]--;
  }
  vector<int> index(N, 0);
  REP(i, N) {
    sports[A[i][0]].insert(i);
    index[i]++;
  }

  priority_queue<tuple<int, int>> pq;
  REP(i, M) {
    int n = sports[i].size();
    pq.emplace(n, i);
  }
  int ans = N;
  vector<bool> removed(M);
  while (pq.size()) {
    // REP(i, M) {
    //   cerr << "sport " << i << ": ";
    //   for (auto j : sports[i]) {
    //     cerr << j << " ";
    //   }
    //   cerr << endl;
    // }

    auto [n, sport] = pq.top();
    pq.pop();
    if (n != int(sports[sport].size())) {
      assert(removed[sport]);
      continue;
    }
    if (removed[sport]) continue;
    removed[sport] = true;
    ans = min(ans, n);
    DEBUG(n, sport, ans);

    map<int, vector<int>> updates;
    bool finish = false;
    for (auto person : sports[sport]) {
      int j;
      do {
        j = index[person]++;
      } while (j < M && removed[A[person][j]]);
      if (j >= M) {
        finish = true;
        break;
      }
      updates[A[person][j]].push_back(person);
      DEBUG(person, sport, A[person][j]);
    }
    if (finish) break;
    sports[sport].clear();
    for (auto it = updates.begin(); it != updates.end(); ++it) {
      int sp = it->first;
      sports[sp].insert(it->second.begin(), it->second.end());
      pq.emplace(int(sports[sp].size()), sp);
      DEBUG(sp, sports[sp].size());
    }
  }
  cout << ans << endl;
}

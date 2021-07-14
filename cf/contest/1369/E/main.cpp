#define MY_DEBUG 1
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#if MY_DEBUG
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
#else
#define debug(...)
#define DEBUG(...)
#endif

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, M;
  cin >> N >> M;
  vector<int> W(N);
  for (auto& x : W) cin >> x;
  vector<pair<int, int>> pref(M);
  vector<set<int>> want(N);
  REP(i, M) {
    int x, y;
    cin >> x >> y;
    x--;
    y--;
    assert(x != y);
    pref[i] = {x, y};
    want[x].insert(i);
    want[y].insert(i);
  }

  deque<int> invitations;
  priority_queue<tuple<int, int>> food_queue;
  vector<bool> food_done(N);
  REP(i, N) { food_queue.emplace(W[i] - want[i].size(), i); }
  while (food_queue.size()) {
    auto [wdiff, f] = food_queue.top();
    food_queue.pop();
    if (food_done[f]) continue;
    food_done[f] = true;
    if (wdiff < 0) {
      cout << "DEAD\n";
      return 0;
    }
    // vector<int> invited(want[f].begin(), want[f].end());
    set<int> updated;
    for (int k : want[f]) {
      invitations.push_front(k);
      auto [f1, f2] = pref[k];
      want[f1].erase(k);
      want[f2].erase(k);
      if (f2 == f)
        updated.insert(f1);
      else
        updated.insert(f2);
    }
    if ((int)invitations.size() == M) break;
    for (int i : updated) {
      if (!food_done[i]) {
        food_queue.emplace(W[i] - want[i].size(), i);
      }
    }
  }
  cout << "ALIVE\n";
  REP(i, M) { cout << (invitations[i] + 1) << ' '; }
  cout << endl;
}

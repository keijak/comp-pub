#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifdef MY_DEBUG
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
  string sfirst, slast;
  cin >> sfirst >> slast;
  size_t L = sfirst.size();
  assert(slast.size() == L);
  int N;
  cin >> N;
  V<string> s(N);
  REP(i, N) {
    cin >> s[i];
    assert(s[i].size() == L);
  }
  s.push_back(sfirst);
  s.push_back(slast);
  sort(s.begin(), s.end());
  s.erase(unique(s.begin(), s.end()), s.end());
  N = s.size();
  int ifirst = -1, ilast = -1;
  REP(i, N) {
    if (s[i] == sfirst) ifirst = i;
    if (s[i] == slast) ilast = i;
  }
  assert(ifirst >= 0);
  assert(ilast >= 0);
  // REP(i, N) { DEBUG(i, s[i]); }
  if (sfirst == slast) {
    cout << 0 << endl;
    cout << sfirst << endl;
    cout << slast << endl;
    return 0;
  }

  V<V<int>> G(N);
  auto is_neighbor = [&](int i, int j) -> bool {
    int cnt = 0;
    REP(k, L) {
      if (s[i][k] != s[j][k]) ++cnt;
    }
    return (cnt == 1);
  };
  REP(i, N) {
    REP(j, i) {
      // DEBUG(s[i], s[j]);
      if (is_neighbor(j, i)) {
        DEBUG(i, j, s[i], s[j]);
        G[i].push_back(j);
        G[j].push_back(i);
      }
    }
  }

  V<int> pre(N, -1);
  V<bool> done(N);
  {
    deque<int> q;
    q.push_back(ifirst);
    done[ifirst] = true;
    while (q.size()) {
      int v = q.front();
      q.pop_front();
      if (v == ilast) break;
      for (int u : G[v]) {
        if (!done[u]) {
          done[u] = true;
          pre[u] = v;
          q.push_back(u);
        }
      }
    }
  }
  if (!done[ilast]) {
    cout << -1 << endl;
    return 0;
  }
  deque<int> ans;
  int v = ilast;
  while (v != ifirst) {
    ans.push_front(v);
    v = pre[v];
  }
  ans.push_front(ifirst);
  cout << (ans.size() - 2) << '\n';
  for (auto v : ans) {
    cout << s[v] << '\n';
  }
}

#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
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

#define chmin(x, y) x = min(x, y)

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

struct Edge {
  int to;
  i64 cost;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  V<string> S(N);
  V<i64> C(N);
  REP(i, N) { cin >> S[i] >> C[i]; }

  // num nodes.
  int n = 0;

  // (prefix,rev) -> ID
  map<pair<string, bool>, int> node_ids;
  node_ids[{"", false}] = n++;
  node_ids[{"", true}] = n++;
  REP(i, N) {
    string rev(S[i].rbegin(), S[i].rend());
    REP(j, S[i].size() + 1) {
      auto p1 = make_pair(rev.substr(j), false);
      auto it1 = node_ids.find(p1);
      if (it1 == node_ids.end()) {
        node_ids[p1] = n++;
      }
      auto p2 = make_pair(rev.substr(0, j), true);
      auto it2 = node_ids.find(p2);
      if (it2 == node_ids.end()) {
        node_ids[p2] = n++;
      }
    }
  }

  // ID -> (prefix,rev)
  V<const pair<string, bool>*> prefixes(n);
  V<bool> is_palindrome(n);
  for (auto it = node_ids.begin(); it != node_ids.end(); ++it) {
    int node_id = it->second;
    prefixes[node_id] = &(it->first);

    const string& s = it->first.first;
    string rev(s.rbegin(), s.rend());
    is_palindrome[node_id] = (s == rev);
  }

  //   for (auto& [p, k] : node_ids) {
  //     auto& [prefix, rev] = p;
  //     cout << rev << " [" << prefix << "] = " << k << '\n';
  //   }

  V<map<int, i64>> g(n);
  REP(i, n) {
    if (is_palindrome[i]) {
      continue;  // no out edge from a palindrome.
    }
    string prefix1 = prefixes[i]->first;
    // true: filling suffix, false: filling prefix
    bool reversed1 = prefixes[i]->second;
    if (reversed1) {
      reverse(prefix1.begin(), prefix1.end());
    }
    int l1 = prefix1.size();
    // DEBUG(prefix1, reversed1);
    REP(j, N) {
      string prefix2 = S[j];
      int l2 = prefix2.size();
      if (reversed1) reverse(prefix2.begin(), prefix2.end());
      // DEBUG(prefix2);
      if (l1 <= l2 && prefix2.substr(0, l1) == prefix1) {
        string rem = prefix2.substr(l1);
        if (!reversed1) reverse(rem.begin(), rem.end());
        auto it = node_ids.find({rem, !reversed1});
        if (it != node_ids.end()) {
          if (g[i].count(it->second)) {
            chmin(g[i][it->second], C[j]);
          } else {
            g[i][it->second] = C[j];
          }
        }
      }
      if (l1 > l2 && prefix1.substr(0, l2) == prefix2) {
        string rem = prefix1.substr(l2);
        if (reversed1) reverse(rem.begin(), rem.end());
        auto it = node_ids.find({rem, reversed1});
        if (it != node_ids.end()) {
          if (g[i].count(it->second)) {
            chmin(g[i][it->second], C[j]);
          } else {
            g[i][it->second] = C[j];
          }
        }
      }
    }
  }

  /*
    REP(i, n) {
      const string& prefix = prefixes[i]->first;
      bool reversed = prefixes[i]->second;
      if (reversed) {
        for (const Edge& e : g[i]) {
          const string& prefix2 = prefixes[e.to]->first;
          bool reversed2 = prefixes[e.to]->second;
          DEBUG(i, prefix, prefix2, reversed2, e.cost);
        }
      }
    }
    */

  // Dijkstra
  const i64 INF = 1e18;
  V<i64> cost(n, INF);

  MinHeap<tuple<i64, int, int>> heap;
  REP(i, N) {
    string r = S[i];
    reverse(r.begin(), r.end());
    int node_id = node_ids[{r, true}];
    heap.emplace(C[i], node_id, -1);
  }
  while (!heap.empty()) {
    auto [c, v, p] = heap.top();
    // DEBUG(c, v, p);
    heap.pop();
    if (c >= cost[v]) continue;
    cost[v] = c;
    for (auto [to, co] : g[v]) {
      i64 newcost = c + co;
      if (newcost < cost[to]) {
        heap.emplace(newcost, to, v);
      }
    }
  }

  i64 ans = INF;
  REP(i, n) {
    if (!is_palindrome[i]) continue;
    ans = min(ans, cost[i]);
  }
  cout << (ans == INF ? -1LL : ans) << endl;
}

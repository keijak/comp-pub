#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
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

struct node_list {
  int task;
  i64 start;
  i64 end;
  node_list* next;
  int len;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  cin >> n;
  vector<tuple<i64, i64, int>> sche(n);
  REP(i, n) {
    i64 a, b;
    cin >> a >> b;
    sche[i] = {a, b, i + 1};
  }
  sort(sche.rbegin(), sche.rend());

  vector<int> table;
  table.push_back(1 << 25);
  i64 last = 1LL << 50;
  REP(i, n) {
    auto [a, b, task] = sche[i];
    if (b <= last) {
      table.push_back(a);
      last = a;
    }
  }

  sort(sche.begin(), sche.end(),
       [](const auto& x, const auto& y) { return get<1>(x) < get<1>(y); });
  vector<int> ans(table.size() - 1, n + 100);
  int plb = -1, j = 0;
  REP(i, table.size() - 1) {
    int k = int(table.size()) - 2 - i;
    int lb = -1;
    for (; j < n; ++j) {
      auto [a, b, task] = sche[j];
      if (b > table[k]) break;
      if (plb <= a && ans[i] > task) {
        ans[i] = task;
        lb = b;
      }
    }
    plb = lb;
  }

  cout << ans.size() << endl;
  REP(i, ans.size()) {
    if (i > 0) cout << " ";
    cout << ans[i];
  }
  cout << endl;
}

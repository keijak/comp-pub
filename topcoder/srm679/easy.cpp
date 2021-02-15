#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}

using namespace std;

struct FiringEmployees {
  int n;
  vector<int> manager;
  vector<int> salary;
  vector<int> productivity;
  vector<vector<int>> child;

  int fire(vector<int> manager_, vector<int> salary_,
           vector<int> productivity_) {
    n = manager_.size();
    child.resize(n + 1);
    manager = manager_;
    salary = salary_;
    productivity = productivity_;
    manager.insert(manager.begin(), 0);
    salary.insert(salary.begin(), 0);
    productivity.insert(productivity.begin(), 0);
    for (int i = 1; i <= n; ++i) {
      int p = manager[i];
      child[p].push_back(i);
    }
    return (int)dfs(0);
  }

  i64 dfs(int v) {
    i64 total = 0;
    for (int u : child[v]) {
      if (u == v) continue;
      i64 x = dfs(u);
      if (x > 0) {
        total += x;
      }
    }
    total += i64(productivity[v]) - salary[v];
    return max(total, 0LL);
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  FiringEmployees task;
  int ans =
      task.fire({0, 0, 1, 1, 2, 2}, {1, 1, 1, 2, 2, 2}, {2, 2, 2, 1, 1, 1});
  cout << ans << "\n";
}

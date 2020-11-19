#include <bits/stdc++.h>

using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

template <typename T>
using V = std::vector<T>;

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}

using namespace std;

struct Player {
  int age;
  int ord;
};

bool operator<(const Player &x, const Player &y) { return x.age < y.age; }

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, k;
  cin >> n >> k;
  V<int> X(n);  // age
  cin >> X;

  priority_queue<Player> pq;
  REP(i, k) { pq.push({X[i], i}); }

  V<int> ans(n - k + 1, -1);
  for (int i = k; i <= n; ++i) {
    auto p = pq.top();
    ans[i - k] = p.ord;
    if (i == n) break;
    pq.push({X[i], i});
    pq.pop();
  }
  for (auto x : ans) {
    cout << x + 1 << '\n';
  }
}

#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream *os = nullptr) {
  if (os == nullptr) os = &std::cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
}

using namespace std;

optional<vector<int>> solve() {
  int n, m;
  cin >> n >> m;
  vector s(n, vector(m, 0));
  REP(i, n) REP(j, m) { cin >> s[i][j]; }

  vector<int> ans = s[0];
  vector<bool> fixed(m, false);
  int fixed_count = 0;

  auto check = [&](auto &check, int i) -> bool {
    if (i == n) return true;

    vector<int> dpos;
    REP(j, m) {
      if (s[i][j] != ans[j]) dpos.push_back(j);
    }

    int dcnt = dpos.size();
    if (dcnt > 4) return false;
    if (dcnt <= 2) return check(check, i + 1);
    if (dcnt == 3) {
      if (fixed_count >= 2) return false;
      fixed_count += 1;
      for (int p : dpos) {
        if (fixed[p]) continue;
        fixed[p] = true;
        int bak = ans[p];
        ans[p] = s[i][p];
        if (check(check, 0)) {
          return true;
        }
        ans[p] = bak;
        fixed[p] = false;
      }
      fixed_count -= 1;
      return false;
    }
    if (dcnt == 4) {
      if (fixed_count >= 1) return false;
      fixed_count += 2;
      REP(j, 4) {
        int p = dpos[j];
        if (fixed[p]) continue;
        fixed[p] = true;
        int bakp = ans[p];
        ans[p] = s[i][p];

        REP(k, j + 1, 4) {
          int q = dpos[k];
          if (fixed[q]) continue;
          fixed[q] = true;
          int bakq = ans[q];
          ans[q] = s[i][q];
          if (check(check, 0)) return true;
          ans[q] = bakq;
          fixed[q] = false;
        }
        ans[p] = bakp;
        fixed[p] = false;
      }
      fixed_count -= 2;
      return false;
    }
    assert(false);
  };
  if (check(check, 0)) {
    return ans;
  }
  return nullopt;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  auto res = solve();
  if (not res.has_value()) {
    cout << "No\n";
  } else {
    cout << "Yes\n";
    print_seq(res.value());
  }
}

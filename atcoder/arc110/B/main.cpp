#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

using namespace std;
const i64 L = 10'000'000'000;

i64 solve() {
  int n;
  cin >> n;
  string t;
  cin >> t;

  if (t == "1") return 2LL * L;
  if (t == "0" or t == "10" or t == "110" or t == "11") return L;
  if (t == "01") return L - 1;
  if (ssize(t) <= 2) return 0;

  auto zi = t.find("0");
  if (zi == string::npos or zi >= 3) return 0;

  int num_zeros = 0;
  for (int i = zi; i < n; i += 3) {
    if (t[i] != '0') return 0;
    if (i + 1 < n and t[i + 1] != '1') return 0;
    if (i + 2 < n and t[i + 2] != '1') return 0;
    ++num_zeros;
  }
  i64 ans = L - (num_zeros - 1);
  if (t[n - 1] != '0') --ans;
  return ans;
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}

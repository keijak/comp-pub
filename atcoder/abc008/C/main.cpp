#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
using namespace std;
using Float = double;

Float solve() {
  int n;
  cin >> n;
  vector<int> C(n);
  cin >> C;

  vector<int> divisors(n);
  REP(i, n) {
    int count = 0;
    REP(j, n) {
      if (j == i) continue;
      if (C[i] % C[j] == 0) {
        ++count;
      }
    }
    divisors[i] = count;
  }

  Float ans = 0;
  REP(i, n) {
    Float a = divisors[i] + 1;
    Float k = divisors[i] / 2 + 1;
    Float p = k / a;
    ans += p;
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  cout << solve() << "\n";
}

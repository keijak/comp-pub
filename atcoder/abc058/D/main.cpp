#include <bits/stdc++.h>
using i64 = long long;

template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}

#include <atcoder/modint>
using Mint = atcoder::modint1000000007;
std::ostream &operator<<(std::ostream &os, const Mint &m) {
  return os << m.val();
}

using namespace std;

Mint f(const vector<int> &X) {
  int n = ssize(X);
  Mint res = 0;
  for (int i = n - 2; i >= 0; --i) {
    Mint d = Mint(X[i + 1]) - X[i];
    res += d * (i + 1) * (n - i - 1);
  }
  return res;
}

Mint solve() {
  int n, m;
  cin >> n >> m;
  vector<int> X(n), Y(m);
  cin >> X >> Y;
  return f(X) * f(Y);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}

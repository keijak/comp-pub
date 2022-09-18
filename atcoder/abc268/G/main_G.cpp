// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)

#include <atcoder/modint>
using Mint = atcoder::modint998244353;

[[noreturn]] void exit_() {
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

using namespace std;

auto solve() {
  int n;
  cin >> n;
  vector<string> S(n);
  REP(i, n) cin >> S[i];
  vector<int> idx(n);
  iota(ALL(idx), 0);
  vector<int> result(n, 2);
  REP(iter, 2) {
    sort(ALL(idx), [&](int i, int j) {
      return S[i] < S[j];
    });
    REP(i, n) {
      result[idx[i]] += i;
    }
    if (iter == 1) break;
    REP(i, n) {
      for (char &ch: S[i]) {
        ch = 'z' - ch;
      }
    }
  }
  const Mint kHalf = Mint(1) / 2;
  REP(i, n) {
    Mint r = result[i] * kHalf;
    cout << r.val() << '\n';
  }
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  solve();
  exit_();
}

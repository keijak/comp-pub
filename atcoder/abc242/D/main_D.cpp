#include <bits/stdc++.h>
using Uint = unsigned long long;
using namespace std;

int bit_width(Uint x) {
  return (x == 0) ? 0 : (64 - __builtin_clzll(x));
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  string S;
  cin >> S;
  int Q;
  cin >> Q;
  for (int i = 0; i < Q; ++i) {
    Uint t, k;
    cin >> t >> k;
    --k;
    Uint start = 0;
    if ((Uint) bit_width(k) > t) {
      start = k / (1ULL << t);
      k &= (1ULL << t) - 1;
    }
    Uint d = ((S[start] - 'A') + t + __builtin_popcountll(k)) % 3;
    cout << (char) ('A' + d) << '\n';
  }
}

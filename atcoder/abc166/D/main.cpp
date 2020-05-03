#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;

bool search(int64 p, int64 q) {
  for (int64 a = 1;; ++a) {
    int64 a2 = a * a;
    if (a2 > q) return false;
    int64 a3 = a2 * a;
    if (a3 > q) return false;
    int64 a4 = a3 * a;
    if (a4 > q) return false;
    int64 b = a - p;
    int64 s = a4 + a3 * b + a2 * b * b + a * b * b * b + b * b * b * b;
    if (s == q) {
      cout << a << " " << b << endl;
      return true;
    }
  }
}

void solve(long long X) {
  for (int64 p = 1; p * p <= X; ++p) {
    int64 r = 1;
    int64 x = X;
    while (x % p == 0) {
      x /= p;
      r *= p;
      if (search(min(x, r), max(x, r))) {
        exit(0);
      }
      if (p == 1) break;
    }
  }
}

int main() {
  long long X;
  cin >> X;
  solve(X);
}

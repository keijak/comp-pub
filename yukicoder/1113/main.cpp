#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

bool is_square(i64 n) {
  i64 s = sqrt(n);
  for (i64 i = s - 1; i <= s + 1; ++i) {
    if (i * i == n) return true;
  }
  return false;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  i64 A, B;
  cin >> A >> B;
  i64 g = std::gcd(A, B);
  if (is_square(g)) {
    cout << "Odd\n";
  } else {
    cout << "Even\n";
  }
}

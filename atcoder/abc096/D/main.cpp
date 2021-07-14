#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  const int M = 55555;
  const int L = 5 * M;
  vector<bool> is_prime(L, true);
  is_prime[0] = is_prime[1] = false;
  for (int i = 4; i < L; i += 2) is_prime[i] = false;
  for (int i = 3; i * i < L; i += 2) {
    if (!is_prime[i]) continue;
    for (int j = 2 * i; j < L; j += i) {
      is_prime[j] = false;
    }
  }
  vector<int> primes;
  REP(i, M + 1) {
    if (is_prime[i] && i % 5 == 1) {
      primes.push_back(i);
    }
  }

  int N;
  cin >> N;
  REP(i, N) {
    if (i > 0) {
      cout << ' ';
    }
    cout << primes[i];
  }
  cout << endl;
}

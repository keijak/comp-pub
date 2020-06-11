#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define DEBUG(...)
#else
template <typename T>
void _debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void _debug(T value, Ts... args) {
  std::cerr << value << ", ";
  _debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    _debug(__VA_ARGS__);               \
    cerr << endl;                      \
  } while (0)
#endif

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  i64 N;
  cin >> N;
  vector<i64> A(N);
  for (auto& x : A) cin >> x;

  vector<bool> is_p(10000);
  REP(i, 10000) { is_p[i] = i % 2 == 1; }
  is_p[0] = is_p[1] = false;
  is_p[2] = true;
  for (int i = 3; i * i < 10000; i += 2) {
    if (!is_p[i]) continue;
    for (int j = 2 * i; j < 10000; j += i) {
      is_p[j] = false;
    }
  }
  vector<i64> primes;
  REP(i, 10000) {
    if (is_p[i]) primes.push_back(i);
  }

  auto find_dpair = [&](i64 x,
                        const vector<pair<i64, int>>& fs) -> pair<i64, i64> {
    int n = (x % 2 == 0) ? 1 : fs.size();
    REP(i, n) {
      i64 d1 = fs[i].first;
      REP(k, fs[i].second) {
        for (int j = i + 1; j < int(fs.size()); ++j) {
          i64 d2 = fs[j].first;
          REP(p, fs[j].second) {
            if (std::gcd(d1 + d2, x) == 1) {
              return make_pair(d1, d2);
            }
            d2 *= fs[j].first;
          }
        }
        d1 *= fs[i].first;
      }
    }
    return make_pair(-1, -1);
  };

  vector<pair<i64, i64>> ans(N);
  REP(i, N) {
    i64 x = A[i];
    if (x < 10000LL && is_p[x]) {
      ans[i] = make_pair(-1LL, -1LL);
      continue;
    }
    vector<pair<i64, int>> fs;
    for (auto p : primes) {
      if (p * p > x) break;
      i64 k = 0;
      while (x % p == 0) {
        x /= p;
        k++;
      }
      if (k > 0) {
        fs.emplace_back(p, k);
      }
      if (x == 1) break;
    }
    if (x > 1) {
      fs.emplace_back(x, 1);
    }
    if (fs.size() < 2) {
      ans[i] = make_pair(-1LL, -1LL);
      continue;
    }
    ans[i] = find_dpair(A[i], fs);
  }

  REP(i, N) { cout << ans[i].first << ' '; }
  cout << '\n';
  REP(i, N) { cout << ans[i].second << ' '; }
  cout << '\n';
}

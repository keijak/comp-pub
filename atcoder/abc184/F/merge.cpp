#pragma GCC optimize("O3")
#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

using namespace std;

int solve() {
  int N, T;
  cin >> N >> T;
  vector<int> A;
  A.reserve(N);
  REP(i, N) {
    int x;
    cin >> x;
    if (x <= T) A.push_back(x);
  }
  N = ssize(A);
  sort(ALL(A));

  int M1 = N / 2;
  int M2 = N - M1;

  vector<int> st1, st2;
  {
    st1.reserve((1 << M1) + 5);
    st1.push_back(0);
    REP(i, M1) {
      int sz = ssize(st1), ai = A[i];
      REP(j, sz) if (int y = st1[j] + ai; y <= T) st1.push_back(y);
      std::inplace_merge(st1.begin(), st1.begin() + sz, st1.end());
    }
  }
  {
    st2.reserve((1 << M2) + 5);
    st2.push_back(0);
    REP(i, M2) {
      int sz = ssize(st2), ai = A[M1 + i];
      REP(j, sz) if (int y = st2[j] + ai; y <= T) st2.push_back(y);
      std::inplace_merge(st2.begin(), st2.begin() + sz, st2.end());
    }
    reverse(ALL(st2));
  }

  int ans = 0, j = 0, sz2 = ssize(st2);
  for (int i = 0, sz1 = ssize(st1); i < sz1; ++i) {
    int ub = T - st1[i];
    while (j < sz2 and st2[j] > ub) ++j;
    if (j == sz2) break;
    if (int x = st1[i] + st2[j]; ans < x) ans = x;
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cout << solve() << '\n';
}

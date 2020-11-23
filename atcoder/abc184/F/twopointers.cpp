#pragma GCC optimize("O3")
#include <bits/stdc++.h>
using namespace std;

int tmp[(1 << 20) + 5];

int solve() {
  int N, T;
  cin >> N >> T;
  vector<int> A;
  A.reserve(N);
  for (int i = 0; i < N; ++i) {
    int x;
    cin >> x;
    if (x <= T) A.push_back(x);
  }
  N = A.size();
  sort(A.begin(), A.end());

  int M1 = N / 2;
  int M2 = N - M1;

  vector<int> st1, st2;
  {
    st1.reserve((1 << M1) + 5);
    st1.push_back(0);
    for (int i = 0; i < M1; ++i) {
      int j = 0, t = 0, ai = A[i], sz1 = st1.size();
      for (int k = 0; k < sz1;) {
        if (j >= sz1 or st1[k] <= st1[j] + ai) {
          tmp[t++] = st1[k++];
        } else if (j < sz1) {
          int x = st1[j] + ai;
          if (x <= T) tmp[t++] = x;
          ++j;
        }
      }
      while (j < sz1) {
        int x = st1[j] + ai;
        if (x <= T) tmp[t++] = x;
        ++j;
      }
      st1.assign(tmp, tmp + t);
    }
  }
  {
    st2.reserve((1 << M2) + 5);
    st2.push_back(0);
    for (int i = 0; i < M2; ++i) {
      int j = 0, t = 0, ai = A[M1 + i], sz2 = st2.size();
      for (int k = 0; k < sz2;) {
        if (j >= sz2 or st2[k] <= st2[j] + ai) {
          tmp[t++] = st2[k++];
        } else if (j < sz2) {
          int x = st2[j] + ai;
          if (x <= T) tmp[t++] = x;
          ++j;
        }
      }
      while (j < sz2) {
        int x = st2[j] + ai;
        if (x <= T) tmp[t++] = x;
        ++j;
      }
      st2.assign(tmp, tmp + t);
    }
    reverse(st2.begin(), st2.end());
  }

  int ans = 0, j = 0, sz2 = st2.size();
  for (int i = 0, sz1 = st1.size(); i < sz1; ++i) {
    int ub = T - st1[i];
    while (j < sz2 and st2[j] > ub) ++j;
    if (j == sz2) break;
    if (int x = st1[i] + st2[j]; x > ans) ans = x;
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cout << solve() << '\n';
}

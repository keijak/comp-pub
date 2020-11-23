#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}
template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}

template <typename T>
using V = std::vector<T>;

using namespace std;

int tmp[(1 << 20) + 5];

int solve() {
  int N, T;
  cin >> N >> T;
  V<int> A;
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

  V<int> st, st2;
  {
    st.reserve((1 << M1) + 5);
    st.push_back(0);
    REP(i, M1) {
      int j = 0, t = 0, ai = A[i], sz1 = ssize(st);
      for (int k = 0; k < sz1;) {
        if (j >= sz1 or st[k] <= st[j] + ai) {
          tmp[t++] = st[k++];
        } else if (j < sz1) {
          int x = st[j] + ai;
          if (x <= T) tmp[t++] = x;
          ++j;
        }
      }
      while (j < sz1) {
        int x = st[j] + ai;
        if (x <= T) tmp[t++] = x;
        ++j;
      }
      st.assign(tmp, tmp + t);
    }
  }
  {
    st2.reserve((1 << M2) + 5);
    st2.push_back(0);
    REP(i, M2) {
      int j = 0, t = 0, ai = A[M1 + i], sz2 = ssize(st2);
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
    reverse(ALL(st2));
  }

  int ans = 0, j = 0, sz2 = ssize(st2);
  for (int i = 0, sz1 = ssize(st); i < sz1; ++i) {
    int ub = T - st[i];
    while (j < sz2 and st2[j] > ub) ++j;
    if (j == sz2) break;
    chmax(ans, st2[j] + st[i]);
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << solve() << '\n';
}

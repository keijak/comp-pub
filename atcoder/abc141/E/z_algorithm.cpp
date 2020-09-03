#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)
#define SZ(a) int((a).size())

template <class T>
bool chmax(T &a, T b) {
  if (a < b) return (a = move(b)), true;
  return false;
}
template <class T>
bool chmin(T &a, T b) {
  if (a > b) return (a = move(b)), true;
  return false;
}

template <typename T>
using V = std::vector<T>;
template <typename T>
vector<T> make_vec(size_t n, T a) {
  return vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
}

struct ZAlgorithm {
  const int N;
  vector<int> Z;
  const string_view S;

  explicit ZAlgorithm(string_view s) : N(s.size()), Z(s.size()), S(move(s)) {}

  void calc() {
    Z[0] = N;
    int i = 1, j = 0;
    while (i < N) {
      while (i + j < N and S[j] == S[i + j]) ++j;
      Z[i] = j;
      if (j == 0) {
        ++i;
        continue;
      }
      int k = 1;
      for (; k < j and k + Z[k] < j; ++k) {
        Z[i + k] = Z[k];
      }
      i += k;
      j -= k;
    }
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  string s;
  cin >> n >> s;
  string_view sv(s);

  int ans = 0;
  REP(i, n) {
    ZAlgorithm za(sv.substr(i));
    za.calc();
    REP(j, za.N) { chmax(ans, min(za.Z[j], j)); }
  }
  cout << ans << endl;
}

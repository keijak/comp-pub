// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;

template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
constexpr T kBig = std::numeric_limits<T>::max() / 2;

template<typename T>
inline T rd() {
  T ret = 0;
  int ch = getchar_unlocked();
  for (; isspace(ch); ch = getchar_unlocked());
  for (; isdigit(ch); ch = getchar_unlocked()) {
    ret = (ret * 10) + (ch - '0');
  }
  // ungetc(ch, stdin);  // assumption: ch is an ignorable whitespace
  return ret;
}

using namespace std;

array<vector<int>, 3> val;
array<vector<int>, 3> idx;

int solve() {
  const int n = rd<int>();
  REP(i, 3) {
    val[i].resize(n);
    idx[i].resize(n);
    iota(ALL(idx[i]), 0);
  }
  REP(i, n) {
    REP(j, 3) val[j][i] = rd<int>();
  }
  REP(k, 3) {
    sort(ALL(idx[k]), [&](int i, int j) {
      return val[k][i] < val[k][j];
    });
  }
  while (not idx[0].empty() and not idx[1].empty() and not idx[2].empty()) {
    const int xi = idx[0].back();
    const int yi = idx[1].back();
    const int zi = idx[2].back();
    if (val[1][xi] >= val[1][yi] or val[2][xi] >= val[2][zi]) {
      idx[0].pop_back();
      continue;
    }
    if (val[0][yi] >= val[0][xi] or val[2][yi] >= val[2][zi]) {
      idx[1].pop_back();
      continue;
    }
    if (val[0][zi] >= val[0][xi] or val[1][zi] >= val[1][yi]) {
      idx[2].pop_back();
      continue;
    }
    return val[0][xi] + val[1][yi] + val[2][zi];
  }
  return -1;
}

int main() {
  printf("%d\n", solve());
  fflush(stdout);
  _Exit(0);
}

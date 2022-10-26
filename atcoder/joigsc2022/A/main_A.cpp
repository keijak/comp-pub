// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)

template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
constexpr T kBig = std::numeric_limits<T>::max() / 2;

template<typename T>
inline void read_unsigned(T &ret) {
  // ret = 0;  // assumption: ret is already zero-initialized.
  int ch = getchar_unlocked();
  for (; isspace(ch); ch = getchar_unlocked());
  for (; isdigit(ch); ch = getchar_unlocked()) {
    ret = (ret * 10) + (ch - '0');
  }
  // ungetc(ch, stdin);  // assumption: ch is an ignorable whitespace
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

using namespace std;

constexpr int N = 200'005;
int A[N];
int B[N];
int idx[N];

bool cmp(int i, int j) {
  return B[i] < B[j];
}

int main() {
  int n = 0;
  read_unsigned(n);
  REP(i, n) {
    read_unsigned(A[i]);
    read_unsigned(B[i]);
  }
  std::iota(idx, idx + n, 0);
  sort(idx, idx + n, cmp);
  array<int, 3> dp = {kBig<int>, kBig<int>, kBig<int>};
  REP(i, n) {
    const int j = idx[i];
    const int a = A[j];
    const int b = B[j];
    const auto tmp = dp;
    chmin(dp[0], a);
    chmin(dp[1], tmp[0] + a + b);
    chmin(dp[2], tmp[1] + a + b);
  }
  printf("%d\n", dp[2]);
  fflush(stdout);
  std::_Exit(0);
}

#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("inline")
#pragma GCC target("avx2")

#include <array>
#include <cstdio>
#include <cstring>
#include <type_traits>

namespace fastio {
#ifdef MY_DEBUG
#define fread_unlocked fread
#define fwrite_unlocked fwrite
#endif

static constexpr int SZ = 1 << 20;
char ibuf[SZ], obuf[SZ];
int pil = 0, pir = 0, por = 0;

inline void load() {
  memcpy(ibuf, ibuf + pil, pir - pil);
  pir = pir - pil + fread_unlocked(ibuf + pir - pil, 1, SZ - pir + pil, stdin);
  pil = 0;
}

inline void rd(char &c) {
  if (pil + 32 > pir) load();
  c = ibuf[pil++];
}
template<typename T>
inline void rd(T &x) {
  if (pil + 32 > pir) load();
  char c;
  do c = ibuf[pil++];
  while (c < '-');
  [[maybe_unused]] bool minus = false;
  if constexpr (std::is_signed<T>::value == true) {
    if (c == '-') minus = true, c = ibuf[pil++];
  }
  x = 0;
  while (c >= '0') {
    x = x * 10 + (c & 15);
    c = ibuf[pil++];
  }
  if constexpr (std::is_signed<T>::value == true) {
    if (minus) x = -x;
  }
}
inline void rd() {}
template<typename Head, typename... Tail>
inline void rd(Head &head, Tail &... tail) {
  rd(head);
  rd(tail...);
}

struct Pre {
  char num[40000];
  constexpr Pre() : num() {
    for (int i = 0; i < 10000; i++) {
      int n = i;
      for (int j = 3; j >= 0; j--) {
        num[i * 4 + j] = n % 10 + '0';
        n /= 10;
      }
    }
  }
} constexpr pre;

}  // namespace fastio
using fastio::rd;
using Int = long long;
using namespace std;

int main() {
  int n, m, a, v;
  rd(n, m, a, v);
  array<pair<Int, int>, 2> dp = {pair{0LL, -1}, pair{v, a}};
  if (dp[0].first < dp[1].first) swap(dp[0], dp[1]); // sort

  for (int i = 1; i < n; ++i) {
    rd(a, v);
    if (dp[0].second == a) {
      Int z = dp[1].first + v;
      if (dp[0].first < z) dp[0] = {z, a};
    } else {
      Int z = dp[0].first + v;
      if (dp[1].first < z) dp[1] = {z, a};
    }
    if (dp[0].first < dp[1].first) swap(dp[0], dp[1]); // sort
  }
  printf("%lld\n", dp[0].first);
}

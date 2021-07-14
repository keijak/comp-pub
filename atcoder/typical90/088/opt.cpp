#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("inline")
#pragma GCC target("avx2")

#include <array>
#include <bitset>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <optional>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)

using namespace std;
using BS = std::bitset<88>;

namespace fastio {
#ifdef ENABLE_DEBUG
#define fread_unlocked fread
#define fwrite_unlocked fwrite
#endif

static constexpr int SZ = 1 << 17;
char ibuf[SZ], obuf[SZ];
int pil = 0, pir = 0, por = 0;

inline void load() {
  memcpy(ibuf, ibuf + pil, pir - pil);
  pir = pir - pil + fread_unlocked(ibuf + pir - pil, 1, SZ - pir + pil, stdin);
  pil = 0;
}

inline void rd(char& c) {
  if (pil + 32 > pir) load();
  c = ibuf[pil++];
}
template <typename T>
inline void rd(T& x) {
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
template <typename Head, typename... Tail>
inline void rd(Head& head, Tail&... tail) {
  rd(head);
  rd(tail...);
}

inline void skip_space() {
  if (pil + 32 > pir) load();
  while (ibuf[pil] <= ' ') pil++;
}

inline void flush() {
  fwrite_unlocked(obuf, 1, por, stdout);
  por = 0;
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

struct Post {
  Post() { std::atexit(flush); }
} post;

inline void wt(char c) {
  if (por > SZ - 32) flush();
  obuf[por++] = c;
}
inline void wt(bool b) {
  if (por > SZ - 32) flush();
  obuf[por++] = b ? '1' : '0';
}
template <typename T>
inline void wt(T x) {
  if (por > SZ - 32) flush();
  if (!x) {
    obuf[por++] = '0';
    return;
  }
  if constexpr (std::is_signed<T>::value == true) {
    if (x < 0) obuf[por++] = '-', x = -x;
  }
  int i = 12;
  char buf[16];
  while (x >= 10000) {
    memcpy(buf + i, pre.num + (x % 10000) * 4, 4);
    x /= 10000;
    i -= 4;
  }
  if (x < 100) {
    if (x < 10) {
      obuf[por] = '0' + x;
      ++por;
    } else {
      uint32_t q = (uint32_t(x) * 205) >> 11;
      uint32_t r = uint32_t(x) - q * 10;
      obuf[por] = '0' + q;
      obuf[por + 1] = '0' + r;
      por += 2;
    }
  } else {
    if (x < 1000) {
      memcpy(obuf + por, pre.num + (x << 2) + 1, 3);
      por += 3;
    } else {
      memcpy(obuf + por, pre.num + (x << 2), 4);
      por += 4;
    }
  }
  memcpy(obuf + por, buf + i + 4, 12 - i);
  por += 12 - i;
}

inline void wt() {}
template <typename Head, typename... Tail>
inline void wt(Head&& head, Tail&&... tail) {
  wt(head);
  wt(std::forward<Tail>(tail)...);
}
template <typename... Args>
inline void wtn(Args&&... x) {
  wt(std::forward<Args>(x)...);
  wt('\n');
}

}  // namespace fastio
using fastio::rd;
using fastio::wt;
using fastio::wtn;

unsigned a[90];
BS ban[90];
constexpr int W = 8889;
tuple<int, int, BS> que[W + 100];

int main() {
  int n, Q;
  rd(n, Q);
  REP(i, n) rd(a[i]);
  REP(i, Q) {
    int x, y;
    rd(x, y);
    ban[y - 1].set(x - 1, true);
  }
  auto calc_pair = [&]() -> array<BS, 2> {
    auto dp = vector(W, optional<BS>());
    dp[0] = BS();
    int ql = 0, qr = 0;
    que[qr++] = {0, 0, BS()};
    while (ql != qr) {
      auto& [i, w, s1] = que[ql++];
      if (i + 1 < n) {
        que[qr++] = {i + 1, w, s1};
      }
      const unsigned x = a[i];
      if (w + x >= W) continue;
      if ((ban[i] & s1).any()) continue;
      s1.set(i, true);
      if (auto& s2 = dp[w + x]; s2) {
        return {s1, *s2};
      }
      if (i + 1 < n) {
        que[qr++] = {i + 1, w + x, s1};
      }
      dp[w + x] = std::move(s1);
    }
    assert(false);
  };

  for (auto& s : calc_pair()) {
    wtn(s.count());
    bool first = true;
    REP(i, 88) {
      if (s[i]) {
        if (not first)
          wt(' ');
        else
          first = false;
        wt(i + 1);
      }
    }
    wt('\n');
  }
}

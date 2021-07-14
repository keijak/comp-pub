#include <algorithm>
#include <array>
#include <cctype>
#include <climits>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)

#if !HAVE_DECL_FREAD_UNLOCKED
#define fread_unlocked fread
#endif
#if !HAVE_DECL_FWRITE_UNLOCKED
#define fwrite_unlocked fwrite
#endif

namespace fastio {
static constexpr int SZ = 1 << 17;
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
inline void rd(Head &head, Tail &...tail) {
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
template<typename T>
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
template<typename Head, typename... Tail>
inline void wt(Head &&head, Tail &&...tail) {
  wt(head);
  wt(std::forward<Tail>(tail)...);
}
template<typename... Args>
inline void wtn(Args &&...x) {
  wt(std::forward<Args>(x)...);
  wt('\n');
}

}  // namespace fastio
using fastio::rd;
using fastio::wt;
using fastio::wtn;

using namespace std;

template<typename T>
constexpr int num_bits = CHAR_BIT * sizeof(T);

// Log base 2 of the most significant bit which is set.
inline int msb_log(unsigned x) {
  // assert(x != 0);
  return num_bits<unsigned> - __builtin_clz(x) - 1;
}

// Range Min/Max Query based on Fischer-Heun Structure.
// - Initialization: O(n)
// - Query: O(1)
template<class BetterOp, class Mask = unsigned>
struct RMQ {
  static_assert(std::is_integral_v<Mask>, "Mask must be integral");
  static_assert(std::is_unsigned_v<Mask>, "Mask must be unsigned");
  static_assert(std::is_invocable_r_v<bool, BetterOp, int, int>);
  static constexpr int block_size_ = num_bits<Mask>;

  int n_;                 // sequence size
  int block_count_;       // total number of blocks
  BetterOp better_than_;  // checks if lhs is strictly better than rhs.
  std::vector<Mask> indicator_;
  std::vector<std::vector<int>> sparse_table_;

  RMQ(int n, BetterOp better)
      : n_(n),
        block_count_((n_ + block_size_ - 1) / block_size_),
        better_than_(std::move(better)),
        indicator_(n_),
        sparse_table_(
            block_count_ == 0 ? 0 : msb_log(unsigned(block_count_)) + 1,
            std::vector<int>(block_count_)) {
    static constexpr int bufsize = block_size_ + 1;
    static int buf[bufsize];       // ring buffer [lp,rp)
    int lp = 1, rp = 1, rpm1 = 0;  // rpm1 = rp-1 (mod bufsize)

    // Build the indicator table.
    for (int r = 0; r < n_; ++r) {
      while (lp != rp and r - buf[lp] >= block_size_) {
        ++lp;
        if (lp == bufsize) lp = 0;
      }
      while (lp != rp and not better_than_(buf[rpm1], r)) {
        rp = rpm1--;
        if (rp == 0) rpm1 = bufsize - 1;
      }
      indicator_[r] = 1;
      if (lp != rp) {
        const int p = buf[rpm1];
        indicator_[r] |= (indicator_[p] << (r - p));
      }
      buf[rp] = r;
      rpm1 = rp++;
      if (rp == bufsize) rp = 0;
    }

    // Build the sparse table.
    for (int i = 0; i < block_count_; ++i) {
      sparse_table_[0][i] =
          best_index_small(std::min(block_size_ * (i + 1), n_) - 1);
    }
    for (int i = 0, height = int(sparse_table_.size()) - 1; i < height; ++i) {
      for (int j = 0; j < block_count_; ++j) {
        sparse_table_[i + 1][j] = better_index(
            sparse_table_[i][j],
            sparse_table_[i][std::min(j + (1 << i), block_count_ - 1)]);
      }
    }
  }

  inline int better_index(int i, int j) const {
    return better_than_(i, j) ? i : j;
  }

  // Returns the in of the best value in [r - width, r] (closed interval).
  inline int best_index_small(int r, int width = block_size_) const {
    // assert(r < n_);
    // assert(width > 0);
    // assert(width <= block_size_);
    Mask ind = indicator_[r];
    if (width < block_size_) {
      ind &= (Mask(1) << width) - 1;
    }
    return r - msb_log(ind);
  }

  // Returns the in of the best value in [l, r] (closed interval).
  inline int fold(int l, int r) const {
    const int width = r - l + 1;
    if (width <= block_size_) {
      return best_index_small(r, width);
    }
    const int al = best_index_small(std::min(l + block_size_, n_) - 1);
    const int ar = best_index_small(r);
    int ans = better_index(al, ar);

    const int bl = l / block_size_ + 1;
    const int br = r / block_size_ - 1;
    if (bl <= br) {
      const int k = msb_log(unsigned(br - bl + 1));
      const int bl2 = br - (1 << k) + 1;
      const int am = better_index(sparse_table_[k][bl], sparse_table_[k][bl2]);
      ans = better_index(ans, am);
    }
    return ans;
  }
};

struct EulerTour {
  using G = vector<vector<int>>;

  const int n;  // number of nodes
  const G &adj;
  vector<int> in;
  // Euler Tour on nodes.
  vector<int> depth_tour;

  explicit EulerTour(const G &g, int root = 0)
      : n(g.size()), adj(g), in(n, -1) {
    depth_tour.reserve(n * 2);
    vector<array<int, 4>> stack;
    stack.push_back({root, -1, -1, 0});
    while (not stack.empty()) {
      auto[v, p, d, back] = std::move(stack.back());
      stack.pop_back();
      if (back) {
        depth_tour.push_back(d);
        continue;
      }
      if (p != -1) {
        stack.push_back({v, p, d, 1});
      }
      in[v] = int(depth_tour.size());
      depth_tour.push_back(++d);
      for (auto u : adj[v]) {
        if (u == p) continue;
        stack.push_back({u, v, d, 0});
      }
    }
  }
};

int main() {
  int n;
  rd(n);
  vector<vector<int>> g(n);
  REP(i, n - 1) {
    int a, b;
    rd(a, b);
    --a, --b;
    g[a].push_back(b);
    g[b].push_back(a);
  }
  EulerTour et(g);
  RMQ rmq(et.depth_tour.size(), [&](int i, int j) {
    return et.depth_tour[i] < et.depth_tour[j];
  });
  vector<int> ord(n);
  unsigned Q, k, v;
  rd(Q);
  REP(_, Q) {
    rd(k);
    REP(i, k) {
      rd(v);
      ord[i] = et.in[--v];
    }
    sort(ord.begin(), ord.begin() + k);
    int lca_ord = rmq.fold(ord[0], ord[k - 1]);
    int res = et.depth_tour[ord[0]] - et.depth_tour[lca_ord];
    for (unsigned i = 1; i < k; ++i) {
      lca_ord = rmq.fold(ord[i - 1], ord[i]);
      res += et.depth_tour[ord[i]] - et.depth_tour[lca_ord];
    }
    wtn(res);
  }
}

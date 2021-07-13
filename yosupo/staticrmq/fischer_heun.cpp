#include <bits/stdc++.h>
using namespace std;

template<typename T>
constexpr int num_bits = CHAR_BIT * sizeof(T);

// Log base 2 of the most significant bit which is set.
inline int msb_log(unsigned x) {
  //  assert(x != 0);
  return std::numeric_limits<unsigned>::digits - __builtin_clz(x) - 1;
}
inline int msb_log(unsigned long long x) {
  //  assert(x != 0);
  return std::numeric_limits<unsigned long long>::digits - __builtin_clzll(x) -
      1;
}

template<class BetterOp, class mask_t = unsigned>
struct RMQ {
  static_assert(std::is_integral<mask_t>::value, "mask_t must be integral");
  static_assert(std::is_unsigned<mask_t>::value, "mask_t must be unsigned");
  static_assert(std::is_invocable_r_v<bool, BetterOp, int, int>);
  static constexpr int block_size_ = num_bits<mask_t>;

  int n_;                       // sequence size
  int block_count_;             // total number of blocks
  std::vector<mask_t> indicator_;
  std::vector<std::vector<int>> sparse_table_;
  BetterOp better_than_;  // checks if lhs is strictly better than rhs.

  RMQ(int n, BetterOp better)
      : n_(n),
        block_count_((n_ + block_size_ - 1) / block_size_),
        indicator_(n_),
        sparse_table_(
            block_count_ == 0 ? 0 : msb_log(unsigned(block_count_)) + 1,
            std::vector<int>(block_count_)),
        better_than_(std::move(better)) {
    static constexpr int bufsize = block_size_ + 1;
    static int buf[bufsize];  // ring buffer [lp,rp)
    int lp = 1, rp = 1, rpm1 = 0;  // rpm1 = rp-1 (mod bufsize)

    // Build the indicator table.
    for (int r = 0; r < n_; ++r) {
      while (lp != rp and (r - buf[lp] >= block_size_)) {
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

  // Returns the index of the best value in [l, r) (half-open interval).
  inline int fold(int l, int r) const {
    // We internally use closed interval.
    return best_index(l, r - 1);
  }

 private:
  inline int better_index(int i, int j) const {
    return better_than_(i, j) ? i : j;
  }

  // Returns the index of the best value in [r - sz, r] (closed interval).
  inline int best_index_small(int r, int sz = block_size_) const {
//    assert(r < n_);
//    assert(sz > 0);
//    assert(sz <= block_size_);
    mask_t ind = indicator_[r];
    if (sz < block_size_) {
      ind &= (mask_t(1) << sz) - 1;
    }
    return r - msb_log(ind);
  }

  // Returns the index of the best value in [l, r] (closed interval).
  inline int best_index(int l, int r) const {
    l = std::clamp(l, 0, n_ - 1);
    r = std::clamp(r, 0, n_ - 1);
    if (r - l + 1 <= block_size_) {
      return best_index_small(r, r - l + 1);
    }
    int ql = best_index_small(std::min(l + block_size_, n_) - 1);
    int qr = best_index_small(r);
    l = l / block_size_ + 1;
    r = r / block_size_ - 1;
    int ans = better_index(ql, qr);
    if (l <= r) {
      int i = msb_log(unsigned(r - l + 1));
      int qs =
          better_index(sparse_table_[i][l], sparse_table_[i][r - (1 << i) + 1]);
      ans = better_index(ans, qs);
    }
    return ans;
  }
};

template<class BetterOp>
RMQ<BetterOp> create_rmq(int n, BetterOp better) {
  return {n, std::move(better)};
}

namespace fastio {

#if !HAVE_DECL_FREAD_UNLOCKED
#define fread_unlocked fread
#endif
#if !HAVE_DECL_FWRITE_UNLOCKED
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
inline void wt(Head &&head, Tail &&... tail) {
  wt(head);
  wt(std::forward<Tail>(tail)...);
}
template<typename... Args>
inline void wtn(Args &&... x) {
  wt(std::forward<Args>(x)...);
  wt('\n');
}

}  // namespace fastio
using fastio::rd;
using fastio::wtn;

int main() {
  unsigned n, q;
  rd(n, q);
  vector<unsigned> a(n);
  for (unsigned i = 0; i < n; ++i) {
    rd(a[i]);
  }
  auto rmq = create_rmq(n, [&](int i, int j) {
    return a[i] < a[j];
  });
  unsigned l, r;
  for (unsigned i = 0; i < q; ++i) {
    rd(l, r);
    wtn(a[rmq.fold(l, r)]);
  }
}

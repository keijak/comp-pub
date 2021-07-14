#include <bits/stdc++.h>
using i64 = std::int64_t;
using u64 = std::uint64_t;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
#define SIZE(a) (int)((a).size())

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
template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &pprint(const Container &a, std::string_view sep = " ",
                     std::string_view ends = "\n", std::ostream *os = nullptr) {
  if (os == nullptr) os = &std::cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
}
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T,
          typename = std::enable_if_t<is_iterable<T>::value &&
                                      !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef MY_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &... args) {
  pdebug(value);
  std::cerr << ", ";
  pdebug(args...);
}
#define DEBUG(...)                                   \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ") "; \
    std::cerr << #__VA_ARGS__ << ":\033[0m ";        \
    pdebug(__VA_ARGS__);                             \
    std::cerr << std::endl;                          \
  } while (0)
#else
#define pdebug(...)
#define DEBUG(...)
#endif

#define HLL_HASH_SEED 313

#if defined(__has_builtin) && (defined(__GNUC__) || defined(__clang__))

#define _GET_CLZ(x, b) (uint8_t) std::min(b, ::__builtin_clz(x)) + 1

#else

inline uint8_t _get_leading_zero_count(uint32_t x, uint8_t b) {

#if defined(_MSC_VER)
  uint32_t leading_zero_len = 32;
  ::_BitScanReverse(&leading_zero_len, x);
  --leading_zero_len;
  return std::min(b, (uint8_t)leading_zero_len);
#else
  uint8_t v = 1;
  while (v <= b && !(x & 0x80000000)) {
    v++;
    x <<= 1;
  }
  return v;
#endif
}
#define _GET_CLZ(x, b) _get_leading_zero_count(x, b)
#endif /* defined(__GNUC__) */

#define FORCE_INLINE __attribute__((always_inline))

inline uint32_t rotl32(uint32_t x, uint8_t r) {
  return (x << r) | (x >> (32 - r));
}

#define ROTL32(x, y) rotl32(x, y)

#define BIG_CONSTANT(x) (x##LLU)

/* NO-OP for little-endian platforms */
#if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__)
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define BYTESWAP(x) (x)
#endif
/* if __BYTE_ORDER__ is not predefined (like FreeBSD), use arch */
#elif defined(__i386) || defined(__x86_64) || defined(__alpha) || defined(__vax)

#define BYTESWAP(x) (x)
/* use __builtin_bswap32 if available */
#elif defined(__GNUC__) || defined(__clang__)
#ifdef __has_builtin
#if __has_builtin(__builtin_bswap32)
#define BYTESWAP(x) __builtin_bswap32(x)
#endif  // __has_builtin(__builtin_bswap32)
#endif  // __has_builtin
#endif  // defined(__GNUC__) || defined(__clang__)
/* last resort (big-endian w/o __builtin_bswap) */
#ifndef BYTESWAP
#define BYTESWAP(x)                                                      \
  ((((x)&0xFF) << 24) | (((x) >> 24) & 0xFF) | (((x)&0x0000FF00) << 8) | \
   (((x)&0x00FF0000) >> 8))
#endif

//-----------------------------------------------------------------------------
// Block read - if your platform needs to do endian-swapping or can only
// handle aligned reads, do the conversion here

#define getblock(p, i) BYTESWAP(p[i])

//-----------------------------------------------------------------------------
// Finalization mix - force all bits of a hash block to avalanche

uint32_t fmix32(uint32_t h) {
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h;
}

void MurmurHash3_x86_32(const void *key, int len, uint32_t seed, void *out) {
  const uint8_t *data = (const uint8_t *)key;
  const int nblocks = len / 4;
  int i;

  uint32_t h1 = seed;

  uint32_t c1 = 0xcc9e2d51;
  uint32_t c2 = 0x1b873593;

  //----------
  // body

  const uint32_t *blocks = (const uint32_t *)(data + nblocks * 4);

  for (i = -nblocks; i; i++) {
    uint32_t k1 = getblock(blocks, i);

    k1 *= c1;
    k1 = ROTL32(k1, 15);
    k1 *= c2;

    h1 ^= k1;
    h1 = ROTL32(h1, 13);
    h1 = h1 * 5 + 0xe6546b64;
  }

  //----------
  // tail
  {
    const uint8_t *tail = (const uint8_t *)(data + nblocks * 4);

    uint32_t k1 = 0;

    switch (len & 3) {
      case 3:
        k1 ^= tail[2] << 16;
      case 2:
        k1 ^= tail[1] << 8;
      case 1:
        k1 ^= tail[0];
        k1 *= c1;
        k1 = ROTL32(k1, 15);
        k1 *= c2;
        h1 ^= k1;
    };
  }

  //----------
  // finalization

  h1 ^= len;

  h1 = fmix32(h1);

  *(uint32_t *)out = h1;
}

namespace hll {

static const double pow_2_32 = 4294967296.0;       ///< 2^32
static const double neg_pow_2_32 = -4294967296.0;  ///< -(2^32)

/** @class HyperLogLog
 *  @brief Implement of 'HyperLogLog' estimate cardinality algorithm
 */
class HyperLogLog {
 public:
  /**
   * Constructor
   *
   * @param[in] b bit width (register size will be 2 to the b power).
   *            This value must be in the range[4,30].Default value is 4.
   *
   * @exception std::invalid_argument the argument is out of range.
   */
  HyperLogLog(uint8_t b = 4) : b_(b), m_(1 << b), M_(m_, 0) {
    if (b < 4 || 30 < b) {
      throw std::invalid_argument("bit width must be in the range [4,30]");
    }

    double alpha;
    switch (m_) {
      case 16:
        alpha = 0.673;
        break;
      case 32:
        alpha = 0.697;
        break;
      case 64:
        alpha = 0.709;
        break;
      default:
        alpha = 0.7213 / (1.0 + 1.079 / m_);
        break;
    }
    alphaMM_ = alpha * m_ * m_;
  }

  /**
   * Adds element to the estimator
   *
   * @param[in] str string to add
   * @param[in] len length of string
   */
  void add(uint32_t value) {
    uint32_t index = value >> (32 - b_);
    uint8_t rank = _GET_CLZ((value << b_), 32 - b_);
    if (rank > M_[index]) {
      M_[index] = rank;
    }
  }

  /**
   * Estimates cardinality value.
   *
   * @return Estimated cardinality value.
   */
  double estimate() const {
    double estimate;
    double sum = 0.0;
    for (uint32_t i = 0; i < m_; i++) {
      sum += 1.0 / (1 << M_[i]);
    }
    estimate = alphaMM_ / sum;  // E in the original paper
    if (estimate <= 2.5 * m_) {
      uint32_t zeros = 0;
      for (uint32_t i = 0; i < m_; i++) {
        if (M_[i] == 0) {
          zeros++;
        }
      }
      if (zeros != 0) {
        estimate = m_ * std::log(static_cast<double>(m_) / zeros);
      }
    } else if (estimate > (1.0 / 30.0) * pow_2_32) {
      estimate = neg_pow_2_32 * log(1.0 - (estimate / pow_2_32));
    }
    return estimate;
  }

  /**
   * Merges the estimate from 'other' into this object, returning the estimate
   * of their union. The number of registers in each must be the same.
   *
   * @param[in] other HyperLogLog instance to be merged
   *
   * @exception std::invalid_argument number of registers doesn't match.
   */
  void merge(const HyperLogLog &other) {
    if (m_ != other.m_) {
      std::stringstream ss;
      ss << "number of registers doesn't match: " << m_ << " != " << other.m_;
      throw std::invalid_argument(ss.str().c_str());
    }
    for (uint32_t r = 0; r < m_; ++r) {
      if (M_[r] < other.M_[r]) {
        M_[r] |= other.M_[r];
      }
    }
  }

  /**
   * Clears all internal registers.
   */
  void clear() { std::fill(M_.begin(), M_.end(), 0); }

  /**
   * Returns size of register.
   *
   * @return Register size
   */
  uint32_t registerSize() const { return m_; }

  /**
   * Exchanges the content of the instance
   *
   * @param[in,out] rhs Another HyperLogLog instance
   */
  void swap(HyperLogLog &rhs) {
    std::swap(b_, rhs.b_);
    std::swap(m_, rhs.m_);
    std::swap(alphaMM_, rhs.alphaMM_);
    M_.swap(rhs.M_);
  }

  /**
   * Dump the current status to a stream
   *
   * @param[out] os The output stream where the data is saved
   *
   * @exception std::runtime_error When failed to dump.
   */
  void dump(std::ostream &os) const {
    os.write((char *)&b_, sizeof(b_));
    os.write((char *)&M_[0], sizeof(M_[0]) * M_.size());
    if (os.fail()) {
      throw std::runtime_error("Failed to dump");
    }
  }

  /**
   * Restore the status from a stream
   *
   * @param[in] is The input stream where the status is saved
   *
   * @exception std::runtime_error When failed to restore.
   */
  void restore(std::istream &is) {
    uint8_t b = 0;
    is.read((char *)&b, sizeof(b));
    HyperLogLog tempHLL(b);
    is.read((char *)&(tempHLL.M_[0]), sizeof(M_[0]) * tempHLL.m_);
    if (is.fail()) {
      throw std::runtime_error("Failed to restore");
    }
    swap(tempHLL);
  }

 protected:
  uint8_t b_;               ///< register bit width
  uint32_t m_;              ///< register size
  double alphaMM_;          ///< alpha * m^2
  std::vector<uint8_t> M_;  ///< registers
};

/**
 * @brief HIP estimator on HyperLogLog counter.
 */
class HyperLogLogHIP : public HyperLogLog {
 public:
  /**
   * Constructor
   *
   * @param[in] b bit width (register size will be 2 to the b power).
   *            This value must be in the range[4,30].Default value is 4.
   *
   * @exception std::invalid_argument the argument is out of range.
   */
  HyperLogLogHIP(uint8_t b = 4)
      : HyperLogLog(b), register_limit_((1 << 5) - 1), c_(0.0), p_(1 << b) {}

  /**
   * Adds element to the estimator
   *
   * @param[in] str string to add
   * @param[in] len length of string
   */
  void add(uint32_t value) {
    uint32_t index = value >> (32 - b_);
    uint8_t rank = _GET_CLZ((value << b_), 32 - b_);
    rank = rank == 0 ? register_limit_ : std::min(register_limit_, rank);
    const uint8_t old = M_[index];
    if (rank > old) {
      c_ += 1.0 / (p_ / m_);
      p_ -= 1.0 / (1 << old);
      M_[index] = rank;
      if (rank < 31) {
        p_ += 1.0 / (uint32_t(1) << rank);
      }
    }
  }

  /**
   * Estimates cardinality value.
   *
   * @return Estimated cardinality value.
   */
  double estimate() const { return c_; }

  /**
   * Merges the estimate from 'other' into this object, returning the estimate
   * of their union. The number of registers in each must be the same.
   *
   * @param[in] other HyperLogLog instance to be merged
   *
   * @exception std::invalid_argument number of registers doesn't match.
   */
  void merge(const HyperLogLogHIP &other) {
    if (m_ != other.m_) {
      std::stringstream ss;
      ss << "number of registers doesn't match: " << m_ << " != " << other.m_;
      throw std::invalid_argument(ss.str().c_str());
    }
    for (uint32_t r = 0; r < m_; ++r) {
      const uint8_t b = M_[r];
      const uint8_t b_other = other.M_[r];
      if (b < b_other) {
        c_ += 1.0 / (p_ / m_);
        p_ -= 1.0 / (1 << b);
        M_[r] |= b_other;
        if (b_other < register_limit_) {
          p_ += 1.0 / (1 << b_other);
        }
      }
    }
  }

  /**
   * Clears all internal registers.
   */
  void clear() {
    std::fill(M_.begin(), M_.end(), 0);
    c_ = 0.0;
    p_ = 1 << b_;
  }

  /**
   * Returns size of register.
   *
   * @return Register size
   */
  uint32_t registerSize() const { return m_; }

  /**
   * Exchanges the content of the instance
   *
   * @param[in,out] rhs Another HyperLogLog instance
   */
  void swap(HyperLogLogHIP &rhs) {
    std::swap(b_, rhs.b_);
    std::swap(m_, rhs.m_);
    std::swap(c_, rhs.c_);
    M_.swap(rhs.M_);
  }

  /**
   * Dump the current status to a stream
   *
   * @param[out] os The output stream where the data is saved
   *
   * @exception std::runtime_error When failed to dump.
   */
  void dump(std::ostream &os) const {
    os.write((char *)&b_, sizeof(b_));
    os.write((char *)&M_[0], sizeof(M_[0]) * M_.size());
    os.write((char *)&c_, sizeof(c_));
    os.write((char *)&p_, sizeof(p_));
    if (os.fail()) {
      throw std::runtime_error("Failed to dump");
    }
  }

  /**
   * Restore the status from a stream
   *
   * @param[in] is The input stream where the status is saved
   *
   * @exception std::runtime_error When failed to restore.
   */
  void restore(std::istream &is) {
    uint8_t b = 0;
    is.read((char *)&b, sizeof(b));
    HyperLogLogHIP tempHLL(b);
    is.read((char *)&(tempHLL.M_[0]), sizeof(M_[0]) * tempHLL.m_);
    is.read((char *)&(tempHLL.c_), sizeof(double));
    is.read((char *)&(tempHLL.p_), sizeof(double));
    if (is.fail()) {
      throw std::runtime_error("Failed to restore");
    }
    swap(tempHLL);
  }

 private:
  const uint8_t register_limit_;
  double c_;
  double p_;
};

}  // namespace hll

template <typename Monoid>
struct SegTree {
  using T = typename Monoid::T;

  inline int n() const { return n_; }
  inline int offset() const { return offset_; }

  explicit SegTree(int n) : n_(n) {
    offset_ = 1;
    while (offset_ < n_) offset_ <<= 1;
    data_.assign(2 * offset_, Monoid::id());
  }

  explicit SegTree(const std::vector<T> &leaves) : n_(leaves.size()) {
    offset_ = 1;
    while (offset_ < n_) offset_ <<= 1;
    data_.assign(2 * offset_, Monoid::id());
    for (int i = 0; i < n_; ++i) {
      data_[offset_ + i] = leaves[i];
    }
    for (int i = offset_ - 1; i > 0; --i) {
      data_[i] = Monoid::op(data_[i * 2], data_[i * 2 + 1]);
    }
  }

  // Sets i-th value (0-indexed) to x.
  void set(int i, const T &x) {
    int k = offset_ + i;
    data_[k] = x;
    // Update its ancestors.
    while (k > 1) {
      k >>= 1;
      data_[k] = Monoid::op(data_[k * 2], data_[k * 2 + 1]);
    }
  }

  // Queries by [l,r) range (0-indexed, half-open interval).
  T fold(int l, int r) const {
    l = std::max(l, 0) + offset_;
    r = std::min(r, offset_) + offset_;
    T vleft = Monoid::id(), vright = Monoid::id();
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) vleft = Monoid::op(vleft, data_[l++]);
      if (r & 1) vright = Monoid::op(data_[--r], vright);
    }
    return Monoid::op(vleft, vright);
  }

  // Returns i-th value (0-indexed).
  T operator[](int i) const { return data_[offset_ + i]; }

  template <bool (*pred)(const T &)>
  int max_right(int l) {
    return max_right(l, [](const T &x) -> bool { return pred(x); });
  }
  template <class Predicate>
  int max_right(int l, Predicate pred) {
    assert(0 <= l && l <= n_);
    assert(pred(Monoid::id()));
    if (l == n_) return n_;
    l += offset_;
    T sm = Monoid::id();
    do {
      while (l % 2 == 0) l >>= 1;
      if (!pred(Monoid::op(sm, data_[l]))) {
        while (l < offset_) {
          l = (2 * l);
          if (pred(Monoid::op(sm, data_[l]))) {
            sm = Monoid::op(sm, data_[l]);
            l++;
          }
        }
        return l - offset_;
      }
      sm = Monoid::op(sm, data_[l]);
      l++;
    } while ((l & -l) != l);
    return n_;
  }

  template <bool (*pred)(const T &)>
  int min_left(int r) {
    return min_left(r, [](const T &x) -> bool { return pred(x); });
  }
  template <class Predicate>
  int min_left(int r, Predicate pred) {
    assert(0 <= r && r <= n_);
    assert(pred(Monoid::id()));
    if (r == 0) return 0;
    r += offset_;
    T sm = Monoid::id();
    do {
      r--;
      while (r > 1 && (r % 2)) r >>= 1;
      if (!pred(Monoid::op(data_[r], sm))) {
        while (r < offset_) {
          r = (2 * r + 1);
          if (pred(Monoid::op(data_[r], sm))) {
            sm = Monoid::op(data_[r], sm);
            r--;
          }
        }
        return r + 1 - offset_;
      }
      sm = Monoid::op(data_[r], sm);
    } while ((r & -r) != r);
    return 0;
  }

  friend std::ostream &operator<<(std::ostream &os, const SegTree &st) {
    os << "[";
    for (int i = 0; i < st.n(); ++i) {
      if (i != 0) os << ", ";
      const auto &x = st[i];
      os << x;
    }
    return os << "]";
  }

 private:
  int n_;                // number of valid leaves.
  int offset_;           // where leaves start
  std::vector<T> data_;  // data size: 2*offset_
};

using namespace std;

const int HLL_BIT_WIDTH = 24;

struct DistinctCount {
  using T = hll::HyperLogLog;
  static T op(const T &x, const T &y) {
    T res(HLL_BIT_WIDTH);
    res.merge(x);
    res.merge(y);
    return res;
  }
  static T id() { return {HLL_BIT_WIDTH}; }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, q;
  cin >> n >> q;
  V<DistinctCount::T> c;
  REP(i, n) {
    uint32_t x;
    cin >> x;
    c.emplace_back(HLL_BIT_WIDTH);
    c.back().add(x);
  }
  SegTree<DistinctCount> seg(c);
  REP(i, q) {
    int l, r;
    cin >> l >> r;
    l--;
    auto ans = seg.fold(l, r).estimate();
    cout << int(ans) << '\n';
  }
}

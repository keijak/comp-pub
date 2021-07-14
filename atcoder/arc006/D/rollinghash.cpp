#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>
using __gnu_pbds::gp_hash_table;

#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream *os = nullptr) {
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

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", &(os << "{")) << "}";
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
void pdebug(const T &value, const Ts &...args) {
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

using namespace std;

struct RollingHash {
  using u64 = unsigned long long;
  using u128 = __uint128_t;
  static const u64 mod = (1ULL << 61) - 1;
  vector<u64> base1_pow, base2_pow;
  vector<vector<u64>> hash_val;

  explicit RollingHash(const vector<string> &g) {
    const int n = g.size();
    const int m = g[0].size();
    hash_val.assign(n + 1, vector<u64>(m + 1, 0));
    base1_pow.assign(n + 1, 0);
    base2_pow.assign(m + 1, 0);
    auto [b1, b2] = base();
    base1_pow[0] = base2_pow[0] = 1;
    REP(i, n) base1_pow[i + 1] = mul(base1_pow[i], b1);
    REP(i, m) base2_pow[i + 1] = mul(base2_pow[i], b2);
    REP(i, n) REP(j, m) {
      u64 h = u64(g[i][j]);
      h = add(h, mul(hash_val[i][j + 1], b1));
      h = add(h, mul(hash_val[i + 1][j], b2));
      h = sub(h, mul(mul(hash_val[i][j], b1), b2));
      hash_val[i + 1][j + 1] = h;
    }
  }

  // Returns the hash value for the [(r0,c0), (r1,c1)) rectangle.
  // (r1,c1) is exclusive.
  u64 get(int r0, int c0, int r1, int c1) const {
    u64 h = hash_val[r1][c1];
    h = sub(h, mul(hash_val[r0][c1], base1_pow[r1 - r0]));
    h = sub(h, mul(hash_val[r1][c0], base2_pow[c1 - c0]));
    u64 tmp = mul(hash_val[r0][c0], base1_pow[r1 - r0]);
    h = add(h, mul(tmp, base2_pow[c1 - c0]));
    return h;
  }

 private:
  static pair<u64, u64> base() {
    static auto val = []() -> pair<u64, u64> {
      random_device seed_gen;
      mt19937_64 engine(seed_gen());
      uniform_int_distribution<u64> rand(1, mod - 1);
      return {rand(engine), rand(engine)};
    }();
    return val;
  }

  static inline u64 add(u64 a, u64 b) {
    a += b;
    return (a >= mod) ? (a - mod) : a;
  }

  static inline u64 sub(u64 a, u64 b) { return add(a, mod - b); }

  static inline u64 mul(u64 a, u64 b) {
    u128 t = (u128)a * b;
    u64 na = t >> 61;
    u64 nb = t & mod;
    na += nb;
    return (na >= mod) ? (na - mod) : na;
  }
};

array<array<string, 7>, 3> T = {array<string, 7>{
                                    "......."s,
                                    "...o..."s,
                                    "..o.o.."s,
                                    ".o...o."s,
                                    ".ooooo."s,
                                    ".o...o."s,
                                    "......."s,
                                },
                                array<string, 7>{
                                    "......."s,
                                    ".oooo.."s,
                                    ".o...o."s,
                                    ".oooo.."s,
                                    ".o...o."s,
                                    ".oooo.."s,
                                    "......."s,
                                },
                                array<string, 7>{
                                    "......."s,
                                    "..ooo.."s,
                                    ".o...o."s,
                                    ".o....."s,
                                    ".o...o."s,
                                    "..ooo.."s,
                                    "......."s,
                                }};

array<int, 3> solve() {
  int H, W;
  cin >> H >> W;
  vector<string> grid(H);
  REP(i, H) cin >> grid[i];

  const int K = min(H, W) / 7;

  array<int, 3> ans = {0, 0, 0};
  const string alphas = "ABC";

  RollingHash g_hash(grid);

  vector<gp_hash_table<u64, int>> hashes(K + 1);

  gp_hash_table<int, vector<tuple<int, int, int, u64>>> streak_map;

  // Indexing
  REP(rot, 4) {
    for (int scale = K; scale >= 1; --scale) {
      int s7 = scale * 7;
      REP(alpha, 3) {
        vector<string> t(s7);
        REP(i, 7) {
          string buf(s7, ' ');
          REP(j, 7) {
            char ch = T[alpha][i][j];
            REP(x, scale) { buf[j * scale + x] = ch; }
          }
          REP(x, scale) { t[i * scale + x] = buf; }
        }
        RollingHash rhash(t);
        u64 h = rhash.get(0, 0, s7, s7);
        hashes[scale][h] = alpha;

        int streak = 0;
        int offset_i = -1, offset_j = -1;
        REP(i, s7) {
          REP(j, s7) {
            if (streak == 0) {
              if (t[i][j] != 'o') continue;
              counting = true;
              offset_i = i;
              offset_j = j;
              ++streak;
            } else {
              if (t[i][j] != 'o') goto fin;
              ++streak;
              if (j == s7 - 1) goto fin;
            }
          }
        }
      fin:
        assert(streak > 0);
        streak_map[streak].emplace_back(offset_i, offset_j, scale, h);
        continue;
      }
    }
    // Rotation
    REP(alpha, 3) {
      array<string, 7> buf = T[alpha];
      REP(i, 7) REP(j, 7) { T[alpha][j][6 - i] = buf[i][j]; }
    }
  }

  vector skip(H + 1, vector(W + 1, 0LL));
  vector<i64> skip_buf(W + 1, 0);

  for (int r = 0; r < H; ++r) {
    REP(c, W) skip[r][c + 1] += skip[r][c];
    if (r > 0) {
      REP(c, W + 1) skip[r][c] += skip[r - 1][c];
    }

    skip_buf.assign(W + 1, 0);
    int streak = 0;

    for (int c = 0; c < W; ++c) {
      if (c > 0) skip_buf[c] += skip_buf[c - 1];
      if (skip[r][c] + skip_buf[c] != 0) {
        streak = 0;
        continue;
      }
      for (int scale = 1; scale <= K; ++scale) {
        if (r + 7 * scale > H or c + 7 * scale > W) break;
        auto h1 = g_hash.get(r, c, r + 7 * scale, c + 7 * scale);
        auto it = hashes[scale].find(h1);
        if (it != hashes[scale].end()) {
          int alpha = it->second;
          DEBUG(alphas[alpha], scale, r, c);
          ++ans[alpha];
          ++skip_buf[c];
          --skip_buf[c + scale * 7];
          --skip[r + scale * 7][c];
          ++skip[r + scale * 7][c + scale * 7];
          break;
        }
      }
    }
    REP(c, W + 1) { skip[r][c] += skip_buf[c]; }
  }
  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  auto res = solve();
  cout << res[0] << ' ' << res[1] << ' ' << res[2] << "\n";
}

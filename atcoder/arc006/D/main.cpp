#include <bits/stdc++.h>

#include <atcoder/dsu>
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

#ifdef ENABLE_DEBUG
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

array<i64, 3> solve() {
  i64 H, W;
  cin >> H >> W;
  vector<string> grid(H);
  REP(i, H) cin >> grid[i];

  const i64 K = min(H, W) / 7;
  array<i64, 3> ans = {0, 0, 0};
  const string alphas = "ABC";
  array<i64, 3> blocks = {12, 16, 11};

  unordered_set<i64> scales;
  for (i64 x = 1; x <= K; ++x) {
    scales.insert(x * x);
  }

  atcoder::dsu uf(H * W);
  REP(i, H) REP(j, W) {
    if (grid[i][j] != 'o') continue;
    i64 v = i * W + j;
    for (int dx = -1; dx <= +1; ++dx) {
      for (int dy = -1; dy <= +1; ++dy) {
        if (dx == 0 and dy == 0) continue;
        int nr = i + dx, nc = j + dy;
        if (nr < 0 or nr >= H or nc < 0 or nc >= W) continue;
        if (grid[nr][nc] != 'o') continue;
        i64 u = nr * W + nc;
        uf.merge(v, u);
      }
    }
  }
  set<i64> done;
  REP(i, H) REP(j, W) {
    if (grid[i][j] != 'o') continue;
    i64 v = i * W + j;
    i64 leader = uf.leader(v);
    if (done.count(leader)) continue;
    done.insert(leader);
    i64 sz = uf.size(leader);
    REP(alpha, 3) {
      i64 b = blocks[alpha];
      if (sz % b != 0) continue;
      if (scales.count(sz / b)) {
        ++ans[alpha];
        break;
      }
    }
  }

  return ans;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  auto res = solve();
  cout << res[0] << ' ' << res[1] << ' ' << res[2] << "\n";
}

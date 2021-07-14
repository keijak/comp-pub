#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

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

using namespace std;

const int N = 18, M = 6;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  V<string> grid(N + 2);
  grid[0] = grid[N + 1] = string(M + 2, '0');
  REP(i, N) {
    string line;
    cin >> line;
    grid[i + 1] = '0' + line + '0';
  }

  auto dp = vector(N + 1, vector(1 << (M * 2), 0LL));

  REP(bits, 1LL << M) {
    bool ok = true;
    REP(c, M) {
      if (bits >> c & 1) {
        if (grid[1][c + 1] == '0') {
          ok = false;
          break;
        }
      } else {
        if (grid[1][c + 1] == '1') {
          ok = false;
          break;
        }
      }
    }
    if (ok) {
      dp[0][bits << M] = 1;
    }
  }

  REP(row, N) {
    REP(bits, 1LL << (M * 3)) {
      int s_before = bits & ((1 << 2 * M) - 1);
      int s_after = bits >> M;
      if (dp[row][s_before] == 0) continue;

      bool ok = true;
      V<string> mini(3, string(M + 2, '0'));
      REP(i, M * 3) {
        int rd = i / M;
        int r = row + rd;
        int c = i % M;
        if (bits >> i & 1) {
          if (grid[r][c + 1] == '0') {
            ok = false;
            break;
          }
          mini[rd][c + 1] = '1';
        } else {
          if (grid[r][c + 1] == '1') {
            ok = false;
            break;
          }
          mini[rd][c + 1] = '0';
        }
      }
      if (not ok) continue;

      REP(c, M) {
        int ones = 0;
        if (mini[0][c + 1] == '1') ++ones;
        if (mini[1][c + 0] == '1') ++ones;
        if (mini[1][c + 1] == '1') ++ones;
        if (mini[1][c + 2] == '1') ++ones;
        if (mini[2][c + 1] == '1') ++ones;

        if (ones >= 3 and mini[1][c + 1] == '0') {
          ok = false;
          break;
        }
        if (ones < 3 and mini[1][c + 1] == '1') {
          ok = false;
          break;
        }
      }
      if (not ok) continue;

      dp[row + 1][s_after] += dp[row][s_before];
    }
  }
  i64 ans = accumulate(ALL(dp.back()), 0LL);
  cout << ans << endl;
}

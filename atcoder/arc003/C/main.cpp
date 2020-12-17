#include <bits/stdc++.h>
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
using Float = double;

array<int, 4> dx = {0, 0, 1, -1}, dy = {1, -1, 0, 0};

optional<Float> solve() {
  int n, m;
  cin >> n >> m;
  int si = -1, sj = -1, gi = -1, gj = -1;
  vector<string> g(n);
  REP(i, n) {
    cin >> g[i];
    REP(j, m) {
      if (g[i][j] == 's') {
        si = i, sj = j;
      } else if (g[i][j] == 'g') {
        gi = i, gj = j;
      }
    }
  }

  vector<Float> scale(n * m);
  scale[0] = 1.0;
  for (int i = 1; i < n * m; ++i) {
    scale[i] = scale[i - 1] * 0.99;
  }

  auto check = [&](Float x, Float &ub) -> bool {
    queue<tuple<int, int, int, Float>> q;
    q.emplace(0, si, sj, 13);
    vector visited(n, vector(m, false));
    visited[si][sj] = true;
    while (q.size()) {
      auto [t, i, j, minlight] = q.front();
      q.pop();
      if (i == gi and j == gj) {
        ub = minlight;
        return x <= ub;
      }
      REP(d, 4) {
        int ni = i + dx[d];
        int nj = j + dy[d];
        if (ni < 0 or ni >= n or nj < 0 or nj >= m) continue;
        if (g[ni][nj] == '#' or visited[ni][nj]) continue;
        visited[ni][nj] = true;
        Float ml = minlight;
        if (isdigit(g[ni][nj])) {
          Float l = g[ni][nj] - '0';
          l *= scale[t + 1];
          if (l < x) continue;
          chmin(ml, l);
        }
        q.emplace(t + 1, ni, nj, ml);
      }
    }
    return false;
  };

  Float tv = -1, fv = 12;
  REP(iter, 80) {
    Float mid = (tv + fv) * 0.5;
    Float z;
    if (check(mid, z)) {
      tv = z;
    } else {
      fv = mid;
    }
  }
  if (tv < 0) return nullopt;
  return tv;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  auto res = solve();
  if (not res) {
    cout << "-1\n";
  } else {
    cout << res.value() << "\n";
  }
}

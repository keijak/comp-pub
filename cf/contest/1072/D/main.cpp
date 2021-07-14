#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
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
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
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
  return print_seq(a, ", ", "", (os << "{")) << "}";
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

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

string solve() {
  int n, k;
  cin >> n >> k;
  vector<string> ma(n);
  REP(i, n) { cin >> ma[i]; }

  vector acount(n, vector(n, -1));
  acount[0][0] = (ma[0][0] == 'a') ? k : (k - 1);
  REP(i, n) {
    REP(j, n) {
      int is_a = ma[i][j] == 'a';
      if (i > 0) chmax(acount[i][j], acount[i - 1][j] - 1 + is_a);
      if (j > 0) chmax(acount[i][j], acount[i][j - 1] - 1 + is_a);
    }
  }

  //   REP(i, n) {
  //     REP(j, n) { cerr << acount[i][j] << " "; }
  //     cerr << endl;
  //   }

  vector done(n, vector(n, false));
  MinHeap<tuple<int, char, int, int>> heap;
  int maxd = 0;
  REP(i, n) {
    REP(j, n) {
      if (acount[i][j] >= 0) {
        done[i][j] = true;
        continue;
      }
      bool zero_next = (i == 0 and j == 0);
      if (i > 0 and acount[i - 1][j] == 0) zero_next = true;
      if (j > 0 and acount[i][j - 1] == 0) zero_next = true;
      if (zero_next) {
        assert(ma[i][j] != 'a');
        int d = i + j;
        chmax(maxd, d);
        heap.emplace(d, ma[i][j], i, j);
        done[i][j] = true;
      }
    }
  }
  if (acount[n - 1][n - 1] >= 0) {
    return string(2 * n - 1, 'a');
  }
  assert(maxd < 2 * n - 1);

  array<int, 2> dy = {0, 1}, dx = {1, 0};
  string ans(maxd, 'a');
  while (heap.size()) {
    auto [d, ch, r, c] = heap.top();
    heap.pop();
    if (d < ssize(ans) - 1) continue;
    if (d == ssize(ans) - 1 and ans[d] < ch) continue;
    if (d == ssize(ans) - 1) assert(ans[d] == ch);
    if (d == ssize(ans)) {
      ans.push_back(ch);
    }
    if (ssize(ans) == 2 * n - 1) {
      assert(r == n - 1);
      assert(c == n - 1);
      break;
    }
    REP(i, 2) {
      int nr = r + dy[i];
      int nc = c + dx[i];
      if (nr < 0 or nr >= n or nc < 0 or nc >= n) continue;
      if (done[nr][nc]) continue;
      done[nr][nc] = true;
      int nd = nr + nc;
      heap.emplace(nd, ma[nr][nc], nr, nc);
    }
  }

  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}

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
using State = tuple<u64, int, int>;

array<int, 4> dx = {1, -1, 0, 0};
array<int, 4> dy = {0, 0, 1, -1};

Float solve() {
  i64 n, m, r, k;
  cin >> n >> m >> r >> k;
  if (n < m) swap(n, m);  // Ensure n >= m.
  // if (k == n * m) return r * r;

  priority_queue<State> q;
  set<State> enqueued;
  auto try_push = [&](int x, int y) -> bool {
    if (x < 0 or x >= n or y < 0 or y >= m) return false;

    i64 x_low = max(x - (r - 1), 0LL);
    i64 x_high = min(x + (r - 1), n - 1) - (r - 1);
    i64 y_low = max(y - (r - 1), 0LL);
    i64 y_high = min(y + (r - 1), m - 1) - (r - 1);
    i64 val = (x_high - x_low + 1) * (y_high - y_low + 1);
    State s = {val, x, y};
    if (enqueued.count(s)) return false;
    enqueued.insert(s);
    q.push(s);
    return true;
  };

  if ((n & 1) and (m & 1)) {
    try_push(n / 2, m / 2);
  } else if (n & 1) {
    try_push(n / 2, m / 2);
    try_push(n / 2, m / 2 - 1);
  } else if (m & 1) {
    try_push(n / 2, m / 2);
    try_push(n / 2 - 1, m / 2);
  } else {
    try_push(n / 2, m / 2);
    try_push(n / 2 - 1, m / 2);
    try_push(n / 2, m / 2 - 1);
    try_push(n / 2 - 1, m / 2 - 1);
  }
  int count = k;
  u64 nume = 0;
  while (count and not q.empty()) {
    auto [val, x, y] = q.top();
    q.pop();
    nume += val;
    count--;
    if (count == 0) break;
    REP(i, 4) {
      int nx = x + dx[i];
      int ny = y + dy[i];
      try_push(nx, ny);
    }
  }
  i64 deno = (n - r + 1) * (m - r + 1);
  return Float(nume) / deno;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);

  cout << solve() << "\n";
}

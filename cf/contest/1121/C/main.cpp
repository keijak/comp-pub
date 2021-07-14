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

i64 solve() {
  int n, k;
  cin >> n >> k;
  vector<int> a(n);
  cin >> a;
  queue<array<int, 4>> q;
  REP(i, n) { q.push({a[i], 0, a[i], i}); }
  deque<array<int, 4>> dq;
  REP(i, k) {
    if (q.empty()) break;
    auto p = move(q.front());
    q.pop();
    dq.push_back(p);
  }
  int progress = 0;
  int done = 0;
  set<int> interesting;
  while (not dq.empty()) {
    sort(ALL(dq));

    DEBUG(done, progress);
    DEBUG(dq);
    auto [tdelta, tested, total, task] = dq.front();
    for (auto &[rem, tt, tot, tj] : dq) {
      if (tt < progress and progress <= tot and tt + tdelta >= progress) {
        interesting.insert(tj);
      }
      tt += tdelta;
      rem -= tdelta;
    }
    while (not dq.empty() and dq[0][0] <= 0) {
      dq.pop_front();
      ++done;
    }
    DEBUG(interesting);
    progress = 100.0 * done / n + 0.5;
    while (not q.empty() and ssize(dq) < k) {
      auto p = move(q.front());
      q.pop();
      dq.push_back(move(p));
    }
  }

  return ssize(interesting);
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}

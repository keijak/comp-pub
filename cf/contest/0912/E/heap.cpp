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

using Int = __int128_t;
const Int BIG = 1e18 + 5;

i64 solve() {
  int n;
  cin >> n;
  vector<int> p(n);
  cin >> p;  // ascending order
  int k;
  cin >> k;

  unordered_set<u64> pushed;
  pushed.reserve(1 << 20);
  pushed.max_load_factor(0.25);

  auto smaller = [&](Int limit) -> int {
    MinHeap<u64> heap;
    heap.push(1);
    pushed.clear();
    pushed.insert(1);
    u64 max_pushed = 1;
    int count = 0;
    while (not heap.empty()) {
      auto val = heap.top();
      heap.pop();
      ++count;
      if (count >= k) return k;
      REP(i, n) {
        Int nval = val * Int(p[i]);
        if (nval >= limit) continue;
        if (ssize(pushed) >= k and nval >= max_pushed) continue;
        bool ok = pushed.insert(nval).second;
        if (ok) {
          heap.push(nval);
          chmax(max_pushed, nval);
        }
      }
    }
    return count;
  };

  Int tv = 0, fv = BIG;
  while (fv - tv > 1) {
    Int mid = (tv + fv) / 2;
    int x = smaller(mid);
    if (x < k) {
      tv = mid;
    } else {
      fv = mid;
    }
  }
  return tv;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}

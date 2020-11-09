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
template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
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

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
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

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

const string_view INSERT = "insert"sv;
const string_view GETMIN = "getMin"sv;
const string_view REMOVEMIN = "removeMin"sv;

V<pair<string_view, int>> solve() {
  int n;
  cin >> n;
  V<pair<string_view, int>> res;
  MinHeap<int> heap;
  REP(i, n) {
    string op;
    cin >> op;
    int x;
    if (op == INSERT) {
      cin >> x;
      heap.push(x);
      res.emplace_back(INSERT, x);
    } else if (op == GETMIN) {
      cin >> x;
      if (heap.empty() or heap.top() > x) {
        heap.push(x);
        res.emplace_back(INSERT, x);
      } else if (heap.top() == x) {
        // ok
      } else {
        assert(heap.top() < x);
        while (not heap.empty() and heap.top() < x) {
          heap.pop();
          res.emplace_back(REMOVEMIN, 0);
        }
        if (heap.empty() or heap.top() != x) {
          heap.push(x);
          res.emplace_back(INSERT, x);
        }
      }
      res.emplace_back(GETMIN, x);
    } else {
      assert(op == REMOVEMIN);
      if (heap.empty()) {
        heap.push(0);
        res.emplace_back(INSERT, 0);
      }
      heap.pop();
      res.emplace_back(REMOVEMIN, 0);
    }
  }
  return res;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  auto res = solve();
  cout << ssize(res) << '\n';
  REP(i, ssize(res)) {
    auto [op, x] = res[i];
    if (op == GETMIN or op == INSERT) {
      cout << op << ' ' << x << '\n';
    } else {
      cout << op << '\n';
    }
  }
}

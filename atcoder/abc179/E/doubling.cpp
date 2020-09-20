#include <bits/stdc++.h>
using i64 = std::int64_t;
using u64 = std::uint64_t;
#define REP(i, n) for (int i = 0; i < (i64)(n); ++i)
#define ALL(x) std::begin(x), std::end(x)
#define SIZE(a) (int)((a).size())

template <class T>
bool chmax(T &a, T b) {
  return (a < b) and ((a = std::move(b)), true);
}
template <class T>
bool chmin(T &a, T b) {
  return (a > b) and ((a = std::move(b)), true);
}

template <typename T>
using V = std::vector<T>;
template <typename T>
std::vector<T> make_vec(size_t n, T a) {
  return std::vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return std::vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
}
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

template <typename Monoid>
struct Doubling {
  using T = typename Monoid::T;
  static const int kMaxBits = 45;
  std::vector<std::vector<int>> next_pos;
  std::vector<std::vector<T>> acc_value;

  Doubling(int n)
      : next_pos(kMaxBits, std::vector<int>(n, -1)),
        acc_value(kMaxBits, std::vector<T>(n, Monoid::unity())) {}

  void set_next(int i, int x) { next_pos[0][i] = x; }

  void set_value(int i, T x) { acc_value[0][i] = x; }

  void build() {
    for (int d = 0; d + 1 < kMaxBits; d++) {
      for (size_t i = 0; i < next_pos[d].size(); i++) {
        if (int p = next_pos[d][i]; p != -1) {
          next_pos[d + 1][i] = next_pos[d][p];
          acc_value[d + 1][i] = Monoid::op(acc_value[d][i], acc_value[d][p]);
        }
      }
    }
  }

  // Folds values in [start, start + k).
  // Starting from `start`, accumulates values in `k` steps.
  std::pair<int, T> query(int start, const long long k) const {
    // Only k < 2^kBits is supported.
    assert(k < (1LL << kMaxBits));
    T res = Monoid::unity();
    int i = start;
    for (int d = kMaxBits - 1; d >= 0; d--) {
      if ((k >> d) & 1) {
        res = Monoid::op(res, acc_value[d][i]);
        i = next_pos[d][i];
      }
    }
    return {i, res};
  }
};

struct AddOp {
  using T = i64;
  static T unity() { return 0; }
  static T op(const T &x, const T &y) { return x + y; }
};

using namespace std;

int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);
  i64 n, x, m;
  cin >> n >> x >> m;
  Doubling<AddOp> doubling(m);
  REP(i, m) {
    int nxt = (i * (i64)i) % m;
    doubling.set_next(i, nxt);
    doubling.set_value(i, i);
  }
  doubling.build();
  cout << doubling.query(x, n).second << endl;
}

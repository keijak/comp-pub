#include <bits/stdc++.h>
using i64 = std::int64_t;
using u64 = std::uint64_t;
#define REP(i, n) for (int i = 0; i < (i64)(n); ++i)
#define ALL(x) std::begin(x), std::end(x)
#define SIZE(a) (int)((a).size())

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
struct DualSegTree {
  using T = typename Monoid::T;
  int size;
  std::vector<T> dat;

  DualSegTree(int n) : size(n), dat(2 * n, Monoid::unity()) {}

  void apply(int l, int r, T val) {
    l += size;
    r += size;
    for (; l < r; l >>= 1, r >>= 1) {
      if (l & 1) {
        dat[l] = Monoid::op(dat[l], val);
        ++l;
      }
      if (r & 1) {
        --r;
        dat[r] = Monoid::op(dat[r], val);
      }
    }
  }

  T operator[](int i) const {
    int k = size + i;
    T res = dat[k];
    while (k > 1) {
      k >>= 1;
      res = Monoid::op(res, dat[k]);
    }
    return res;
  }
};
template <typename T>
std::ostream &operator<<(std::ostream &os, const DualSegTree<T> &st) {
  os << "[";
  for (int i = 0; i < st.size; ++i) {
    if (i != 0) os << ", ";
    os << st[i];
  }
  return os << "]";
}

struct MinOp {
  using T = int;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static inline T unity() { return std::numeric_limits<T>::max(); }
};

using namespace std;

int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);
  int N, Q;
  cin >> N >> Q;
  DualSegTree<MinOp> sv(N), sh(N);
  i64 ans = (N - 2) * (i64)(N - 2);
  REP(i, Q) {
    int t, x;
    cin >> t >> x;
    x--;
    if (t == 1) {
      int r = min(sv[x], N - 1);
      ans -= r - 1;
      sh.apply(0, r, x);
    } else {
      assert(t == 2);
      int c = min(sh[x], N - 1);
      ans -= c - 1;
      sv.apply(0, c, x);
    }
  }
  cout << ans << endl;
}

#include <bits/stdc++.h>

#include <atcoder/lazysegtree>

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

struct S {
  int ones;
  int size;
  i64 inversion;
};
using F = bool;

S op(S a, S b) {
  return {
      a.ones + b.ones,
      a.size + b.size,
      a.inversion + b.inversion + i64(a.ones) * (b.size - b.ones),
  };
}
S e() { return {0, 0, 0}; }

S mapping(F f, S x) {
  if (not f) return x;
  i64 inv = i64(x.size) * (x.size - 1) / 2LL;
  inv -= i64(x.ones) * (x.ones - 1) / 2LL;
  i64 zeros = x.size - x.ones;
  inv -= zeros * (zeros - 1) / 2LL;
  inv -= x.inversion;
  return {x.size - x.ones, x.size, inv};
}
F composition(F f, F g) { return f ^ g; }
F id() { return false; }

int main() {
  cin.tie(nullptr);
  ios::sync_with_stdio(false);
  int N, Q;
  cin >> N >> Q;
  V<S> A(N);
  REP(i, N) {
    int a;
    cin >> a;
    if (a) {
      A[i] = {1, 1, 0};
    } else {
      A[i] = {0, 1, 0};
    }
  }
  atcoder::lazy_segtree<S, op, e, F, mapping, composition, id> seg(A);
  REP(i, Q) {
    int t, l, r;
    cin >> t >> l >> r;
    l--;
    if (t == 1) {
      seg.apply(l, r, true);
    } else {
      assert(t == 2);
      cout << seg.prod(l, r).inversion << '\n';
    }
  }
}

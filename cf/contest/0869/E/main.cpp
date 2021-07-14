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

template <typename T>
class BIT_2D_RangeAdd_RangeSum {
 private:
  const int n, m;
  vector<vector<T>> bitxy, bitx, bity, bitc;
  void add(const int i, const int j, const T valxy, const T valx, const T valy,
           const T valc) {
    for (int i_ = i + 1; i_ < n; i_ += i_ & -i_)
      for (int j_ = j + 1; j_ < m; j_ += j_ & -j_)
        bitxy[i_][j_] += valxy, bitx[i_][j_] += valx, bity[i_][j_] += valy,
            bitc[i_][j_] += valc;
  }
  // [0, i] x [0, j]
  T sum(const int i, const int j) {
    T s = 0;
    for (int i_ = i + 1; i_ > 0; i_ -= i_ & -i_)
      for (int j_ = j + 1; j_ > 0; j_ -= j_ & -j_)
        s += bitxy[i_][j_] * i * j + bitx[i_][j_] * i + bity[i_][j_] * j +
             bitc[i_][j_];
    return s;
  }

 public:
  BIT_2D_RangeAdd_RangeSum(const int sz1, const int sz2)
      : n(sz1 + 1),
        m(sz2 + 1),
        bitxy(n, vector<T>(m, 0)),
        bitx(n, vector<T>(m, 0)),
        bity(n, vector<T>(m, 0)),
        bitc(n, vector<T>(m, 0)) {}
  // [lx, rx)×[ly, ry) に val を足す
  void add(const int lx, const int ly, const int rx, const int ry,
           const T val) {
    add(lx, ly, val, -val * (ly - 1), -val * (lx - 1),
        val * (lx - 1) * (ly - 1));
    add(rx, ly, -val, val * (ly - 1), val * (rx - 1),
        -val * (rx - 1) * (ly - 1));
    add(lx, ry, -val, val * (ry - 1), val * (lx - 1),
        -val * (lx - 1) * (ry - 1));
    add(rx, ry, val, -val * (ry - 1), -val * (rx - 1),
        val * (rx - 1) * (ry - 1));
  }
  // [lx, rx)×[ly, ry) の和を求める
  T sum(const int lx, const int ly, const int rx, const int ry) {
    return sum(rx - 1, ry - 1) - sum(lx - 1, ry - 1) - sum(rx - 1, ly - 1) +
           sum(lx - 1, ly - 1);
  }
  void print() {
    for (int i = 0; i < n - 1; ++i) {
      for (int j = 0; j < m - 1; ++j) {
        cout << sum(i, j, i + 1, j + 1) << " ";
      }
      cout << endl;
    }
  }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int n, m, q;
  cin >> n >> m >> q;
  BIT_2D_RangeAdd_RangeSum<u64> bit(n + 1, m + 1);
  map<tuple<int, int, int, int>, u64> barrier_id;
  random_device seed_gen;
  mt19937_64 engine(seed_gen());
  uniform_int_distribution<u64> rand(1, numeric_limits<u64>::max());

  REP(i, q) {
    int t, r1, c1, r2, c2;
    cin >> t >> r1 >> c1 >> r2 >> c2;
    if (t == 1) {
      u64 bid = rand(engine);
      barrier_id[{r1, c1, r2, c2}] = bid;
      bit.add(r1, c1, r2 + 1, c2 + 1, bid);
    } else if (t == 2) {
      auto it = barrier_id.find({r1, c1, r2, c2});
      u64 bid = it->second;
      bit.add(r1, c1, r2 + 1, c2 + 1, -bid);
      barrier_id.erase(it);
    } else {
      int p1 = bit.sum(r1, c1, r1 + 1, c1 + 1);
      int p2 = bit.sum(r2, c2, r2 + 1, c2 + 1);
      if (p1 == p2) {
        cout << "Yes\n";
      } else {
        cout << "No\n";
      }
    }
  }
}

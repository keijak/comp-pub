#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
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

template <class SemiLattice>
struct SparseTable {
  using T = typename SemiLattice::T;

  explicit SparseTable(const std::vector<T> &vec) { init(vec); }

  // Queries by [l,r) range (0-indexed, half-open interval).
  T fold(int l, int r) const {
    l = std::max(l, 0);
    r = std::min(r, n_);
    if (l >= r) {
      return SemiLattice::id();
    }
    const T &lval = data_[height_[r - l]][l];
    const T &rval = data_[height_[r - l]][r - (1 << height_[r - l])];
    return SemiLattice::op(lval, rval);
  }

  // Returns i-th value (0-indexed).
  T operator[](int i) const {
    assert(0 <= i and i < n_);
    return data_[height_[1]][i];
  }

 private:
  void init(const std::vector<T> &vec) {
    int n = vec.size(), h = 0;
    n_ = n;
    while ((1 << h) <= n) ++h;
    data_.assign(h, std::vector<T>(1 << h, SemiLattice::id()));
    height_.assign(n + 1, 0);
    for (int i = 2; i <= n; i++) {
      height_[i] = height_[i >> 1] + 1;
    }
    for (int i = 0; i < n; ++i) {
      data_[0][i] = vec[i];
    }
    for (int i = 1; i < h; ++i)
      for (int j = 0; j < n; ++j)
        data_[i][j] = SemiLattice::op(
            data_[i - 1][j], data_[i - 1][std::min(j + (1 << (i - 1)), n - 1)]);
  }

  int n_;  // number of elements.
  std::vector<std::vector<T>> data_;
  std::vector<int> height_;
};

struct GCD {
  using T = int;
  static T op(const T &x, const T &y) {
    if (x == id()) return y;
    if (y == id()) return x;
    return std::gcd(x, y);
  }
  static constexpr T id() { return -1; }
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n, m;
  cin >> n >> m;
  vector<int> a(n);
  cin >> a;

  SparseTable<GCD> st(a);
  unordered_map<int, i64> freq;
  REP(i, n) {
    int j = i;
    int gj = a[i];
    const int g0 = st.fold(0, i + 1);
    while (j >= 0) {
      if (g0 == gj) {
        freq[gj] += j + 1;
        break;
      }
      int l = 0, r = j;
      while (r - l > 1) {
        int mid = (r + l) / 2;
        int g = st.fold(mid, i + 1);
        if (g != gj) {
          l = mid;
        } else {
          r = mid;
        }
      }
      freq[gj] += j + 1 - r;
      gj = st.fold(l, i + 1);
      j = l;
    }
  }

  REP(i, m) {
    int x;
    cin >> x;
    cout << freq[x] << "\n";
  }
}

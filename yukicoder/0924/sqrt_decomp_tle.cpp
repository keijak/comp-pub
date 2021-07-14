#include <bits/stdc++.h>

#include <atcoder/fenwicktree>

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
const int INF = 1e9 + 100;

template <typename T>
struct Compress {
  std::vector<T> vec;

  explicit Compress(std::vector<T> v) : vec(v) {
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
  }

  int size() const { return vec.size(); }

  int index(T x) const {
    return lower_bound(vec.begin(), vec.end(), x) - vec.begin();
  }

  const T &value(int i) const { return vec[i]; }
};

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);

  int n, q;
  cin >> n >> q;
  int bsize = 1000;  // sqrt(n) + 1;
  int bcount = n / bsize + 1;

  vector<int> a(n);
  cin >> a;
  Compress<int> comp(a);

  vector<atcoder::fenwick_tree<int>> bits;
  vector<atcoder::fenwick_tree<i64>> bits2;
  REP(bi, bcount) {
    bits.emplace_back(comp.size());
    bits2.emplace_back(comp.size());
    REP(j, bsize) {
      int p = bi * bsize + j;
      if (p < n) {
        int ai = comp.index(a[p]);
        bits[bi].add(ai, 1);
        bits2[bi].add(ai, a[p]);
      }
    }
  }

  auto check = [&](int mi, int l, int r) -> bool {
    int lb = l / bsize, rb = r / bsize;
    assert(lb < rb);
    i64 count = 0;
    for (int i = l; i < (lb + 1) * bsize; ++i) {
      if (comp.index(a[i]) < mi) ++count;
    }
    for (int i = rb * bsize; i < r; ++i) {
      if (comp.index(a[i]) < mi) ++count;
    }
    for (int bi = lb + 1; bi < rb; ++bi) {
      count += bits[bi].sum(0, mi);
    }
    return count <= (r - l) / 2;
  };

  REP(qi, q) {
    int l, r;
    cin >> l >> r;
    --l;

    int lb = l / bsize, rb = r / bsize;
    if (lb == rb) {
      vector<int> xs;
      xs.reserve(r - l);
      for (int i = l; i < r; ++i) {
        xs.push_back(a[i]);
      }
      int m = (r - l) / 2;
      nth_element(xs.begin(), xs.begin() + m, xs.end());
      int medval = xs.at(m);
      i64 dsum = 0;
      for (auto x : xs) {
        dsum += abs(x - medval);
      }
      cout << dsum << "\n";
    } else {
      int tv = 0, fv = comp.size();
      while (fv - tv > 1) {
        int mid = (tv + fv) / 2;
        if (check(mid, l, r)) {
          tv = mid;
        } else {
          fv = mid;
        }
      }
      assert(0 <= tv and tv < comp.size());
      int medi = tv;
      int medval = comp.value(medi);
      DEBUG(medi, medval);

      i64 dsum = 0;
      for (int i = l; i < (lb + 1) * bsize; ++i) {
        dsum += abs(a[i] - medval);
      }
      for (int i = rb * bsize; i < r; ++i) {
        dsum += abs(a[i] - medval);
      }
      for (int bi = lb + 1; bi < rb; ++bi) {
        i64 lower_cnt = bits[bi].sum(0, medi);
        i64 lower_sum = bits2[bi].sum(0, medi);
        dsum += medval * lower_cnt - lower_sum;

        i64 upper_cnt = bits[bi].sum(medi, comp.size());
        i64 upper_sum = bits2[bi].sum(medi, comp.size());
        dsum += upper_sum - medval * upper_cnt;
      }
      cout << dsum << '\n';
    }
  }
}

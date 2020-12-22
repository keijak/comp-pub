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

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  int n, q;
  cin >> n >> q;

  vector<int> a(n);
  REP(i, n) a[i] = i;

  int bsize = sqrt(n) + 1;
  int bn = n / bsize + 2;
  vector<atcoder::fenwick_tree<int>> segs;
  segs.reserve(bn);
  REP(i, bn) {
    segs.emplace_back(n);
    REP(j, bsize) {
      int x = i * bsize + j;
      if (x < n) {
        segs[i].add(x, 1);
      }
    }
  }

  i64 inversion = 0;
  REP(qi, q) {
    int l, r;
    cin >> l >> r;
    if (l == r) {
      cout << inversion << '\n';
      continue;
    }
    if (l > r) swap(l, r);
    --l, --r;

    int lb = l / bsize;
    int rb = r / bsize;

    // assert(a[l] < n);
    // assert(a[r] < n);
    // assert(segs[lb][a[l]] == 1);
    // assert(segs[rb][a[r]] == 1);

    i64 lt_l = 0, gt_l = 0, lt_r = 0, gt_r = 0;
    auto collect_count = [&](int start, int end) -> void {
      for (int i = start; i < end; ++i) {
        if (a[i] > a[l]) {
          ++gt_l;
        } else {
          ++lt_l;
        }
        if (a[i] > a[r]) {
          ++gt_r;
        } else {
          ++lt_r;
        }
      }
    };

    if (lb == rb) {
      collect_count(l + 1, r);
    } else {
      collect_count(l + 1, (lb + 1) * bsize);
      collect_count(rb * bsize, r);
      for (int i = lb + 1; i < rb; ++i) {
        int cnt = segs[i].sum(a[l] + 1, n);
        gt_l += cnt;
        lt_l += bsize - cnt;
        cnt = segs[i].sum(a[r] + 1, n);
        gt_r += cnt;
        lt_r += bsize - cnt;
      }
      segs[lb].add(a[l], -1);
      segs[lb].add(a[r], 1);
      segs[rb].add(a[r], -1);
      segs[rb].add(a[l], 1);
    }
    inversion += gt_l - lt_l - gt_r + lt_r;
    if (a[l] < a[r]) {
      ++inversion;
    } else {
      --inversion;
    }
    cout << inversion << '\n';
    swap(a[l], a[r]);
  }
}

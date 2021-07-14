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

  vector<int> a(n);
  cin >> a;
  Compress<int> comp(a);

  vector<pair<int, int>> queries(q);
  REP(qi, q) {
    int l, r;
    cin >> l >> r;
    --l;
    queries[qi] = {l, r};
  }

  vector<int> tv(q, 0), fv(q, comp.size());
  vector<vector<int>> left(n + 1), right(n + 1);
  vector<int> pc(q);
  for (int iter = 0;; ++iter) {
    REP(i, n + 1) {
      left[i].clear();
      right[i].clear();
    }
    bool done = true;
    REP(i, q) {
      if (fv[i] - tv[i] > 1) {
        auto [l, r] = queries[i];
        left[l].push_back(i);
        right[r].push_back(i);
        done = false;
      }
    }
    if (done) break;

    atcoder::fenwick_tree<int> bit_c(comp.size());
    fill(ALL(pc), 0);
    REP(i, n + 1) {
      for (int j : left[i]) {
        int mid = (tv[j] + fv[j]) / 2;
        pc[j] = bit_c.sum(0, mid);
      }
      for (int j : right[i]) {
        int mid = (tv[j] + fv[j]) / 2;
        i64 after_c = bit_c.sum(0, mid);
        int smaller_c = after_c - pc[j];
        auto [l, r] = queries[j];
        int length = r - l;
        if (smaller_c <= length / 2) {
          tv[j] = mid;
        } else {
          fv[j] = mid;
        }
      }
      if (i < n) {
        int aix = comp.index(a[i]);
        bit_c.add(aix, 1);
      }
    }
  }

  // Compute f.
  {
    vector<i64> ans(q);
    vector<i64> ps(q), pa(q);
    REP(i, n + 1) {
      left[i].clear();
      right[i].clear();
    }
    REP(i, q) {
      auto [l, r] = queries[i];
      left[l].push_back(i);
      right[r].push_back(i);
    }
    atcoder::fenwick_tree<int> bit_c(comp.size());
    atcoder::fenwick_tree<i64> bit_s(comp.size());
    fill(ALL(pc), 0);
    fill(ALL(ps), 0LL);
    fill(ALL(pa), 0LL);
    i64 all_sum = 0;
    REP(i, n + 1) {
      for (int j : left[i]) {
        int med = tv[j];
        pc[j] = bit_c.sum(0, med);
        ps[j] = bit_s.sum(0, med);
        pa[j] = all_sum;
      }
      for (int j : right[i]) {
        int med = tv[j];
        i64 after_c = bit_c.sum(0, med);
        i64 after_s = bit_s.sum(0, med);
        int smaller_c = after_c - pc[j];
        auto [l, r] = queries[j];
        i64 smaller_s = after_s - ps[j];
        i64 val = comp.value(med);
        ans[j] = smaller_c * val - smaller_s;
        ans[j] += (all_sum - pa[j] - smaller_s) - (r - l - smaller_c) * val;
      }
      if (i < n) {
        int aix = comp.index(a[i]);
        bit_c.add(aix, 1);
        bit_s.add(aix, a[i]);
        all_sum += a[i];
      }
    }
    REP(qi, q) { cout << ans[qi] << '\n'; }
  }
}

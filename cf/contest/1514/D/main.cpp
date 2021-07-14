#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;
using usize = unsigned;

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

class range_mode_query {
  class b_node {
   public:
    usize value;
    usize rank;

    b_node(const usize value, const usize rank) : value(value), rank(rank) {}
  };

  class s_node {
   public:
    usize mode;
    usize freq;

    s_node(const usize mode, const usize freq) : mode(mode), freq(freq) {}
  };

  std::vector<b_node> b;
  std::vector<std::vector<usize>> q;
  std::vector<std::vector<s_node>> s;
  usize t;

 public:
  range_mode_query() = default;

  range_mode_query(const std::vector<usize> a) : t(1) {
    const usize n = a.size();
    b.assign(n, b_node(0, 0));
    q.assign(n, {});
    while (t * t < n) t += 1;
    for (usize i = 0; i != n; i += 1) {
      const usize e = a[i];
      assert(e < n);
      b[i] = b_node(e, q[e].size());
      q[e].push_back(i);
    }
    s.assign(n / t + 1, std::vector<s_node>(n / t + 1, s_node(0, 0)));
    for (usize f = 0; f * t <= n; f += 1) {
      std::vector<usize> freq(n, 0);
      s_node cur(0, 0);
      for (usize l = f + 1; l * t <= n; l += 1) {
        for (usize i = (l - 1) * t; i != l * t; i += 1) {
          const usize e = a[i];
          freq[e] += 1;
          if (freq[e] > cur.freq) cur = s_node(e, freq[e]);
        }
        s[f][l] = cur;
      }
    }
  }

  usize query(const usize first, const usize last) const {
    assert(first < last);
    assert(last <= b.size());
    usize bf = first / t + 1;
    usize bl = last / t;
    if (bf >= bl) {
      s_node cur(0, 0);
      for (usize i = first; i != last; i += 1) {
        const b_node &e = b[i];
        const std::vector<usize> &v = q[e.value];
        usize idx;
        while ((idx = e.rank + cur.freq) < v.size() && v[idx] < last)
          cur = s_node(e.value, cur.freq + 1);
      }
      return cur.freq;
    }
    s_node cur = s[bf][bl];
    for (usize i = first; i != bf * t; i += 1) {
      const b_node &e = b[i];
      const std::vector<usize> &v = q[e.value];
      usize idx;
      while ((idx = e.rank + cur.freq) < v.size() && v[idx] < last)
        cur = s_node(e.value, cur.freq + 1);
    }
    for (usize i = bl * t; i != last; i += 1) {
      const b_node &e = b[i];
      const std::vector<usize> &v = q[e.value];
      usize idx;
      while ((idx = e.rank - cur.freq) < v.size() && v[idx] >= first)
        cur = s_node(e.value, cur.freq + 1);
    }
    return cur.freq;
  }
};

const int M = 300005;

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int n, q;
  cin >> n >> q;
  vector<usize> a(n);
  REP(i, n) {
    int x;
    cin >> x;
    a[i] = x - 1;
  }
  vector<vector<int>> idx(M);
  REP(i, n) {
    usize x = a[i];
    idx[x].push_back(i);
  }
  range_mode_query rmq(a);

  REP(qi, q) {
    usize l, r;
    cin >> l >> r;
    --l;
    auto mf = rmq.query(l, r);
    const usize width = r - l;
    if (2 * mf <= width + 1) {
      cout << 1 << "\n";
      continue;
    }
    int ans = 2 * mf - width;
    cout << ans << "\n";
  }
}

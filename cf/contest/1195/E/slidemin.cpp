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

#ifdef ENABLE_DEBUG
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

template <class T, class Container = std::vector<T>,
          class Compare = std::less<typename Container::value_type>>
struct SlideMinQueue {
  Container vals;
  Compare compare;              // comparison funcation.
  int left, right;              // [left, right)
  std::deque<int> index_queue;  // indices where min values are stored.

  SlideMinQueue() : compare(), left(0), right(0) {}

  explicit SlideMinQueue(Container v)
      : vals(std::move(v)), compare(), left(0), right(0) {}

  SlideMinQueue(const SlideMinQueue &) = default;
  SlideMinQueue &operator=(const SlideMinQueue &) = default;
  SlideMinQueue(SlideMinQueue &&) = default;
  SlideMinQueue &operator=(SlideMinQueue &&) = default;

  // Shifts the window to the right.
  // Sets `left` to `l`, and `right` to `r`.
  void slide(int l, int r) {
    assert(l >= left);
    assert(r >= right);
    pop_left(l);
    left = l;
    for (int i = right; i < r; ++i) {
      push_right(i);
    }
    right = r;
  }

  // Returns the minimum value in [left, right).
  std::optional<T> fold() const {
    if (empty()) return std::nullopt;
    return vals[index_queue.front()];
  }

  bool empty() const { return index_queue.empty(); }

 private:
  // Enqueues the i-th value.
  void push_right(int i) {
    while (!index_queue.empty() &&
           !compare(vals[index_queue.back()], vals[i])) {
      index_queue.pop_back();
    }
    index_queue.emplace_back(i);
  }

  // Dequeues indices less than i.
  void pop_left(int i) {
    while (!index_queue.empty() && index_queue.front() < i) {
      index_queue.pop_front();
    }
  }
};

const int BIG = 1e9 + 5000;

i64 solve() {
  i64 n, m, a, b, g0, x, y, z;
  cin >> n >> m >> a >> b;
  cin >> g0 >> x >> y >> z;
  vector h(n, vector(m, 0));
  h[0][0] = g0;
  {
    i64 g = g0;
    REP(i, 1, n * m) {
      g = (g * x + y) % z;
      int r = i / m;
      int c = i % m;
      h[r][c] = g;
    }
  }

  vector<SlideMinQueue<int>> vq;
  vq.reserve(m);
  REP(j, m) {
    vector<int> v(n);
    REP(i, n) { v[i] = h[i][j]; }
    vq.emplace_back(move(v));
  }
  i64 ans = 0;
  REP(i, n - a + 1) {
    vector<int> mins(m);
    REP(j, m) {
      vq[j].slide(i, i + a);
      auto res = vq[j].fold();
      // assert(res.has_value());
      mins[j] = *res;
    }
    SlideMinQueue<int> hq(move(mins));
    REP(j, m - b + 1) {
      hq.slide(j, j + b);
      auto res = hq.fold();
      // assert(res.has_value());
      ans += *res;
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}

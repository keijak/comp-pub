#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}
template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
using V = std::vector<T>;
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

template <typename Monoid>
struct FunctionalGraph {
  using T = typename Monoid::T;
  static const int kMaxBits = 61;

  // number of nodes.
  int size;

  // acc_value[d][i] := starting from i, what's the value accumulated after 2^d
  // steps.
  std::vector<std::vector<T>> acc_value;

  // next_pos[d][i] := starting from i, what's the position after 2^d steps.
  std::vector<std::vector<int>> next_pos;

  explicit FunctionalGraph(int n)
      : size(n),
        acc_value(kMaxBits, std::vector<T>(n, Monoid::id())),
        next_pos(kMaxBits, std::vector<int>(n, -1)) {}

  // Sets value `x` at node `i`.
  void set_value(int i, T x) { acc_value[0][i] = x; }

  // Sets next position of node `i`.
  void set_next(int i, int pos) { next_pos[0][i] = pos; }

  // Builds transition tables.
  void build() {
    for (int d = 0; d + 1 < kMaxBits; d++) {
      for (int i = 0; i < size; i++) {
        if (int p = next_pos[d][i]; p != -1) {
          next_pos[d + 1][i] = next_pos[d][p];
          acc_value[d + 1][i] = Monoid::op(acc_value[d][i], acc_value[d][p]);
        }
      }
    }
  }

  // Starting from `start`, `steps` times goes forward and accumulates values.
  pair<T, int> transition(int start, u64 steps) const {
    // steps >= 2^kMaxBits is not supported.
    assert(steps < (1ULL << kMaxBits));
    T res = Monoid::id();
    int i = start;
    for (int d = kMaxBits - 1; d >= 0; d--) {
      if ((steps >> d) & 1) {
        res = Monoid::op(res, acc_value[d][i]);
        i = next_pos[d][i];
      }
    }
    return {res, i};
  }
};

struct Sum {
  using T = long long;
  static T op(const T &x, const T &y) { return x + y; }
  static constexpr T id() { return 0; }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  i64 N, K;
  cin >> N >> K;
  V<int> A(N);
  cin >> A;

  FunctionalGraph<Sum> fg(N);
  map<int, int> next_index;
  REP(i, N) {  // set first index
    if (not next_index.count(A[i])) {
      next_index[A[i]] = i;
    }
  }
  for (int i = N - 1; i >= 0; --i) {
    int j = next_index[A[i]];
    int pos = (j + 1) % N;
    fg.set_next(i, pos);
    int val = j <= i ? 1 : 0;
    if (j == N - 1) ++val;
    fg.set_value(i, val);
    next_index[A[i]] = i;
  }
  fg.build();

  i64 fv = -1, tv = 2LL * K * N;
  i64 k_cnt = 0;
  int last_pos = 0;
  while (tv - fv > 1) {
    u64 mid = (tv + fv) / 2;
    auto [cnt, pos] = fg.transition(0, mid);
    if (cnt >= K - 1) {
      tv = mid;
      k_cnt = cnt;
      last_pos = pos;
    } else {
      fv = mid;
    }
  }
  assert(last_pos >= 0);
  assert(last_pos < N);
  assert(k_cnt == K - 1);

  vector<int> ans;
  set<int> present;
  for (int i = last_pos; i < N; ++i) {
    if (present.count(A[i])) {
      while (true) {
        int val = ans.back();
        ans.pop_back();
        present.erase(val);
        if (val == A[i]) break;
      }
    } else {
      ans.push_back(A[i]);
      present.insert(A[i]);
    }
  }
  pprint(ans);
}

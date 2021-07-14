#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}
template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
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

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
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
const int INF = 1e9 + 5;

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

i64 solve() {
  int n;
  cin >> n;
  i64 first_begin = -1, second_end = INF;
  i64 single_best = 0;
  vector<pair<int, int>> ranges(n);
  REP(i, n) {
    i64 l, r;
    cin >> l >> r;
    ranges[i] = {l, r};
    chmax(single_best, r - l + 1);
    chmax(first_begin, l);
    chmin(second_end, r);
  }
  DEBUG(single_best, first_begin, second_end);

  i64 second_begin = -1;
  MinHeap<pair<i64, int>> pq;
  REP(i, n) {
    auto [l, r] = ranges[i];
    if (r >= first_begin) {
      pq.emplace(r, i);
      DEBUG("first", i);
    } else {
      chmax(second_begin, l);
      DEBUG("second", i);
    }
  }

  i64 ans = single_best;
  while (not pq.empty()) {
    auto [r, ri] = pq.top();
    i64 a1 = r - first_begin + 1;
    DEBUG(first_begin, r, a1);
    if (second_begin > 0) {
      i64 a2 = second_end - second_begin + 1;
      chmax(ans, a1 + a2);
      DEBUG(second_begin, second_end, a2);
    }
    DEBUG(ans);
    while (not pq.empty() and pq.top().first == r) {
      int j = pq.top().second;
      i64 l = ranges[j].first;
      chmax(second_begin, l);
      pq.pop();
    }
  }

  // Case 2.
  if (first_begin <= second_end) {
    chmax(ans, single_best + second_end - first_begin + 1);
  }

  return ans;
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}

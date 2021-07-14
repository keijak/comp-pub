#include <bits/stdc++.h>
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

i64 solve() {
  int n;
  cin >> n;
  deque<tuple<int, bool, int>> segs;
  V<int> positions;
  positions.reserve(n * 2 + 5);

  REP(i, n) {
    int l, r;
    cin >> l >> r;
    segs.emplace_back(l, false, i);
    segs.emplace_back(r, true, i);
    positions.push_back(l);
    positions.push_back(r);
  }
  sort(ALL(segs));
  sort(ALL(positions));
  positions.erase(unique(ALL(positions)), positions.end());

  int base_count = 0;
  V<int> seg_count(n);
  set<int> active;
  for (auto x : positions) {
    // Add.
    while (not segs.empty()) {
      auto [pos, is_end, index] = segs.front();
      if (pos > x) break;
      if (pos == x and is_end) break;
      segs.pop_front();
      active.insert(index);
    }

    // Remove.
    bool removed = false;
    bool connected = active.size() > 1;
    while (not segs.empty()) {
      auto [pos, is_end, index] = segs.front();
      if (pos > x) break;
      segs.pop_front();
      active.erase(index);
      if (not connected) seg_count[index] -= 1;
      removed = true;
    }

    // Increment.
    if (removed) {
      if (active.empty()) {
        ++base_count;
      } else if (SIZE(active) == 1) {
        int k = *active.begin();
        ++seg_count[k];
      }
    }
  }

  assert(segs.empty());
  assert(active.empty());
  DEBUG(base_count);
  DEBUG(seg_count);
  return base_count + *max_element(ALL(seg_count));
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;
  while (t--) {
    cout << solve() << endl;
  }
}

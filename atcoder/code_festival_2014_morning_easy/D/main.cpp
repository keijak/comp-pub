#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), N_##i = (b); i < N_##i; ++i)
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

const int E_BEGIN = 1, E_PILLOW = 2, E_END = 3;

struct Event {
  int point;
  int typ;
  int other;
  int eid;
};
bool operator<(const Event &x, const Event &y) {
  if (x.point != y.point) return x.point < y.point;
  if (x.typ != y.typ) return x.typ < y.typ;
  if (x.other != y.other) return x.other < y.other;
  return x.eid < y.eid;
}
ostream &operator<<(ostream &os, const Event &e) {
  return os << "(point=" << e.point << ", typ=" << e.typ << ")";
}

i64 solve() {
  int n, m;
  cin >> n >> m;
  vector<Event> events;
  events.reserve(2 * n + m + 10);
  REP(i, n) {
    int x, y;
    cin >> x >> y;
    events.push_back({x, E_BEGIN, y, i});
    events.push_back({y, E_END, x, i});
  }
  REP(i, m) {
    int a;
    cin >> a;
    events.push_back({a, E_PILLOW, 0, i});
  }
  sort(ALL(events));

  const int ne = ssize(events);
  multiset<pair<int, int>> pending;
  vector<int> matched(n, -1);
  int match_count = 0;
  REP(i, ne) {
    const Event &e = events[i];
    if (e.typ == E_BEGIN) {
      pending.emplace(e.other, e.eid);
    } else if (e.typ == E_PILLOW) {
      auto it = pending.lower_bound({e.point, -1});
      if (it == pending.end()) {
        // cerr << "no person found for pillow " << e.eid << " : " << e.point
        //      << endl;
        continue;
      }
      matched[it->second] = e.eid;
      ++match_count;
      pending.erase(it);
    } else {
      auto it = pending.find({e.point, e.eid});
      if (matched[e.eid] == -1) {
        assert(it != pending.end());
        // cerr << "no pillow found for person " << e.other << ".." << e.point
        //      << endl;
        pending.erase(it);
      } else {
        assert(it == pending.end());
      }
    }
  }
  DEBUG(matched);
  return match_count;
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}

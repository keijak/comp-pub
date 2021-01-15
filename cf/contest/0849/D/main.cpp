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

struct Dancer {
  int x;
  int y;
  int id;
};
ostream &operator<<(ostream &os, const Dancer &d) {
  return os << "(" << d.id << ":" << d.x << "," << d.y << ")";
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  int n, w, h;
  cin >> n >> w >> h;
  map<i64, vector<Dancer>> hor, ver;
  REP(i, n) {
    int g, p, t;
    cin >> g >> p >> t;
    if (g == 1) {
      int x = p;
      int y = -t;
      ver[x + y].push_back(Dancer{x, y, i});
    } else {
      int y = p;
      int x = -t;
      hor[x + y].push_back(Dancer{x, y, i});
    }
  }
  for (auto &[k, v] : hor) {
    sort(ALL(v), [](const Dancer &a, const Dancer &b) { return a.y < b.y; });
  }
  for (auto &[k, v] : ver) {
    sort(ALL(v), [](const Dancer &a, const Dancer &b) { return a.x < b.x; });
  }
  DEBUG(hor);
  DEBUG(ver);

  for (auto &[k, dh] : hor) {
    auto it = ver.find(k);
    if (it == ver.end()) continue;
    auto &dv = it->second;
    int sv = ssize(dv);
    int sh = ssize(dh);
    vector<int> tmp;
    tmp.reserve(sv + sh);
    REP(i, sh) { tmp.push_back(dh[sh - 1 - i].id); }
    REP(i, sv) { tmp.push_back(dv[i].id); }
    REP(i, sv) { dv[i].id = tmp[i]; }
    REP(i, sh) { dh[i].id = tmp[sv + sh - 1 - i]; }

    // REP(i, ssize(dh)) {
    //   auto &di = dh[i];
    //   REP(j, ssize(dv)) {
    //     auto &dj = dv[j];
    //     swap(di.id, dj.id);
    //   }
    // }
  }
  DEBUG(hor);
  DEBUG(ver);
  vector<pair<int, int>> ans(n);
  for (const auto &[k, v] : hor) {
    for (const auto &d : v) {
      ans[d.id] = {w, d.y};
    }
  }
  for (const auto &[k, v] : ver) {
    for (const auto &d : v) {
      ans[d.id] = {d.x, h};
    }
  }
  REP(i, n) {
    auto [x, y] = ans[i];
    cout << x << " " << y << "\n";
  }
}

#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;
using u32 = unsigned;

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

bool solve() {
  auto start = std::chrono::system_clock::now();

  // std::random_device seed_gen;
  std::mt19937_64 engine((u64)(std::time(0)));

  // n <= 2^18 (=262,144)
  int n, m;
  cin >> n >> m;
  vector<u32> a(m);
  cin >> a;
  vector<bool> aset(n, false);
  for (auto x : a) aset[x] = true;

  set<u32> yet;
  REP(i, 1, n) { yet.insert(i); }
  vector<pair<int, int>> edges;
  vector<u32> expand;
  expand.push_back(0);
  vector<u32> to_erase;
  to_erase.reserve(n);
  const u32 tl_mask = 31;
  for (u32 iter = 0; not yet.empty(); ++iter) {
    if (not(iter & tl_mask)) {
      auto now = chrono::system_clock::now();
      auto dur = chrono::duration_cast<chrono::milliseconds>(now - start);
      if (dur.count() >= 1900) return false;
    }
    if (expand.empty()) return false;
    std::uniform_int_distribution<int> rand(0, ssize(expand) - 1);
    int k = rand(engine);
    u32 v = expand[k];
    DEBUG(expand);
    DEBUG(ssize(expand), k, v);
    if (int last = ssize(expand) - 1; k != last) {
      swap(expand[k], expand[last]);
    }
    expand.pop_back();

    to_erase.clear();
    for (auto u : yet) {
      if (not aset[u ^ v]) {
        edges.emplace_back(v, u);
        to_erase.push_back(u);
        expand.push_back(u);
      }
    }
    for (auto u : to_erase) {
      yet.erase(u);
    }
  }

  for (const auto &[u, v] : edges) {
    cout << u << " " << v << "\n";
  }
  return true;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  bool ok = solve();
  if (!ok) {
    cout << "-1\n";
  }
}

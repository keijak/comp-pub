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
  string s;
  cin >> s;
  int n = ssize(s);
  assert(n > 0);
  vector<pair<int, list<int>>> rl;
  REP(i, n) {
    int x = s[i] - '0';
    if (rl.empty() or rl.back().first != x) {
      rl.emplace_back(x, list<int>{i});
    } else {
      rl.back().second.push_back(i);
    }
  }
  if (rl[0].first == 1 or rl.back().first == 1) return false;
  vector<vector<int>> ans;
  while (not rl.empty()) {
    vector<int> z;
    REP(i, ssize(rl)) {
      int j = rl[i].second.front();
      rl[i].second.pop_front();
      z.push_back(j + 1);
    }
    ans.push_back(move(z));
    vector<pair<int, list<int>>> nx;
    REP(i, ssize(rl)) {
      if (rl[i].second.empty()) continue;
      if (nx.empty() or nx.back().first != rl[i].first) {
        nx.push_back(move(rl[i]));
      } else {
        auto &lis = nx.back().second;
        lis.splice(lis.end(), move(rl[i].second));
      }
    }
    swap(rl, nx);
    if (rl.empty()) break;
    if (rl.front().first == 1 or rl.back().first == 1) return false;
  }
  cout << ssize(ans) << "\n";
  for (auto &ss : ans) {
    sort(ALL(ss));
    cout << ssize(ss);
    for (int x : ss) {
      cout << ' ' << x;
    }
    cout << "\n";
  }
  return true;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  if (not solve()) {
    cout << -1 << "\n";
  }
}

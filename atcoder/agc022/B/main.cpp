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

bool verify(vector<int> &v) {
  int s = accumulate(ALL(v), 0);
  int g = v.front();
  for (auto x : v) {
    if (gcd(x, s) == 1) {
      DEBUG(x, s);
      return false;
    }
    g = gcd(g, x);
  }
  if (g != 1) return false;
  return true;
}

bool solve(int n, vector<int> &v, int s) {
  if (n == 0) {
    int g = v.front();
    for (auto x : v) {
      if (gcd(x, s - x) == 1) return false;
      g = gcd(g, x);
    }
    if (g != 1) return false;
    assert(verify(v));
    pprint(v);
    return true;
  }

  int start = 2;
  if (not v.empty()) {
    start = v.back();
  }
  for (int i = start + 1; i <= 30000; ++i) {
    v.push_back(i);
    if (solve(n - 1, v, s + i)) return true;
    v.pop_back();
  }
  return false;
}

const int VMAX = 30000;

void solve2(int n) {
  vector<int> ans;
  for (int i = 2; i <= VMAX; i += 2) {
    if (i % 6 == 0) continue;
    ans.push_back(i);
  }
  for (int i = 3; i <= VMAX; i += 3) {
    if (i % 6 == 0) continue;
    ans.push_back(i);
  }
  for (int i = 6; i <= VMAX; i += 6) {
    if ((int)ans.size() == n) break;
    ans.push_back(i);
  }
  pprint(ans);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;

  if (n <= 15) {
    vector<int> v;
    solve(n, v, 0);
    exit(0);
  }
  if (n > 15000) {
    solve2(n);
    exit(0);
  }

  deque<int> p6, p3, p2;
  for (int i = 6; i <= VMAX; i += 6) p6.push_back(i);
  for (int i = 3; i <= VMAX; i += 3) {
    if (i % 6 == 0) continue;
    p3.push_back(i);
  }
  for (int i = 2; i <= VMAX; i += 2) {
    p2.push_back(i);
  }

  V<int> ans;
  ans.reserve(n);

  while ((int)ans.size() < n) {
    if ((int)ans.size() + 5 > n) break;
    if ((int)p2.size() < 3) break;
    if ((int)p3.size() < 2) break;
    REP(i, 3) {
      int x = p2.front();
      if (x % 6 == 0) {
        assert(x == p6.front());
        p6.pop_front();
      }
      ans.push_back(x);
      p2.pop_front();
    }
    REP(i, 2) {
      ans.push_back(p3.front());
      p3.pop_front();
    }
  }
  while ((int)ans.size() < n) {
    if ((int)ans.size() + 3 > n) break;
    if ((int)p2.size() < 3) break;
    REP(i, 3) {
      int x = p2.front();
      if (x % 6 == 0) {
        assert(x == p6.front());
        p6.pop_front();
      }
      ans.push_back(x);
      p2.pop_front();
    }
  }
  while ((int)ans.size() < n) {
    assert(not p6.empty());
    ans.push_back(p6.back());
    p6.pop_back();
  }
  DEBUG(ans);
  assert(verify(ans));
  pprint(ans);
}

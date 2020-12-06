#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}
template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
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

const int L = 1 << 8;

i64 solve() {
  int n;
  cin >> n;
  vector<int> a(n);
  cin >> a;
  vector<int> count(8, 0);
  REP(i, n) {
    int d = a[i] - 1;
    count[d]++;
  }
  REP(i, 8) if (count[i] == 0) {
    int c = 0;
    REP(j, 8) {
      if (count[j] > 0) ++c;
    }
    return c;
  }

  int max_lb = 0;
  vector<vector<int>> max_perms;
  vector<int> perm = {1, 2, 3, 4, 5, 6, 7, 8};
  auto check = [&](int x) -> bool {
    int j = 0;
    for (int i = 0; i < 8; ++i) {
      int cur = perm[i];
      int cnt = 0;
      while (j < n) {
        if (a[j++] == cur) {
          if (++cnt == x) {
            goto next_card;
          }
        }
      }
      if (j == n) return false;
    next_card:
      continue;
    }
    return true;
  };
  do {
    int tv = 0, fv = n;
    while (fv - tv > 1) {
      int mid = (fv + tv) / 2;
      if (check(mid)) {
        tv = mid;
      } else {
        fv = mid;
      }
    }
    if (max_lb < tv) {
      max_lb = tv;
      max_perms.clear();
      max_perms.push_back(perm);
    } else if (max_lb == tv) {
      max_perms.push_back(perm);
    };
  } while (next_permutation(ALL(perm)));

  auto check2 = [&](const vector<int> &p, int s) -> bool {
    int j = 0;
    for (int i = 0; i < 8; ++i) {
      int cur = p[i];
      int x = max_lb;
      if (s & (1 << i)) ++x;

      int cnt = 0;
      while (j < n) {
        if (a[j++] == cur) {
          if (++cnt == x) {
            goto next_card2;
          }
        }
      }
      if (j == n) return false;
    next_card2:
      continue;
    }
    return true;
  };
  int ans = max_lb * 8;
  for (const auto &p : max_perms) {
    REP(s, 1 << 8) {
      int pcount = __builtin_popcount(s);
      int x = max_lb * 8 + pcount;
      if (x > ans and check2(p, s)) {
        chmax(ans, x);
      }
    }
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}

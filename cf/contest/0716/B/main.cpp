#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

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

optional<string> solve() {
  string s;
  cin >> s;
  int n = ssize(s);
  if (n < 26) return nullopt;
  V<int> count(26);
  int wild = 0;
  REP(i, 26) {
    if (s[i] == '?') {
      ++wild;
    } else {
      int a = s[i] - 'A';
      ++count[a];
    }
  }
  auto check = [&]() -> bool {
    int unused = 0;
    REP(i, 26) {
      if (count[i] == 0) ++unused;
    }
    return wild >= unused;
  };
  auto make_answer = [&](int start) -> string {
    REP(i, 26) {
      int j = start + i;
      if (s[j] == '?') {
        REP(k, 26) {
          if (count[k] == 0) {
            s[j] = 'A' + k;
            ++count[k];
            break;
          }
        }
      }
    }
    REP(i, n) {
      if (s[i] == '?') s[i] = 'Z';
    }
    return s;
  };
  if (check()) {
    return make_answer(0);
  }
  for (int i = 26; i < n; ++i) {
    if (s[i] == '?') {
      ++wild;
    } else {
      int a = s[i] - 'A';
      ++count[a];
    }

    int j = i - 26;
    if (s[j] == '?') {
      --wild;
    } else {
      int a = s[j] - 'A';
      --count[a];
    }

    if (check()) return make_answer(j + 1);
  }
  return nullopt;
}
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << solve().value_or("-1"s) << endl;
}

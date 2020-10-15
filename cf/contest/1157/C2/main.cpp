#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  deque<int> a;
  REP(i, n) {
    int x;
    cin >> x;
    a.push_back(x);
  }
  V<int> vals;
  V<char> ans;
  while (a.size()) {
    bool ok = false;
    int x, y;
    char cx, cy;

    if (a.size() == 1) {
      x = a.front();
      if (vals.empty() or x > vals.back()) {
        ans.push_back('L');
        vals.push_back(x);
        a.pop_front();
        break;
      }
    }

    if (a.front() == a.back()) {
      if (not vals.empty() and a.front() <= vals.back()) {
        break;
      }
      int i = 1, k = SIZE(a);
      while (i < k and a[i] > a[i - 1]) ++i;
      int j = 1;
      while (j < k and a[k - j - 1] > a[k - j]) ++j;

      if (i > j) {
        REP(t, i) ans.push_back('L');
      } else {
        REP(t, j) ans.push_back('R');
      }
      break;
    }

    if (a.front() <= a.back()) {
      x = a.front();
      y = a.back();
      cx = 'L';
      cy = 'R';
    } else {
      x = a.back();
      y = a.front();
      cx = 'R';
      cy = 'L';
    }

    if (vals.empty() or x > vals.back()) {
      ans.push_back(cx);
      vals.push_back(x);
      if (cx == 'L')
        a.pop_front();
      else
        a.pop_back();
      ok = true;
    } else if (vals.empty() or y > vals.back()) {
      ans.push_back(cy);
      vals.push_back(y);
      if (cy == 'L')
        a.pop_front();
      else
        a.pop_back();
      ok = true;
    }
    if (not ok) break;
  }
  cout << SIZE(ans) << '\n';
  for (char c : ans) {
    cout << c;
  }
  cout << '\n';
}
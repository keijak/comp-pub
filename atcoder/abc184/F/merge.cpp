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

int solve() {
  int N, T;
  cin >> N >> T;
  V<int> A;
  A.reserve(N);
  REP(i, N) {
    int x;
    cin >> x;
    if (x <= T) A.push_back(x);
  }
  N = ssize(A);
  sort(ALL(A));

  int M1 = N / 2;
  int M2 = N - M1;

  V<int> st1, st2;
  {
    st1.reserve((1 << M1) + 5);
    st1.push_back(0);
    REP(i, M1) {
      int sz = ssize(st1), ai = A[i];
      REP(j, sz) {
        int y = st1[j] + ai;
        if (y <= T) st1.push_back(y);
      }
      std::inplace_merge(st1.begin(), st1.begin() + sz, st1.end());
    }
  }
  {
    st2.reserve((1 << M2) + 5);
    st2.push_back(0);
    REP(i, M2) {
      int sz = ssize(st2), ai = A[M1 + i];
      REP(j, sz) {
        int y = st2[j] + ai;
        if (y <= T) st2.push_back(y);
      }
      std::inplace_merge(st2.begin(), st2.begin() + sz, st2.end());
    }
    reverse(ALL(st2));
  }

  int ans = 0, j = 0, sz2 = ssize(st2);
  for (int i = 0, sz1 = ssize(st1); i < sz1; ++i) {
    int ub = T - st1[i];
    while (j < sz2 and st2[j] > ub) ++j;
    if (j == sz2) break;
    chmax(ans, st2[j] + st1[i]);
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << solve() << '\n';
}

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
inline i64 ssize(const T &a) {
  return (i64)std::size(a);
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

const string S0 =
    "What are you doing at the end of the world? Are you busy? Will you save "
    "us?";
const string P0 = "What are you doing while sending \"";
const string P1 = "\"? Are you busy? Will you send \"";
const string P2 = "\"?";

using Int = __int128_t;
vector<Int> flen_memo(100005, -1);

Int flen(int n) {
  if (n == 0) {
    return ssize(S0);
  }
  if (n >= 64) {
    return 1e19;
  }
  if (flen_memo[n] == -1) {
    flen_memo[n] = flen(n - 1) * 2LL + (ssize(P0) + ssize(P1) + ssize(P2));
  }
  return flen_memo[n];
}

char f(int n, Int k) {
  if (n == 0) {
    if (k >= ssize(S0)) return '.';
    return S0.at(k);
  }
  if (k < ssize(P0)) return P0.at(k);
  k -= ssize(P0);
  if (k < flen(n - 1)) {
    return f(n - 1, k);
  }
  k -= flen(n - 1);
  if (k < ssize(P1)) return P1.at(k);
  k -= ssize(P1);
  if (k < flen(n - 1)) {
    return f(n - 1, k);
  }
  k -= flen(n - 1);
  if (k < ssize(P2)) return P2.at(k);
  return '.';
}

string solve() {
  int q;
  cin >> q;
  string ans(q, '.');
  REP(i, q) {
    i64 n, k;
    cin >> n >> k;
    ans[i] = f(n, k - 1);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}

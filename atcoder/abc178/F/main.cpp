#include <bits/stdc++.h>
using i64 = std::int64_t;
using u64 = std::uint64_t;
#define REP(i, n) for (int i = 0; i < (i64)(n); ++i)
#define WHOLE(x) std::begin(x), std::end(x)
#define LEN(a) int((a).size())

template <class T>
bool chmax(T &a, T b) {
  return a >= b ? false : (a = std::move(b)), true;
}
template <class T>
bool chmin(T &a, T b) {
  return a <= b ? false : (a = std::move(b)), true;
}

template <typename T>
using V = std::vector<T>;
template <typename T>
std::vector<T> make_vec(size_t n, T a) {
  return std::vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return std::vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
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

#ifdef MY_DEBUG
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

  int N;
  cin >> N;
  V<int> A(N), B(N);
  cin >> A >> B;

  V<tuple<int, bool, int>> combined;
  combined.reserve(2 * N);
  REP(i, N) { combined.emplace_back(A[i], false, i); }
  REP(i, N) { combined.emplace_back(B[i], true, i); }
  sort(WHOLE(combined));

  V<int> pa, pb;
  REP(i, N) {
    auto [numx, abx, ix] = combined[i];
    auto [numy, aby, iy] = combined[N + i];
    if (numx == numy) {
      cout << "No\n";
      exit(0);
    }
    if (abx != aby) continue;
    (abx ? pb : pa).push_back(i);
  }
  assert(LEN(pa) == LEN(pb));
  REP(i, LEN(pa)) {
    auto [numa1, _a1, ia1] = combined[pa[i]];
    auto [numa2, _a2, ia2] = combined[N + pa[i]];
    auto [numb1, _b1, ib1] = combined[pb[i]];
    auto [numb2, _b2, ib2] = combined[N + pb[i]];
    if (numa1 == numb1 or numa2 == numb2) {
      swap(combined[pa[i]], combined[pb[i]]);
    } else {
      int k1 = min(pa[i], pb[i]);
      int k2 = max(pa[i], pb[i]);
      swap(combined[k1], combined[N + k2]);
    }
  }

  V<int> ans(N, -1);
  REP(i, N) {
    auto [numx, abx, ix] = combined[i];
    auto [numy, aby, iy] = combined[N + i];
    assert(numx != numy);
    assert(abx != aby);
    if (abx) {
      // Ensure x=>a, y=>b
      swap(abx, aby);
      swap(numx, numy);
      swap(ix, iy);
    }
    ans[ix] = numy;
  }

  cout << "Yes\n";
  pprint(ans);
  DEBUG(ans);
}

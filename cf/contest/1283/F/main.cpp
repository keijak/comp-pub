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

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  V<int> A(n - 1);
  cin >> A;
  const int root = A[0];

  set<int> leaves;
  for (int i = 1; i <= n; ++i) {
    leaves.insert(i);
  }
  V<int> child_count(n + 1);
  for (auto x : A) {
    leaves.erase(x);
    child_count[x]++;
  }

  MinHeap<tuple<int, int>> wires;
  for (auto l : leaves) {
    wires.emplace(l, l);
  }

  V<int> maxval(n + 1);
  REP(i, n + 1) { maxval[i] = i; }
  V<tuple<int, int>> edges;
  edges.reserve(n - 1);

  REP(i, n - 1) {
    int ri = n - 2 - i;
    if (wires.empty()) {
      cout << -1 << endl;
      exit(0);
    }
    auto [mx, aux] = wires.top();
    wires.pop();
    int main = A[ri];
    chmax(maxval[main], mx);
    edges.emplace_back(aux, main);
    child_count[main]--;
    if (child_count[main] == 0) {
      wires.emplace(maxval[main], main);
    }
  }
  cout << root << '\n';
  REP(i, n - 1) {
    auto [aux, main] = edges[n - 2 - i];
    cout << aux << ' ' << main << '\n';
  }
}

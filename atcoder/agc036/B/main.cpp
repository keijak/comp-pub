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
template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  i64 N, K;
  cin >> N >> K;
  vector<int> A(N);
  cin >> A;
  map<int, int> next_index;
  REP(i, N) {  // set first index
    if (not next_index.count(A[i])) {
      next_index[A[i]] = i;
    }
  }
  vector<int> next_pos(N, -1);
  for (int i = N - 1; i >= 0; --i) {
    int j = next_index[A[i]];
    next_pos[i] = j;
    next_index[A[i]] = i;
  }
  DEBUG(next_pos);

  vector<i64> seen(N, -1);
  i64 cur = 0, round = 0;
  bool cycle_done = false;
  while (round < K - 1) {
    auto last_round = seen[cur];
    if (last_round != -1 and not cycle_done) {
      i64 round_delta = round - last_round;
      assert(round_delta > 0);
      i64 x = (K - 1 - round) / round_delta;
      round += round_delta * x;
      cycle_done = true;
    } else {
      if (not cycle_done) seen[cur] = round;
      int np = next_pos[cur];
      if (np <= cur) ++round;
      if (np == N - 1) ++round;
      cur = (np + 1) % N;
    }
  }
  if (round == K) {
    assert(cur == 0);
    cout << endl;
    return 0;
  }
  assert(round == K - 1);

  vector<int> ans;
  set<int> present;
  for (int i = cur; i < N; ++i) {
    if (present.count(A[i])) {
      while (true) {
        int val = ans.back();
        ans.pop_back();
        present.erase(val);
        if (val == A[i]) break;
      }
    } else {
      ans.push_back(A[i]);
      present.insert(A[i]);
    }
  }
  pprint(ans);
}

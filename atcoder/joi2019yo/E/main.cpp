#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)
#define SZ(a) int((a).size())

template <class T>
void chmax(T &a, T b) {
  if (a < b) a = std::move(b);
}
template <class T>
void chmin(T &a, T b) {
  if (a > b) a = std::move(b);
}

template <typename T>
using V = std::vector<T>;
template <typename T>
vector<T> make_vec(size_t n, T a) {
  return vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
}
template <typename Container>
ostream &pprint(const Container &a, string_view sep = " ",
                string_view ends = "\n", ostream *os = nullptr) {
  if (os == nullptr) os = &cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
}
template <typename T>
ostream &operator<<(ostream &os, const vector<T> &a) {
  return pprint(a, ", ", "", &(os << "[")) << "]";
}
template <typename T>
ostream &operator<<(ostream &os, const set<T> &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const map<T, U> &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef ENABLE_DEBUG
template <typename T>
void debug(T value) {
  cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                              \
  do {                                          \
    cerr << " \033[33m (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ":\033[0m ";        \
    debug(__VA_ARGS__);                         \
    cerr << endl;                               \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

template <class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M;
  cin >> N >> M;
  V<i64> A(N);
  REP(i, N) cin >> A[i];
  V<tuple<int, int>> ranges(M);
  REP(i, M) {
    int l, r;
    cin >> l >> r;
    --l;
    ranges[i] = {l, r};
  }
  sort(ranges.begin(), ranges.end());

  MinHeap<tuple<int, int>> heap;
  V<i64> dp(N + 1);
  int ri = 0;
  REP(i, N) {
    while (ri < M && get<0>(ranges[ri]) <= i) {
      auto [l, r] = ranges[ri++];
      heap.emplace(l, r);
    }

    while (!heap.empty() && get<1>(heap.top()) <= i) {
      heap.pop();
    }

    if (heap.empty()) {
      dp[i + 1] = dp[i] + A[i];
    } else {
      auto [hl, hr] = heap.top();
      chmax(dp[i + 1], dp[hl] + A[i]);
      chmax(dp[i + 1], dp[i]);
    }
  }
  cout << dp[N] << endl;
}

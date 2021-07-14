#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

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
template <typename T>
ostream &operator<<(ostream &os, const vector<T> &v) {
  os << '[';
  for (auto &&e : v) os << e << ' ';
  return os << ']';
}
template <typename T>
ostream &operator<<(ostream &os, const set<T> &v) {
  os << '[';
  for (auto &&e : v) os << e << ' ';
  return os << ']';
}

#ifdef MY_DEBUG
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
  int N, K;
  cin >> N >> K;
  V<tuple<i64, int>> sushi(N);
  REP(i, N) {
    int t, d;
    cin >> t >> d;
    --t;
    sushi[i] = {(i64)d, t};
  }
  sort(sushi.rbegin(), sushi.rend());

  MinHeap<i64> heap;
  V<optional<i64>> maxp(N);
  i64 score = 0;
  i64 count = 0;
  REP(i, K) {
    auto [d, t] = sushi[i];
    score += d;
    if (not maxp[t].has_value()) {
      maxp[t] = d;
      ++count;
    } else {
      heap.push(d);
    }
  }
  i64 ans = score + count * count;

  for (int i = K; i < N; ++i) {
    auto [d, t] = sushi[i];
    if (heap.empty()) break;
    if (maxp[t].has_value()) continue;
    score += d;
    maxp[t] = d;
    ++count;

    auto v = heap.top();
    heap.pop();
    score -= v;

    ans = max(ans, score + count * count);
  }

  cout << ans << endl;
}

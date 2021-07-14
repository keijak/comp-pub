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

#define chmin(x, y) x = min(x, y)

const int INF = 1e9;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, K;
  cin >> N >> K;
  string chars = "JOI";
  string S;
  cin >> S;
  assert((int)S.size() == N);
  array<deque<int>, 3> qs;
  auto skips = make_vec(3, N, -1);
  V<int> fwd1(N, -1), fwd2(N, -1);

  for (int i = N - 1; i >= 0; --i) {
    REP(j, 3) {
      if (S[i] == chars[j]) {
        qs[j].push_front(i);
        while ((int)qs[j].size() > K) qs[j].pop_back();
      }
    }
    REP(j, 3) {
      assert(qs[j].size() <= K);
      if (S[i] != chars[j]) continue;
      if (qs[j].size() == K) {
        skips[j][i] = qs[j].back();
      }
    }

    if (S[i] == 'J' && !qs[1].empty()) {
      fwd1[i] = qs[1][0];
    } else if (S[i] == 'O' && !qs[2].empty()) {
      fwd2[i] = qs[2][0];
    }
  }

  auto calc = [&](int start) -> int {
    int i = start;
    if (S[i] != 'J') return -1;
    i = skips[0][i];
    if (i < 0) return -1;
    i = fwd1[i];
    if (i < 0) return -1;
    i = skips[1][i];
    if (i < 0) return -1;
    i = fwd2[i];
    if (i < 0) return -1;
    i = skips[2][i];
    if (i < 0) return -1;
    return i + 1;
  };

  int ans = INF;
  REP(i, N) {
    int tail = calc(i);
    if (tail >= i + 3 * K) {
      chmin(ans, tail - i - 3 * K);
    }
  }

  if (ans == INF) {
    cout << -1 << endl;
  } else {
    cout << ans << endl;
  }
}

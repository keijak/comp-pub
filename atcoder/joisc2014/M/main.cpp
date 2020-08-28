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

#define chmax(x, y) x = max(x, y)

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  V<int> A(N), B(N);
  V<int> leaves;
  i64 extender = 0;
  V<pair<int, int>> neg;
  i64 happiness = 0;
  int legs = 1;
  REP(i, N) {
    cin >> A[i] >> B[i];
    if (A[i] == 0 && B[i] > 0) {
      leaves.push_back(B[i]);
    } else if (A[i] == 1 && B[i] > 0) {
      extender += B[i];
    } else if (A[i] > 1 && B[i] >= 0) {
      happiness += B[i];
      legs += A[i] - 1;
    } else if (A[i] > 1) {
      neg.emplace_back(A[i], B[i]);
    }
  }

  sort(leaves.rbegin(), leaves.rend());
  if (leaves.empty()) {
    if (extender > 0) leaves.push_back(extender);
  } else {
    leaves[0] += extender;
  }
  int leafp = 0;
  REP(i, legs) {
    if (leafp < (int)leaves.size()) {
      happiness += leaves[leafp++];
    }
  }
  DEBUG("baseline", happiness);

  sort(neg.begin(), neg.end());
  DEBUG(neg);
  DEBUG(leaves);

  const int M = leaves.size() - leafp;
  V<i64> cum(M + 1);
  for (int i = 1; i <= M; ++i) {
    cum[i] = cum[i - 1] + leaves[leafp + i - 1];
  }

  const i64 INF = 1e18;
  const int K = neg.size();
  auto dp = make_vec(M + 1, -INF);
  dp[0] = 0;
  for (int i = 1; i <= K; ++i) {
    auto dp_next = dp;
    auto [a, b] = neg[i - 1];
    for (int j = 0; j <= M; ++j) {
      int x = min(j + (a - 1), M);
      if (x <= M && dp[j] != -INF) {
        chmax(dp_next[x], dp[j] + b);
      }
    }
    swap(dp_next, dp);
  }

  i64 ans = happiness;
  REP(i, M + 1) {
    if (dp[i] != -INF) {
      chmax(ans, happiness + dp[i] + cum[i]);
    }
  }

  cout << ans << endl;
}

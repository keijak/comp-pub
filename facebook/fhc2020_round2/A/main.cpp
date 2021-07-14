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

i64 solve() {
  // input
  int N, K;  // < 1e6
  cin >> N >> K;
  V<i64> S(N), X(N), Y(N);
  i64 AS, BS, CS, DS, AX, BX, CX, DX, AY, BY, CY, DY;
  REP(i, K) { cin >> S[i]; }
  cin >> AS >> BS >> CS >> DS;
  REP(i, K) { cin >> X[i]; }
  cin >> AX >> BX >> CX >> DX;
  REP(i, K) { cin >> Y[i]; }
  cin >> AY >> BY >> CY >> DY;

  for (int i = K; i < N; ++i) {
    S[i] = ((AS * S[i - 2]) % DS + (BS * S[i - 1]) % DS + CS) % DS;
    X[i] = ((AX * X[i - 2]) % DX + (BX * X[i - 1]) % DX + CX) % DX;
    Y[i] = ((AY * Y[i - 2]) % DY + (BY * Y[i - 1]) % DY + CY) % DY;
  }

  i64 over = 0;
  i64 under = 0;
  REP(i, N) {
    if (S[i] > X[i] + Y[i]) {
      over += S[i] - (X[i] + Y[i]);
    }
    if (S[i] < X[i]) {
      under += X[i] - S[i];
    }
  }
  if (over == under) {
    return over;
  }
  if (over > under) {
    i64 moves = over;
    over -= under;
    REP(i, N) {
      if (S[i] < X[i]) {
        over -= Y[i];
      } else if (S[i] < Y[i]) {
        over -= Y[i] - S[i];
      }
      if (over <= 0) return moves;
    }
  } else {
    i64 moves = under;
    under -= over;
    REP(i, N) {
      if (S[i] >= X[i] + Y[i]) {
        under -= Y[i];
      } else if (S[i] >= X[i]) {
        under -= S[i] - X[i];
      }
      if (under <= 0) return moves;
    }
  }

  return -1;  // impossible
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T;
  cin >> T;
  REP(i, T) { cout << "Case #" << (i + 1) << ": " << solve() << '\n'; }
}

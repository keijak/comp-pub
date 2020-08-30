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

vector<int> min_factors(int n) {
  vector<int> min_factor(n + 1);
  REP(i, n + 1) { min_factor[i] = i; }
  for (int i = 4; i <= n; i += 2) min_factor[i] = 2;
  for (int i = 3; i * i <= n; i += 2) {
    if (min_factor[i] != i) continue;
    for (int j = 2 * i; j <= n; j += i) min_factor[j] = i;
  }
  return min_factor;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  V<int> A(N);
  REP(i, N) cin >> A[i];

  int g = A[0];
  for (int i = 1; i < N; ++i) {
    g = std::gcd(g, A[i]);
  }
  if (g != 1) {
    cout << "not coprime" << endl;
    return 0;
  }
  auto min_factor = min_factors(1'000'005);
  auto factorize = [&](int x) -> set<int> {
    set<int> res;
    for (; x > 1;) {
      int f = min_factor[x];
      res.insert(f);
      if (f == x) {
        break;
      }
      while (x % f == 0) {
        x /= f;
      }
    }
    return res;
  };
  bool ok = [&]() -> bool {
    unordered_set<int> primes;
    REP(i, N) {
      auto fs = factorize(A[i]);
      for (auto p : fs) {
        if (primes.count(p)) return false;
        primes.insert(p);
      }
    }
    return true;
  }();

  if (ok) {
    cout << "pairwise coprime" << endl;
  } else {
    cout << "setwise coprime" << endl;
  }
}

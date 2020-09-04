#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; i < (i64)(n); ++i)
#define WHOLE(x) std::begin(x), std::end(x)
#define LEN(a) int((a).size())

template <class T>
bool chmax(T &a, T b) {
  if (a < b) return (a = move(b)), true;
  return false;
}
template <class T>
bool chmin(T &a, T b) {
  if (a > b) return (a = move(b)), true;
  return false;
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
template <typename T>
istream &operator>>(istream &is, vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, K;
  cin >> N >> K;
  V<u64> A(N);
  cin >> A;
  V<u64> cum(N + 1);
  REP(i, N) { cum[i + 1] = cum[i] + A[i]; }
  const int L = 64;
  V<int> bit_count(L);
  V<u64> B;
  for (int i = 0; i < N; ++i) {
    for (int j = i + 1; j <= N; ++j) {
      u64 b = cum[j] - cum[i];
      REP(k, L) {
        if (b >> k & 1ULL) {
          bit_count[k]++;
        }
      }
      B.emplace_back(b);
    }
  }
  assert(LEN(B) == N * (N + 1) / 2);

  int k = L;
  u64 ans = 0;
  V<u64> B_next;
  B_next.reserve(B.size());
  for (int i = 0; not B.empty(); ++i) {
    do {
      --k;
    } while (k >= 0 and bit_count[k] < K);
    if (k < 0) break;
    assert(bit_count[k] >= K);
    ans |= 1ULL << k;

    B_next.clear();
    for (auto bits : B) {
      if (bits >> k & 1ULL) {
        B_next.emplace_back(bits);
        continue;
      }
      for (int j = L - 1; j >= 0; --j) {
        if (bits >> j & 1ULL) {
          bit_count[j]--;
        }
      }
    }
    swap(B, B_next);
  }
  cout << ans << endl;
}

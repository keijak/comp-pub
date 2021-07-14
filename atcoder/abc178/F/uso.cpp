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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  V<int> A(N), B(N);
  cin >> A >> B;
  V<int> count(N + 1);
  V<int> acount(N + 1), bcount(N + 1);
  REP(i, N) {
    count[A[i]]++;
    count[B[i]]++;
    acount[A[i]]++;
    bcount[B[i]]++;
  }
  REP(i, N + 1) {
    if (count[i] > N) {
      cout << "No\n";
      return 0;
    }
  }
  V<tuple<int, int, int>> sa(N), sb(N);
  REP(i, N) {
    sa[i] = {acount[A[i]], A[i], i};
    sb[i] = {acount[B[i]], B[i], i};
  }
  sort(sa.rbegin(), sa.rend());
  sort(sb.rbegin(), sb.rend());

  int am = get<1>(sa[0]);  // A's most frequent.
  V<int> aval(N);
  REP(i, N) { aval[i] = get<1>(sa[i]); }
  DEBUG(aval);

  int cur = 0;
  int rev = N - 1;
  V<int> bval(N);
  REP(i, N) {
    auto [ac, b, j] = sb[i];
    if (b == am) {
      bval[rev--] = b;
      continue;
    }
    bval[cur++] = b;
  }
  DEBUG(bval);
  V<int> ans(N);
  REP(i, N) {
    auto [ac, a, j] = sa[i];
    ans[j] = bval[i];
  }
  cout << "Yes\n";
  pprint(ans);
}

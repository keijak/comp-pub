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

template <class SemiLattice>
struct SparseTable {
  vector<vector<SemiLattice> > dat;
  vector<int> height;

  SparseTable() {}
  SparseTable(const vector<SemiLattice> &vec) { init(vec); }
  void init(const vector<SemiLattice> &vec) {
    int n = LEN(vec), h = 0;
    while ((1 << h) < n) ++h;
    dat.assign(h, vector<SemiLattice>(1 << h));
    height.assign(n + 1, 0);
    for (int i = 2; i <= n; i++) height[i] = height[i >> 1] + 1;
    for (int i = 0; i < n; ++i) dat[0][i] = vec[i];
    for (int i = 1; i < h; ++i)
      for (int j = 0; j < n; ++j)
        dat[i][j] =
            min(dat[i - 1][j], dat[i - 1][min(j + (1 << (i - 1)), n - 1)]);
  }

  SemiLattice get(int a, int b) {
    return min(dat[height[b - a]][a],
               dat[height[b - a]][b - (1 << height[b - a])]);
  }
};

// Suffix Array ( Manber&Myers: O(n (logn)^2) )
struct SuffixArray {
  string_view str;
  vector<int> sa;   // sa[i] : the starting index of the i-th smallest suffix (i
                    // = 0, 1, ..., n)
  vector<int> lcp;  // lcp[i]: the lcp of sa[i] and sa[i+1] (i = 0, 1, ..., n-1)
  vector<int> rsa;
  SparseTable<int> st;

  SuffixArray(string_view str_) : str(move(str_)) {
    buildSA();
    calcLCP();
  }

  inline int &operator[](int i) { return sa[i]; }

  int getLCP(int a, int b) {  // lcp of str.sutstr(a) and str.substr(b)
    return st.get(min(rsa[a], rsa[b]), max(rsa[a], rsa[b]));
  }

 private:
  void init(string_view str_) {
    str = move(str_);
    buildSA();
    calcLCP();
  }

  // build SA
  vector<int> rank_sa, tmp_rank_sa;
  struct CompareSA {
    int n, k;
    const vector<int> &rank;
    CompareSA(int n, int k, const vector<int> &rank_sa)
        : n(n), k(k), rank(rank_sa) {}
    bool operator()(int i, int j) {
      if (rank[i] != rank[j])
        return (rank[i] < rank[j]);
      else {
        int rank_ik = (i + k <= n ? rank[i + k] : -1);
        int rank_jk = (j + k <= n ? rank[j + k] : -1);
        return (rank_ik < rank_jk);
      }
    }
  };
  void buildSA() {
    int n = LEN(str);
    sa.resize(n + 1), lcp.resize(n + 1), rank_sa.resize(n + 1),
        tmp_rank_sa.resize(n + 1);
    for (int i = 0; i < n; ++i) sa[i] = i, rank_sa[i] = (int)str[i];
    sa[n] = n, rank_sa[n] = -1;
    for (int k = 1; k <= n; k *= 2) {
      CompareSA csa(n, k, rank_sa);
      sort(sa.begin(), sa.end(), csa);
      tmp_rank_sa[sa[0]] = 0;
      for (int i = 1; i <= n; ++i) {
        tmp_rank_sa[sa[i]] = tmp_rank_sa[sa[i - 1]];
        if (csa(sa[i - 1], sa[i])) ++tmp_rank_sa[sa[i]];
      }
      for (int i = 0; i <= n; ++i) rank_sa[i] = tmp_rank_sa[i];
    }
  }

  void calcLCP() {
    int n = LEN(str);
    rsa.resize(n + 1);
    for (int i = 0; i <= n; ++i) rsa[sa[i]] = i;
    lcp.resize(n + 1);
    lcp[0] = 0;
    int cur = 0;
    for (int i = 0; i < n; ++i) {
      int pi = sa[rsa[i] - 1];
      if (cur > 0) --cur;
      for (; pi + cur < n && i + cur < n; ++cur) {
        if (str[pi + cur] != str[i + cur]) break;
      }
      lcp[rsa[i] - 1] = cur;
    }
    st.init(lcp);
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n;
  string s;
  cin >> n >> s;
  SuffixArray sa(s);

  int ans = 0;
  REP(i, n) {
    for (int j = i + 1; j < n; ++j) {
      chmax(ans, min(sa.getLCP(i, j), j - i));
    }
  }
  cout << ans << endl;
}

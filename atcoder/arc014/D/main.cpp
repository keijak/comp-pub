#include <bits/stdc++.h>

#include <atcoder/fenwicktree>

#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}

template <typename T>
struct Compress {
  std::vector<T> vec;

  explicit Compress(std::vector<T> v) : vec(v) {
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
  }

  int size() const { return vec.size(); }

  int index(T x) const {
    return lower_bound(vec.begin(), vec.end(), x) - vec.begin();
  }

  const T &value(int i) const { return vec[i]; }
};

using namespace std;

void solve() {
  int K, N, M;
  cin >> K >> N >> M;
  vector<int> L(N), gap(N - 1);
  cin >> L;
  REP(i, N - 1) { gap[i] = L[i + 1] - L[i] - 1; }
  int first = L[0] - 1, last = K - L[N - 1];
  sort(ALL(gap));
  Compress<int> compress(gap);
  atcoder::fenwick_tree<int> fwc(compress.size()), fws(compress.size());
  {
    int j = 0;
    REP(i, N - 1) {
      int x = gap[i];
      while (x != compress.vec[j]) ++j;
      fwc.add(j, 1);
      fws.add(j, x);
    }
  }
  REP(qi, M) {
    int x, y;
    cin >> x >> y;
    int j = compress.index(x + y);
    int res = fws.sum(0, j) + (N - 1 - fwc.sum(0, j)) * (x + y) +
              min(first, x) + min(last, y) + N;
    cout << res << '\n';
  }
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  solve();
}

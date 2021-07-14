#include <bits/stdc++.h>

using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

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

#define chmax(x, y) x = max(x, y)

const i64 MOD = 1'000'000'007;

struct mint {
  long long x;
  mint(long long x = 0) : x((x % MOD + MOD) % MOD) {}
  mint operator-() const { return mint(-x); }
  mint& operator+=(const mint a) {
    if ((x += a.x) >= MOD) x -= MOD;
    return *this;
  }
  mint& operator-=(const mint a) {
    if ((x += MOD - a.x) >= MOD) x -= MOD;
    return *this;
  }
  mint& operator*=(const mint a) {
    (x *= a.x) %= MOD;
    return *this;
  }
  mint operator+(const mint a) const { return mint(*this) += a; }
  mint operator-(const mint a) const { return mint(*this) -= a; }
  mint operator*(const mint a) const { return mint(*this) *= a; }
  mint pow(long long t) const {
    if (!t) return 1;
    mint a = pow(t >> 1);
    a *= a;
    if (t & 1) a *= *this;
    return a;
  }

  // for prime MOD
  mint inv() const { return pow(MOD - 2); }
  mint& operator/=(const mint a) { return *this *= a.inv(); }
  mint operator/(const mint a) const { return mint(*this) /= a; }
};
istream& operator>>(istream& is, mint& a) { return is >> a.x; }
ostream& operator<<(ostream& os, const mint& a) { return os << a.x; }

struct IntervalSet {
  // Disjoint set of right-open intervals.
  set<pair<i64, i64>> _set;  // {{end, start}}

  // Sum of all interval lengths.
  i64 _length_sum;

  IntervalSet() : _length_sum(0) {}

  i64 length_sum() const { return _length_sum; }

  int count() const { return _set.size(); }

  // Adds an interval. It's merged with all existing intervals.
  // [istart, iend) - right-open interval
  void emplace(i64 istart, i64 iend) {
    pair<i64, i64> inew = {iend, istart};
    auto it1 = _set.upper_bound({istart, 0});
    if (it1 != _set.end() && it1->second <= istart) {
      if (it1->first >= iend) return;
      inew.second = it1->second;
    }
    auto it2 = _set.lower_bound({iend, 0});
    if (it2 != _set.end() && it2->second < iend) {
      if (it2->second <= istart) return;
      inew.first = it2->first;
      ++it2;
    }
    for (auto it = it1; it != it2; ++it) {
      _length_sum -= it->first - it->second;
    }
    _set.erase(it1, it2);

    _set.insert(inew);
    _length_sum += inew.first - inew.second;
  }

  // Returns true if the point is included in an interval in the set.
  bool find(i64 point) const {
    auto it = _set.upper_bound({point, 0});
    if (it == _set.end()) return false;
    return (it->second <= point);
  }
};

mint remove_points(set<i64>& vs, i64 istart, i64 iend) {
  auto it1 = vs.upper_bound(istart);
  auto it2 = vs.lower_bound(iend);
  if (it1 == it2) return 0;
  i64 cnt = 0;
  for (auto it = it1; it != it2; ++it) {
    ++cnt;
  }
  vs.erase(it1, it2);
  return cnt;
}

mint solve() {
  // Input.
  int N, K;
  cin >> N >> K;
  V<i64> L(N), W(N), H(N);
  i64 AL, BL, CL, DL, AW, BW, CW, DW, AH, BH, CH, DH;

  REP(i, K) { cin >> L[i]; }
  cin >> AL >> BL >> CL >> DL;
  REP(i, K) { cin >> W[i]; }
  cin >> AW >> BW >> CW >> DW;
  REP(i, K) { cin >> H[i]; }
  cin >> AH >> BH >> CH >> DH;

  // Create L, W, H.
  for (int i = K; i < N; ++i) {
    L[i] = (AL % DL * L[i - 2] % DL + BL % DL * L[i - 1] % DL + CL % DL) % DL;
    L[i]++;
    W[i] = (AW % DW * W[i - 2] % DW + BW % DW * W[i - 1] % DW + CW % DW) % DW;
    W[i]++;
    H[i] = (AH % DH * H[i - 2] % DH + BH % DH * H[i - 1] % DH + CH % DH) % DH;
    H[i]++;
  }

  /*
    debug("=== L:\n");
    REP(i, N) { cerr << L[i] << ' '; }
    cerr << endl;
    debug("=== H:\n");
    REP(i, N) { cerr << H[i] << ' '; }
    cerr << endl;
    debug("===\n");
  */

  IntervalSet intervals;
  set<i64> starts, ends;
  mint ans = 1;
  mint vperim = 0;

  for (int i = 0; i < N; ++i) {
    i64 istart = L[i];
    i64 iend = L[i] + W[i];

    bool add_start = false, add_end = false;
    if (!starts.count(istart) && !ends.count(istart) &&
        !intervals.find(istart)) {
      add_start = true;
    }
    if (!starts.count(iend) && !ends.count(iend) && !intervals.find(iend)) {
      add_end = true;
    }
    mint removed = remove_points(starts, istart, iend + 1);
    removed += remove_points(ends, istart - 1, iend);
    vperim -= removed * H[i];
    if (add_start) {
      vperim += H[i];
      starts.insert(istart);
    }
    if (add_end) {
      vperim += H[i];
      ends.insert(iend);
    }
    intervals.emplace(istart, iend);
    mint hperim = intervals.length_sum();
    mint perim = vperim + hperim * 2;
    ans *= perim;
    DEBUG(i, L[i], W[i], H[i], intervals.length_sum(), removed, add_start,
          add_end);
    DEBUG(i, vperim.x, hperim.x, perim.x, ans.x);
  }
  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) {
    cerr << "case" << (i + 1) << endl;
    cout << "Case #" << (i + 1) << ": " << solve() << '\n';
  }
}

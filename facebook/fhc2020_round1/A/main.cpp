#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
template <typename T>
using V = std::vector<T>;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

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

i64 solve() {
  // Input.
  int N, K, W;
  cin >> N >> K >> W;
  V<i64> L(N), H(N);
  REP(i, K) { cin >> L[i]; }
  i64 AL, BL, CL, DL, AH, BH, CH, DH;
  cin >> AL >> BL >> CL >> DL;
  REP(i, K) { cin >> H[i]; }
  cin >> AH >> BH >> CH >> DH;

  // Create L and H.
  for (int i = K; i < N; ++i) {
    L[i] =
        (AL % DL * L[i - 2] % DL + BL % DL * L[i - 1] % DL + CL % DL) % DL + 1;
    H[i] =
        (AH % DH * H[i - 2] % DH + BH % DH * H[i - 1] % DH + CH % DH) % DH + 1;
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
  i64 perim = 2 * W + 2 * H[0];  // = P[0]
  mint ans = perim;
  V<i64> h(W, H[0]);  // heights in [L[0], L[0]+W]
  DEBUG(0, perim, ans);

  for (int i = 1; i < N; ++i) {
    if (L[i] > L[i - 1] + W) {
      perim += 2 * W + 2 * H[i];
      fill(h.begin(), h.end(), H[i]);
    } else if (L[i] == L[i - 1] + W) {
      perim -= H[i - 1];
      perim += abs(H[i] - H[i - 1]);
      perim += 2 * W + H[i];
      fill(h.begin(), h.end(), H[i]);
    } else {
      i64 d = L[i] - L[i - 1];
      V<i64> newh(W);
      REP(j, W) {
        newh[j] = H[i];
        if (d + j < W) {
          chmax(newh[j], h[d + j]);
        }
      }

      i64 pperim = 0;
      for (int j = d; j < W; ++j) {
        pperim += abs(h[j] - h[j - 1]);  // left
      }
      pperim += h[W - 1];  // right

      i64 nperim = 0;
      for (int j = d; j < W; ++j) {
        int k = j - d;
        i64 ph = k == 0 ? h[d - 1] : newh[k - 1];
        i64 y = abs(newh[k] - ph);
        // DEBUG(j, k, y);
        nperim += y;
      }
      i64 ph = (W - d == 0) ? h[W - 1] : newh[W - d - 1];
      nperim += abs(H[i] - ph);
      nperim += H[i];
      nperim += 2 * d;

      // DEBUG(perim, pperim, nperim);

      perim = perim - pperim + nperim;
      swap(h, newh);
    }
    ans *= perim;
    DEBUG(i, perim, ans);
  }
  return ans.x;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  REP(i, t) {
    if (i % 10 == 0) cerr << "case" << (i + 1) << endl;
    cout << "Case #" << (i + 1) << ": " << solve() << '\n';
  }
}

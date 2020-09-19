const int MOD = 1'000'000'007;

struct mint {
  int x;

  mint(long long val = 0) {
    if (val < 0) {
      long long k = (abs(val) + MOD - 1) / MOD;
      val += k * MOD;
    }
    assert(val >= 0);
    x = val % MOD;
  }
  mint operator-() const { return mint(-x); }
  mint &operator+=(const mint &a) {
    if ((x += a.x) >= MOD) x -= MOD;
    return *this;
  }
  mint &operator-=(const mint &a) {
    if ((x += MOD - a.x) >= MOD) x -= MOD;
    return *this;
  }
  mint &operator*=(const mint &a) {
    x = (x * (long long)a.x) % MOD;
    return *this;
  }
  mint pow(long long t) const {
    assert(t >= 0);
    mint base = *this;
    mint res = 1;
    while (t) {
      if (t & 1) res *= base;
      base *= base;
      t >>= 1;
    }
    return res;
  }
  // for prime MOD
  mint inv() const { return pow(MOD - 2); }
  mint &operator/=(const mint &a) { return *this *= a.inv(); }
};
mint operator+(const mint &a, const mint &b) { return mint(a) += b; }
mint operator-(const mint &a, const mint &b) { return mint(a) -= b; }
mint operator*(const mint &a, const mint &b) { return mint(a) *= b; }
mint operator/(const mint &a, const mint &b) { return mint(a) /= b; }
bool operator==(const mint &a, const mint &b) { return a.x == b.x; }
bool operator!=(const mint &a, const mint &b) { return a.x != b.x; }
bool operator<(const mint &a, const mint &b) { return a.x < b.x; }
bool operator>(const mint &a, const mint &b) { return a.x > b.x; }
bool operator<=(const mint &a, const mint &b) { return a.x <= b.x; }
bool operator>=(const mint &a, const mint &b) { return a.x >= b.x; }
istream &operator>>(istream &is, mint &a) { return is >> a.x; }
ostream &operator<<(ostream &os, const mint &a) { return os << a.x; }

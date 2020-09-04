struct RollingHash {
  using u64 = unsigned long long;
  using u128 = __uint128_t;
  static const u64 mod = (1ULL << 61) - 1;
  vector<u64> hashed, power;

  RollingHash(string_view s) : hashed(s.size() + 1), power(s.size() + 1) {
    int n = s.size();
    power[0] = 1;
    for (int i = 0; i < n; i++) {
      power[i + 1] = mul(power[i], base());
      hashed[i + 1] = add(mul(hashed[i], base()), s[i]);
    }
  }

  // Returns the hash value for the [l,r) interval.
  u64 get(int l, int r) {
    return add(hashed[r], mod - mul(hashed[l], power[r - l]));
  }

 private:
  static u64 base() {
    static u64 val = []() -> u64 {
      random_device seed_gen;
      mt19937_64 engine(seed_gen());
      uniform_int_distribution<u64> rand(1, mod - 1);
      return rand(engine);
    }();
    return val;
  }

  static u64 add(u64 a, u64 b) {
    a += b;
    if (a >= mod) a -= mod;
    return a;
  }

  static u64 mul(u64 a, u64 b) {
    u128 t = (u128)a * b;
    u64 na = t >> 61;
    u64 nb = t & mod;
    na += nb;
    if (na >= mod) na -= mod;
    return na;
  }
};

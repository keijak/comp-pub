// Turn off all bits of x except the rightmost one.
int rightmost_bit(int x) { return x & -x; }

// GCC Builtins.

int popcount(unsigned int x) { return __builtin_popcount(x); }
int popcount(unsigned long x) { return __builtin_popcountl(x); }
int popcount(unsigned long long x) { return __builtin_popcountll(x); }

int count_leading_zero(unsigned int x) { return __builtin_clz(x); }
int count_leading_zero(unsigned long x) { return __builtin_clzl(x); }
int count_leading_zero(unsigned long long x) { return __builtin_clzll(x); }

int count_trailing_zero(unsigned int x) { return __builtin_ctz(x); }
int count_trailing_zero(unsigned long x) { return __builtin_ctzl(x); }
int count_trailing_zero(unsigned long long x) { return __builtin_ctzll(x); }

// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;

template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
constexpr T kBig = std::numeric_limits<T>::max() / 2;

using namespace std;

template<typename T>
inline T rd() {
  T ret = 0;
  int ch = getchar_unlocked();
  for (; isspace(ch); ch = getchar_unlocked());
  for (; isdigit(ch); ch = getchar_unlocked()) {
    ret = (ret * 10) + (ch - '0');
  }
  // ungetc(ch, stdin);  // assumption: ch is an ignorable whitespace
  return ret;
}

template<class F>
struct Rec {
  F f_;
  explicit Rec(F f) : f_(std::move(f)) {}
  template<class... Args>
  decltype(auto) operator()(Args &&... args) {
    return f_(*this, std::forward<Args>(args)...);
  }
};
template<class F> Rec(F) -> Rec<F>;

int main() {
  const int n = rd<int>();
  array<vector<Int>, 4> pos{};
  REP(i, 4) pos[i].reserve(100005);
  array<int, 26> char_index{};
  char_index['J' - 'A'] = 0;
  char_index['O' - 'A'] = 1;
  char_index['I' - 'A'] = 2;
  char_index['G' - 'A'] = 3;
  REP(i, n) {
    const Int a = rd<Int>();
    const char ch = getchar_unlocked();
    const int j = char_index[ch - 'A'];
    pos[j].push_back(a);
  }

  auto f = Rec([&](auto self, Int s, Int t, int i) -> Int {
    if (i == 4) return abs(t - s);
    auto it = std::lower_bound(ALL(pos[i]), s);
    Int ret = kBig<Int>;
    if (it != pos[i].end()) {
      chmin(ret, abs(*it - s) + self(*it, t, i + 1));
    }
    if (it != pos[i].begin()) {
      --it;
      chmin(ret, abs(*it - s) + self(*it, t, i + 1));
    }
    return ret;
  });

  const int Q = rd<int>();
  REP(i, Q) {
    const Int S = rd<Int>(), T = rd<Int>();
    printf("%lld\n", f(S, T, 0));
  }
  fflush(stdout);
  _Exit(0);
}

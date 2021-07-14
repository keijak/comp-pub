#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
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
template <typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream &os = std::cout) {
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) os << sep;
    os << *it;
  }
  return os << ends;
}
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef MY_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &...args) {
  pdebug(value);
  std::cerr << ", ";
  pdebug(args...);
}
#define DEBUG(...)                                   \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ") "; \
    std::cerr << #__VA_ARGS__ << ":\033[0m ";        \
    pdebug(__VA_ARGS__);                             \
    std::cerr << std::endl;                          \
  } while (0)
#else
#define pdebug(...)
#define DEBUG(...)
#endif

using namespace std;

optional<vector<i64>> solve(array<string, 3> s) {
  set<char> used;
  REP(i, 3) {
    for (char c : s[i]) {
      used.insert(c);
    }
  }
  const int m = ssize(used);
  if (m > 10) return nullopt;

  vector<int> ch2id(26, -1);
  {
    int cnt = 0;
    for (auto c : used) {
      ch2id[c - 'a'] = cnt++;
    }
  }

  auto check = [&](const vector<int> nums) -> optional<vector<i64>> {
    vector<i64> vals(3, 0LL);
    REP(i, 3) {
      vals[i] = 0;
      REP(j, ssize(s[i])) {
        int cid = ch2id[s[i][j] - 'a'];
        assert(0 <= cid && cid < m);
        int d = nums[cid];
        if (d == 0 and j == 0) return nullopt;
        vals[i] *= 10;
        vals[i] += d;
      }
    }
    if (vals[0] + vals[1] != vals[2]) {
      return nullopt;
    }
    return vals;
  };

  vector<int> sel(10, -1);
  REP(i, m) { sel[i] = i; }
  sort(ALL(sel));
  vector<int> nums(m, -1);
  do {
    REP(i, 10) {
      if (sel[i] >= 0) {
        nums[sel[i]] = i;
      }
    }
    auto res = check(nums);
    if (res.has_value()) {
      return res;
    }
  } while (next_permutation(ALL(sel)));
  return nullopt;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  array<string, 3> s;
  REP(i, 3) { cin >> s[i]; }
  auto res = solve(s);
  if (!res) {
    cout << "UNSOLVABLE\n";
  } else {
    auto ns = res.value();
    REP(i, 3) { cout << ns[i] << "\n"; }
  }
}

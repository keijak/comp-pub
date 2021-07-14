#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
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

int main() {
  int n;
  cin >> n;
  vector<bool> board(n, false);

  auto scan = [&]() -> vector<int> {
    vector<int> stones;
    int cur = 0;
    REP(i, n) {
      if (board[i]) {
        if (cur > 0) {
          stones.push_back(cur);
          cur = 0;
        }
      } else {
        cur++;
      }
    }
    if (cur > 0) stones.push_back(cur);
    sort(ALL(stones));
    return stones;
  };

  map<int, int> memo;

  auto nimber = [&](auto &rec, vector<int> stones) -> int {
    if (stones.empty()) return 0;
    if (ssize(stones) > 1) {
      int res = 0;
      for (auto x : stones) {
        assert(x > 0);
        res ^= rec(rec, vector<int>(1, x));
      }
      return res;
    }
    const int x = stones[0];
    if (x == 1) return 1;
    if (x == 2) return 2;
    assert(x > 2);

    auto it = memo.find(x);
    if (it != memo.end()) return it->second;
    set<int> s;
    s.insert(rec(rec, vector<int>(1, x - 1)));
    s.insert(rec(rec, vector<int>(1, x - 2)));
    {
      vector<int> v = {x - 2, 1};
      while (v[0] > 0 and v[0] >= v[1]) {
        s.insert(rec(rec, v));
        v[0]--, v[1]++;
      }
    }
    {
      vector<int> v = {x - 3, 1};
      while (v[0] > 0 and v[0] >= v[1]) {
        s.insert(rec(rec, v));
        v[0]--, v[1]++;
      }
    }
    // Return the MEX.
    for (int i = 0;; ++i) {
      if (not s.count(i)) {
        memo[x] = i;
        return i;
      }
    }
    assert(false);
  };

  auto next_move = [&]() -> pair<int, int> {
    // Take one.
    REP(i, n) {
      if (board[i]) continue;
      board[i] = true;
      int val = nimber(nimber, scan());
      if (val == 0) {
        return {1, i + 1};
      }
      board[i] = false;
    }

    // Take two.
    REP(i, n - 1) {
      if (board[i] or board[i + 1]) continue;
      board[i] = board[i + 1] = true;
      int val = nimber(nimber, scan());
      if (val == 0) {
        return {2, i + 1};
      }
      board[i] = board[i + 1] = false;
    }

    assert(false);
  };

  auto query = [&]() -> void {
    int k, x;
    tie(k, x) = next_move();
    cout << k << ' ' << x << endl;
    int t;
    cin >> t;
    if (t == 0 or t == 1) {
      exit(0);
    }
    assert(t == 3);
    cin >> k >> x;
    REP(i, k) {
      int p = x - 1 + i;
      assert(not board[p]);
      board[p] = true;
    }
  };

  while (true) {
    query();
  }
}

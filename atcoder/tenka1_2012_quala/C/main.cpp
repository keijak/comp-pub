#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), N_##i = (b); i < N_##i; ++i)
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
                        std::ostream *os = nullptr) {
  if (os == nullptr) os = &std::cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
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
  return print_seq(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef ENABLE_DEBUG
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

const int L = 100000;

// s |= t;
// Merges two sets into their union.
// The first argument will be updated to the result.
template <typename T>
void merge_sets_by_union(set<T> &s, const set<T> &t) {
  s.insert(t.begin(), t.end());
}

// s &= t;
// Merges two sets into their intersection.
// The first argument will be updated to the result.
template <typename T>
void merge_sets_by_intersection(set<T> &s, const set<T> &t) {
  auto it = s.begin();
  auto jt = t.begin();
  while (it != s.end()) {
    while (jt != t.end() and *jt < *it) ++jt;
    if (jt == t.end()) {
      s.erase(it, s.end());
      break;
    }
    if (*it < *jt) {
      it = s.erase(it);
    } else {
      ++it, ++jt;
    }
  }
}

i64 solve() {
  int n, m;
  cin >> n >> m;
  vector<set<int>> g(n);
  REP(i, m) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    g[b].insert(a);
  }
  string s;
  cin >> s;
  int ns = ssize(s);

  deque<bool> antiq;
  int gnum = -1;
  {
    string_view sv = s;
    int l = 0, r = ns;
    while (true) {
      if (sv[l] == '"') {
        if (sv[r - 1] == '"') {
          antiq.push_front(false);
          ++l;
          --r;
        } else {
          assert(r - 3 > l and sv.substr(r - 3, 3) == "\"ww"sv);
          antiq.push_front(true);
          ++l;
          r -= 3;
        }
        continue;
      }
      assert(sv.substr(l, 5) == "group"sv);
      l += 5;
      if (sv[r - 1] == 'w') {
        antiq.push_front(true);
        --r;
      } else {
        antiq.push_front(false);
      }
      gnum = std::stoi(s.substr(l, r - l)) - 1;
      break;
    }
    DEBUG(gnum);
    DEBUG(antiq);
    assert(gnum >= 0);
  }

  set<int> bst = {gnum};
  REP(round, ssize(antiq)) {
    set<int> nxt;
    if (antiq[round]) {
      for (auto x : bst) {
        merge_sets_by_union(nxt, g[x]);
      }
    } else {
      bool first = true;
      set<int> tmp;
      for (auto x : bst) {
        if (first) {
          first = false;
          tmp = g[x];
        } else {
          merge_sets_by_intersection(tmp, g[x]);
        }
      }
      REP(i, n) {
        if (not tmp.count(i)) nxt.insert(i);
      }
    }
    bst = move(nxt);
  }
  return ssize(bst);
}

int main() {
  std::ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}

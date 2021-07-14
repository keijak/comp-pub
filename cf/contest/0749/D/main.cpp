#include <bits/stdc++.h>
#define REP(i, n) for (int i = 0, REP_N_ = (n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <class T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}
template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &pprint(const Container &a, std::string_view sep = " ",
                     std::string_view ends = "\n", std::ostream *os = nullptr) {
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
  return pprint(a, ", ", "", &(os << "{")) << "}";
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
  ios::sync_with_stdio(false), cin.tie(nullptr);

  int n;
  cin >> n;
  vector<vector<i64>> bids(n);
  REP(i, n) {
    i64 a, b;
    cin >> a >> b;
    --a;
    bids[a].push_back(b);
  }

  map<i64, int> maxbid;
  maxbid[0] = -1;
  REP(i, n) {
    if (bids[i].empty()) continue;
    maxbid[bids[i].back()] = i;
  }

  int q;
  cin >> q;
  REP(i, q) {
    int k;
    cin >> k;
    vector<int> l(k);
    vector<pair<i64, int>> restores;
    restores.reserve(k);
    REP(j, k) {
      int x;
      cin >> x;
      --x;
      l[j] = x;

      if (not bids[x].empty()) {
        i64 m = bids[x].back();
        auto it = maxbid.find(m);
        if (it != maxbid.end()) {
          restores.emplace_back(it->first, it->second);
          maxbid.erase(it);
        }
      }
    }

    int winner = -1;
    if (not maxbid.empty()) {
      auto it = maxbid.rbegin();
      winner = it->second;
      restores.emplace_back(it->first, it->second);
      maxbid.erase(it->first);
    }

    i64 lb = 0;
    if (not maxbid.empty()) {
      auto it = maxbid.rbegin();
      lb = it->first;
    }

    if (winner == -1) {
      cout << 0 << ' ' << 0 << '\n';
    } else {
      auto it = upper_bound(ALL(bids[winner]), lb);
      assert(it != bids[winner].end());
      cout << winner + 1 << ' ' << *it << '\n';
    }

    for (auto [b, a] : restores) {
      maxbid[b] = a;
    }
  }
}

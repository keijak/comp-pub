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

optional<vector<int>> solve() {
  int n;
  cin >> n;
  vector<int> P(n), inds(n);
  REP(i, n) {
    int p;
    cin >> p;
    P[i] = p - 1;
    inds[p - 1] = i;
  }

  vector<bool> done(n);
  vector<int> ans;
  int l = 0, r = n - 1;
  while (l <= r) {
    DEBUG(P);
    DEBUG(inds);
    if (done[l]) {
      DEBUG(l, "DONE");
      ++l;
    } else {
      int i = inds[l];
      assert(P[i] == l);
      DEBUG(l, i);
      if (i <= l) {
        DEBUG("BAD");
        return nullopt;
      }
      assert(i > 0);
      if (P[i - 1] <= l) {
        DEBUG("BAD");
        return nullopt;
      }
      swap(P[i], P[i - 1]);
      inds[P[i]] = i;
      inds[P[i - 1]] = i - 1;
      ans.push_back(i);
      if (P[i] == i) {
        done[i] = true;
      }
      for (int j = i - 2; j >= l; --j) {
        if (P[j] != j + 1) {
          DEBUG("BAD");
          return nullopt;
        }
        swap(P[j], P[j + 1]);
        inds[P[j]] = j;
        inds[P[j + 1]] = j + 1;
        done[j + 1] = true;
        ans.push_back(j + 1);
      }
      done[l] = true;
      ++l;
    }

    if (done[r]) {
      DEBUG(r, "DONE");
      --r;
    } else {
      int i = inds[r];
      DEBUG(r, i);
      assert(P[i] == r);
      if (i >= r) return nullopt;
      assert(i < n - 1);
      if (P[i + 1] >= r) return nullopt;
      swap(P[i], P[i + 1]);
      inds[P[i]] = i;
      inds[P[i + 1]] = i + 1;
      if (P[i] == i) {
        done[i] = true;
      }
      ans.push_back(i + 1);
      for (int j = i + 2; j <= r; ++j) {
        if (P[j] != j - 1) return nullopt;
        swap(P[j], P[j - 1]);
        inds[P[j]] = j;
        inds[P[j - 1]] = j - 1;
        done[j - 1] = true;
        ans.push_back(j);
      }
      done[r] = true;
      --r;
    }
  }
  if (ssize(ans) != n - 1) return nullopt;
  return ans;
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  auto res = solve();
  if (not res.has_value()) {
    cout << "-1\n";
  } else {
    pprint(res.value(), "\n");
  }
}

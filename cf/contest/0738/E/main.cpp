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

const i64 INF = 1e18;

// Set of disjoint half-open intervals.
struct IntervalSet {
  using PSet = std::set<std::pair<i64, i64>>;
  PSet set_;  // {{start, end}}

  IntervalSet() {}

  int count() const { return set_.size(); }

  const PSet &get() const { return set_; }

  // Adds an interval. It's merged with all existing intervals.
  // [istart, iend) - right-open interval
  void emplace(i64 istart, i64 iend) {
    if (istart >= iend) {
      return;  // Ignore empty intervals.
    }
    if (auto it = this->find(istart);
        it != this->end() and it->first <= istart and iend <= it->second) {
      return;  // Completely contained. Skip.
    }

    // New interval to be inserted.
    std::pair<i64, i64> inew = {istart, iend};

    auto it = set_.upper_bound({istart, INF});
    if (it != set_.begin()) --it;
    while (it != set_.end()) {
      if (it->first > iend) break;
      if (it->second < istart) {
        ++it;
        continue;
      }
      // Merge.
      inew.first = std::min(inew.first, it->first);
      inew.second = std::max(inew.second, it->second);
      it = set_.erase(it);
    }
    set_.insert(inew);
  }

  PSet::iterator end() const { return set_.end(); }

  PSet::iterator find(i64 point) const {
    auto it = set_.upper_bound({point, INF});
    if (it == set_.begin()) return set_.end();
    --it;
    if (it->second <= point) return set_.end();
    return it;
  }

  // Returns true if the point is included in an interval in the set.
  bool contains(i64 point) const { return this->find(point) != this->end(); }
};

i64 solve() {
  int n, s;
  cin >> n >> s;
  --s;
  vector<int> a(n);  // num superiors
  int num_chiefs = 0;
  REP(i, n) {
    cin >> a[i];
    if (a[i] == 0) ++num_chiefs;
  }
  int mistakers;
  if (a[s] != 0) {
    ++num_chiefs;
    mistakers = num_chiefs;
    a[s] = 0;
  } else {
    mistakers = num_chiefs - 1;
  }
  map<int, int> ac;
  REP(i, n) { ac[a[i]]++; }

  IntervalSet iset;
  REP(i, n) { iset.emplace(a[i], a[i] + 1); }

  for (int i = 0; i < num_chiefs - 1; ++i) {
    assert(iset.count() > 0);
    if (iset.count() == 1) goto finally;
    auto it = iset.set_.begin();
    assert(it != iset.set_.end());
    int j = it->second;
    iset.emplace(j, j + 1);
  }

  for (auto it = ac.rbegin(); it != ac.rend(); ++it) {
    if (iset.count() == 1) goto finally;
    int key = it->first;
    int cnt = it->second;
    DEBUG(key, cnt);
    for (int i = 0; i < cnt - 1; ++i) {
      auto jt = iset.set_.begin();
      assert(jt != iset.set_.end());
      int j = jt->second;
      iset.emplace(j, j + 1);
      ++mistakers;
      if (iset.count() == 1) goto finally;
    }
    {
      auto jt = iset.set_.end();
      --jt;
      assert(jt->second - 1 == key);
      int jstart = jt->first;
      iset.set_.erase(jt);
      if (jstart < key) {
        iset.emplace(jstart, key);
      }
    }
    {
      auto jt = iset.set_.begin();
      assert(jt != iset.set_.end());
      int j = jt->second;
      iset.emplace(j, j + 1);
      ++mistakers;
      if (iset.count() == 1) goto finally;
    }
  }

finally:
  return mistakers;
}

int main() {
  ios::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}

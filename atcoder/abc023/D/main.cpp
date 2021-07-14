#include <bits/stdc++.h>
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0, REP_N_ = int(n); i < REP_N_; ++i)
#define ALL(x) std::begin(x), std::end(x)

template <class T>
inline bool chmax(T &a, T b) {
  return a < b and ((a = std::move(b)), true);
}
template <class T>
inline bool chmin(T &a, T b) {
  return a > b and ((a = std::move(b)), true);
}

template <typename T>
using V = std::vector<T>;
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

template <typename T,
          typename = std::enable_if_t<is_iterable<T>::value &&
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
void pdebug(const T &value, const Ts &... args) {
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

  // Sum of all interval lengths.
  i64 total_length_;

  IntervalSet() : total_length_(0) {}

  i64 total_length() const { return total_length_; }

  int count() const { return set_.size(); }

  const PSet &get() const { return set_; }

  // Adds an interval. It's merged with all existing intervals.
  // [istart, iend) - right-open interval
  void emplace(i64 istart, i64 iend) {
    if (istart >= iend) {
      return;  // Ignore empty intervals.
    }
    {
      auto it = this->find(istart);
      if (it != this->end() and it->first <= istart and iend <= it->second) {
        return;  // Completely contained. Skip.
      }
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
      total_length_ -= it->second - it->first;
      it = set_.erase(it);
    }
    set_.insert(inew);
    total_length_ += inew.first - inew.second;
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  V<pair<i64, i64>> bs(n);
  i64 hmax = 0;
  REP(i, n) {
    cin >> bs[i].first >> bs[i].second;
    chmax(hmax, bs[i].second);
  }

  auto check = [&](i64 x) -> bool {
    IntervalSet iset;
    REP(i, n) {
      auto [h, s] = bs[i];
      if (h > x) return false;
      i64 tmax = (x - h) / s;
      auto it = iset.find(tmax);
      if (it == iset.end()) {
        iset.emplace(tmax, tmax + 1);
      } else if (it->first == 0) {
        return false;
      } else {
        iset.emplace(it->first - 1, it->first);
      }
    }
    return true;
  };

  i64 fv = -1, tv = INF;
  while (tv - fv > 1) {
    i64 mid = (fv + tv) / 2;
    (check(mid) ? tv : fv) = mid;
  }
  cout << tv << endl;
}

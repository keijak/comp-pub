// #define NDEBUG
#include <bits/stdc++.h>

#define REP_(i, a_, b_, a, b, ...) for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Uint = unsigned long long;
using Real = long double;

template<typename T, typename U>
inline bool chmax(T &a, U b) { return a < b and ((a = b), true); }
template<typename T, typename U>
inline bool chmin(T &a, U b) { return a > b and ((a = b), true); }
template<typename T>
constexpr T kBig = std::numeric_limits<T>::max() / 2;
#if __cplusplus < 202002L
template<typename T>
inline int ssize(const T &a) { return (int) a.size(); }
#endif

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    assert(bool(std::cin));
    return x;
  }
  struct Sized {
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        std::cin >> x;
        assert(bool(std::cin));
      }
      return xs;
    }
  };
  Sized operator()(int n) const { return {n}; }
} in;

template<typename Container>
std::ostream &out_seq(const Container &seq, const char *sep = " ",
                      const char *ends = "\n", std::ostream &os = std::cout) {
  const auto itl = std::begin(seq), itr = std::end(seq);
  for (auto it = itl; it != itr; ++it) {
    if (it != itl) os << sep;
    os << *it;
  }
  return os << ends;
}

template<typename T>
std::ostream &out_one(const T &x, char endc) {
  if constexpr (std::is_same<T, bool>::value) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<typename T>
std::ostream &out(const T &x) {
  return out_one(x, '\n');
}
template<typename T, typename... Ts>
std::ostream &out(const T &head, Ts... tail) {
  return out_one(head, ' '), out(tail...);
}

void init_(bool interactive = false) {
  std::ios::sync_with_stdio(false);
  if (not interactive) std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
#ifdef MY_DEBUG
  std::string _;
  assert((std::cin >> _).fail());
#endif
  std::cout.flush(), std::cerr.flush(), std::_Exit(0);
}

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define test_case(...)
#define cerr if(false)cerr
#endif

template<class T, class Compare = std::less<T>>
struct SlideMinQueue {
  const std::vector<T> &vals;   // reference to static values.
  Compare compare;              // comparison funcation.
  int left, right;              // [left, right)
  std::deque<int> index_queue;  // indices where min values are stored.

  SlideMinQueue() : compare(), left(0), right(0) {}

  explicit SlideMinQueue(const std::vector<T> &v)
      : vals(v), compare(), left(0), right(0) {}

  // Shifts the window to the right.
  // Sets `left` to `l`, and `right` to `r`.
  void slide(int l, int r) {
    l = std::max(l, 0);
    r = std::min(r, (int) vals.size());
    assert(l >= left);
    assert(r >= right);
    for (int i = right; i < r; ++i) {
      push_right(i);
    }
    right = r;
    pop_left(l);
    left = l;
  }

  // Returns the minimum value in [left, right).
  std::optional<int> fold_index() const {
    if (empty()) return std::nullopt;
    return index_queue.front();
  }

  bool empty() const { return index_queue.empty(); }

 private:
  // Enqueues the i-th value.
  void push_right(int i) {
    while (!index_queue.empty() && compare(vals[i], vals[index_queue.back()])) {
      index_queue.pop_back();
    }
    index_queue.emplace_back(i);
  }

  // Dequeues indices less than i.
  void pop_left(int i) {
    while (!index_queue.empty() && index_queue.front() < i) {
      index_queue.pop_front();
    }
  }
};

using namespace std;

auto solve() -> vector<int> {
  const int n = in, K = in;
  vector<int> P = in(n);
  vector<int> pi(n);
  REP(i, n) {
    pi[--P[i]] = i;
  }

  auto f0 = [&](int pos, int k, const vector<int> &p) -> vector<int> {
    vector<int> ret;
    int l = pos;
    if (l + k >= ssize(p)) return ret;
    SlideMinQueue<int> minq(p);
    while (l + k < ssize(p) and k >= 0) {
      minq.slide(l, min(l + k + 1, ssize(p)));
      auto j = minq.fold_index();
      if (not j) break;
      pos = j.value();
      assert(pos - l <= k);
      ret.push_back(p[pos]);
      k -= (pos - l);
      l = pos + 1;
    }
    return ret;
  };

  vector<int> ans(n, n + 1);
  REP(x, n) {
    int j = pi[x];
    bool ok = false;
    if (j <= K) {
      auto sub = f0(j + 1, K - j, P);
      sub.insert(sub.begin(), x);
      chmin(ans, sub);
      ok = true;
    }
    if (n - j <= K) {
      vector<int> P2(P.begin(), P.begin() + j);
      auto suffix = f0(0, K - (n - j), P2);
      vector<int> prefix = {x};
      int head = suffix.empty() ? -1 : suffix.front();
      set<pair<int, int>> s;
      for (int i = j + 1; i < n; ++i) {
        s.insert({P[i], i});
      }
      int l = j + 1;
      while (l < n and not s.empty()) {
        auto [y, pos] = *s.begin();
        s.erase(s.begin());
        if (pos < l or y >= head) continue;
        prefix.push_back(y);
        l = pos + 1;
      }
      DUMP(prefix, suffix, head);
      for (int e: suffix) prefix.push_back(e);
      chmin(ans, prefix);
      ok = true;
    }
    if (ok) return ans;
  }
  assert(false);
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    auto ans = (solve());
    for (auto &e: ans) ++e;
    out_seq(ans);
  }
  exit_();
}

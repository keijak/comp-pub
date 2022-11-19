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

template<size_t BufSize>
class InputReader {
 public:
  void skip() {
    [[maybe_unused]] static const bool lazy_init = [this]() {
      const size_t len = fread(buf_, 1, sizeof(buf_) - 1, stdin);
      buf_[len] = '\0';
      ptr_ = buf_;
      bufend_ = buf_ + len;
      return true;
    }();
    while (isspace(*ptr_)) ++ptr_;
  }

  template<typename T>
  operator T() {
    T x;
    skip();
    assert(not is_eof());  // Couldn't read reached the end of input.
    read_one(x);
    return x;
  }

  struct Sized {
    InputReader<BufSize> &reader;
    int n;
    template<typename T>
    operator T() const {
      T xs(n);
      for (auto &x: xs) {
        reader.skip();
        assert(not reader.is_eof());
        reader.read_one(x);
      }
      return xs;
    }
  };
  Sized operator()(int n) { return {*this, n}; }

  bool is_eof() const { return ptr_ >= bufend_; }

 private:
  template<class T>
  std::enable_if_t<std::is_integral_v<T>> read_one(T &x) {
    [[maybe_unused]] int sign = 1;
    if constexpr (std::is_signed_v<T>) {
      sign = (*ptr_ == '-') ? (++ptr_, -1) : 1;
    }
    x = 0;
    while (isdigit(*ptr_)) x = x * 10 + (*ptr_++ & 0x0F);
    if constexpr (std::is_signed_v<T>) {
      x *= sign;
    }
  }
  void read_one(std::string &s) {
    char *p0 = ptr_;
    while (not isspace(*ptr_)) ptr_++;
    s.assign(p0, ptr_);
  }
  void read_one(std::string_view &s) {
    const char *p0 = ptr_;
    while (not isspace(*ptr_)) ptr_++;
    s = std::string_view(p0, ptr_ - p0);
  }

  static inline char buf_[BufSize];
  char *ptr_, *bufend_;
};
InputReader<(1 << 26)> in;

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

void init_() {
  // std::ios::sync_with_stdio(false);
  // std::cin.tie(nullptr);
  std::cout << std::fixed << std::setprecision(18);
}

[[noreturn]] void exit_() {
#ifdef MY_DEBUG
  in.skip();
  assert(in.is_eof());  // Some input is left.
#endif
  fflush(stdout), fflush(stderr);
  std::cout.flush(), std::cerr.flush();
  std::_Exit(0);
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

using namespace std;

template<class T>
using MinHeap = priority_queue<T, vector<T>, greater<T>>;

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
  int fold_index() const {
    if (empty()) return -1;
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

auto solve() {
  int n = in, K = in;
  vector<int> P0 = in(n);
  vector<pair<int, int>> pi(n);
  if (K == 0) {
    out_seq(P0);
    return;
  }
  REP(i, n) pi[i] = {P0[i], i};

  auto f = [&](vector<int> P, int resource, int zerocost) -> vector<int> {
    vector<int> ans;
    ans.push_back(P[0]);
    SlideMinQueue<int> q(P);
    int r = 1;
    while (resource > 0 or r < zerocost) {
      q.slide(r, min(max(zerocost, r) + resource + 1, ssize(P)));
      auto j = q.fold_index();
      if (j == -1) break;
      ans.push_back(P[j]);
      if (j > max(zerocost, r)) {
        resource -= j - max(zerocost, r);
      }
      r = j + 1;
    }
    for (int j = r; j < ssize(P); ++j) {
      ans.push_back(P[j]);
    }
    while (not ans.empty() and resource > 0) {
      ans.pop_back();
      --resource;
    }
    return ans;
  };

  vector<int> ans;

  REP(trial, 2) {
    MinHeap<tuple<int, int, bool>> heap;
    if (trial) {
      REP(i, K) heap.push({pi[n - 1 - i].first, pi[n - 1 - i].second, true});
    } else {
      REP(i, K + 1) heap.push({pi[i].first, pi[i].second, false});
    }
    auto [smallest, si, tail] = heap.top();
    DUMP(smallest, si, tail);
    auto P = P0;
    if (trial) {
      rotate(P.begin(), P.begin() + si, P.end());
      chmin(ans, f(P, K - (n - si), n - si));
    } else {
      P.erase(P.begin(), P.begin() + si);
      ans = f(P, K - si, 0);
    }
    DUMP(trial, ans);
  }
  out_seq(ans);

}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    (solve());
  }
  exit_();
}

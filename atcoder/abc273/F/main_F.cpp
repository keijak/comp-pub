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

using namespace std;

template<class T>
using MinHeap = std::priority_queue<T, vector<T>, greater<T>>;

struct Edge {
  int to;
  Int cost;
};
using Graph = std::vector<std::vector<Edge>>;

struct State {
  int pos;
  int other;
  Int cost;
};
bool operator>(const State &x, const State &y) { return x.cost > y.cost; }

template<typename T>
struct Compressed {
  std::vector<T> values;

  explicit Compressed(std::vector<T> v) : values(std::move(v)) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
  }

  int size() const { return values.size(); }

  const T &value(int i) const { return values[i]; }

  int index(const T &x) const {
    return std::lower_bound(values.begin(), values.end(), x) - values.begin();
  }

  int lower_index(const T &x) const {
    int ub = std::upper_bound(values.begin(), values.end(), x) - values.begin();
    return ub - 1;
  }

  bool contains(const T &x) const {
    return std::binary_search(values.begin(), values.end(), x);
  }
};

struct Event {
  Int coord;
  int etype;  // 0 origin, 1 goal, 2 wall, 3 hammer.
  int id;
};

template<int kMaxSize, int kMaxValue>
struct SparseIntSet {
  int size_;
  int dense_[kMaxSize];
  int sparse_[kMaxValue + 1];

  SparseIntSet() noexcept: size_(0) {}

  // O(1)
  bool contains(int x) const {
    // May access uninitialized memory, but safe.
    const size_t i = sparse_[x];
    return i < (size_t) size_ and dense_[i] == x;
  }

  // O(1)
  void insert(int x) {
    if (contains(x)) return;
    dense_[size_] = x;
    sparse_[x] = size_++;
  }

  // O(1)
  void erase(int x) {
    const int i = sparse_[x];
    if (i < 0 or i >= size_ or dense_[i] != x) return;
    const auto y = dense_[--size_];
    dense_[i] = y;
    sparse_[y] = i;
  }

  // O(1)
  inline void clear() { size_ = 0; }
  inline int size() const { return size_; }
  inline bool empty() const { return size_ == 0; }

  // O(size)
  template<class F>
  void for_each(F func) {
    for (int i = 0; i < size_; ++i) {
      func(dense_[i]);
    }
  }

  using iterator = int *;
  using const_iterator = const int *;
  iterator begin() { return dense_; }
  const_iterator begin() const { return dense_; }
  iterator end() { return dense_ + size_; }
  const_iterator end() const { return dense_ + size_; }

  // For silencing warnings on accessing uninitialized memory.
  void safe_init() {
    dense_.fill(0);
    sparse_.fill(0);
  }
};
using IntSet = std::bitset<4000>;

auto solve() -> Int {
  Int n = in, X = in;
  // Y = wall, Z = hammer
  vector<Int> Y = in(n), Z = in(n);
  if (X < 0) {
    X *= -1;
    REP(i, n) {
      Y[i] *= -1;
      Z[i] *= -1;
    }
  }
  vector<Int> coords = Y;
  for (Int x: Z) coords.push_back(x);
  coords.push_back(0);
  coords.push_back(X);
  Compressed<Int> cc(coords);
  const int origin_index = cc.index(0);
  const int goal_index = cc.index(X);
  vector<Event> events(cc.size());
  vector<int> hammer_pos(n);
  REP(i, n) {
    Int coord = Y[i];
    int pos = cc.index(coord);
    events[pos].id = i;
    events[pos].coord = coord;
    events[pos].etype = 2;
  }
  REP(i, n) {
    Int coord = Z[i];
    int pos = cc.index(coord);
    events[pos].id = i;
    events[pos].coord = coord;
    events[pos].etype = 3;
    hammer_pos[i] = pos;
  }
  {
    events[origin_index].coord = 0;
    events[origin_index].etype = 0;
    events[goal_index].coord = X;
    events[goal_index].etype = 1;
  }

  auto mincost = vector(cc.size(), vector(cc.size(), kBig<Int>));
  MinHeap<State> que;
  auto push = [&](int pos, int other, Int cost) -> bool {
    if (chmin(mincost[pos][other], cost)) {
      que.push(State{pos, other, cost});
      //DUMP(pos, other, cost, events[pos].coord, events[pos].etype, events[pos].id);
      return true;
    }
    return false;
  };
  push(origin_index, origin_index, 0LL);

  while (not que.empty()) {
    State cur = que.top();
    que.pop();
    if (cur.cost != mincost[cur.pos][cur.other]) continue;
    if (cur.pos == goal_index) {
      return cur.cost;
    }
    assert(cur.pos < goal_index);
    Int cur_coord = cc.value(cur.pos);
    if (cur.pos >= origin_index) {
      if (int to = cur.pos + 1; to < cc.size()) {
        Int coord = cc.value(to);
        Int delta = abs(coord - cur_coord);
        Event ev = events[to];
        if (ev.etype != 2 or (cur.other <= hammer_pos[ev.id] and hammer_pos[ev.id] <= cur.pos)) {
          push(cur.pos + 1, cur.other, cur.cost + delta);
        }
      }
      if (int to = cur.other - 1; to >= 0) {
        Int coord = cc.value(to);
        Int delta = abs(coord - cur_coord);
        Event ev = events[to];
        if (ev.etype != 2 or (cur.other <= hammer_pos[ev.id] and hammer_pos[ev.id] <= cur.pos)) {
          push(to, cur.pos, cur.cost + delta);
        }
      }
    } else {
      if (int to = cur.other + 1; to < cc.size()) {
        Int coord = cc.value(to);
        Int delta = abs(coord - cur_coord);
        Event ev = events[to];
        if (ev.etype != 2 or (cur.pos <= hammer_pos[ev.id] and hammer_pos[ev.id] <= cur.other)) {
          push(to, cur.pos, cur.cost + delta);
        }
      }
      if (int to = cur.pos - 1; to >= 0) {
        Int coord = cc.value(to);
        Int delta = abs(coord - cur_coord);
        Event ev = events[to];
        if (ev.etype != 2 or (cur.pos <= hammer_pos[ev.id] and hammer_pos[ev.id] <= cur.other)) {
          push(to, cur.other, cur.cost + delta);
        }
      }
    }
  }
  return -1;
}

int main() {
  init_();
  const int T = 1;//in;
  REP(t, T) {
    test_case(t, T);
    out(solve());
  }
  exit_();
}

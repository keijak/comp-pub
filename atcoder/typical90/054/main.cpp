#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

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
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
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

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);

  int n, m;
  cin >> n >> m;
  vector<int> tak(n, -1);  // takahashi number
  tak[0] = 0;
  vector<vector<int>> R(m);    // researcher
  vector<vector<int>> r2p(n);  // researcher -> paper
  REP(i, m) {
    int k;
    cin >> k;
    R[i].resize(k);
    REP(j, k) {
      cin >> R[i][j];
      --R[i][j];
      r2p[R[i][j]].push_back(i);
    }
  }
  set<int> paper_done;
  deque<int> pq;  // paper queue
  for (auto paper : r2p[0]) {
    if (not paper_done.count(paper)) {
      paper_done.insert(paper);
      pq.push_back(paper);
    }
  }
  for (int i = 0;; ++i) {
    DUMP(i, paper_done, pq, tak);
    if (pq.empty()) break;
    set<int> next_pq;
    while (not pq.empty()) {
      int paper = pq.front();
      pq.pop_front();
      for (auto r : R[paper]) {
        if (tak[r] < 0) {
          tak[r] = i + 1;
          for (auto p : r2p[r]) {
            if (paper_done.count(p)) continue;
            paper_done.insert(p);
            next_pq.insert(p);
          }
        }
      }
    }
    for (auto p : next_pq) {
      pq.push_back(p);
    }
  }
  REP(i, n) { cout << tak[i] << "\n"; }
}

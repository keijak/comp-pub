#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;

template<typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template<typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template<typename T>
inline int ssize(const T &a) {
  return (int) a.size();
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  if constexpr (std::is_same_v<T, bool>) {
    std::cout << (x ? "Yes" : "No") << "\n";
  } else {
    std::cout << x << "\n";
  }
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

template<typename Container>
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

template<typename T, typename = void>
struct is_iterable : std::false_type {};
template<typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {
};

template<typename T, typename = std::enable_if_t<
    is_iterable<T>::value && !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

struct Input {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct SizedInput {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  SizedInput operator()(std::size_t n) const { return {n}; }
} const in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

struct Event {
  int model;
  bool maintenance;
  int added_time;
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  const int n = in, m = in;
  vector<pair<i64, i64>> trains(n);
  for (auto&[x, y]: trains) {
    x = i64(in);
    y = i64(in);
  }
  vector<vector<Event>> days(m);
  vector<optional<int>> added_time(n, nullopt);
  set<int> in_maintenance;
  REP(d, m) {
    for (const auto &e: days[d]) {
      if (not added_time[e.model].has_value() or added_time[e.model].value() != e.added_time) {
        continue;
      }
      auto[x, y] = trains[e.model];
      if (e.maintenance) {
        in_maintenance.insert(e.model);
        if (d + y < m) {
          days[d + y].push_back(Event{e.model, false, e.added_time});
        }
      } else {
        in_maintenance.erase(e.model);
        if (d + x < m) {
          days[d + x].push_back(Event{e.model, true, e.added_time});
        }
      }
    }
    int op = in, t = in;
    --t;
    if (op == 1) {
      added_time[t] = d;
      auto[x, y] = trains[t];
      if (d + x < m) {
        days[d + x].push_back(Event{t, true, d});
      }
    } else {
      in_maintenance.erase(t);
      added_time[t] = nullopt;
    }
    print(in_maintenance.size());
  }
}
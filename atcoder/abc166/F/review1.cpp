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
  return (int) std::size(a);
}

template<typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template<typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
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
    is_iterable<T>::value &&
        !std::is_same<T, std::string_view>::value &&
        !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", (os << "{")) << "}";
}

void print() { std::cout << "\n"; }
template<class T>
void print(const T &x) {
  std::cout << x << "\n";
}
template<typename Head, typename... Tail>
void print(const Head &head, Tail... tail) {
  std::cout << head << " ";
  print(tail...);
}

void read_from_cin() {}
template<typename T, typename... Ts>
void read_from_cin(T &value, Ts &...args) {
  std::cin >> value;
  read_from_cin(args...);
}
#define INPUT(type, ...) \
  type __VA_ARGS__;      \
  read_from_cin(__VA_ARGS__)

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#else
#define DUMP(...)
#endif

using namespace std;

inline bool contains(const string &s, char c) {
  return s.find(c) != string::npos;
}

auto solve() -> optional<vector<char>> {
  INPUT(int, n, a, b, c);
  array<i64, 3> v = {a, b, c};
  vector<char> ans(n, '?');
  vector<string> s(n);
  REP(i, n) {
    cin >> s[i];
  }

  REP(i, n) {
    int k0 = s[i][0] - 'A';
    int k1 = s[i][1] - 'A';
    assert(k0 != k1);

    if (v[k0] == 0 and v[k1] == 0) {
      return nullopt;
    }
    if (v[k0] == 0) {
      ans[i] = s[i][0];
      v[k0]++;
      v[k1]--;
      continue;
    }
    if (v[k1] == 0) {
      ans[i] = s[i][1];
      v[k1]++;
      v[k0]--;
      continue;
    }

    if (i == n - 1) {
      ans[i] = s[i][0];
      break;
    }
    if (s[i] == s[i + 1]) {
      ans[i] = s[i][0];
      ans[i + 1] = s[i][1];
      ++i;
      continue;
    }

    int t0 = s[i + 1][0] = 'A';
    int t1 = s[i + 1][1] = 'A';
    if (k0 == t0 or k0 == t1) {
      ans[i] = s[i][0];
      v[k0]++;
      v[k1]--;
    } else {
      ans[i] = s[i][0];
      v[k0]++;
      v[k1]--;
    }
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << std::fixed << std::setprecision(15);
  int t = 1;
  REP(test_case, t) {
    auto ans = solve();
    if (not ans) {
      print("No");
    } else {
      print("Yes");
      for (char c : *ans) {
        print(c);
      }
    }
  }
}

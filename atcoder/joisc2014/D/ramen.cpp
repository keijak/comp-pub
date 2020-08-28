#include "ramen.h"

#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

template <typename T>
using V = std::vector<T>;
template <typename T>
vector<T> make_vec(size_t n, T a) {
  return vector<T>(n, a);
}
template <typename... Ts>
auto make_vec(size_t n, Ts... ts) {
  return vector<decltype(make_vec(ts...))>(n, make_vec(ts...));
}
template <typename Container>
ostream &pprint(const Container &a, string_view sep = " ",
                string_view ends = "\n", ostream *os = nullptr) {
  if (os == nullptr) os = &cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
}
template <typename T>
ostream &operator<<(ostream &os, const vector<T> &a) {
  return pprint(a, ", ", "", &(os << "[")) << "]";
}
template <typename T>
ostream &operator<<(ostream &os, const set<T> &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const map<T, U> &a) {
  return pprint(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
ostream &operator<<(ostream &os, const pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef ENABLE_DEBUG
template <typename T>
void debug(T value) {
  cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                              \
  do {                                          \
    cerr << " \033[33m (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ":\033[0m ";        \
    debug(__VA_ARGS__);                         \
    cerr << endl;                               \
  } while (0)
#else
#define debug(...)
#define DEBUG(...)
#endif

void Ramen(int N) {
  V<int> v(N);
  REP(i, N) v[i] = i;
  if (N % 2 == 1) {
    v.push_back(v[N - 1]);
  }

  for (int i = 0; i < N; i += 2) {
    if (v[i] == v[i + 1]) continue;
    int res = Compare(v[i], v[i + 1]);
    if (res == 1) {  // v[i] is more kotteri
      swap(v[i], v[i + 1]);
    }
  }

  // Merge
  while (v.size() > 2) {
    DEBUG(v.size(), v);
    assert(v.size() % 2 == 0);

    V<int> nex;
    nex.reserve(v.size());

    for (int i = 0; i < (int)v.size(); i += 4) {
      if (i + 2 == (int)v.size()) {
        nex.push_back(v[i]);
        nex.push_back(v[i + 1]);
        break;
      }
      assert(i + 4 <= (int)v.size());
      int res_min = Compare(v[i], v[i + 2]);
      nex.push_back(res_min == 1 ? v[i + 2] : v[i]);
      int res_max = Compare(v[i + 1], v[i + 3]);
      nex.push_back(res_max == 1 ? v[i + 1] : v[i + 3]);
    }
    swap(nex, v);
  }
  DEBUG(v);
  Answer(v[0], v[1]);
}

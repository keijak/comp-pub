#include <bits/stdc++.h>

#include <cassert>
typedef long long int ll;
using namespace std;

// @@ !! LIM(debug)
// --> f:<< debug
// ---- inserted function << from util.cc
template <typename T1, typename T2>
ostream& operator<<(ostream& os, const pair<T1, T2>& p) {
  os << "(" << p.first << ", " << p.second << ")";
  return os;
}

template <typename T1, typename T2, typename T3>
ostream& operator<<(ostream& os, const tuple<T1, T2, T3>& t) {
  os << "(" << get<0>(t) << ", " << get<1>(t) << ", " << get<2>(t) << ")";
  return os;
}

template <typename T1, typename T2, typename T3, typename T4>
ostream& operator<<(ostream& os, const tuple<T1, T2, T3, T4>& t) {
  os << "(" << get<0>(t) << ", " << get<1>(t) << ", " << get<2>(t) << ", "
     << get<3>(t) << ")";
  return os;
}

template <typename T>
ostream& operator<<(ostream& os, const vector<T>& v) {
  os << '[';
  for (auto it = v.begin(); it != v.end(); it++) {
    if (it != v.begin()) os << ", ";
    os << *it;
  }
  os << ']';

  return os;
}

template <typename T, typename C>
ostream& operator<<(ostream& os, const set<T, C>& v) {
  os << '{';
  for (auto it = v.begin(); it != v.end(); it++) {
    if (it != v.begin()) os << ", ";
    os << *it;
  }
  os << '}';

  return os;
}

template <typename T, typename C>
ostream& operator<<(ostream& os, const unordered_set<T, C>& v) {
  os << '{';
  for (auto it = v.begin(); it != v.end(); it++) {
    if (it != v.begin()) os << ", ";
    os << *it;
  }
  os << '}';

  return os;
}

template <typename T, typename C>
ostream& operator<<(ostream& os, const multiset<T, C>& v) {
  os << '{';
  for (auto it = v.begin(); it != v.end(); it++) {
    if (it != v.begin()) os << ", ";
    os << *it;
  }
  os << '}';

  return os;
}

template <typename T1, typename T2, typename C>
ostream& operator<<(ostream& os, const map<T1, T2, C>& mp) {
  os << '[';
  for (auto it = mp.begin(); it != mp.end(); it++) {
    if (it != mp.begin()) os << ", ";
    os << it->first << ": " << it->second;
  }
  os << ']';

  return os;
}

template <typename T1, typename T2, typename C>
ostream& operator<<(ostream& os, const unordered_map<T1, T2, C>& mp) {
  os << '[';
  for (auto it = mp.begin(); it != mp.end(); it++) {
    if (it != mp.begin()) os << ", ";
    os << it->first << ": " << it->second;
  }
  os << ']';

  return os;
}

template <typename T, typename T2>
ostream& operator<<(ostream& os, const queue<T, T2>& orig) {
  queue<T, T2> que(orig);
  bool first = true;
  os << '[';
  while (!que.empty()) {
    T x = que.front();
    que.pop();
    if (!first) os << ", ";
    os << x;
    first = false;
  }
  return os << ']';
}

template <typename T, typename T2>
ostream& operator<<(ostream& os, const deque<T, T2>& orig) {
  deque<T, T2> que(orig);
  bool first = true;
  os << '[';
  while (!que.empty()) {
    T x = que.front();
    que.pop_front();
    if (!first) os << ", ";
    os << x;
    first = false;
  }
  return os << ']';
}

template <typename T, typename T2, typename T3>
ostream& operator<<(ostream& os, const priority_queue<T, T2, T3>& orig) {
  priority_queue<T, T2, T3> pq(orig);
  bool first = true;
  os << '[';
  while (!pq.empty()) {
    T x = pq.top();
    pq.pop();
    if (!first) os << ", ";
    os << x;
    first = false;
  }
  return os << ']';
}

template <typename T>
ostream& operator<<(ostream& os, const stack<T>& st) {
  stack<T> tmp(st);
  os << '[';
  bool first = true;
  while (!tmp.empty()) {
    T& t = tmp.top();
    if (first)
      first = false;
    else
      os << ", ";
    os << t;
    tmp.pop();
  }
  os << ']';
  return os;
}

#if __cplusplus >= 201703L
template <typename T>
ostream& operator<<(ostream& os, const optional<T>& t) {
  if (t.has_value())
    os << "v(" << t.value() << ")";
  else
    os << "nullopt";
  return os;
}
#endif

ostream& operator<<(ostream& os, int8_t x) {
  os << (int32_t)x;
  return os;
}

// ---- end <<
// ---- inserted library file debug.cc
template <class... Args>
string dbgFormat(const char* fmt, Args... args) {
  size_t len = snprintf(nullptr, 0, fmt, args...);
  char buf[len + 1];
  snprintf(buf, len + 1, fmt, args...);
  return string(buf);
}

template <class Head>
void dbgLog(Head&& head) {
  cerr << head << endl;
}

template <class Head, class... Tail>
void dbgLog(Head&& head, Tail&&... tail) {
  cerr << head << " ";
  dbgLog(forward<Tail>(tail)...);
}

#if DEBUG
#define DLOG(...) dbgLog(__VA_ARGS__)
#define DFMT(...) cerr << dbgFormat(__VA_ARGS__) << endl
#define DCALL(func, ...) func(__VA_ARGS__)
#else
#define DLOG(...)
#define DFMT(...)
#define DCALL(func, ...)
#endif

#if DEBUG_LIB
#define DLOG_LIB(...) dbgLog(__VA_ARGS__)
#define DFMT_LIB(...) cerr << dbgFormat(__VA_ARGS__) << endl
#define DCALL_LIB(func, ...) func(__VA_ARGS__)
#else
#define DLOG_LIB(...)
#define DFMT_LIB(...)
#define DCALL_LIB(func, ...)
#endif

// ---- end debug.cc
// @@ !! LIM  -- end mark --

int main(/* int argc, char *argv[] */) {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << setprecision(20);

  ll n;
  cin >> n;
  vector<ll> A(n), B(n);
  for (ll i = 0; i < n; i++) cin >> A[i] >> B[i];
  vector<bool> seen(n + 1);
  vector<ll> ord;
  vector<ll> last_pos(n + 1);
  for (ll i = n - 1; i >= 0; i--) {
    ll p = A[i];
    if (seen[p]) continue;
    seen[p] = true;
    ord.push_back(p);
    last_pos[p] = i;
  }
  ord.push_back(-1);  // centinel
  reverse(ord.begin(), ord.end());
  vector<map<ll, ll>> info(n + 1);
  ll o = 0;
  for (ll i = 0; i < n; i++) {
    ll p = A[i];
    if (info[p].find(o) == info[p].end()) {
      info[p][o] = i;
    }
    if (last_pos[p] == i) {
      assert(ord[o + 1] == p);
      o++;
    }
  }

  vector<bool> present(n + 1, true);
  ll q;
  cin >> q;
  for (ll qqq = 0; qqq < q; qqq++) {
    ll k;
    cin >> k;
    vector<ll> L(k);
    for (ll i = 0; i < k; i++) {
      ll l;
      cin >> l;
      L[i] = l;
      present[l] = false;
    }
    auto pres = [&]() -> pair<ll, ll> {
      ll ret0 = -1;
      ll ret1 = -1;
      DLOG("ord=", ord);
      // assert((ll)ord.size() == n + 1);
      for (ll i = (ll)ord.size() - 1; i > 0; i--) {  // excl. centinel
        ll p = ord[i];
        if (present[p]) {
          if (ret0 < 0)
            ret0 = i;
          else if (ret1 < 0) {
            ret1 = i;
            break;
          }
        }
      }
      return make_pair(ret0, ret1);
    };
    auto [o_top, o_nxt] = pres();
    if (o_top < 0) {
      cout << "0 0\n";
    } else {
      ll o_start = o_nxt < 0 ? 0 : o_nxt;
      ll top = ord[o_top];
      const auto& mp = info[top];
      auto it = mp.lower_bound(o_start);
      assert(it != mp.end());
      ll i = it->second;
      cout << A[i] << " " << B[i] << "\n";
    }

    for (ll i = 0; i < k; i++) {
      present[L[i]] = true;
    }
  }

  return 0;
}

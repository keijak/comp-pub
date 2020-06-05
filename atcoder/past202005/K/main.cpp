//#define DEBUGGING  // Enables DEBUG macro.
#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
using u64 = unsigned long long;
#define REP(i, n) for (int i = 0; (i64)(i) < (i64)(n); ++i)

#ifndef DEBUGGING
#define DEBUG(...)
#else
template <typename T>
void debug(T value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void debug(T value, Ts... args) {
  std::cerr << value << ", ";
  debug(args...);
}
#define DEBUG(...)                     \
  do {                                 \
    cerr << " (L" << __LINE__ << ") "; \
    cerr << #__VA_ARGS__ << ": ";      \
    debug(__VA_ARGS__);                \
    cerr << endl;                      \
  } while (0)
#endif

struct clist {
  int id;
  clist* next;

  explicit clist(int n) : id(n), next(nullptr) {}

  //   void detach(clist* root, clist* x) {}
};

struct desk {
  int id;
  vector<clist*> skip;
  unordered_map<int, int> offsets;
  list<clist*> tail;
  explicit desk(int n) : id(n) {}

  bool empty() const { return skip.empty() && tail.empty(); }

  list<clist*> detach(int x) {
    auto it = offsets.find(x);
    if (it != offsets.end()) {
      int j = it->second;
      if (j >= int(skip.size()) || skip[j]->id != x) {
        offsets.erase(it);
      } else {
        DEBUG("DETACH with skip!");
        list<clist*> res;
        res.push_back(skip[j]);
        res.splice(res.end(), tail);
        tail.clear();
        skip.resize(j);
        if (j > 0) {
          clist* p = skip[j - 1];
          p->next = nullptr;
        }
        return res;
      }
    }
    DEBUG("DETACH without skip!");

    while (!tail.empty()) {
      clist* p = tail.front();
      if (p->id == x) {
        list<clist*> res;
        swap(res, tail);
        return res;
      }
      tail.pop_front();
      offsets[p->id] = skip.size();
      skip.push_back(p);
      while (p->next != nullptr) {
        if (p->next->id == x) {
          list<clist*> res;
          swap(res, tail);
          res.push_front(p->next);
          p->next = nullptr;
          return res;
        }
        p = p->next;
        offsets[p->id] = skip.size();
        skip.push_back(p);
      }
    }
    assert(false);
    return {};
  }

  void attach(list<clist*> c) { tail.splice(tail.end(), move(c)); }

  void dump(vector<int>& ans) {
    if (!skip.empty()) {
      clist* p = skip[0];
      while (p != nullptr) {
        ans[p->id] = this->id + 1;
        p = p->next;
      }
    }
    for (clist* p : tail) {
      while (p != nullptr) {
        ans[p->id] = this->id + 1;
        p = p->next;
      }
    }
  }

  void pr() {
    if (empty()) {
      cerr << "(EMPTY)" << endl;
      return;
    }
    if (!skip.empty()) {
      clist* p = skip[0];
      while (p != nullptr) {
        cerr << " " << p->id;
        p = p->next;
      }
    }
    for (clist* p : tail) {
      cerr << " | ";
      while (p != nullptr) {
        cerr << " " << p->id;
        p = p->next;
      }
    }
    cerr << endl;
  }
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, Q;
  cin >> N >> Q;

  vector<desk*> desks(N);
  REP(i, N) {
    desks[i] = new desk(i);
    desks[i]->tail.push_back(new clist(i));
  }

  REP(q, Q) {
    int f, t, x;
    cin >> f >> t >> x;
    f--;
    t--;
    x--;
    assert(f != t);

    // REP(j, N) {
    //   cerr << "desk " << j << ": ";
    //   desks[j]->pr();
    // }

    DEBUG("detach", f, x);
    auto xc = desks[f]->detach(x);
    // REP(j, N) {
    //   cerr << "desk " << j << ": ";
    //   desks[j]->pr();
    // }
    DEBUG("attach", t, x);
    desks[t]->attach(move(xc));
  }

  //   REP(j, N) {
  //     cerr << "desk " << j << ": ";
  //     desks[j]->pr();
  //   }

  vector<int> ans(N);
  REP(i, N) { desks[i]->dump(ans); }
  REP(i, N) { cout << ans[i] << '\n'; }
}

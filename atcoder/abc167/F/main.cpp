#include <bits/stdc++.h>
#define REP(i, n) for (decltype(n) i = 0; i < (n); ++i)
using namespace std;
using int64 = long long;

const string YES = "Yes";
const string NO = "No";

bool solve(int N, std::vector<std::string> S) {
  vector<int> lonly, ronly, lboth, rboth;
  vector<pair<int, int>> lr(N);

  REP(i, N) {
    int l = 0, r = 0;
    for (char c : S[i]) {
      if (c == '(') {
        l++;
      } else if (l > 0) {
        l--;
      } else {
        r++;
      }
    }
    lr[i] = make_pair(l, r);
    if (l == 0 && r == 0) {
      continue;
    } else if (r == 0) {
      lonly.push_back(i);
    } else if (l == 0) {
      ronly.push_back(i);
    } else if (l > r) {
      lboth.push_back(i);
    } else {
      rboth.push_back(i);
    }
  }
  sort(lboth.begin(), lboth.end(),
       [&](int i, int j) -> bool { return lr[i].second < lr[j].second; });
  sort(rboth.begin(), rboth.end(),
       [&](int i, int j) -> bool { return lr[i].first < lr[j].first; });

  int64 lstack = 0;
  for (int i : lonly) {
    assert(lr[i].second == 0);
    lstack += lr[i].first;
  }
  REP(i, lboth.size()) {
    int j = lboth[i];
    auto [l, r] = lr[j];
    if (r > lstack) {
      return false;
    }
    lstack += l - r;
  }

  int64 rstack = 0;
  for (int i : ronly) {
    assert(lr[i].first == 0);
    rstack += lr[i].second;
  }
  REP(i, rboth.size()) {
    int j = rboth[i];
    auto [l, r] = lr[j];
    if (l > rstack) {
      return false;
    }
    rstack += r - l;
  }

  return lstack == rstack;
}

int main() {
  int N;
  cin >> N;
  cin.ignore();
  std::vector<std::string> S(N);
  for (int i = 0; i < N; i++) {
    getline(cin, S[i]);
  }
  if (solve(N, std::move(S))) {
    cout << YES << endl;
  } else {
    cout << NO << endl;
  }
}

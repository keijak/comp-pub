#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

#include <cctype>
#include <cstdio>
#include <iostream>
#include <stack>
#include <type_traits>
#include <vector>

struct Input {
  inline operator int() { return read_int<int>(); }
  inline operator long long() { return read_int<long long>(); }
  inline operator unsigned() { return read_int<unsigned>(); }

 private:
  template <typename T>
  static T read_int() {
    T ret = 0, sgn = 1;
    int ch = getchar_unlocked();
    while (isspace(ch)) {
      ch = getchar_unlocked();
    }
    if constexpr (!std::is_unsigned<T>::value) {
      if (ch == '-') {
        sgn = -1;
        ch = getchar_unlocked();
      }
    }
    for (; isdigit(ch); ch = getchar_unlocked()) {
      ret = (ret * 10) + (ch - '0');
    }
    ungetc(ch, stdin);
    if constexpr (std::is_unsigned<T>::value) {
      return ret;
    } else {
      return ret * sgn;
    }
  }
} input;

using namespace std;

template <typename Monoid>
struct SWAGQueue {
  using T = typename Monoid::T;

  struct Node {
    T val, agg;
    Node(T val, T agg) : val(std::move(val)), agg(std::move(agg)) {}
  };
  std::stack<Node> front_stack, back_stack;

  SWAGQueue() = default;

  bool empty() const { return front_stack.empty() and back_stack.empty(); }

  size_t size() const { return front_stack.size() + back_stack.size(); }

  void push_back(const T &x) {
    if (back_stack.empty()) {
      back_stack.emplace(x, x);
    } else {
      back_stack.emplace(x, Monoid::op(back_stack.top().agg, x));
    }
  }

  void pop_front() {
    if (front_stack.empty()) {
      front_stack.emplace(back_stack.top().val, back_stack.top().val);
      back_stack.pop();
      while (!back_stack.empty()) {
        T agg = Monoid::op(back_stack.top().val, front_stack.top().agg);
        front_stack.emplace(back_stack.top().val, std::move(agg));
        back_stack.pop();
      }
    }
    front_stack.pop();
  }

  T fold() const {
    if (empty()) return Monoid::id();
    if (front_stack.empty()) return back_stack.top().agg;
    if (back_stack.empty()) return front_stack.top().agg;
    return Monoid::op(front_stack.top().agg, back_stack.top().agg);
  }
};

struct Min {
  using T = pair<char, int>;
  static T op(const T &x, const T &y) { return std::min(x, y); }
  static constexpr T id() {
    return pair(std::numeric_limits<char>::max(),
                std::numeric_limits<int>::max());
  }
};

int main() {
  const int n = input;
  const int k = input;
  vector<char> s(n);
  for (int i = 0; i < n; ++i) {
    char ch;
    do {
      ch = getchar_unlocked();
    } while (isspace(ch));
    s[i] = ch;
  };
  int head = 0;
  int tail = n - k + 1;
  SWAGQueue<Min> que;
  for (int i = 0; i < tail; ++i) {
    que.push_back(pair(s[i], i));
  }
  vector<char> ans(k);
  for (int i = 0; i < k; ++i) {
    auto [ch, j] = que.fold();
    ans[i] = ch;
    while (head <= j) {
      que.pop_front();
      ++head;
    }
    if (tail < n) {
      que.push_back(pair(s[tail], tail));
      ++tail;
    }
  }
  for (char ch : ans) {
    putchar_unlocked(ch);
  }
  putchar_unlocked('\n');
}

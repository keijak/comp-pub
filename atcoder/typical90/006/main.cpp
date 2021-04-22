#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

#include <cctype>
#include <cstdio>
#include <iostream>
#include <optional>
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

template <class T, class Container = std::vector<T>,
          class Compare = std::greater<typename Container::value_type>>
struct SlideMinQueue {
  Container vals;
  Compare compare;              // comparison funcation.
  int left, right;              // [left, right)
  std::deque<int> index_queue;  // indices where min values are stored.

  SlideMinQueue() : compare(), left(0), right(0) {}

  explicit SlideMinQueue(Container v)
      : vals(std::move(v)), compare(), left(0), right(0) {}

  // Shifts the window to the right.
  // Sets `left` to `l`, and `right` to `r`.
  void slide(int l, int r) {
    r = min(r, (int)vals.size());
    pop_left(l);
    left = l;
    for (int i = right; i < r; ++i) {
      push_right(i);
    }
    right = r;
  }

  // Returns the minimum value in [left, right).
  std::pair<T, int> fold() const {
    int i = index_queue.front();
    return {vals[i], i};
  }

  bool empty() const { return index_queue.empty(); }

 private:
  // Enqueues the i-th value.
  void push_right(int i) {
    while (!index_queue.empty() && compare(vals[index_queue.back()], vals[i])) {
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
  int tail = n - k + 1;
  SlideMinQueue<char> que(s);
  que.slide(0, tail);
  vector<char> ans(k);
  for (int i = 0; i < k; ++i) {
    auto [ch, j] = que.fold();
    ans[i] = ch;
    que.slide(j + 1, ++tail);
  }
  for (char ch : ans) {
    putchar_unlocked(ch);
  }
  putchar_unlocked('\n');
}

#include <cctype>
#include <cstdio>
#include <deque>
#include <iostream>
#include <optional>
#include <stack>
#include <type_traits>
#include <vector>

template <typename T>
inline void read_unsigned(T& ret) {
  // assert(ret == 0);  // assuming ret is zero-initialized.
  int ch = getchar_unlocked();
  for (; isspace(ch); ch = getchar_unlocked())
    ;
  for (; isdigit(ch); ch = getchar_unlocked()) {
    ret = (ret * 10) + (ch - '0');
  }
  // ungetc(ch, stdin);  // assuming ch is an ignorable whitespace
}

using namespace std;

int N, K;
char s[100005];

struct SlideMinQueue {
  int left, right;              // [left, right)
  std::deque<int> index_queue;  // indices where min values are stored.

  SlideMinQueue() : left(0), right(0), index_queue{} {}

  // Shifts the window to the right.
  // Sets `left` to `l`, and `right` to `r`.
  void slide(int l, int r) {
    r = min(r, N);

    while (!index_queue.empty() && index_queue.front() < l) {
      index_queue.pop_front();
    }
    left = l;

    for (int i = right; i < r; ++i) {
      while (!index_queue.empty() && s[index_queue.back()] > s[i]) {
        index_queue.pop_back();
      }
      index_queue.emplace_back(i);
    }
    right = r;
  }

  // Returns the minimum value in [left, right).
  int fold() const { return index_queue.front(); }

  bool empty() const { return index_queue.empty(); }
};

int main() {
  read_unsigned(N);
  read_unsigned(K);
  char ch;
  do {
    ch = getchar_unlocked();
  } while (isspace(ch));
  for (int i = 0; i < N; ++i, ch = getchar_unlocked()) {
    s[i] = ch;
  };
  int tail = N - K + 1;
  SlideMinQueue que;
  que.slide(0, tail);
  for (int i = 0; i < K; ++i) {
    int j = que.fold();
    putchar_unlocked(s[j]);
    que.slide(j + 1, ++tail);
  }
  putchar_unlocked('\n');
}

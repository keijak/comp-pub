#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), END_##i = (b); i < END_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using Int = long long;
using Real = long double;

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
inline void check(bool cond, const char *message = "!ERROR!") {
  if (not cond) {
    std::cout.flush(), std::cerr.flush();
    throw std::runtime_error(message);
  }
}

template<class T>
inline std::ostream &print_one(const T &x, char endc) {
  if constexpr (std::is_same_v<T, bool>) {
    return std::cout << (x ? "Yes" : "No") << endc;
  } else {
    return std::cout << x << endc;
  }
}
template<class T>
inline std::ostream &print(const T &x) { return print_one(x, '\n'); }
template<typename T, typename... Ts>
std::ostream &print(const T &head, Ts... tail) {
  return print_one(head, ' '), print(tail...);
}
inline std::ostream &print() { return std::cout << '\n'; }

template<typename Container>
std::ostream &print_seq(const Container &a, const char *sep = " ",
                        const char *ends = "\n",
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

struct CastInput {
  template<typename T>
  operator T() const {
    T x;
    std::cin >> x;
    return x;
  }
  struct Sized {
    std::size_t n;
    template<typename T>
    operator T() const {
      T x(n);
      for (auto &e: x) std::cin >> e;
      return x;
    }
  };
  Sized operator()(std::size_t n) const { return {n}; }
} in;

#ifdef MY_DEBUG
#include "debug_dump.hpp"
#include "backward.hpp"
backward::SignalHandling kSignalHandling;
#else
#define DUMP(...)
#define cerr if(false)std::cerr
#endif

using namespace std;

template<typename T>
struct PersistentStack {
  struct Node;
  using NodePtr = std::shared_ptr<Node>;

  struct Node {
    T val;
    NodePtr next;
    Node(T val, NodePtr next) : val(std::move(val)), next(std::move(next)) {}
  };

  PersistentStack() : node_{}, reversed_(false), size_(0) {}

  PersistentStack push(T item) const {
    resolve_reverse();
    return PersistentStack(std::make_shared<Node>(std::move(item), node_), false, size_ + 1);
  }

  PersistentStack pop() const {
    assert(not empty());
    resolve_reverse();
    return PersistentStack(node_->next, false, size_ - 1);
  }

  optional<T> top() const {
    if (empty()) return nullopt;
    resolve_reverse();
    return node_->val;
  }

  bool empty() const {
    return node_ == nullptr;
  }

  int size() const {
    return size_;
  }

  const PersistentStack &reverse() const {
    if (reverse_stack_ == nullptr) {
      reverse_stack_.reset(new PersistentStack(node_, not reversed_, size_));
    }
    return *reverse_stack_;
  }

 private:
  mutable NodePtr node_;
  mutable bool reversed_;
  mutable std::shared_ptr<PersistentStack<T>> reverse_stack_;
  int size_;

  PersistentStack(NodePtr node, bool reversed, int size)
      : node_(std::move(node)), reversed_(reversed), size_(size) {}

  void resolve_reverse() const {
    if (not reversed_) return;
    NodePtr rev;
    for (Node *p = node_.get(); p != nullptr; p = p->next.get()) {
      rev = std::make_shared<Node>(p->val, std::move(rev));
    }
    std::swap(node_, rev);
    reversed_ = not reversed_;
  }
};

template<typename T>
struct PersistentQueue {
  mutable PersistentStack<T> front_, rear_;

  PersistentQueue() = default;

  PersistentQueue push(T item) const {
    return PersistentQueue(front_, rear_.push(item));
  }

  PersistentQueue pop() const {
    if (not front_.empty()) {
      return PersistentQueue(front_.pop(), rear_);
    }
    assert(not rear_.empty());
    return PersistentQueue(rear_.reverse(), PersistentStack<T>());
  }

  optional<T> top() const {
    if (empty()) return nullopt;
    if (front_.empty()) {
      front_ = rear_.reverse();
      rear_ = PersistentStack<T>();
    }
    return front_.top();
  }

  bool empty() const {
    return front_.empty() and rear_.empty();
  }

  int size() const {
    return front_.size() + rear_.size();
  }

 private:
  PersistentQueue(PersistentStack<T> f, PersistentStack<T> r) : front_(std::move(f)), rear_(std::move(r)) {}
};

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  const int Q = in;
  vector<PersistentQueue<int>> qs(Q + 1);
  REP(i, Q) {
    const int qtype = in;
    if (qtype == 0) {  // push
      const int t = int(in) + 1, x = in;
      qs[i + 1] = qs[t].push(x);
    } else {  // pop
      const int t = int(in) + 1;
      print(qs[t].top().value());
      qs[i + 1] = qs[t].pop();
    }
  }
}

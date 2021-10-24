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
// immutable stack interface
template<typename T>
struct PersistentStack {
  virtual ~PersistentStack() = default;

  virtual PersistentStack<T> *push(T item) const = 0;

  virtual PersistentStack<T> *pop() const = 0;

  virtual std::optional<T> top() const = 0;

  virtual int size() const = 0;

  bool empty() const { return size() == 0; }

 private:
  mutable std::optional<PersistentStack<T> *> rev_;

  template<typename U>
  friend PersistentStack<U> *reverse_stack(PersistentStack<U> *st);
};

template<typename T>
struct LinkedList : public PersistentStack<T> {
  struct Node;
  using NodePtr = Node *;

  struct Node {
    T val;
    NodePtr next;
    Node(T val, NodePtr next) : val(std::move(val)), next(std::move(next)) {}
  };

  LinkedList() : node_{}, size_(0) {}
  ~LinkedList() override = default;

  static LinkedList<T> *&nil() {
    static LinkedList<T> *kNil = new LinkedList<T>();
    return kNil;
  }

  PersistentStack<T> *push(T item) const override {
    return new LinkedList<T>(new Node(std::move(item), node_), size_ + 1);
  }

  PersistentStack<T> *pop() const override {
    assert(node_ != nullptr);
    return new LinkedList<T>(node_->next, size_ - 1);
  }

  std::optional<T> top() const override {
    if (node_ == nullptr) return std::nullopt;
    return node_->val;
  }

  int size() const override { return size_; }

 private:
  NodePtr node_;
  int size_;

  LinkedList(NodePtr node, int size) : node_(std::move(node)), size_(size) {}
};

template<typename T>
struct ReversedStack : public PersistentStack<T> {
  explicit ReversedStack(PersistentStack<T> *st) : wrapped_(std::move(st)) {}
  ~ReversedStack() override = default;

  PersistentStack<T> *push(T item) const override {
    return evaluate()->push(std::move(item));
  }

  PersistentStack<T> *pop() const override {
    return evaluate()->pop();
  }

  std::optional<T> top() const override { return evaluate()->top(); }

  int size() const override { return wrapped_->size(); }

 private:
  PersistentStack<T> *wrapped_;
  mutable std::optional<PersistentStack<T> *> evaluated_;

  PersistentStack<T> *evaluate() const {
    if (not evaluated_) {
      PersistentStack<T> *rev = LinkedList<T>::nil();
      for (PersistentStack<T> *p = wrapped_; not p->empty(); p = p->pop()) {
        rev = rev->push(*p->top());
      }
      evaluated_ = std::move(rev);
    }
    return *evaluated_;
  }
};

template<typename T>
PersistentStack<T> *reverse_stack(PersistentStack<T> *st) {
  if (not st->rev_) {
    st->rev_ = new ReversedStack<T>(st);
  }
  return *(st->rev_);
}

template<typename T>
struct ConcatenatedStack : public PersistentStack<T> {
  explicit ConcatenatedStack(PersistentStack<T> *st1,
                             PersistentStack<T> *st2)
      : stack1_(std::move(st1)), stack2_(std::move(st2)) {}
  ~ConcatenatedStack() override = default;

  PersistentStack<T> *push(T item) const override {
    return new ConcatenatedStack<T>(stack1_, stack2_->push(item));
  }

  PersistentStack<T> *pop() const override {
    if (stack1_->empty()) return stack2_->pop();
    auto st1 = stack1_->pop();
    if (st1->empty()) return stack2_;
    return new ConcatenatedStack<T>(std::move(st1), stack2_);
  }

  std::optional<T> top() const override {
    if (stack1_->empty()) return stack2_->top();
    return stack1_->top();
  }

  int size() const override { return stack1_->size() + stack2_->size(); }

 private:
  PersistentStack<T> *stack1_, *stack2_;
};

// Banker's Queue
template<typename T>
struct PersistentQueue {
  mutable PersistentStack<T> *front_, *rear_;

  PersistentQueue()
      : front_(LinkedList<T>::nil()), rear_(LinkedList<T>::nil()) {}

  PersistentQueue push(T item) const {
    auto r = rear_->push(item);
    if (r->size() > front_->size() + 1) {
      auto f = new ConcatenatedStack<T>(front_, reverse_stack(std::move(r)));
      return PersistentQueue(std::move(f), LinkedList<T>::nil());
    }
    return PersistentQueue(front_, std::move(r));
  }

  PersistentQueue pop() const {
    if (not front_->empty()) {
      return PersistentQueue(front_->pop(), rear_);
    }
    assert(not rear_->empty());
    return PersistentQueue(reverse_stack(rear_), LinkedList<T>::nil());
  }

  std::optional<T> top() const {
    if (empty()) return std::nullopt;
    if (front_->empty()) {
      front_ = reverse_stack(rear_);
      rear_ = LinkedList<T>::nil();
    }
    return front_->top();
  }

  bool empty() const { return front_->empty() and rear_->empty(); }

  int size() const { return front_->size() + rear_->size(); }

 private:
  PersistentQueue(PersistentStack<T> *f, PersistentStack<T> *r)
      : front_(std::move(f)), rear_(std::move(r)) {}
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

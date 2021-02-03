#include <bits/stdc++.h>
#define REP_(i, a_, b_, a, b, ...) \
  for (int i = (a), _Z_##i = (b); i < _Z_##i; ++i)
#define REP(i, ...) REP_(i, __VA_ARGS__, __VA_ARGS__, 0, __VA_ARGS__)
#define ALL(x) std::begin(x), std::end(x)
using i64 = long long;
using u64 = unsigned long long;

template <typename T, typename U>
inline bool chmax(T &a, U b) {
  return a < b and ((a = std::move(b)), true);
}
template <typename T, typename U>
inline bool chmin(T &a, U b) {
  return a > b and ((a = std::move(b)), true);
}
template <typename T>
inline int ssize(const T &a) {
  return (int)std::size(a);
}

template <typename T>
std::istream &operator>>(std::istream &is, std::vector<T> &a) {
  for (auto &x : a) is >> x;
  return is;
}
template <typename Container>
std::ostream &print_seq(const Container &a, std::string_view sep = " ",
                        std::string_view ends = "\n",
                        std::ostream *os = nullptr) {
  if (os == nullptr) os = &std::cout;
  auto b = std::begin(a), e = std::end(a);
  for (auto it = std::begin(a); it != e; ++it) {
    if (it != b) *os << sep;
    *os << *it;
  }
  return *os << ends;
}
template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())),
                                  decltype(std::end(std::declval<T>()))>>
    : std::true_type {};

template <typename T, typename = std::enable_if_t<
                          is_iterable<T>::value &&
                          !std::is_same<T, std::string_view>::value &&
                          !std::is_same<T, std::string>::value>>
std::ostream &operator<<(std::ostream &os, const T &a) {
  return print_seq(a, ", ", "", &(os << "{")) << "}";
}
template <typename T, typename U>
std::ostream &operator<<(std::ostream &os, const std::pair<T, U> &a) {
  return os << "(" << a.first << ", " << a.second << ")";
}

#ifdef ENABLE_DEBUG
template <typename T>
void pdebug(const T &value) {
  std::cerr << value;
}
template <typename T, typename... Ts>
void pdebug(const T &value, const Ts &...args) {
  pdebug(value);
  std::cerr << ", ";
  pdebug(args...);
}
#define DEBUG(...)                                   \
  do {                                               \
    std::cerr << " \033[33m (L" << __LINE__ << ") "; \
    std::cerr << #__VA_ARGS__ << ":\033[0m ";        \
    pdebug(__VA_ARGS__);                             \
    std::cerr << std::endl;                          \
  } while (0)
#else
#define pdebug(...)
#define DEBUG(...)
#endif

using namespace std;

struct Enemy {
  i64 max_health;
  i64 start_health;
  i64 regen;
  vector<pair<i64, i64>> events;
};

template <typename T>
struct Compress {
  std::vector<T> vec;

  explicit Compress(std::vector<T> v) : vec(v) {
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
  }

  int size() const { return vec.size(); }

  int index(T x) const {
    return lower_bound(vec.begin(), vec.end(), x) - vec.begin();
  }

  const T &value(int i) const { return vec[i]; }
};

const i64 TMAX = 2e9;

i64 solve() {
  int n, m;
  cin >> n >> m;
  i64 bounty, increase, damage;
  cin >> bounty >> increase >> damage;
  bool low_max_exists = false;
  vector<Enemy> enemies(n);
  REP(i, n) {
    i64 max_health, start_health, regen;
    cin >> max_health >> start_health >> regen;
    if (max_health <= damage) {
      low_max_exists = true;
    }
    enemies[i] = {max_health, start_health, regen, {}};
  }

  REP(i, m) {
    i64 time, enemy, health;
    cin >> time >> enemy >> health;
    --enemy;
    enemies[enemy].events.emplace_back(time, health);
  }
  if (low_max_exists and increase > 0) return -1;

  map<i64, i64> tline;

  REP(i, n) {
    i64 sh = enemies[i].start_health;
    enemies[i].events.emplace_back(0, sh);
    sort(ALL(enemies[i].events));

    if (enemies[i].regen == 0) {
      auto [t, h] = enemies[i].events.back();
      if (h <= damage and increase > 0) return -1;
    }
  }

  REP(i, n) {
    const Enemy &enemy = enemies[i];
    int en = ssize(enemy.events);
    REP(j, en) {
      auto [t, h] = enemy.events[j];
      if (h > damage) continue;
      i64 nt = (j < en - 1) ? enemy.events[j + 1].first : TMAX;
      tline[t]++;
      if (enemy.regen == 0) {
        if (j < en - 1) tline[nt]--;
      } else {
        i64 x = (damage - h) / enemy.regen + 1;
        i64 y = (j < en - 1) ? min(t + x, nt) : (t + x);
        tline[y]--;
      }
    }
  }
  DEBUG(tline);
  i64 ans = 0;
  i64 acc = 0;
  for (auto it = tline.begin(); it != tline.end(); ++it) {
    i64 t = it->first - 1;
    i64 gain = bounty + t * increase;
    chmax(ans, gain * acc);

    t = it->first;
    acc += it->second;
    gain = bounty + t * increase;
    chmax(ans, gain * acc);
    // DEBUG(ti, t, tline[ti], gain, bonus, ans);
  }
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false), cin.tie(nullptr);
  cout << solve() << "\n";
}

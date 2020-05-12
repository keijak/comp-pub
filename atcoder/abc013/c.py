n, life = map(int, input().split())
cost1, heal_delta1, cost2, heal_delta2, damage = map(int, input().split())


def f(n, h, c):
    if n <= 0:
        return 0
    k = n // h + int(n % h != 0)
    return k * c


def solve():
    heal1, heal2 = heal_delta1 + damage, heal_delta2 + damage
    necessity = n * damage + 1
    if life >= necessity:
        return 0
    necessity -= life
    ans = f(necessity, heal2, cost2)
    for k in range(1, necessity // heal1 + 2):
        ans = min(ans, k * cost1 + f(necessity - k * heal1, heal2, cost2))
    return ans


print(solve())

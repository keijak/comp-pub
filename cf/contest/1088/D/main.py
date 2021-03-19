import sys


def ask(c, d):
    print("? {} {}".format(c, d))
    return int(input())


def solve_same():
    ans = 0
    for i in range(29, -1, -1):
        print(f">> {i=} {ans=}", file=sys.stderr)
        bit = 1 << i
        res1 = ask(ans ^ bit, ans)
        res2 = ask(ans, ans ^ bit)
        if res1 == -1 and res2 == 1:
            ans |= bit
    return ans


def solve1():
    print("solve1")
    a = b = 0
    c = d = 0
    for i in range(29, -1, -1):
        print(f">> {i=} {a=} {b=} {c=} {d=}", file=sys.stderr)
        bit = 1 << i
        res1 = ask(c ^ bit, d ^ bit)
        if res1 == -1:
            a |= bit
            c |= bit
            res2 = ask(c, d)
        else:
            res2 = ask(c ^ bit, d)
            if res2 == -1:
                a |= bit
                b |= bit
    return a, b


def solve2():
    print("solve2")
    a = b = 0
    c = d = 0
    for i in range(29, -1, -1):
        print(f">> {i=} {a=} {b=} {c=} {d=}", file=sys.stderr)
        bit = 1 << i
        res1 = ask(c ^ bit, d ^ bit)
        if res1 == 1:
            b |= bit
            d |= bit
        else:
            res2 = ask(c, d ^ bit)
            if res2 == 1:
                a |= bit
                b |= bit
    return a, b


base = ask(0, 0)
if base == 0:
    a = solve_same()
    b = a
elif base == 1:
    a, b = solve1()
else:
    a, b = solve2()
print("! {} {}".format(a, b))

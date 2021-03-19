import sys


def ask(c, d):
    print("? {} {}".format(c, d))
    return int(input())


a = b = c = d = 0


def solve(mi, base):
    # print(f"# solve: {mi=} {base=}", file=sys.stderr)

    def solve_same():
        global a, b, c, d
        z = c
        for i in range(mi, -1, -1):
            # print(f">> {i=} {z=}", file=sys.stderr)
            bit = 1 << i
            res1 = ask(z ^ bit, z)
            res2 = ask(z, z ^ bit)
            if res1 == -1 and res2 == 1:
                z |= bit
                a |= bit
                b |= bit

    def solve1():
        global a, b, c, d
        # print("solve1", file=sys.stderr)
        for i in range(mi, -1, -1):
            # print(f">> {i=} {a=} {b=} {c=} {d=}", file=sys.stderr)
            bit = 1 << i
            res1 = ask(c ^ bit, d ^ bit)
            if res1 == -1:
                a |= bit
                c |= bit
                res2 = ask(c, d)
                return solve(i - 1, res2)
            else:
                res2 = ask(c ^ bit, d)
                if res2 == -1:
                    a |= bit
                    b |= bit

    def solve2():
        global a, b, c, d
        # print("solve2", file=sys.stderr)
        for i in range(mi, -1, -1):
            # print(f">> {i=} {a=} {b=} {c=} {d=}", file=sys.stderr)
            bit = 1 << i
            res1 = ask(c ^ bit, d ^ bit)
            if res1 == 1:
                b |= bit
                d |= bit
                res2 = ask(c, d)
                return solve(i - 1, res2)
            else:
                res2 = ask(c, d ^ bit)
                if res2 == 1:
                    a |= bit
                    b |= bit

    if base == 0:
        solve_same()
    elif base == 1:
        solve1()
    else:
        solve2()


base = ask(0, 0)
solve(29, base)
print("! {} {}".format(a, b))

import sys


def ask(c, d):
    print("? {} {}".format(c, d))
    return int(input())


c = d = 0


def solve(mi, base):
    def solve_same():
        global c, d
        print("# solve_same", file=sys.stderr)
        for i in range(mi, -1, -1):
            print(f">> {i=} {c=} {d=}", file=sys.stderr)
            bit = 1 << i
            res1 = ask(c ^ bit, d)
            res2 = ask(c, d ^ bit)
            if res1 == -1 and res2 == 1:
                c |= bit
                d |= bit

    def solve1():
        global c, d
        print("# solve1", file=sys.stderr)
        for i in range(mi, -1, -1):
            print(f">> {i=} {c=} {d=}", file=sys.stderr)
            bit = 1 << i
            res1 = ask(c ^ bit, d ^ bit)
            if res1 == -1:
                # a[i] == 1, b[i] == 0
                c |= bit
                return solve(i - 1, ask(c, d))
            else:
                # a[i] == b[i]
                res2 = ask(c ^ bit, d)
                if res2 == -1:
                    # a[i] == b[i] == 1
                    c |= bit
                    d |= bit

    def solve2():
        global c, d
        print("# solve2", file=sys.stderr)
        for i in range(mi, -1, -1):
            print(f">> {i=} {c=} {d=}", file=sys.stderr)
            bit = 1 << i
            res1 = ask(c ^ bit, d ^ bit)
            if res1 == 1:
                # a[i] == 0, b[i] == 1
                d |= bit
                return solve(i - 1, ask(c, d))
            else:
                # a[i] == b[i]
                res2 = ask(c, d ^ bit)
                if res2 == 1:
                    # a[i] == b[i] == 1
                    c |= bit
                    d |= bit

    if base == 0:
        solve_same()
    elif base == 1:
        solve1()
    else:
        solve2()


solve(29, ask(0, 0))
print("! {} {}".format(c, d))

import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


Q, L = inl()


def solve():
    stk = []
    size = 0

    def do_size():
        print(size)

    def do_top():
        if not stk:
            return False
        val, _ = stk[-1]
        print(val)
        return True

    def do_push(n, m):
        nonlocal size
        if size > L - n:
            return False
        if stk and stk[-1][0] == m:
            cnt = stk[-1][1]
            stk[-1] = (m, cnt + n)
        else:
            stk.append((m, n))
        size += n
        return True

    def do_pop(n):
        nonlocal size
        if size < n:
            return False
        size -= n
        while n > 0:
            val, cnt = stk[-1]
            if cnt > n:
                stk[-1] = (val, cnt - n)
                break
            stk.pop()
            n -= cnt
        return True

    for qi in range(Q):
        query = ins().split()
        if query[0] == "Top":
            ok = do_top()
            if not ok:
                print("EMPTY")
                return
        elif query[0] == "Size":
            do_size()
        elif query[0] == "Push":
            ok = do_push(int(query[1]), int(query[2]))
            if not ok:
                print("FULL")
                return
        elif query[0] == "Pop":
            ok = do_pop(int(query[1]))
            if not ok:
                print("EMPTY")
                return
    print("SAFE")


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    solve()

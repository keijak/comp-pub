import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def debug(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


dp = [None] * 105
dp[0] = dp[1] = False
for k in range(2, 105):
    win = False
    if k - 2 >= 0 and not dp[k - 2]:
        win = True
    if k - 3 >= 0 and not dp[k - 3]:
        win = True
    if k - 5 >= 0 and not dp[k - 5]:
        win = True
    dp[k] = win


t = int(sys.stdin.readline())
for i in range(t):
    n = int(sys.stdin.readline())
    print(["Second", "First"][dp[n]])

import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


class Solution:
    def solve(self, nums, queries):
        n = len(nums)
        m = len(queries)
        if n == 0 or m == 0:
            return 0
        if n == 1:
            cnt = 0
            for q in queries:
                if q == [0, 0]:
                    cnt += 1
            return cnt

        delta = nums[n - 1] - nums[n - 2]
        last_valid = [None] * n
        last_valid[n - 1] = n - 1
        last_valid[n - 2] = n - 1
        for i in range(n - 3, -1, -1):
            d = nums[i + 1] - nums[i]
            if d == delta:
                last_valid[i] = last_valid[i + 1]
            else:
                last_valid[i] = i + 1
                delta = d

        debug(last_valid)
        res = 0
        for (i, j) in queries:
            if i >= n:
                continue
            if i < 0:
                i = 0
            if j > last_valid[i]:
                continue
            if j < i:
                continue
            res += 1

        return res


try:
    import frosch

    frosch.hook()
except ImportError:
    pass
s = Solution()
nums = [1, 3, 5, 7, 6, 5, 4, 1]
queries = [[0, 3], [3, 4], [2, 4]]
print(s.solve(nums, queries))

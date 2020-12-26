import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

from collections import Counter

MOD = 10 ** 9 + 7


class Solution:
    def solve(self, nums, target):
        if not target:
            return 0
        target = set(target)
        counter = Counter()
        n = len(nums)
        m = len(target)
        l = 0
        if nums[0] in target:
            counter[nums[0]] += 1
        res = 0
        for r in range(1, n + 1):
            # debug(r, counter)
            while l < r and len(counter) == m:
                x = nums[l]
                if x in target:
                    counter[x] -= 1
                    if counter[x] == 0:
                        del counter[x]
                l += 1
            # debug(f"{r=} {l=} {r-l}")
            res += r - l
            res %= MOD
            if r < n and nums[r] in target:
                counter[nums[r]] += 1
        return res


try:
    import frosch

    frosch.hook()
except ImportError:
    pass

s = Solution()
# nums = [0, 4]
# target = [0] # expect 1
# nums = [2, 0]
# target = [2, 0]  # expect 2
nums = [3, 0, 0]
target = [0]
print(s.solve(nums, target))

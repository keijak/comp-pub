import heapq


class MinHeap(object):
    def __init__(self):
        self.h = []

    def __getitem__(self, i):
        return self.h[i]

    def __len__(self):
        return len(self.h)

    def __bool__(self):
        return bool(self.h)

    def peek(self):
        return self.h[0]

    def push(self, x):
        heapq.heappush(self.h, x)

    def pop(self):
        return heapq.heappop(self.h)


class MaxHeap(object):
    class Negator(object):
        def __init__(self, val):
            self.val = val

        def __lt__(self, other):
            return self.val > other.val

        def __eq__(self, other):
            return self.val == other.val

        def __str__(self):
            return str(self.val)

    def __init__(self):
        self.h = []

    def __getitem__(self, i):
        return self.h[i]

    def __len__(self):
        return len(self.h)

    def __bool__(self):
        return bool(self.h)

    def peek(self):
        return self.h[0].val

    def push(self, x):
        heapq.heappush(self.h, self.Negator(x))

    def pop(self):
        return heapq.heappop(self.h).val

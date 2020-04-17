K = int(input())

def search(d, digits, count):
    if d == 1:
        return (digits if count == 1 else None), (count-1)
    s = digits[-1]
    for i in range(max(s-1, 0), min(s+2, 10)):
        answer, count = search(d-1, digits + [i], count)
        if answer:
             return answer, count
    return None, count

def solve():
    count = K  # count down
    d = 1  # num of digits
    while True:
        for i in range(1,10):
            answer, count = search(d, [i], count)
            if answer:
                return answer
        d += 1

print(''.join(str(d) for d in solve()))

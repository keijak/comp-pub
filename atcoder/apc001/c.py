import sys

N = int(input())
seats = [None] * (N + 1)


def main():
    print(0)
    answer = input()
    if answer == "Vacant":
        return
    seats[0] = answer == "Male"
    l, r = 0, N
    while r - l > 1:
        # Vacant must be in [l, r).
        assert seats[l] is not None
        q = (l + r) // 2
        sys.stderr.write(f">>> l={l} r={r} q={q}\n")
        print(q)
        answer = input()
        if answer == "Vacant":
            return
        seats[q] = answer == "Male"
        if ((q - l) % 2 == 0) ^ (seats[l] == seats[q]):
            r = q
        else:
            l = q
    raise ValueError("something went wrong")


if __name__ == "__main__":
    main()

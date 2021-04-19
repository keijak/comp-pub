for n in range(2, 10000):
    with open(f"test/my-{n}.in", "w") as f:
        print(n, file=f)

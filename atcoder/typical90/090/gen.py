for n in range(1, 8):
    for k in range(1, 8):
        with open(f"test/small_{n}_{k}.in", "w") as f:
            print(n, k, file=f)

t = 0
for n in range(1, 19):
    for k in range(1, n + 1):
        t += 1
        with open(f"test/test-{t}.in", "w") as f:
            f.write(f"{n} {k}\n")

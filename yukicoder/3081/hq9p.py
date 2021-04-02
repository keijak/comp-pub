import sys

if len(sys.argv) <= 1:
    print("Usage: python3 HQ9p.py codefile", file=sys.stderr)
    exit(1)

with open(sys.argv[1]) as codefile:
    code = codefile.readline().rstrip("\n")

accumulator = 0

for op in code:

    if op == "H":
        print("Hello, World!")

    elif op == "Q":
        print(code, end="")

    elif op == "9":
        for i in range(99, 1, -1):
            print(
                str(i)
                + " bottles of beer on the wall, "
                + str(i)
                + " bottles of beer.\n"
                + "Take one down, pass it around, "
                + str(i - 1)
                + (" bottle" if i - 1 == 1 else " bottles")
                + " of beer on the wall.\n",
            )

        print(
            "1 bottle of beer on the wall, "
            + "1 bottle of beer.\n"
            + "Take one down, pass it around, "
            + "No bottles of beer on the wall.",
        )

    elif op == "+":
        accumulator += 1

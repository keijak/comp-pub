import sys
import re

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def snake_to_camel(s):
    if not re.fullmatch(r"^[a-z0-9_]+$", s):
        return None
    words = s.split("_")
    res = []
    first = True
    for w in words:
        if not w:
            return None
        if not re.fullmatch(r"^[a-z].*", w):
            return None
        if first:
            first = False
            word = w
        else:
            word = w[0].upper() + w[1:]
        res.append(word)
    return "".join(res)


def camel_to_snake(s):
    if not re.fullmatch(r"^[a-z0-9][A-Za-z0-9]+$", s):
        return None
    res = []
    first = True
    for w in re.findall(r"[A-Z]?[a-z0-9]*", s):
        if not w:
            continue
        if first:
            first = False
        elif not w[0].isupper():
            return None
        res.append(w[0].lower() + w[1:])
    return "_".join(res)


def solve():
    s = ins()
    if re.fullmatch(r"^_+$", s):
        return s
    m = re.fullmatch(r"^(_*)(.*[^_])(_*)$", s)
    if not m:
        return s
    head = m.group(1)
    body = m.group(2)
    tail = m.group(3)
    r = snake_to_camel(body)
    if r:
        debug("snake => camel")
        return head + r + tail
    r = camel_to_snake(body)
    if r:
        debug("camel => snake")
        return head + r + tail
    debug("unchanged")
    return s


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    print(solve())

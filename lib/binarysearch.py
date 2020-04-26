from typing import Callable


def binary_search(pred: Callable[[int], bool], ok: int, ng: int) -> int:
    """Finds the largest x that satisfies pred(x).

    Precondition: ok < ng

    Args:
      ok: A known int value that satisfies pred(x).
      ng: A known int value that satisfies !pred(x).

    Returns:
      The largest x that satisfies pred(x).
    """
    assert ng - ok >= 1
    assert pred(ok)
    assert not pred(ng)
    while ng - ok > 1:
        mid = (ok + ng) // 2  # ok < mid < ng
        if pred(mid):
            ok = mid
        else:
            ng = mid
    assert ng - ok == 1
    return ok

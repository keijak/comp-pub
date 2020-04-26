from typing import Callable


def binary_search(pred: Callable[[int], bool], ok: int, ng: int) -> int:
    """Finds the boundary that satisfies pred(x).

    Precondition: ok < ng

    Args:
      pred(x): A monotonic predicate (either decreasing or increasing).
      ok: A known int value that satisfies pred(x).
      ng: A known int value that satisfies !pred(x).

    Returns:
      The largest or the smallest x that satisfies pred(x).
    """
    assert abs(ng - ok) >= 1
    assert pred(ok)
    assert not pred(ng)
    while abs(ng - ok) > 1:
        mid = (ok + ng) // 2  # ok < mid < ng
        if pred(mid):
            ok = mid
        else:
            ng = mid
    assert abs(ng - ok) == 1
    return ok

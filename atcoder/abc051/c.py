sx, sy, tx, ty = map(int,input().split())

def go():
    out = []
    x, y = sx, sy
    while y < ty:
        out.append('U'); y += 1
    while x < tx:
        out.append('R'); x += 1
    while y > sy:
        out.append('D'); y -= 1
    while x > sx:
        out.append('L'); x -= 1
    out.append('L'); x -= 1
    while y <= ty:
        out.append('U'); y += 1
    while x < tx:
        out.append('R'); x += 1
    out.append('D'); y -= 1
    out.append('R'); x += 1
    while y >= sy:
        out.append('D'); y -= 1
    while x > sx:
        out.append('L'); x -= 1
    out.append('U');
    return ''.join(out)

print(go())

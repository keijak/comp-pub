s = input()
K = int(input())
after = []
rem = K
for i in range(len(s)-1):
    if s[i] == 'a':
        after.append('a')
        continue
    c = ord('z') - ord(s[i]) + 1
    if c <= rem:
        rem -= c
        after.append('a')
    else:
        after.append(s[i])

last = ord(s[-1])
if rem > 0:
    last += rem%26
    if last > ord('z'):
        last -= 26
after.append(chr(last))

print(''.join(after))
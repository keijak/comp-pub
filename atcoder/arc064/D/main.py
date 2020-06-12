s = input()
print("First" if len(s)%2 ^ (s[0] == s[-1]) else "Second")

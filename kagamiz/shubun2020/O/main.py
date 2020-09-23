import re
p = r"シ(ー|ィー|イー|ィ|イ)ク(ア|ァ|ワ|ヮ)(ー|)(シャ|サ)ー"
s = input()
if re.fullmatch(p, s):
    print("はい")
else:
    print("いいえ")

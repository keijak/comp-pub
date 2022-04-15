import numpy as np
import sys

n = int(sys.stdin.buffer.readline())
a = np.array(sys.stdin.buffer.readline().split(), dtype=np.float32)
afreq = 200 + np.argmax(np.abs(np.fft.rfft(a))[200:500])
freqs = np.array([261.6, 294.3, 327.0, 348.8, 392.4, 436.0, 490.5])
labels = ["C4", "D4", "E4", "F4", "G4", "A4", "B4"]
print(labels[np.argmin(np.abs(afreq - freqs))])

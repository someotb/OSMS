import matplotlib.pyplot as plt

with open("../data/gold_sequence.txt") as f:
    bits_gold = [int(line.strip()) for line in f]

plt.figure(figsize=(10, 4))
plt.plot(range(len(bits_gold)), bits_gold)
plt.title("Gold sequence")
plt.xlabel("Bit index")
plt.ylabel("Value")
plt.ylim(-0.1, 1.1)

with open("../data/samples_sequence.txt") as f:
    bits_samples = [int(line.strip()) for line in f]

plt.figure(figsize=(10, 4))
plt.plot(range(len(bits_samples)), bits_samples)
plt.title("Samples sequence")
plt.xlabel("Bit index")
plt.ylabel("Value")
plt.ylim(-0.1, 1.1)

with open("signal_sequence.txt") as f:
    bits_signal = [int(line.strip()) for line in f]

plt.figure(figsize=(10, 4))
plt.plot(range(len(bits_signal)), bits_signal)
plt.title("Signal sequence")
plt.xlabel("Bit index")
plt.ylabel("Amplitude")
plt.ylim(-0.1, 1.1)

with open("../data/noisy_signal_sequence.txt") as f:
    noisy_bits_signal = [float(line.strip()) for line in f]

plt.figure(figsize=(10, 4))
plt.plot(range(len(noisy_bits_signal)), noisy_bits_signal, label="Noisy Signal")
plt.plot(range(len(bits_signal)), bits_signal, label="Original Signal")
plt.title("Noisy signal and original signal")
plt.xlabel("Bit index")
plt.ylabel("Amplitude")
plt.ylim(min(noisy_bits_signal) - 0.1, max(noisy_bits_signal) + 0.1)
plt.legend()

with open("../data/aligned_signal_sequence.txt") as f:
    aligned_bits_signal = [float(line.strip()) for line in f]

plt.figure(figsize=(10, 4))
plt.plot(range(len(aligned_bits_signal)), aligned_bits_signal, label="Aligned Signal")
plt.plot(range(len(noisy_bits_signal)), noisy_bits_signal, label="Noisy Signal")
plt.plot(range(len(bits_signal)), bits_signal, label="Original Signal")
plt.title("Noisy signal and original signal")
plt.xlabel("Bit index")
plt.ylabel("Amplitude")
plt.ylim(min(aligned_bits_signal) - 0.1, max(aligned_bits_signal) + 0.1)
plt.legend()

plt.show()

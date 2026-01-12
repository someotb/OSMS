import matplotlib.pyplot as plt
import numpy as np


def plot_spectrum(signal, fs, label):
    N = len(signal)
    yf = np.fft.fft(signal)
    xf = np.fft.fftfreq(N, 1 / fs)

    xf = xf[: N // 2]
    yf = np.abs(yf[: N // 2])

    plt.plot(xf, yf, label=label)


with open("../data/gold_sequence.txt") as f:
    bits_gold = [int(line.strip()) for line in f]
plt.figure(figsize=(10, 4))
plt.plot(range(len(bits_gold)), bits_gold)
plt.title("Gold sequence")
plt.xlabel("Bit index")
plt.ylabel("Value")
plt.ylim(-0.1, 1.1)

with open("../data/bits_sequence.txt") as f:
    bits_seq = [int(line.strip()) for line in f]
plt.figure(figsize=(10, 4))
plt.plot(range(len(bits_seq)), bits_seq)
plt.title("Bits sequence")
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

with open("../data/signal_sequence.txt") as f:
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
plt.title("Noisy signal and original signals")
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
plt.title("Aligned, Noisy and Original signals")
plt.xlabel("Bit index")
plt.ylabel("Amplitude")
plt.ylim(min(aligned_bits_signal) - 0.1, max(aligned_bits_signal) + 0.1)
plt.legend()

with open("../data/soluted_signal_sequence.txt") as f:
    soluted_bits_signal = [float(line.strip()) for line in f]
plt.figure(figsize=(10, 4))
plt.plot(range(len(soluted_bits_signal)), soluted_bits_signal, label="Soluted Bits")
plt.plot(range(len(bits_seq)), bits_seq, label="Original Bits", linestyle="--")
plt.title("Soluted signal")
plt.xlabel("Bit index")
plt.ylabel("Amplitude")
plt.ylim(min(soluted_bits_signal) - 0.1, max(soluted_bits_signal) + 0.1)
plt.legend()

with open("../data_for_13ex/short_sequence.txt") as f:
    short_signal = [float(line.strip()) for line in f]

with open("../data_for_13ex/medium_sequence.txt") as f:
    medium_signal = [float(line.strip()) for line in f]

with open("../data_for_13ex/long_sequence.txt") as f:
    long_signal = [float(line.strip()) for line in f]

fs = 1000

plt.figure(figsize=(12, 6))
plot_spectrum(bits_seq, fs, "Transmitted signal")
plot_spectrum(aligned_bits_signal, fs, "Received Signal")
plt.title("Spectra of transmitted and received signals")
plt.xlabel("Frequency [Hz]")
plt.ylabel("Amplitude")
plt.legend()
plt.grid(True)

plt.figure(figsize=(12, 6))
plot_spectrum(short_signal, fs, "Short Symbol(5)")
plot_spectrum(medium_signal, fs, "Medium Symbol(10)")
plot_spectrum(long_signal, fs, "Long Symbol(20)")

plt.title("Spectra of transmitted and received signals")
plt.xlabel("Frequency [Hz]")
plt.ylabel("Amplitude")
plt.legend()
plt.grid(True)

plt.show()

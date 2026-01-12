import matplotlib.pyplot as plt

with open("gold_sequence.txt") as f:
    bits_gold = [int(line.strip()) for line in f]

plt.figure(figsize=(10, 4))
plt.step(range(len(bits_gold)), bits_gold)
plt.title("Gold sequence")
plt.xlabel("Bit index")
plt.ylabel("Value")
plt.ylim(-0.1, 1.1)

with open("signal_sequence.txt") as f:
    bits_signal = [int(line.strip()) for line in f]

plt.figure(figsize=(10, 4))
plt.step(range(len(bits_signal)), bits_signal)
plt.title("Signal sequence")
plt.xlabel("Bit index")
plt.ylabel("Value")
plt.ylim(-0.1, 1.1)


plt.show()

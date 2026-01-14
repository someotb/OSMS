import matplotlib.pyplot as plt

sigma_data = {}
with open("../data_for_additional_task/p_crc_vs_N.txt") as f:
    for line in f:
        sigma, N, p = map(float, line.strip().split())
        if sigma not in sigma_data:
            sigma_data[sigma] = {"N": [], "p": []}
        sigma_data[sigma]["N"].append(N)
        sigma_data[sigma]["p"].append(p)

plt.figure(figsize=(10, 6))
for sigma, data in sigma_data.items():
    plt.plot(data["N"], data["p"], marker="o", label=f"sigma={sigma}")

plt.title("Вероятность успешного декодирования блока от числа сэмплов на 1 бит ")
plt.xlabel("Число семплов на бит N")
plt.ylabel("Вероятность того, что пакет дошел без ошибки")
plt.ylim(0, 1.1)
plt.grid(True)
plt.legend()
plt.show()

import matplotlib.pyplot as plt

mode = int(input("Which experiment you wanna try: #1 - (1), #2 - (2): "))

if mode == 1:
    sigma_data = {}
    with open("../data_for_additional_task/p_crc_vs_N(trials=10, norm_corr).txt") as f:
        for line in f:
            sigma, N, p = map(float, line.strip().split())
            if sigma not in sigma_data:
                sigma_data[sigma] = {"N": [], "p": []}
            sigma_data[sigma]["N"].append(N)
            sigma_data[sigma]["p"].append(p)

    plt.figure(figsize=(12, 6))
    for sigma, data in sigma_data.items():
        plt.plot(data["N"], data["p"], marker="o", label=f"sigma={sigma}")

    plt.title("Вероятность успешного декодирования блока от числа сэмплов на 1 бит ")
    plt.xlabel("Число семплов на бит N")
    plt.ylabel("Вероятность того, что пакет дошел без ошибки")
    plt.ylim(0, 1.1)
    plt.grid(True)
    plt.legend()
    plt.show()

elif mode == 2:
    data = {}
    with open(
        "../data_for_additional_task/corr_and_crc_vs_sigma(trials=10,norm_corr).txt"
    ) as f:
        for line in f:
            N, sigma, corr, p = map(float, line.split())
            if N not in data:
                data[N] = {"sigma": [], "corr": [], "p": []}
            data[N]["sigma"].append(sigma)
            data[N]["corr"].append(corr)
            data[N]["p"].append(p)

    fig = plt.figure(figsize=(12, 6))
    ax1 = fig.add_subplot(111)

    for N, d in data.items():
        ax1.plot(d["sigma"], d["corr"], label=f"Corr peak, N={int(N)}")  # type: ignore

    ax1.set_xlabel("σ")  # type: ignore
    ax1.set_ylabel("Максимум автокорреляции")  # type: ignore
    ax1.grid(True)  # type: ignore

    ax2 = ax1.twinx()  # type: ignore
    for N, d in data.items():
        ax2.plot(d["sigma"], d["p"], linestyle="--", label=f"P(CRC OK), N={int(N)}")

    ax2.set_ylabel("Вероятность CRC=OK")
    ax2.set_ylim(0, 1.05)

    lines1, labels1 = ax1.get_legend_handles_labels()  # type: ignore
    lines2, labels2 = ax2.get_legend_handles_labels()
    ax1.legend(lines1 + lines2, labels1 + labels2, loc="best")  # type: ignore

    plt.title("Автокорреляция и вероятность декодирования от σ")
    plt.show()

else:
    print("Please enter a valid experiment number (1 or 2).\n")

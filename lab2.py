import numpy as np
from matplotlib import pyplot as plt

# Исходные данные
P_BS = 46  # Мощность передатчиков BS (дБм)
P_UE = 24  # Мощность передатчика пользовательского терминала UE (дБм)
G_BS = 21  # Коэффициент усиления антенны BS (дБи)

f = 1.8  # Диапазон частот (ГГц)
num_of_cells = 3  # Число секторов на одной BS
feeder_loss = (
    2 + 0.4 + 0.5
)  # Потери в фидере, соединителях и направленности антенны (дБ)
mimo_gain = 3.0  # Число приемо-передающих антенн на BS

NF_BS = 2.4  # Коэффициент шума приемника BS (дБ)
NF_UE = 6  # Коэффициент шума приемника пользователя (дБ)

BW_UL = 10 * 10**6  # Полоса частот в UL (10 МГц)
BW_DL = 20 * 10**6  # Полоса частот в DL (20 МГц)

SINR_DL = 2  # Требуемое отношение SINR для DL (дБ)
SINR_UL = 4  # Требуемое отношение SINR для UL (дБ)

M_pen = 15  # Запас мощности сигнала на проникновения сквозь стены (дБ)
M_int = 1  # Запас мощности сигнала на интерференцию (дБ)

S = 100  # Площадь территории, на которой требуется спроектировать сеть (км**2)
S_in = 4  # Площадь торговых и бизнес центров, где требуется спроектировать сеть на базе микро- и фемтосот (км**2)

N_BS = -174 + 10 * np.log10(BW_UL) + NF_BS  # дБм
N_UE = -174 + 10 * np.log10(BW_DL) + NF_UE  # дБм

RxSens_BS = N_BS + SINR_UL  # дБм
RxSens_UE = N_UE + SINR_DL  # дБм

d = np.linspace(10, 5000, 5000)

# FSPM
c = 3e8

fspl = 20 * np.log10((4 * np.pi * d * f * 1e9) / c)

# UMINLOS
PL_UNI = []

for i in range(0, len(d)):
    pl = 26 * np.log10(f) + 22.7 + 36.7 * np.log10(d[i])
    PL_UNI.append(pl)

# COST231
f_MHz = f * 1000.0  # перевод в МГц

PL_COST = []
A = 46.3
B = 33.9
hBS = 30
hms = 1.5
a = (1.1 * np.log10(f_MHz)) * hms - (
    1.56 * np.log10(f_MHz) - 0.8
)  # для SU, RURAL, ROAD

Lclutter = 0

for i in range(0, len(d)):
    d_km = d[i] / 1000.0

    if d_km >= 1:
        s = 44.9 - 6.55 * np.log10(f_MHz)
    else:
        s = (47.88 + 13.9 * np.log10(f_MHz) - 13.9 * np.log10(hBS)) * (1 / np.log10(50))

    pl = (
        A
        + B * np.log10(f_MHz)
        - 13.82 * np.log10(hBS)
        - a
        + s * np.log10(d_km)
        + Lclutter
    )
    PL_COST.append(pl)

# Walfish-Ikegami
PL_WALF_NLOS = []
PL_WALF_LOS = []

f = 1.8  # ГГц
f_MHz = f * 1000.0  # перевод в МГц
w = 20.0  # средняя ширина улиц (м)
h = 28.0  # средняя высота зданий (м)
hBS = 30.0  # высота подвеса антенны БС (м)
hms = 1.5  # высота на которой находится телефон пользователя (м)
b = 12.0  # среднее расстояние между зданиями (м)
phi = 45.0  # угол между направлением сигнала и улицей (градусы)

kf = -4.0 + 0.7 * (f_MHz / 925.0 - 1.0)

if 0.0 <= phi < 35.0:
    phi_coef = -10.0 + 0.354 * phi
elif 35.0 <= phi < 55.0:
    phi_coef = 2.5 + 0.075 * phi
else:
    phi_coef = 4.0 - 0.114 * phi

if (h - hms) <= 0:
    delta_h_building_mobile = 0.1
else:
    delta_h_building_mobile = h - hms

L2 = (
    -16.9
    - 10.0 * np.log10(w)
    + 10.0 * np.log10(f_MHz)
    + 20.0 * np.log10(delta_h_building_mobile)
    + phi_coef
)

if hBS > h:
    L11 = -18.0 * np.log10(1.0 + hBS - h)
else:
    L11 = 0.0

for di in d:
    d_km = di / 1000.0
    PL_LOS = 42.6 + 20.0 * np.log10(f_MHz) + 26.0 * np.log10(d_km)
    PL_WALF_LOS.append(PL_LOS)
    d_km_safe = max(d_km, 0.01)
    L0 = 32.44 + 20.0 * np.log10(f_MHz) + 20.0 * np.log10(d_km_safe)

    if hBS > h:
        ka = 54.0
    else:
        if d_km <= 0.5:
            ka = 54.0 - 0.8 * (h - hBS) * (d_km / 0.5)
        else:
            ka = 54.0 - 0.8 * (h - hBS)

    if hBS > h:
        kd = 18.0
    else:
        kd = 18.0 - 15.0 * ((h - hBS) / h)

    L1 = L11 + ka + kd * np.log10(d_km_safe) + kf * np.log10(f_MHz) - 9.0 * np.log10(b)

    if (L1 + L2) > 0:
        L_NLOS = L0 + L1 + L2
    else:
        L_NLOS = L0

    PL_WALF_NLOS.append(L_NLOS)


# 1 Выполните расчет бюджета восходящего канала, используя
# входные данные и определите уровень максимально допустимых потерь
# сигнала MAPL_UL.

MAPL_UL = P_UE - feeder_loss + mimo_gain - M_pen - M_int + G_BS - RxSens_BS  # 126.6 Дб

# 2 Выполните расчет бюджета нисходящего канала, используя
# входные данные и определите уровень максимально допустимых потерь
# сигнала MAPL_DL.

MAPL_DL = P_BS - feeder_loss + mimo_gain - M_int - M_pen + G_BS - RxSens_UE  # 144 Дб


# MAPL для разных температур
k = 1.380649e-23
temprature = 290

N_BS_test = 30 + 10 * np.log10(BW_UL * k * temprature)
N_UE_test = 30 + 10 * np.log10(BW_DL * k * temprature)
RxSens_BS_test = N_BS_test + SINR_UL  # дБм
RxSens_UE_test = N_UE_test + SINR_DL  # дБм
MAPL_UL_test = P_UE - feeder_loss + mimo_gain - M_pen - M_int + G_BS - RxSens_BS_test
MAPL_DL_test = P_BS - feeder_loss + mimo_gain - M_int - M_pen + G_BS - RxSens_UE_test

# Построение графика
plt.figure(figsize=(8, 5))
plt.plot(d, PL_UNI, "b", label="UMINLOS", linewidth=2)
plt.plot(d, PL_COST, "r", label="COST231", linewidth=2)
plt.plot(d, PL_WALF_NLOS, "g", label="Walfish-Ikegami(NLOS)", linewidth=2)
plt.plot(d, PL_WALF_LOS, color="purple", label="Walfish-Ikegami(LOS)", linewidth=2)
plt.plot(d, fspl, color="orange", label="FSPL", linewidth=2)
plt.axhline(MAPL_UL, color="g", linestyle="--", label="MAPL_UL")
plt.axhline(MAPL_DL, color="m", linestyle="--", label="MAPL_DL")
plt.axhline(MAPL_DL_test, color="c", linestyle="--", label="MAPL_DL (-50 градусов)")
plt.axhline(MAPL_UL_test, color="y", linestyle="--", label="MAPL_UL (-50 градусов)")

plt.xlabel("Расстояние, м")
plt.ylabel("Потери PL, дБ")
plt.title("Зависимость потерь от расстояния")
plt.grid()
plt.legend()
plt.show()

R_COST = d[np.argmin(np.abs(np.array(PL_COST) - MAPL_UL))]
R_UMiNLOS = d[np.argmin(np.abs(np.array(PL_UNI) - MAPL_UL))]
R_WALFISH_LOS = d[np.argmin(np.abs(np.array(PL_WALF_LOS) - MAPL_UL))]
R_WALFISH_NLOS = d[np.argmin(np.abs(np.array(PL_WALF_NLOS) - MAPL_UL))]

print(f"Максимальный радиус покрытия COST231: {R_COST} м")
print(f"Максимальный радиус покрытия UMINLOS: {R_UMiNLOS} м")
print(f"Максимальный радиус покрытия Walfish-Ikegami(LOS): {R_WALFISH_LOS} м")
print(f"Максимальный радиус покрытия Walfish-Ikegami(NLOS): {R_WALFISH_NLOS} м")

# Кол-во сайтов
S_site = 1.95 * (R_COST / 1000) ** 2  # шестигранник

N_site = np.ceil(S / S_site)

S_site_in = np.pi * (R_UMiNLOS / 1000) ** 2  # круг

N_site_in = np.ceil(S_in / S_site_in)

print(f"Количество макросот COST: {N_site}")
print(f"Количество фемтосот UMINLOS: {N_site_in}")

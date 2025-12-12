clc; clear; close all;

% Исходные данные
Power_BS = 46; % (дБм)
Gain_BS = 21; % (дБи)
M_int = 1; % (дБ)
M_pen = 15; % (дБ)
BW_DL = 20 * 10^6; % Полоса частот в DL (20 МГц)
NF_UE = 6; % Коэффициент шума приемника пользователя (дБ)
SINR_DL = 2; % (дБ)
feeder_loss = 2 + 0.4 + 0.5; % Потери в фидере, МШУ, Джампер (Дб)
temrature = linspace(-70+273.15, 70+273.15, 141);
k = 1.380649e-23; % Постоянная Больцмана
N_UE = 30 + 10 * log10(BW_DL * k * temrature);
RxSens_UE = N_UE + SINR_DL + NF_UE;
mimo = 3; % Выйгрыш от MIMO (Дб)
frequency = 1600 * 1000; % МГц
distance = linspace(0.001, 10, 9990); % Км
hBS = 60; % м
hms = 1.5;
A = 46.3;
B = 33.9;

a = (1.1 * log10(frequency)) * hms - (1.56 * log10(frequency) - 0.8);
Lclutter = -(4.78 * (log10(frequency))^2 - 18.33 * log10(frequency) + 40.94);

s = zeros(size(distance));

idx_ge_1 = distance >= 1000;
s(idx_ge_1) = 44.9 - 6.55 * log10(frequency);

idx_lt_1 = distance < 1000;
s(idx_lt_1) = (47.88 + 13.9 * log10(frequency) - 13.9 * log10(hBS)) * (1 / log10(50));

PL = A + B * log10(frequency) - 13.82 * log10(hBS) - a + s .* log10(distance) + Lclutter;
MAPL_DL = Power_BS + mimo + Gain_BS - feeder_loss - M_int - M_pen - RxSens_UE;
max_dist = zeros(size(MAPL_DL));

for i = 1 : length(MAPL_DL)
    diff = abs(MAPL_DL(i) - PL);
    [~, idx] = min(diff);
    max_dist(i) = distance(idx)^2 * 1.95;
end

figure;
plot(temrature, max_dist);
title("Зависимости радиуса соты от температуры")
xlabel("Temprature (K)")
ylabel("Squre (km^2)");
grid("on");
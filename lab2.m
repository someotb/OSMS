clc; clear; close all;

Power_BS = 46; % (дБм)
Gain_BS = 21; % (дБи)
M_int = 1; % (дБ)
M_pen = 15; % (дБ)
BW_DL = 20 * 10^6; % Полоса частот в DL (20 МГц)
NF_UE = 6; % Коэффициент шума приемника пользователя (дБ)
SINR_DL = 2; % (дБ)
feeder_loss = 2 + 0.4 + 0.5; % Потери в фидере, МШУ, Джампер (Дб)
mimo = 3; % Выйгрыш от MIMO (Дб)
frequency = 1800; % МГц

hBS = 30;
hms = 1.5;
A = 46.3;
B = 33.9;
a = (1.1 * log10(frequency)) * hms - (1.56 * log10(frequency) - 0.8);
Lclutter = 0;

% Диапазон температур от -70C до +70C (в Кельвинах)
temperature_K = linspace(-70+273.15, 70+273.15, 141);
temperature_C = temperature_K - 273.15; % для оси X на графике

k = 1.380649e-23; % Постоянная Больцмана
max_dist = zeros(size(temperature_K));

distance_km = linspace(0.001, 10, 10000); % км (достаточно до 10 км)

% Расчет потерь PL по COST231 (не зависит от температуры)
s = zeros(size(distance_km));
idx_ge_1 = distance_km >= 1;
s(idx_ge_1) = 44.9 - 6.55 * log10(frequency);
idx_lt_1 = distance_km < 1;
s(idx_lt_1) = (47.88 + 13.9 * log10(frequency) - 13.9 * log10(hBS)) / log10(50);

PL = A + B * log10(frequency) - 13.82 * log10(hBS) - a + s .* log10(distance_km) + Lclutter;

% Для каждой температуры рассчитываем радиус
for i = 1:length(temperature_K)
    N_UE = 10 * log10(k * temperature_K(i) * BW_DL) + 30; % в дБм
    RxSens_UE = N_UE + SINR_DL + NF_UE;
    
    MAPL_DL = Power_BS + mimo + Gain_BS - feeder_loss - M_int - M_pen - RxSens_UE;
    
    [~, idx] = min(abs(PL - MAPL_DL));
    max_dist(i) = distance_km(idx) * 1000;
end

figure;
plot(temperature_C, max_dist, 'b-', 'LineWidth', 2);
grid on;
title("Зависимость радиуса соты от температуры (COST231-Hata)");
xlabel("Температура (°C)");
ylabel("Радиус (м)");
set(gca, 'FontSize', 12);

[~, idx_290K] = min(abs(temperature_K - 290));
hold on;
plot(temperature_C(idx_290K), max_dist(idx_290K), 'ro', 'MarkerSize', 10, 'LineWidth', 2);
text(temperature_C(idx_290K)+2, max_dist(idx_290K), sprintf('290K: %.0f м', max_dist(idx_290K)), 'FontSize', 12);

fprintf('При температуре 290K (17°C):\n');
fprintf('  Радиус = %.1f м\n', max_dist(idx_290K));
fprintf('  MAPL_DL = %.2f дБ\n', Power_BS + mimo + Gain_BS - feeder_loss - M_int - M_pen - ...
    (10 * log10(k * 290 * BW_DL) + 30 + SINR_DL + NF_UE));

if exist('radius_from_python', 'var')
    yline(radius_from_python, 'g--', 'LineWidth', 1.5, 'DisplayName', sprintf('Python: %.0f м', radius_from_python));
end
legend show;
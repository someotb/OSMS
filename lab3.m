clc; clear; close all;

% Вариант 16
f1 = 16;
f2 = f1 + 4;
f3 = f1 * 2 + 1;

x = [7.0, 3.0, 2.0, -2.0, -2.0, -4.0, 1.0, 5.0];
y = [2.0, 1.0, 5.0, 0.0, -2.0, -3.0, 2.0, 4.0];

t = linspace(0, 99, 100);

s1 = cos(2 * pi * f1 * t);
s2 = cos(2 * pi * f2 * t);
s3 = cos(2 * pi * f3 * t);

a = 4 * s1 + 2 * s2 + 2 * s3;
b = 2 * s1 + s2;

% Используя Matlab определите корреляцию и нормализованую корреляцию между сигналом s1(t) и сигналами a и b.
corr_s1_a = corr(s1, a);
corr_norm_s1_a = corr_norm(x, y);

corr_s1_b = corr(s1, b);
corr_norm_s1_b = corr_norm(s1, b);

fprintf("Кореляция между s1 и a: %.2f\n",corr_s1_a);
fprintf("Кореляция между s1 и b: %.2f\n",corr_s1_b);
fprintf("Нормализованая кореляция между s1 и a: %.2f\n",corr_norm_s1_a);
fprintf("Нормализованая кореляция между s1 и b: %.2f\n",corr_norm_s1_b);

fprintf("\n");
% Возьмите два массива значений и выведите их на графиках друг под другом

a = [0.3, 0.2, -0.1, 4.2, -2, 1.5, 0];
b = [0.3, 4,-2.2, 1.6, 0.1, 0.1, 0.2];
len_a_b = linspace(0, length(a) - 1, 7);

corr_a_b = corr_norm(a,b);
fprintf("Кореляция между a и b: %.2f\n", corr_a_b);
fprintf("\n");

figure('Name', 'Графики a и b', 'NumberTitle', 'off');
subplot(2, 1, 1);
plot(len_a_b, a);
title("A");
grid("on");
subplot(2, 1, 2);
plot(len_a_b, b);
title("B");
grid("on");

% Сдвигайте последовательность b поэлементно вправо и на каждом шаге сдвига вычисляйте значение взаимной корреляции между a и
% сдвинутой последовательностью b
corr_massive = zeros(length(a));
sdvig_massive = zeros(length(a));
max_corr = -1;
max_sdvig = 0;
for i = 1:length(a)
    sdvig_massive(i) = i;
    b_sdvig = sdvig(b,i);
    corr_massive(i) = corr_norm(a, b_sdvig);
    if corr_massive(i) > max_corr
        max_corr = corr_massive(i);
        max_sdvig = i;
    end
end

fprintf("Значение сдвига, при котором достигается максимальная корреляция: %d", max_sdvig);

figure('Name', 'Корреляция с разным сдвигом', 'NumberTitle', 'off');
plot(sdvig_massive, corr_massive);
xlabel("Сдвиг")
ylabel("Нормированная кореляция")
title("Зависимость взаимной корреляции последовательностей от величины циклического сдвига");
grid("on");

% Нарисуйте графики a и b, сдвинутой на величину, где зафиксирована максимальная корреляция
b_so_max_sdvig = sdvig(b, max_sdvig);

figure('Name', 'Графики a и b, сдвинутой на величину, где зафиксирована максимальная корреляция', 'NumberTitle', 'off');
subplot(2, 1, 1);
plot(len_a_b, a);
title("A");
grid("on");
subplot(2, 1, 2);
plot(len_a_b, b_so_max_sdvig);
title("B");
grid("on");

function massive = sdvig(a, times)
    N = length(a);
    if N == 0
        return;
    end
    massive = a;
    for i = 1:times
        last = massive(end);
        for n = N:-1:2
            massive(n) = massive(n-1);
        end
        massive(1) = last;
    end
end

function [val] = corr(a, b)
    val = sum(a .* b);
end

function [summa] = corr_norm(a, b)
    xnyn = sum(a .* b);
    a2 = sum(a .* a);
    b2 = sum(b .* b);
    niz = sqrt(a2 * b2);
    if niz == 0
        summa = 0;
    else
        summa = xnyn / niz;
    end
end

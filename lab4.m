clc; clear; close all;

journal_num = 16;
m = 5;
N = 2^m - 1;

x = de2bi(journal_num, m, 'left-msb');
y = de2bi(journal_num + 7, m, 'left-msb');

fprintf('Начальное состояние x: ');
disp(x);
fprintf('Начальное состояние y: ');
disp(y);

% Последовательность Голда
G = zeros(1, N);

x_reg = x;
y_reg = y;

for i = 1:N
    G(i) = xor(x_reg(5), y_reg(5));

    fx = xor(x_reg(4), x_reg(5));
    fy = xor(y_reg(2), y_reg(5));

    % сдвиг вправо
    x_reg = [fx x_reg(1:4)];
    y_reg = [fy y_reg(1:4)];
end

disp('Последовательность Голда:');
disp(G);

G_pm = 2*G - 1;   % {0,1} → {-1,+1}

fprintf('\nТаблица автокорреляции:\n');
fprintf('Сдвиг | ');
for i = 1:N
    fprintf('b%-2d ', i);
end
fprintf('| R\n');

for k = 0:N
    Gs = circshift(G, k);

    fprintf('%5d | ', k);
    fprintf('%3d ', Gs);

    R = sum(G_pm .* (2*Gs - 1)) / N;
    fprintf('| %8.6f\n', R);
end

% Вторая последовательность Голда (x = x + 1, y = y - 5)
x2 = [1 0 0 0 1];  % 17
y2 = [1 0 1 1 0];  % 18

G2 = zeros(1,N);

for i = 1:N
    fx = xor(x2(4), x2(5));
    fy = xor(y2(2), y2(5));

    G2(i) = xor(x2(5), y2(5));

    x2 = [fx x2(1:4)];
    y2 = [fy y2(1:4)];
end

disp('Вторая последовательность Голда:');
disp(G2);

% Графики с использованием встроенных функций
figure;
autocorr(G_pm, N-1);
title('Автокорреляция (autocorr)');

figure;
[c,lags] = xcorr(G_pm, 2*G2-1, 'normalized');
stem(lags, c);
title('Взаимная корреляция (xcorr)');
xlabel('lag');
ylabel('R');
grid on;

clear; close all; clc;

%% 8) Запишите аудиофайл со своим голосом (например, в формате wav) и проанализируйте его
% Максимальная частота дискретизации в спектре моего голоса примерно 19700 Hz, следовательно требуемая для оцифровки частота дискретизации 39400 Hz

%% 9) Используя библиотеки работы со звуком Matlab (или Pyton) проанализируйте имеющуюся у вас запись голоса
[y, fs] = audioread('voice.wav'); % Считываем файл с помошью audioread() ('voice.wav', duration - 4.7232) ('voice1.wav', duration - 9.5267)

%% 10) Определите частоту дискретизации, которая была использована при записи голоса на цифровой носитель
samples = length(y); % Кол-во элементов в файле
duration = 4.7232;
fs_raschetnoe = samples / duration;
fprintf('Count of samples: %d\n', samples);
fprintf('Частота дискретизации посчитанная по формуле "samples / duration": %.2f Гц\n', fs_raschetnoe);
fprintf('Настоящая частота дискретизации: %.2f Гц\n', fs);
 
%% 11) Проредите полученный массив y с помощью функции downsample
y1 = downsample(y, 10);          % Оставляем каждый 10-й отсчет
zvuk_y = audioplayer(y, fs);    % Исходный звук
zvuk_y1 = audioplayer(y1, fs/10);% Прореженный звук

t = (0:length(y)-1) / fs;      % Ось времени для исходного сигнала
t1 = (0:length(y1)-1) / (fs/10); % Ось времени для прореженного сигнала

% playblocking(zvuk_y);            % Воспроизводим исходный звук и ждём окончания
% playblocking(zvuk_y1);          % Потом прореженный звук

filename = 'my_audio1.wav';
Fs = 44100/10;
audiowrite(filename, y1, Fs);

%% 12) Выполните прямое дискретное преобразование Фурье для оригинального звучания и для прореженного сигнала
y_spectrum_orig = fft(y); % Спектр сигнала в комплексной форме оригинальной версии
y_spectrum_down = fft(y1); % Спектр сигнала в комплексной форме downsample версии

N_orig = length(y); % Число samples оригинальной версии
N_down = length(y1); % Число samples downsample версии

f_axis_orig = (0:N_orig-1)*(fs/N_orig); % Шкала частот оригинальной версии
f_axis_down = (0:N_down-1)*(Fs/N_down); % Шкала частот 

ampl_orig = abs(y_spectrum_orig(1:N_orig/2));
ampl_down = abs(y_spectrum_down(1:N_down/2));

figure
plot(f_axis_orig(1:N_orig/2), 20 * log(ampl_orig/max(ampl_orig))); % Визуализируем амплитудный спект оригинального сигнала
xlabel('Частота')
ylabel('Амплитуда, dB')
title('Амплитудный спектр оригинального сигнала, fs = 44100 Hz')
grid on;

figure
plot(f_axis_down(1:N_down/2), 20 * log(ampl_down/max(ampl_down))); % Визуализируем амплитудный спект downsample сигнала
xlabel('Частота')
ylabel('Амплитуда, dB')
title('Амплитудный спектр downsample сигнала, fs = 44100 Hz')
grid on;

%% 13) Оцените влияние разрядности АЦП на спектр сигнала
% Исходные данные
A = 12;
phase = pi/6;
f = 14;
fs = 1000;
t = 0:1/fs:1;
y = A * cos(2 * pi * f * t + phase);

bits_list = [3,4,5,6];
errors = zeros(size(bits_list));

y_normis = y / max(y);

figure;
for i = 1:length(bits_list)
    bits = bits_list(i);
    [yq_r, yq_n] = quantize_signal(y_normis, bits);

    errors(i) = mean(abs(y_normis - yq_n));

    subplot(length(bits_list),1,i);
    plot(t, yq_r);
    title(['Сигнал после квантования, ', num2str(bits), ' бит']);
    xlabel('Время, с');
    ylabel('Амплитуда');
end

disp('Средние ошибки квантования:')
disp(table(bits_list', errors', 'VariableNames', {'Разрядность', 'Средняя_ошибка'}))

function [yq_r, yq_n] = quantize_signal(y, bits)
    levels = 2^bits; % количество уровней
    y_norm = (y + 1) / 2; % перевод диапазона [-1,1] -> [0,1]
    y_scaled = y_norm * (levels - 1); % масштабирование под уровни
    yq_r = round(y_scaled); % округление до ближайшего уровня
    yq_n = round(y_scaled)/max(y_scaled) * 2 - 1;
end
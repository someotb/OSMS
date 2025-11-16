clear; close all; clc;

%% 8) Запишите аудиофайл со своим голосом (например, в формате wav) и проанализируйте его
% Максимальная частота дискретизации в спектре моего голоса примерно 19700 Hz, следовательно требуемая для оцифровки частота дискретизации 39400 Hz

%% 9) Используя библиотеки работы со звуком Matlab (или Pyton) проанализируйте имеющуюся у вас запись голоса
[y, fs] = audioread('voices/voice.wav'); % Считываем файл с помошью audioread() ('voice.wav', duration - 4.7232) ('voice1.wav', duration - 9.5267)

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

% t = (0:length(y)-1) / fs;      % Ось времени для исходного сигнала
t1 = (0:length(y1)-1) / (fs/10); % Ось времени для прореженного сигнала

% playblocking(zvuk_y);            % Воспроизводим исходный звук и ждём окончания
% playblocking(zvuk_y1);          % Потом прореженный звук

% filename = 'my_audio1.wav';
Fs = 44100/10;
% audiowrite(filename, y1, Fs);

%% 12) Выполните прямое дискретное преобразование Фурье для оригинального звучания и для прореженного сигнала
y = y(:, 1);
y1 = y1(:, 1);

window_size = 131072;      
window_size_down = min(length(y1), 16384);
overlap = 0.5;

step = round(window_size * (1 - overlap));
N = length(y);
num_windows = floor((N - window_size) / step) + 1;

window = hanning(window_size);
spectra = zeros(window_size/2, num_windows);

for i = 1:num_windows
    start_idx = (i - 1) * step + 1;
    end_idx = start_idx + window_size - 1;
    if end_idx > N, break; end
    
    segment = y(start_idx:end_idx);
    segment_windowed = segment .* window;
    Y = fft(segment_windowed);
    Y_pos = Y(1:window_size/2);
    ampl = abs(Y_pos);
    spectra(:, i) = ampl;
end

avg_spectrum = mean(spectra, 2);
scaling_factor = 2 / sum(window);
avg_spectrum = avg_spectrum * scaling_factor;

Y_db_orig = 20 * log10(avg_spectrum + eps);
Y_db_orig(Y_db_orig < -120) = -120;
f_axis_orig = (0 : window_size/2 - 1) * (fs / window_size);

step_down = round(window_size_down * (1 - overlap));
N_down = length(y1);
num_windows_down = floor((N_down - window_size_down) / step_down) + 1;

window_down = hanning(window_size_down);
spectra_down = zeros(window_size_down/2, num_windows_down);

for i = 1:num_windows_down
    start_idx = (i - 1) * step_down + 1;
    end_idx = start_idx + window_size_down - 1;
    if end_idx > N_down, break; end
    
    segment = y1(start_idx:end_idx);
    segment_windowed = segment .* window_down;
    Y = fft(segment_windowed);
    Y_pos = Y(1:window_size_down/2);
    ampl = abs(Y_pos);
    spectra_down(:, i) = ampl;
end

avg_spectrum_down = mean(spectra_down, 2);
scaling_factor_down = 2 / sum(window_down);
avg_spectrum_down = avg_spectrum_down * scaling_factor_down;

Y_db_down = 20 * log10(avg_spectrum_down + eps);
Y_db_down(Y_db_down < -120) = -120;
f_axis_down = (0 : window_size_down/2 - 1) * ((fs/10) / window_size_down);

figure;
semilogx(f_axis_orig, Y_db_orig, 'Color', [0.4 0 0.8], 'LineWidth', 1.3);
xlabel('Frequency (Hz)');
ylabel('Amplitude (dB)');
title('Original Signal Spectrum');
grid on;
xlim([20 2000]);
ylim([-120 0]);
set(gca, 'FontSize', 12, 'GridAlpha', 0.3);
box on;

figure;
semilogx(f_axis_down, Y_db_down, 'Color', [0.85 0 0], 'LineWidth', 1.3);
xlabel('Frequency (Hz)');
ylabel('Amplitude (dB)');
title('Downsampled Signal Spectrum');
grid on;
xlim([20 2000]);
ylim([-120 0]);
set(gca, 'FontSize', 12, 'GridAlpha', 0.3);
box on;

%% 13) Оцените влияние разрядности АЦП на спектр сигнала
% Исходные данные
A = 12;
phase = pi/6;
f = 4;
fs = 44100;
t = 0:1/fs:1;
y = A * cos(2 * pi * f * t + phase);

% Спектральный график для сигнала y = A * cos(2 * pi * f * t + phase)
y_spec = fft(y);
N_y_spec = length(y_spec);
axis_y_spec = (0:N_y_spec-1)*(fs/N_y_spec);
ample_y_spec = abs(y_spec(1:N_y_spec/2));

figure
plot(axis_y_spec(1:N_y_spec/2), 20 * log10(ample_y_spec/max(ample_y_spec)));
xlabel('Частота')
ylabel('Амплитуда, dB')
title('Амплитудный спектр оригинального сигнала(13), fs = 44100 Hz')
xlim([0 20])
grid on;

bits_list = [3,4,5,6];
errors = zeros(size(bits_list));

y_normis = y / max(y); % Сигнал y: от -1 до 1

% Надо сделать график спектра оцифрованного сигнала ТУТ!

[yq_r, yq_n] = quantize_signal(y_normis, 3);
y_spec_a = fft(yq_r);

N_y_spec_a = length(yq_r);
axis_y_spec_a = (0:N_y_spec_a-1)*(fs/N_y_spec_a); % Ось частот
ample_y_spec_a = abs(y_spec_a(1:N_y_spec_a/2));

figure
plot(axis_y_spec_a(1:N_y_spec_a/2), 20 * log10(ample_y_spec_a/max(ample_y_spec_a)));
xlabel('Частота')
ylabel('Амплитуда, dB')
title('Амплитудный спектр квантованного (оцифрованного) сигнала, 3 бит')
xlim([0 1000])
grid on;

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
    grid on;
end

disp('Средние ошибки квантования:')
disp(table(bits_list', errors', 'VariableNames', {'Разрядность', 'Средняя_ошибка'}))

% Сделаем так, чтобы получился clipping
bit = 5;
y_c = y / max(y);
coef = 10; % Во сколько раз сигнал больше максимального уровня
[y_cq] = quantize_signal_clip(y_c, bit, coef);
[y_q, errors] = quantize_signal(y_c, bit);

figure
subplot(2, 1, 1);
plot(t, y_cq);
title(['Усиленный сигнал после квантования, ', num2str(bit), ' бит']);
xlabel('Время, с');
ylabel('Амплитуда');
grid on;
subplot(2, 1, 2);
plot(t, y_q);
title(['Обычный сигнал после квантования, ', num2str(bit), ' бит']);
xlabel('Время, с');
ylabel('Амплитуда');
grid on;

% График спектра клиппированного и обычного сигнала
N = length(y_cq);
f = (0:N/2-1) * (fs / N);

Y_cq = fft(y_cq)/N;
Y_q  = fft(y_q)/N;

Y_cq = Y_cq(1:N/2);
Y_q  = Y_q(1:N/2);

Y_cq(2:end-1) = 2*Y_cq(2:end-1);
Y_q(2:end-1)  = 2*Y_q(2:end-1);

% Перевод в dB
Y_cq_dB = 20*log10(abs(Y_cq) + eps);
Y_q_dB  = 20*log10(abs(Y_q)  + eps);

% График
figure;
plot(f, Y_cq_dB, 'LineWidth', 1.2);
hold on;
plot(f, Y_q_dB, 'LineWidth', 1.2);
grid on;

xlabel('Частота, Гц');
ylabel('Амплитуда, dB');
legend('Clip + Quant', 'Normal Quant');
xlim([0 2000]);
ylim([-120 10]);
title('Спектр клиппированного и обычного квантованного сигнала');

function [yq_r, yq_n] = quantize_signal(y, bits)
    levels = 2^bits; % количество уровней
    y_norm = (y + 1) / 2; % перевод диапазона [-1,1] -> [0,1]
    y_scaled = y_norm * (levels - 1); % масштабирование под уровни
    yq_r = round(y_scaled); % округление до ближайшего уровня
    yq_n = round(y_scaled)/max(y_scaled) * 2 - 1;
end

function yq_r = quantize_signal_clip(y, bits, coef)
    y = max(min(y, 1), -1);
    y = y * coef;
    y(y > 1) = 1;
    y(y < -1) = -1;
    levels = 2^bits;
    y_scaled = (y + 1)/2 * (levels - 1);
    yq_r = round(y_scaled);
end

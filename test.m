f = 300;
t = 0:0.001:1;
A = 5;
signal = A * cos(2 * pi * f * t);

Y = fft(signal);
N = length(Y);
fs = 1000;
Y_axis = (1:N-1) * fs/N;
Y_abs = abs(Y);
Y_absa = Y_abs(1:N-1);

figure 
plot(Y_axis, Y_absa);
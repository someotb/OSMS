fs = 44100;
t = (0:1/fs:1-1/fs);
f0 = 88;
y1 = sin(2*pi*f0*t);
Y = fft(y1);
N = length(Y);
ampl = abs(Y(1:N/2)) * 2 / N;
ampl_db = 20 * log10(ampl + eps)
asix = (0:N/2-1)*(fs/N);

figure
semilogx(asix, ampl_db);

audiowrite('test88.wav', y1, fs);

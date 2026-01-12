#include <cstdio>
#include<iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include <random>
#include "crc_utils.hpp"

using namespace std;

int main() {
    vector<int> x = {1, 0, 0, 0, 0}; // 16
    vector<int> y = {1, 0, 1, 1, 1}; // 23
    vector<int> G = {1, 0, 0, 0, 0, 1, 1, 1}; // Пораждающий полином из 5-ой лабораторной работы
    vector<int> res(pow(2, (int)x.size()) - 1);
    vector<int> bin_fullname;
    vector<int> samples;
    int N = 10; // Upsample koef

    // 1,2) Fullname in ASCII
    fullname_to_binary(bin_fullname);

    output("Your fullname in binary:", bin_fullname);

    // 3) CRC
    auto crc = CRC(bin_fullname, G);
    cout << "CRC-sequence: ";
    for (auto i : crc) cout << i << " ";
    cout << "\n\n";

    vector<int> crc_bin_fullname = bin_fullname;
    crc_bin_fullname.insert(crc_bin_fullname.end(), crc.begin(), crc.end());

    output("Your fullname in binary + CRC:", crc_bin_fullname);

    // 4) Выведите получившуюся последовательность Голда на экран и добавите к данным
    gold(x, y, res);

    // Выводим в файл, чтобы потом визуализировать на Python
    ofstream fout_gold("../data/gold_sequence.txt");
    for (int bit : res) fout_gold << bit << "\n";
    fout_gold.close();

    bool i = check_for_m_pos(res);
    if (i) cout << "Последовательность ЯВЛЯЕТСЯ M-последовательностью\n";
    else { cout << "Последовательность НЕ ЯВЛЯЕТСЯ M-последовательностью\n"; return 0;}

    cout << "Последовательность Голда: " << endl;
    for (int i = 0; i < pow(2, (int)x.size()) - 1; ++i) {
        cout << res[i] << " ";
    }
    cout << "\n\n";

    vector<int> tx_sequence = res;
    tx_sequence.insert(tx_sequence.end(), crc_bin_fullname.begin(), crc_bin_fullname.end());

    output("Gold + Data + CRC:", tx_sequence);

    // 5) Преобразуем биты в сигнал, на каждый бит приходится N отсчетов
    samples = bits_to_samples(tx_sequence, N);
    ofstream fout_samples("../data/samples_sequence.txt");
    for (int bit : samples) fout_samples << bit << "\n";
    fout_samples.close();

    // 6) Сделать массив signal, в начале 0, затем samples
    vector<int> zeros(samples.size() * 2, 0);
    int pos;
    cout << "Enter position to insert the signal (0.." << zeros.size() - samples.size() << "): ";
    cin >> pos;

    if (pos < 0) pos = 0;
    if (pos > (int)(zeros.size() - samples.size())) pos = zeros.size() - samples.size();

    vector<int> signal = zeros;

    for (size_t i = 0; i < samples.size(); ++i)
        signal[pos + i] = samples[i];

    ofstream signal_samples("../data/signal_sequence.txt");
    for (int bits : signal) signal_samples << bits << "\n";
    signal_samples.close();

    // 7) Добавление шума
    float sigma = 0;
    cout << "Enter standard deviation of noise sigma: ";
    cin >> sigma;

    default_random_engine generator;
    normal_distribution<float> distribution(0.0, sigma);

    vector<float> noise(signal.size(), 0.0f);
    for (int i = 0; i < signal.size(); ++i) {
        noise[i] = distribution(generator);
    }

    vector<float> noisy_signal(signal.size(), 0.0f);
    for (int i = 0; i < signal.size(); ++i) {
        noisy_signal[i] = signal[i] + noise[i];
    }

    ofstream fout_noisy("../data/noisy_signal_sequence.txt");
    for (float bits : noisy_signal) fout_noisy << bits << "\n";
    fout_noisy.close();

    // 8) Найдем начало синхросигнала
    vector<int> gold_samples = bits_to_samples(res, N);
    int sync_pos = correlation_receiver(noisy_signal, gold_samples);
    vector<float> aligned_signal(noisy_signal.begin() + sync_pos, noisy_signal.end());

    ofstream fout_aligned("../data/aligned_signal_sequence.txt");
    for (float bits : aligned_signal) fout_aligned << bits << "\n";
    fout_aligned.close();

    // 9) Принятие решений 1 или 0

}

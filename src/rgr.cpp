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
    int N = 20; // Upsample koef

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

    ofstream fout_bits("../data/bits_sequence.txt");
    for (int bit : tx_sequence) fout_bits << bit << "\n";
    fout_bits.close();

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
    float P = 0.5;
    float mean = 0;
    vector<int> soluted_signal;
    for (size_t i = 0; i + N <= aligned_signal.size(); i+=N) {
        float summa = 0.0f;
        for (int j = 0; j < N; ++j) {
             summa += aligned_signal[i + j];
        }
        mean = summa / N;
        soluted_signal.push_back(mean >= P ? 1 : 0);
    }

    int total_bits = tx_sequence.size(); // L+M+G
    if (soluted_signal.size() > total_bits) {
        soluted_signal.resize(total_bits);
    }

    ofstream fout_soluted("../data/soluted_signal_sequence.txt");
    for (float bits : soluted_signal) fout_soluted << bits << "\n";
    fout_soluted.close();

    // 10) Удалите из полученного массива G-бит последовательности cинхронизации
    size_t gold_len = res.size();
    vector<int> received_data(soluted_signal.begin() + gold_len, soluted_signal.end());

    // 11) Проверьте корректность приема бит с помощью CRC
    int crc_len = G.size() - 1;
    vector<int> data_bits(received_data.begin(), received_data.end() - crc_len);
    vector<int> crc_bits(received_data.end() - crc_len, received_data.end());

    vector<int> crc_rev = CRC_reverse(data_bits, crc_bits, G);
    crc_check_error(crc_rev);

    // 12) Если нет ошибок, то декодировать обратно в текст
    bool error_found = false;
    for (size_t i = 0; i < min(soluted_signal.size(), tx_sequence.size()); ++i) {
        if (soluted_signal[i] != tx_sequence[i]) {
            cout << "Bit " << i << " is incorrect!" << endl;
            error_found = true;
        }
    }

    if (error_found == false) {
        string decoded_text;
        for (size_t i = 0; i + 7 < data_bits.size(); i += 8) {
            decoded_text += bits_to_char(data_bits, i);
        }

        cout << "Decoded text: " << decoded_text << endl;
    } else cout << "Correct decoding is not possible\n";

    /*
     * 13) Надо запустить программу 3 раза, для N = 5, 10, 20
     * Данные сохранять в специальную папку `data_for_13ex`
     * В ней уже есть 3 текстовых файла с данными, которые я тестировал
     * с входными данными:
     * 1) fullname: Kirill Liubimov
     * 2) Position to insert signal: 100
     * 3) Deviation of noise: 0.2
     */
}

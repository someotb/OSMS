#include "crc_utils.hpp"
#include <cstddef>
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <vector>
#include <random>

using namespace std;

void decimal_to_binary(int decimal, vector<int>& binary) {
    binary.clear();
    for (int i = 7; i >= 0; --i) {
        binary.push_back((decimal >> i) & 1);
    }
}

void fullname_to_binary(vector<int>& binary_string_fullname) {
    string input;
    vector<int> binary_word;

    cout << "Enter your name and surname: ";
    getline(cin, input);

    for (char c : input) {
        decimal_to_binary(static_cast<unsigned char>(c), binary_word);
        binary_string_fullname.insert(binary_string_fullname.end(), binary_word.begin(), binary_word.end());
    }
}

vector<int> CRC(vector<int> data, const vector<int>& generator) {
    int n = generator.size();
    int k = n - 1;
    data.resize(data.size() + k, 0);
    vector<int> remainder(data);

    for (size_t i = 0; i <= data.size() - n; ++i)
        if (remainder[i] == 1)
            for (int j = 0; j < n; ++j)
                remainder[i + j] ^= generator[j];

    return vector<int>(remainder.end() - k, remainder.end());
}

vector<int> CRC_reverse(vector<int> data, vector<int> crc, const vector<int> generator) {
    int n = generator.size();
    int k = n - 1;

    data.insert(data.end(), crc.begin(), crc.end());
    vector<int> remainder(data);

    for (size_t i = 0; i <= data.size() - n; ++i)
        if (remainder[i] == 1)
            for (int j = 0; j < n; ++j)
                remainder[i + j] ^= generator[j];

    return vector<int>(remainder.end() - k, remainder.end());
}

void crc_check_error(vector<int> crc_rev) {
    size_t cnt = count(crc_rev.begin(), crc_rev.end(), 0);
    bool ok = (cnt == crc_rev.size());
    if (ok) cout << "No Errors in data!" << endl;
    else cout << "Errors in data!" << endl;
}

int summator(int a, int b) { return a ^ b ? 1 : 0; }

void gold(vector<int> x, vector<int> y, vector<int> &res) {
    int first_x, first_y = 0;
    for (int i = 0; i < pow(2, (int)x.size()) - 1; ++i) {
        first_x = summator(x[2], x[4]);
        first_y = summator(y[2], y[4]);
        res[i] = summator(x.back(), y.back());
        x.pop_back();
        x.insert(x.begin(), first_x);
        y.pop_back();
        y.insert(y.begin(), first_y);
    }
}

vector<int> cyclic_shift(const vector<int>& bits, int k) {
    int N = bits.size();
    vector<int> res(N);
    for (int i = 0; i < N; ++i)
        res[(i + k) % N] = bits[i];
    return res;
}

double auto_corr(vector<int> a, vector<int> a_sdvig) {
    int cnt_pohoji = 0, cnt_ne_pohoji = 0;
    for (int i = 0; i < (int)a.size(); ++i)
        if (a[i] == a_sdvig[i]) cnt_pohoji++;
        else cnt_ne_pohoji++;

    return (1.0 / a.size()) * (cnt_pohoji - cnt_ne_pohoji);
}

bool check_for_m_pos(const vector<int>& bits) {
    int N = bits.size();
    bool is_m = true;

    int ones = count(bits.begin(), bits.end(), 1);
    int zeros = N - ones;
    if (abs(ones - zeros) != 1) { cout << "Нарушен баланс\n"; is_m = false; }

    double expected = -1.0 / N;
    const double eps = 1e-6;

    for (int k = 1; k < N; ++k) {
        vector<int> shifted = cyclic_shift(bits, k);

        if (shifted == bits) { cout << "Период меньше N\n"; is_m = false; break; }

        double r = auto_corr(bits, shifted);
        if (abs(r - expected) > eps) { cout << "Нарушена автокорреляция\n"; is_m = false; break; }
    }

    if (is_m) return 1;
    else return 0;
}

void output(std::string title, vector<int>& data) {
    cout << title;
    for (size_t i = 0; i < data.size(); ++i) {
        if (i % 8 == 0) cout << "\n";
        cout << data[i] << " ";
    }
    cout << "\n\n";
}

vector<int> bits_to_samples(const vector<int>& data, int N) {
    vector<int> samples;
    samples.reserve(data.size() * N);

    for (int b : data) {
        for (int i = 0; i < N; ++i) {
            samples.push_back(b);
        }
    }

    return samples;
}

CorrResult correlation_receiver(const vector<float>& received, const vector<int>& sync_samples) {
    int R = received.size();
    int S = sync_samples.size();

    double max_corr = -1e18;
    int best_pos = 0;

    for (int k = 0; k <= R - S; ++k) {
        double corr = 0.0;
        for (int i = 0; i < S; ++i)
            corr += received[k + i] * sync_samples[i];

        if (corr > max_corr) {
            max_corr = corr;
            best_pos = k;
        }
    }

    return { best_pos, max_corr };
}


char bits_to_char(const vector<int>& bits, size_t start) {
    int value = 0;
    for (int i = 0; i < 8; ++i) {
        value = (value << 1) | bits[start + i];
    }
    return static_cast<char>(value);
}

Result run_experiment(int N, float sigma, int signal_pos, const vector<int>& tx_sequence, const vector<int>& gold_bits, const vector<int>& G) {
    Result res;
    res.crc_ok = false;
    res.corr_peak = 0.0;

    vector<int> samples = bits_to_samples(
        const_cast<vector<int>&>(tx_sequence), N
    );

    vector<int> zeros(samples.size() * 2, 0);
    if (signal_pos < 0) signal_pos = 0;
    if (signal_pos > (int)(zeros.size() - samples.size()))
        signal_pos = zeros.size() - samples.size();

    vector<int> signal = zeros;
    for (size_t i = 0; i < samples.size(); ++i)
        signal[signal_pos + i] = samples[i];

    static default_random_engine generator(random_device{}());
    normal_distribution<float> distribution(0.0, sigma);

    vector<float> noisy_signal(signal.size());
    for (size_t i = 0; i < signal.size(); ++i)
        noisy_signal[i] = signal[i] + distribution(generator);

    vector<int> gold_samples = bits_to_samples(const_cast<vector<int>&>(gold_bits), N);

    CorrResult corr = correlation_receiver(noisy_signal, gold_samples);

    res.corr_peak = corr.peak;

    if (corr.pos + gold_samples.size() >= noisy_signal.size()) return res;

    vector<float> aligned_signal(noisy_signal.begin() + corr.pos, noisy_signal.end());

    vector<int> decided_bits;
    for (size_t i = 0; i + N <= aligned_signal.size(); i += N) {
        float sum = 0.0f;
        for (int j = 0; j < N; ++j)
            sum += aligned_signal[i + j];

        float mean = sum / N;
        decided_bits.push_back(mean >= 0.5f ? 1 : 0);
    }

    if (decided_bits.size() < tx_sequence.size())
        return res;

    decided_bits.resize(tx_sequence.size());

    size_t gold_len = gold_bits.size();
    if (decided_bits.size() <= gold_len) return res;

    vector<int> received_data(decided_bits.begin() + gold_len, decided_bits.end());

    size_t crc_len = G.size() - 1;
    if (received_data.size() <= crc_len)
        return res;

    vector<int> data_bits(received_data.begin(), received_data.end() - crc_len);
    vector<int> crc_bits(received_data.end() - crc_len, received_data.end());

    vector<int> crc_rev = CRC_reverse(data_bits, crc_bits, G);

    res.crc_ok = (count(crc_rev.begin(), crc_rev.end(), 0) == (int)crc_rev.size());

    return res;
}

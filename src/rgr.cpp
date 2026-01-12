#include <cstdio>
#include<iostream>
#include <vector>
#include <math.h>
#include <fstream>
#include "crc_utils.hpp"

using namespace std;

int main() {
    vector<int> x = {1, 0, 0, 0, 0}; // 16
    vector<int> y = {1, 0, 1, 1, 1}; // 23
    vector<int> G = {1, 0, 0, 0, 0, 1, 1, 1}; // Пораждающий полином из 5-ой лабораторной работы
    vector<int> res(pow(2, (int)x.size()) - 1);
    vector<int> bin_fullname;
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
    ofstream fout_gold("gold_sequence.txt");
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

    vector<int> signal;
    signal = bits_to_samples(tx_sequence, N);
    ofstream fout_signal("signal_sequence.txt");
    for (int bit : signal) fout_signal << bit << "\n";
    fout_signal.close();
}

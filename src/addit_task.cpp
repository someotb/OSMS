#include <vector>
#include <fstream>
#include <cmath>
#include <chrono>
#include <iostream>
#include "crc_utils.hpp"

constexpr int TRIALS = 1;
using namespace std;

/*
 *  Задание на защиту по РГР:
 *  1) построй  зависимость вероятности успешного декодирования блока с данными (CRC=OK)
 *  в зависимости от числа семплов на 1 бит (от 1 до 200) и для сигмы = 0,4; 0,6; 0,8; 1,0; 1,8
 *  2) и максимум пика автокорреляции при синхронизации от сигмы (от 0 до 4 сигма) и на этом же
 *  графике - вероятность декодирования блоков с данными (несколько кривых - 5/20/100/200 семплов на 1 бит)
 */

int main() {
    int mode = 0;
    cout << "Which experiment you wanna try: #1 - (1), #2 - (2): ";
    cin >> mode;

    vector<int> x = {1,0,0,0,0};
    vector<int> y = {1,0,1,1,1};
    vector<int> G = {1,0,0,0,0,1,1,1};
    vector<int> gold_bits(pow(2, x.size()) - 1);

    gold(x, y, gold_bits);

    vector<int> bin;
    fullname_to_binary(bin);

    vector<int> crc = CRC(bin, G);
    vector<int> tx = gold_bits;
    tx.insert(tx.end(), bin.begin(), bin.end());
    tx.insert(tx.end(), crc.begin(), crc.end());

    // 1)
    if (mode == 1) {
        cout << "Starting experiment #1...\n";
        vector<float> sigmas = {0.4f,0.6f,0.8f,1.0f,1.8f};
        vector<int> nas;
        for (int i = 1; i < 200; i++) nas.push_back(i);
        ofstream fout("../data_for_additional_task/p_crc_vs_N.txt");

        auto start_time = std::chrono::high_resolution_clock::now();

        for (float sigma : sigmas) {
            for (int N : nas) {
                int ok = 0;
                cout << "Experiment #1 with: Sigma=" << sigma << ", N=" << N << " is running..." << endl;
                for (int t = 0; t < TRIALS; ++t) {
                    Result r = run_experiment(N, sigma, 100, tx, gold_bits, G);
                    if (r.crc_ok) ok++;
                }
                fout << sigma << " " << N << " " << (double)ok / TRIALS << "\n";
                cout << "Experiment #1 complited with: Sigma=" << sigma << ", N=" << N << endl;
            }
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end_time - start_time;
        std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
    }

    // 2)
    else if (mode == 2) {
        cout << "Starting experiment #2...\n";
        ofstream fout2("../data_for_additional_task/corr_and_crc_vs_sigma.txt");
        vector<float> sigmas2;
        for (float s = 0.0f; s <= 4.0f; s += 0.2f) sigmas2.push_back(s);
        vector<int> nas2 = {5, 20, 100, 200};

        auto start_time = std::chrono::high_resolution_clock::now();

        for (int N : nas2) {
            for (float sigma : sigmas2) {
                double corr_sum = 0.0;
                int crc_ok_cnt = 0;
                for (int t = 0; t < TRIALS; ++t) {
                    cout << "Experiment #2 with: Sigma=" << sigma << ", N=" << N << " is running..." << endl;
                    Result r = run_experiment(N, sigma, 100, tx, gold_bits, G);
                    corr_sum += r.corr_peak;
                    if (r.crc_ok) crc_ok_cnt++;
                }
                double avg_corr = corr_sum / TRIALS;
                double p_crc = (double)crc_ok_cnt / TRIALS;
                fout2 << N << " " << sigma << " " << avg_corr << " " << p_crc << "\n";
                cout << "Experiment #2 complited with: Sigma=" << sigma << ", N=" << N << endl;
            }
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end_time - start_time;
        std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
        fout2.close();
    }
    else cout << "Please enter a valid experiment number (1 or 2).\n";
}

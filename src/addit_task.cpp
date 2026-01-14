#include <vector>
#include <fstream>
#include <cmath>
#include <chrono>
#include <iostream>
#include "crc_utils.hpp"

constexpr int TRIALS = 20;
using namespace std;

/*
 *  Задание на защиту по РГР:
 *  1) построй  зависимость вероятности успешного декодирования блока с данными (CRC=OK)
 *  в зависимости от числа семплов на 1 бит (от 1 до 200) и для сигмы = 0,4; 0,6; 0,8; 1,0; 1,8
 *  2) и максимум пика автокорреляции при синхронизации от сигмы (от 0 до 4 сигма) и на этом же
 *  графике - вероятность декодирования блоков с данными (несколько кривых - 5/20/100/200 семплов на 1 бит)
 */

int main() {
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

    vector<float> sigmas = {0.4f,0.6f,0.8f,1.0f,1.8f};
    vector<int> Nas = {5, 20, 100, 200};
    ofstream fout("../data/p_crc_vs_N.txt");

    auto start_time = std::chrono::high_resolution_clock::now();

    for (float sigma : sigmas) {
        for (int N : Nas) {
            int ok = 0;
            for (int t = 0; t < TRIALS; ++t) {
                Result r = run_experiment(N, sigma, 100, tx, gold_bits, G);
                if (r.crc_ok) ok++;
            }
            fout << sigma << " " << N << " " << (double)ok / TRIALS << "\n";
            cout << "Experiment complited with: Sigma=" << sigma << ", N=" << N << endl;
        }
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
}

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

#define N 20 + 16

using namespace std;

vector<int> CRC(vector<int> data, const vector<int>& generator) {
    int n = generator.size();
    int k = n - 1;

    data.resize(data.size() + k, 0);

    vector<int> remainder(data);

    for (size_t i = 0; i <= data.size() - n; ++i) {
        if (remainder[i] == 1) {
            for (int j = 0; j < n; ++j) {
                remainder[i + j] ^= generator[j];
            }
        }
    }

    return vector<int>(remainder.end() - k, remainder.end());
}

vector<int> CRC_reverse(vector<int> data, vector<int> crc, const vector<int> generator) {
    int n = generator.size();
    int k = n - 1;

    data.insert(data.end(), crc.begin(), crc.end());

    vector<int> remainder(data);

    for (size_t i = 0; i <= data.size() - n; ++i) {
        if (remainder[i] == 1) {
            for (int j = 0; j < n; ++j) {
                remainder[i + j] ^= generator[j];
            }
        }
    }

    return vector<int>(remainder.end() - k, remainder.end());
}

void crc_check_error(vector<int> crc_rev) {
    int cnt = count(crc_rev.begin(), crc_rev.end(), 0) == (int)crc_rev.size();
    if (cnt) {
        cout << "No Errors in data!" << endl;
    } else {
        cout << "Errors in data!" << endl;
    }
}


int main() {
    // Напишите программу на языке С/С++ для вычисления CRC для пакета данных длиной N бит (N= 20 + порядковый номер в журнале) и
    // определения факта наличия ошибки при передаче пакета по каналу связи.
    vector<int> data(N);
    vector<int> generator = {0, 0, 1, 1};
    vector<int> crc(generator.size() - 1);
    vector<int> crc_rev(generator.size() - 1);

    for (int i = 0; i < N; ++i) data[i] = rand() % 2;

    crc = CRC(data, generator);
    crc_rev = CRC_reverse(data, crc, generator);
    crc_check_error(crc_rev);


    return 0;
}

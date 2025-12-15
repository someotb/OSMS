#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <iomanip>

#define N 20 + 16
#define N_NEW 250

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
    cout << endl;
}

void output(string title, vector<int> crc, vector<int> data, vector<int> generator) {
    vector<int> crc_rev(generator.size() - 1);
    cout << title << endl;
    cout << "CRC код: ";
    for (size_t i = 0; i < crc.size(); ++i) cout << crc[i];
    cout << endl;
    crc_rev = CRC_reverse(data, crc, generator);
    crc_check_error(crc_rev);
}

int main() {
    // 1) Напишите программу на языке С/С++ для вычисления CRC для пакета данных длиной N бит (N= 20 + порядковый номер в журнале) и
    // определения факта наличия ошибки при передаче пакета по каналу связи.

    vector<int> data(N);
    vector<int> data_new(N_NEW);
    vector<int> packet = data_new;
    vector<int> generator = {0, 0, 1, 1};
    vector<int> crc(generator.size() - 1);
    vector<int> G = {1, 0, 0, 0, 0, 1, 1, 1};

    for (int i = 0; i < N; ++i) data[i] = rand() % 2;
    for (int i = 0; i < N_NEW; ++i) data_new[i] = rand() % 2;

    crc = CRC(data, generator);
    output("Передача бит, с пораждающим полиномом [0, 0, 1, 1]: ", crc, data, generator);

    // 2) Порождающий полином G для делителя выберите в соответствии с
    // вариантом. Номер варианта – порядковый номер в журнале группы.
    // G=x7+x2+x+1

    crc = CRC(data, G);
    output("Передача бит, с пораждающим полиномом [1, 0, 0, 0, 0, 1, 1, 1]: ", crc, data, G);

    // 3) Возьмите N, равное 250 битам. Проделайте п.1-3

    crc = CRC(data_new, generator);
    output("Передача бит, с пораждающим полиномом [0, 0, 1, 1], N = 250: ", crc, data_new, generator);

    crc = CRC(data_new, G);
    output("Передача бит, с пораждающим полиномом [1, 0, 0, 0, 0, 1, 1, 1], N = 250: ", crc, data_new, G);

    // Сделайте цикл из 250+CRC length итераций и в этом цикле по очереди искажайте по одному биту

    crc = CRC(data_new, G);
    packet.insert(packet.end(), crc.begin(), crc.end());

    for (size_t i = 0; i < packet.size(); ++i) {
        packet[i] ^= 1;

        vector<int> recv_data(packet.begin(), packet.end() - crc.size());
        vector<int> recv_crc(packet.end() - crc.size(), packet.end());

        vector<int> rem = CRC_reverse(recv_data, recv_crc, G);

        bool ok = all_of(rem.begin(), rem.end(), [](int x){ return x == 0; });
        cout << "Если бит " << setw(3) << i << " исказился: " << (ok ? "OK" : "ERROR") << endl;

        packet[i] ^= 1;  // возвращаем обратно
    }

    return 0;
}

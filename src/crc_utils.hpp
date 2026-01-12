#pragma once
#include <vector>
#include <string>
using std::vector;

// Преобразование десятичного числа в бинарный вектор (8 бит)
void decimal_to_binary(int decimal, vector<int>& binary);

// Преобразование полного имени в бинарный вектор
void fullname_to_binary(vector<int>& binary_string_fullname);

// CRC вычисление
vector<int> CRC(vector<int> data, const vector<int>& generator);

// Проверка CRC
vector<int> CRC_reverse(vector<int> data, vector<int> crc, const vector<int> generator);

// Проверка ошибок CRC
void crc_check_error(vector<int> crc_rev);

// Золотая последовательность (Gold sequence)
int summator(int a, int b);
void gold(vector<int> x, vector<int> y, vector<int> &res);

// Циклический сдвиг
vector<int> cyclic_shift(const vector<int>& bits, int k);

// Автокорреляция
double auto_corr(vector<int> a, vector<int> a_sdvig);

// Проверка M-последовательности
bool check_for_m_pos(const vector<int>& bits);

// Вывод данных
void output(std::string title, vector<int>& data);

// Перевод битов в сэмплы
vector<int> bits_to_samples(vector<int>& data, int N);

// Нахождение начала синхросигнала
int correlation_receiver(const vector<float>& received, const vector<int>& sync_samples);

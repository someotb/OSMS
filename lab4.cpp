#include <iostream>
#include <vector>
#include <math.h>
#include <iomanip>

using namespace std;

// Вариант 16, Любимов Кирилл, ИА-331

int summator(int a, int b) {
    if (a ^ b) return 1;
    return 0;
}

void gold(vector<int> x, vector<int> y, vector<int> &res) {
    int first_x, first_y = 0;
    for (int i = 0; i < pow(2, (int)x.size()) - 1; ++i) {
        first_x = summator(x[3], x[4]); // Если сделать x[2], x[4] и y[1], y[4], то получится примитивный полином и нормальная m-последовательность.
        first_y = summator(y[1], y[4]);
        res[i] = summator(x.back(), y.back());
        x.pop_back();
        x.insert(x.begin(), first_x);
        y.pop_back();
        y.insert(y.begin(), first_y);
    }
}

vector<int> cyclic_shift(vector<int> bits, vector<int> bits_sdvig, int num_of_sdvig) {
    bits_sdvig = bits;
    for (int i = 0; i < num_of_sdvig; ++i) {
        int last_el = bits_sdvig.back();
        for (int i = bits_sdvig.size() - 1; i > 0; --i) {
            bits_sdvig[i] = bits_sdvig[i - 1];
        }
        bits_sdvig[0] = last_el;
    }
    return bits_sdvig;
}

double auto_corr(vector<int> a, vector<int> a_sdvig) {
    int cnt_pohoji = 0;
    int cnt_ne_pohoji = 0;
    double val_auto_corr = 0;

    for (int i = 0; i < (int)a.size(); ++i) {
        if (a[i] == a_sdvig[i]) {
            cnt_pohoji++;
        } else {
            cnt_ne_pohoji++;
        }
    }

    val_auto_corr = (1 / (double)a.size()) * (cnt_pohoji - cnt_ne_pohoji);
    return val_auto_corr;
}

void show_arr(vector<int> arr) {
    cout << "    ";
    for (int i = 0; i < (int)arr.size(); ++i) {
        cout << arr[i] << "    ";
    }
}

double corr(const vector<int>& x, const vector<int>& y) {
    double sum = 0;
    for (int i = 0; i < x.size(); ++i) {
        int xb = 2 * x[i] - 1;
        int yb = 2 * y[i] - 1;
        sum += xb * yb;
    }
    return sum;
}


double corr_norm1(vector<int> x, vector<int> y){
    double corr_val = 0;
    double top = 0;
    double x2 = 0;
    double y2 = 0;

    for (int i = 0; i < (int)x.size(); ++i) {
        top += x[i] * y[i];
        x2 += x[i]*x[i];
        y2 += y[i]*y[i];
    }

    corr_val = top / sqrt(x2 * y2);
    return corr_val;
}

int main() {
    vector<int> x = {1, 0, 0, 0, 0}; // 16
    vector<int> y = {1, 0, 1, 1, 1}; // 23
    vector<int> res(pow(2, (int)x.size()) - 1);
    vector<int> res_sdvig(pow(2, (int)x.size()) - 1);

    // Выведите получившуюся последовательность Голда на экран.
    gold(x, y, res);
    cout << "Последовательность Голда: " << endl;
    for (int i = 0; i < pow(2, (int)x.size()) - 1; ++i) {
        cout << res[i] << " ";
    }
    cout << endl;
    cout << endl;


    // Сделайте поэлементный циклический сдвиг последовательности и посчитайте автокорреляцию исходной последовательности и сдвинутой
    cout << "Поэлементый циклический сдвиг последовательности и авто корреляция(b - Биты): " << endl;
    cout << "Сдвиг:    b   b+1  b+2  ..." << endl;
    for (int i = 0; i <= (int)res.size(); ++i) {
        res_sdvig = cyclic_shift(res, res_sdvig, i);
        double val_auto_corr = auto_corr(res, res_sdvig);

        cout << setw(5) << i << ":";
        show_arr(res_sdvig);
        cout << setw(10) << val_auto_corr << endl;
    }

    /*
     * «Автокорреляционная функция не обладает свойством двухуровневости,
     * поскольку один из регистров (x) использует непримитивный полином,
     * и его период меньше 31.
     */

    cout << endl;

    // Сформируйте еще одну последовательность Голда, используя свою схему (рис.4.4 или 4.5), такую что x=x+1, а y= у-5
    vector<int> x_new = {1, 0, 0, 0, 1}; // 17
    vector<int> y_new = {1, 0, 1, 1, 0}; // 18
    vector<int> res_new(pow(2, (int)x_new.size()) - 1);

    gold(x_new, y_new, res_new);
    
    for (int i = 0; i < pow(2, (int)x.size()) - 1; ++i) {
        cout << res[i] << " ";
    }
    cout << "| старая последовательность Голда" << endl;

    for (int i = 0; i < pow(2, (int)x_new.size()) - 1; ++i) {
        cout << res_new[i] << " ";
    }
    cout << "| новая последовательность Голда" << endl;
    cout << endl;

    // Вычислите значение взаимной корреляции исходной и новой последовательностей и выведите в терминал.
    cout << "Значение взаимной корреляции исходной и новой последовательностей: " << endl;
    auto N = pow(2, (int)x_new.size()) - 1;
    for (int i = 0; i < N; ++i) {
        auto shifted = cyclic_shift(res_new, res_sdvig, i);
        cout << corr(res, shifted) / N << endl;
    }

    return 0;
}
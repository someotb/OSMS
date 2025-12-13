#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

void output3x3(double corr_ab, double corr_ac, double corr_cb) {
    printf("      a      b     c\n");
    printf("a    -   %6.2f%6.2f\n", corr_ab, corr_ac);
    printf("b %6.2f   -   %6.2f\n", corr_ab, corr_cb);
    printf("c %6.2f %6.2f   -   \n", corr_ac, corr_cb);
    cout << endl;
}

int corr(vector<double> a, vector<double> b) {
    int sum = 0;
    for (int i = 0; i < (int)a.size(); ++i) {
        sum += a[i] * b[i];
    }

    return sum;
}

double corr_norm(vector<double> a, vector<double> b) {
    double xnyn = 0;
    double a2 = 0;
    double b2 = 0;
    double sum = 0;

    for (int i = 0; i < (int)a.size(); ++i) {
        xnyn += (a[i] * b[i]);
    }
    for (int i = 0; i < (int)a.size(); ++i) {
        a2 += a[i] * a[i];
    }
    for (int i = 0; i < (int)a.size(); ++i) {
        b2 += b[i] * b[i];
    }

    int niz = pow(a2 * b2, 0.5);
    sum = xnyn/niz;

    return sum;
}
// Вариант 16
int main() {
    vector<double> a = {7.0, 3.0, 2.0, -2.0, -2.0, -4.0, 1.0, 5.0};
    vector<double> b = {2.0, 1.0, 5.0, 0.0, -2.0, -3.0, 2.0, 4.0};
    vector<double> c = {2.0, -1.0, 3.0, -9.0, -2.0, -8.0, 4.0, -1.0};

    double corr_ab = corr(a, b);
    double corr_ac = corr(a, c);
    double corr_cb = corr(c, b);

    double corr_norm_ab = corr_norm(a, b);
    double corr_norm_ac = corr_norm(a, c);
    double corr_norm_cb = corr_norm(c, b);

    cout << "Обычная кореляция: " << endl;
    output3x3(corr_ab, corr_ac, corr_cb);
    cout << "Нормализованная кореляция: " << endl;
    output3x3(corr_norm_ab, corr_norm_ac, corr_norm_cb);


    return 0;
}

#include<iostream>
#include <string>
#include <vector>


using namespace std;

void decimal_to_binary(int decimal, vector<int>& binary) {
    binary.clear();
    for (int i = 7; i >= 0; --i) {
        binary.push_back((decimal >> i) & 1);
    }
}

void fullname_to_binary(vector<int>& binary_string_fullname) {
    string tmp, input;
    vector<string> string_fullname;
    vector<int> binary_word;

    cout << "Enter your name and surname: ";
    getline(cin, input);

    for (char c : input) {
        int ascii = static_cast<unsigned char>(c);
        decimal_to_binary(ascii, binary_word);
        binary_string_fullname.insert(binary_string_fullname.end(), binary_word.begin(), binary_word.end());
    }
}

int main() {
    vector<int> bin_fullname;
    fullname_to_binary(bin_fullname);

    for (size_t i = 0; i < bin_fullname.size(); ++i) {
        if (i % 8 == 0) cout << "\n";
        cout << bin_fullname[i] << " ";
    }

    cout << "\n";
}

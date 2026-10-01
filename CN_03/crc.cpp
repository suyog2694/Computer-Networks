#include <iostream>
#include <string>
using namespace std;

int main() {
    string data, divisor;

    cout << "Enter data bits: ";
    cin >> data;

    cout << "Enter generator bits: ";
    cin >> divisor;

    int n = divisor.length();
    string temp = data;

    for (int i = 0; i < n - 1; i++) {
        temp += '0';
    }

    for (int i = 0; i <= temp.length() - n; i++) {
        if (temp[i] == '1') {
            for (int j = 0; j < n; j++) {
                if (temp[i + j] == divisor[j]) {
                    temp[i + j] = '0';
                }
                else {
                    temp[i + j] = '1';
                }
            }
        }
    }

    string crc = temp.substr(temp.length() - (n - 1));

    cout << "CRC bits: " << crc << endl;

    string transmitted = data + crc;

    cout << "Transmitted data: " << transmitted << endl;

    int error;
    cout << "Enter error position: ";
    cin >> error;

    if (error != 0) {
        if (transmitted[error - 1] == '1') {
            transmitted[error - 1] = '0';
        }
        else {
            transmitted[error - 1] = '1';
        }
    }

    cout << "Received data: " << transmitted << endl;

    string check = transmitted;

    for (int i = 0; i <= check.length() - n; i++) {
        if (check[i] == '1') {
            for (int j = 0; j < n; j++) {
                if (check[i + j] == divisor[j]) {
                    check[i + j] = '0';
                }
                else {
                    check[i + j] = '1';
                }
            }
        }
    }

    bool errorFound = false;

    for (int i = 0; i < check.length(); i++) {
        if (check[i] == '1') {
            errorFound = true;
            break;
        }
    }

    if (errorFound) {
        cout << "Error detected in received data" << endl;
    }
    else {
        cout << "No error detected" << endl;
    }

    return 0;
}
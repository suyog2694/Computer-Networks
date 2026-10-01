#include <iostream>
#include <string>
using namespace std;

int main() {
    string bits;

    cout << "Enter 4 data bits: ";
    cin >> bits;

    int arr[8];

    arr[3] = bits[0] - '0';
    arr[5] = bits[1] - '0';
    arr[6] = bits[2] - '0';
    arr[7] = bits[3] - '0';

    arr[1] = arr[3] ^ arr[5] ^ arr[7];
    arr[2] = arr[3] ^ arr[6] ^ arr[7];
    arr[4] = arr[5] ^ arr[6] ^ arr[7];

    cout << "7 bit code: ";

    for (int i = 1; i <= 7; i++) {
        cout << arr[i];
    }
    cout << endl;

    int pos;

    cout << "Enter position to insert error 1 TO 7: ";
    cin >> pos;

    if (pos != 0) {
        if (arr[pos] == 1) {
            arr[pos] = 0;
        }
        else {
            arr[pos] = 1;
        }
    }

    cout << "Data received: ";

    for (int i = 1; i <= 7; i++) {
        cout << arr[i];
    }

    cout << endl;

    // Checking the received data
    int parity1 = arr[1] ^ arr[3] ^ arr[5] ^ arr[7];
    int parity2 = arr[2] ^ arr[3] ^ arr[6] ^ arr[7];
    int parity4 = arr[4] ^ arr[5] ^ arr[6] ^ arr[7];

    int wrongBit = parity1 + parity2 * 2 + parity4 * 4;

    if (wrongBit == 0) {
        cout << "The received data is correct." << endl;
    }
    else {
        cout << "Error found at bit position: " << wrongBit << endl;

        if (arr[wrongBit] == 1) {
            arr[wrongBit] = 0;
        }
        else {
            arr[wrongBit] = 1;
        }

        cout << "Data after correction: ";
        for (int i = 1; i <= 7; i++) {
            cout << arr[i];
        }
    }

    return 0;
}
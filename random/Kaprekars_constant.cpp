#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <fstream> // for file output

using namespace std;

vector<int> intToVector(int num) {
    vector<int> digits;
    for (int i = 1000; i >= 1; i /= 10)
        digits.push_back((num / i) % 10);
    return digits;
}

int vectorToInt(const vector<int>& v) {
    return v[0] * 1000 + v[1] * 100 + v[2] * 10 + v[3];
}

bool allDigitsSame(int num) {
    int d = num % 10;
    for (int i = 0; i < 4; ++i) {
        if (num % 10 != d) return false;
        num /= 10;
    }
    return true;
}

int kaprekars() {
    vector<vector<int>> log; // each row: [original_number, steps_to_6174]

    for (int AttemptValue = 1000; AttemptValue <= 9999; ++AttemptValue) {
        if (allDigitsSame(AttemptValue)) continue;

        int result = AttemptValue;
        int steps = 0;

        while (result != 6174 && steps < 100) {
            vector<int> digits = intToVector(result);

            vector<int> asc = digits;
            sort(asc.begin(), asc.end());

            vector<int> desc = digits;
            sort(desc.begin(), desc.end(), greater<int>());

            int big = vectorToInt(desc);
            int small = vectorToInt(asc);
            result = big - small;

            steps++;
        }

        log.push_back({AttemptValue, steps});
    }

    // Write to CSV file
    ofstream csv("kaprekar_steps.csv");
    if (!csv) {
        cerr << "Error opening file for writing.\n";
        return 1;
    }

    csv << "Number,Steps\n";
    for (const auto& row : log) {
        csv << setw(4) << setfill('0') << row[0] << "," << row[1] << "\n";
    }

    csv.close();
    cout << "Data written to kaprekar_steps.csv\n";
    return 0;
}

int main() {
    kaprekars();
    return 0;
}

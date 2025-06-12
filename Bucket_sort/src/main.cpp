#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>

// Functie om het aantal cijfers in een getal te bepalen
template <typename T>
int getNumDigits(T num) {
    if constexpr (std::is_signed<T>::value) {
        num = std::abs(num);
    }
    int digits = 1;
    while (num >= 10) {
        num /= 10;
        digits++;
    }
    return digits;
}

// Distributie-pass: Getallen in buckets plaatsen
template <typename T>
std::array<std::vector<T>, 19> distributionPass(const std::vector<T>& lst, int index) {
    std::array<std::vector<T>, 19> buckets;  // -9 t/m 9 → 0 t/m 18

    for (T num : lst) {
        int digit = (num / static_cast<int>(pow(10, index))) % 10;
        int bucketIndex = digit + 9;  // Offset zodat -9 → 0 en 9 → 18

        buckets[bucketIndex].push_back(num);
    }

    return buckets;
}

// Gathering-pass: Getallen uit buckets terug in de lijst zetten
template <typename T>
std::vector<T> gatheringPass(const std::array<std::vector<T>, 19>& buckets) {
    std::vector<T> sortedList;
    
    for (const auto& bucket : buckets) {
        for (T num : bucket) {
            sortedList.push_back(num);
        }
    }

    return sortedList;
}

// Bucket Sort implementatie
template <typename T>
std::vector<T> bucketSort(std::vector<T> lst) {
    int maxDigits = 0;
    for (T num : lst) {
        maxDigits = std::max(maxDigits, getNumDigits(num));
    }

    for (int index = 0; index < maxDigits; index++) {
        std::array<std::vector<T>, 19> buckets = distributionPass(lst, index);
        lst = gatheringPass(buckets);
    }

    return lst;
}

// Functie om testcases uit te voeren en tijden te meten
template <typename T>
void runSortTest(const std::string& filename, T minVal, T maxVal, unsigned int step, unsigned int maxSize) {
    std::array<double, 1000> timings;
    srand(0);

    for (unsigned int i = step; i <= maxSize; i += step) {
        std::vector<T> lst;
        for (unsigned int j = 0; j < i; j++) {
            lst.push_back(rand() % (maxVal - minVal + 1) + minVal);
        }

        auto t1 = std::chrono::high_resolution_clock::now();
        for (unsigned int j = 0; j < 10; j++) {
            lst = bucketSort(lst);
        }
        auto t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

        timings[int(i / step)] = time_span.count() / 10;
    }

    std::ofstream myfile(filename);
    for (auto timing : timings) {
        myfile << timing << '\n';
    }
    myfile.close();
}

// Speciale testcase voor worst-case scenario
void runWorstCaseTest(const std::string& filename) {
    std::array<double, 1000> timings;

    for (unsigned int i = 10; i <= 10000; i += 10) {
        std::vector<long long int> lst;
        for (unsigned int j = 0; j < i; j++) {
            lst.push_back(static_cast<long long int>(pow(9, j - i)));
        }

        auto t1 = std::chrono::high_resolution_clock::now();
        for (unsigned int j = 0; j < 10; j++) {
            lst = bucketSort(lst);
        }
        auto t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);

        timings[int(i / 10)] = time_span.count() / 10;
    }

    std::ofstream myfile(filename);
    for (auto timing : timings) {
        myfile << timing << '\n';
    }
    myfile.close();
}

void runSortTest(const std::string& filename, int minVal, int maxVal, unsigned int step, unsigned int maxSize) {
    std::ofstream file(filename);
    file << "n,time\n"; // CSV-header

    srand(0); // Consistente random waarden

    for (unsigned int i = step; i <= maxSize; i += step) {
        std::vector<int> lst;
        for (unsigned int j = 0; j < i; j++) {
            lst.push_back(rand() % (maxVal - minVal + 1) + minVal);
        }

        auto start = std::chrono::high_resolution_clock::now();
        lst = bucketSort(lst);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed = end - start;
        file << i << "," << elapsed.count() << "\n"; // Opslaan als CSV
    }

    file.close();
}

int main() {
    runSortTest("bucketSortTimings.csv", -1000, 1000, 100, 10000);
    std::cout << "Timing data opgeslagen in bucketSortTimings.csv\n";
    return 0;
}
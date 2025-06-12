// ompArraySum.cpp - Parallel versie met OpenMP
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <omp.h>

using std::cout, std::endl;

typedef std::shared_ptr<const std::vector<double>> input_array;
typedef std::shared_ptr<std::vector<double>> temporary_array;

input_array readArray(const std::string& fileName);
double sumArray_parallel(input_array);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << endl << "*** Usage: ompArraySum <inputFile>" << endl << endl;
        exit(1);
    }

    input_array a = readArray(std::string(argv[1]));

    double start = omp_get_wtime();
    double sum = sumArray_parallel(a);
    double end = omp_get_wtime();

    cout << "The sum of the values in the input file '" << argv[1] << "' is " << sum << endl;
    cout << "Elapsed time = " << (end - start) << " sec" << endl;

    return 0;
}

input_array readArray(const std::string& fileName) {
    size_t howMany;
    unsigned int count;

    std::ifstream fin(fileName);
    if (!fin.is_open()) {
        cout << endl << "*** Unable to open input file '" << fileName << "'" << endl << endl;
        exit(1);
    }

    fin >> howMany;
    temporary_array tempA = std::make_shared<std::vector<double>>(howMany);

    for (count = 0; count < howMany; count++)
        fin >> tempA->at(count);

    fin.close();
    return tempA;
}

double sumArray_parallel(input_array a) {
    double result = 0.0;
    size_t n = a->size();

    #pragma omp parallel for reduction(+:result)
    for (size_t i = 0; i < n; i++) {
        result += a->at(i);
    }

    return result;
}

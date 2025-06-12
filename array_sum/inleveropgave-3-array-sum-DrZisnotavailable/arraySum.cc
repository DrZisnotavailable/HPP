/* arraySum.cc uses an array to sum the values in an input file,
 *  whose name is specified on the command-line.
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <omp.h> // OpenMP

using std::cout, std::endl;

// Type aliases for array smart pointers. Use input_array after reading data.
typedef std::shared_ptr<const std::vector<double>> input_array;
typedef std::shared_ptr<std::vector<double>> temporary_array;

input_array readArray(const std::string& fileName);
double sumArray(input_array);

int main(int argc, char *argv[]) {
    double sum;

    if (argc != 2) {
        cout << endl << "*** Usage: arraySum <inputFile>" << endl << endl;
        exit(1);
    }

    input_array a = readArray(std::string(argv[1]));

    double start = omp_get_wtime();
    sum = sumArray(a);
    double end = omp_get_wtime();

    cout << "Elapsed time = " << (end - start) << " sec" << endl;

    cout << "The sum of the values in the input file '" << argv[1]
      << "' is " << sum << endl;

    return 0;
}

/* readArray returns an array with values from a file.
 * Receive: fileName, a constant string reference.
 * Return: a finalised dynamically allocated array containing
 *          the values from fileName.
 */

input_array readArray(const std::string& fileName)
{
    size_t howMany;
    unsigned int count;

    std::ifstream fin(fileName);

    if (!fin.is_open()) {
        cout << endl << "*** Unable to open input file ''" << fileName
          << "'" << endl << endl;

        exit(1);
    }

    fin >> howMany;

    // Without const, as array needs to be filled here
    temporary_array tempA = std::make_shared<std::vector<double>>(howMany);

    for (count = 0; count < howMany; count++)
        fin >> tempA->at(count);

    fin.close();

    return tempA; // Return as input_array, i.e. const
}

/* sumArray sums the values in a vector of doubles.
 * Receive: a, a pointer to the vector.
 * Return: the sum of the values in the array.
 */

double sumArray(input_array a) {
    unsigned int i;
    unsigned int numValues = a->size();
    double result = 0.0;

    for (i = 0; i < numValues; i++) {
        result += a->at(i);
    }

    return result;
}

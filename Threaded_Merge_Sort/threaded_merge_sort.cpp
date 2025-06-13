// parallel_merge_sort.cpp
// Proof-of-Concept: parallel merge sort met std::thread
// Verbeterde variabelenamen en duidelijke structuur

#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <thread>
#include <vector>
#include <array>

#include <fstream>
#include <memory>
#include <vector>
#include <iostream>

typedef std::shared_ptr<const std::vector<int>> input_array;
typedef std::shared_ptr<std::vector<int>> temporary_array;

template<size_t N>
std::array<int, N> createRandomList(){
    std::array<int, N> lst;
    for(int j=0; j<N; j++){
        lst[j] = rand() % 1000;
    }
    return lst;
}

// Recursive parallel merge sort template
// - RandomIt: iterator type
// - max_threads: totaal maximaal toe te wijzen threads
// - active_threads: huidige hoeveelheid actieve threads (1 = main thread)

template <typename RandomIt>
void parallelMergeSort(RandomIt first, RandomIt last,
                       int max_threads, int active_threads) {
    auto length = std::distance(first, last);
    if (length <= 1)
        return;

    RandomIt middle = first + length / 2;

    // Bepaal of we een nieuwe thread mogen starten
    if (active_threads < max_threads) {
        // Sorteer rechterhelft in een nieuwe thread
        std::thread right_thread(
            parallelMergeSort<RandomIt>, middle, last,
            max_threads, active_threads * 2);

        // Sorteer linkerhelft in huidige thread
        parallelMergeSort<RandomIt>(first, middle,
                                   max_threads, active_threads * 2);

        // Wacht op rechter thread
        right_thread.join();
    } else {
        // Te veel threads, voer sequentieel beide helften uit
        parallelMergeSort<RandomIt>(first, middle, max_threads, active_threads);
        parallelMergeSort<RandomIt>(middle, last, max_threads, active_threads);
    }

    // Merge beide gesorteerde helften in-place
    std::inplace_merge(first, middle, last);
}

int main() {
    auto list = createRandomList<100000>();    const int runs = 10;
    const std::vector<int> thread_counts = {1, 2, 4, 8, 16};

    for (int threads : thread_counts) {
        auto start_time = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < runs; ++i) {
            auto temp = list; // reset originele data
            parallelMergeSort(temp.begin(), temp.end(), threads, 1);
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            end_time - start_time);
        std::cout << "Threads=" << threads
                  << "\tTime(us)=" << duration.count() << std::endl;
    }

    return 0;
}

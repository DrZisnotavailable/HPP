#include <iostream>
#include <vector>
#include <algorithm>

void merge_sort(std::vector<int>& xs) {
    int n = xs.size();
    int unit = 1;

    while (unit <= n) {
        for (int h = 0; h < n; h += unit * 2) {
            int l = h;
            int r = std::min(n, h + 2 * unit);
            int mid = h + unit;
            if (mid >= r) continue;  // niets om te mergen

            int p = l;
            int q = mid;

            while (p < mid && q < r) {
                if (xs[p] <= xs[q]) {
                    p++;
                } else {
                    int temp = xs[q];
                    for (int k = q; k > p; --k) {
                        xs[k] = xs[k - 1];
                    }
                    xs[p] = temp;

                    p++;
                    mid++;
                    q++;
                }
            }
        }
        unit *= 2;
    }
}

int main() {
    std::vector<int> xs = {5, 2, 4, 6, 1, 3, 2, 6};
    merge_sort(xs);

    for (int x : xs) {
        std::cout << x << " ";
    }
    std::cout << std::endl;

    return 0;
}
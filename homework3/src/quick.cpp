#include <iostream>
#include <algorithm> 
#include <chrono>
#include <climits>
#include <vector>
using namespace std;


template <class T>
T Median(T* a, int left, int right)
{
    int mid = (left + right) / 2;

    if (a[mid] < a[left])  swap(a[left], a[mid]);
    if (a[right] < a[left]) swap(a[left], a[right]);
    if (a[right] < a[mid]) swap(a[mid], a[right]);


    swap(a[left], a[mid]);
    return a[left];
}

template <class T>
void QuickSort(T* a, int left, int right)
{
    if (left < right)
    {
        T pivot = Median(a, left, right);

        int i = left,
            j = right + 1;

        do {
            do i++; while (a[i] < pivot);
            do j--; while (a[j] > pivot);
            if (i < j) swap(a[i], a[j]);
        } while (i < j);

        swap(a[left], a[j]);

        QuickSort(a, left, j - 1);
        QuickSort(a, j + 1, right);
    }
}

void printArray(int* a, int n)
{
    for (int i = 1; i <= n; i++)
        cout << a[i] << " ";
    cout << endl;
}
void Permute(int* a, int n)
{
    for (int i = n; i >= 2; i--)
    {
        int j = rand() % i + 1;
        swap(a[j], a[i]);
    }
}
int main()
{
    srand(42);
    int sizes[6] = { 500, 1000, 2000, 3000, 4000, 5000 };

    const int RUNS = 100;    
    const int TRIALS = 10;  

    for (int idx = 0; idx < 6; idx++)
    {
        int n = sizes[idx];

        //average-case
        long long sum_us = 0;
        for (int t = 0; t < RUNS; t++)
        {
            int* d = new int[n + 2];
            d[n + 1] = INT_MAX;        
            for (int k = 1; k <= n; k++) d[k] = k;
            Permute(d, n);

            auto start = chrono::high_resolution_clock::now();
            QuickSort(d, 1, n);
            auto end = chrono::high_resolution_clock::now();

            sum_us += (long long)chrono::duration<double, micro>(end - start).count();
            delete[] d;
        }
        cout << "n=" << n << " QuickSort average-case: " << (sum_us / RUNS) << " us\n";

        //worst-case
        double max_us = 0;
        vector<int> worst_perm(n + 2);

        for (int t = 0; t < TRIALS; t++)
        {
            int* d = new int[n + 2];
            d[n + 1] = INT_MAX;
            for (int k = 1; k <= n; k++) d[k] = k;
            Permute(d, n);

            auto start = chrono::high_resolution_clock::now();
            QuickSort(d, 1, n);
            auto end = chrono::high_resolution_clock::now();

            double us = chrono::duration<double, micro>(end - start).count();
            if (us > max_us) {
                max_us = us;
                for (int k = 1; k <= n; k++) worst_perm[k] = d[k];
            }
            delete[] d;
        }

        cout << "n=" << n << " QuickSort worst-case: " << max_us << " us\n";

        int preview = 10;
        cout << "\n";
    }

    return 0;
}

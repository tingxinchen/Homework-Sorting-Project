#include <iostream>
#include <algorithm>
#include <climits>
#include <chrono>
using namespace std;

void Adjust(int* a, const int root, const int n)
{
    int j;
    int e = a[root];

    for (j = 2 * root; j <= n; j *= 2)
    {
        if (j < n && a[j] < a[j + 1]) j++;
        if (e >= a[j]) break;
        a[j / 2] = a[j];
    }
    a[j / 2] = e;
}

void HeapSort(int* a, const int n)
{
    for (int i = n / 2; i >= 1; i--)
        Adjust(a, i, n);

    for (int i = n - 1; i >= 1; i--)
    {
        swap(a[1], a[i + 1]);
        Adjust(a, 1, i);
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
    int size[6] = { 500, 1000, 2000, 3000, 4000, 5000 };

    for (int i = 0; i < 6; i++)
    {
        int n = size[i];
        double maxTime = 0;

        // 跑 10 次取最大值（worst-case）
        for (int t = 0; t < 10; t++)
        {
            int* h = new int[n + 2];
            h[n + 1] = INT_MAX;
            for (int k = 1; k <= n; k++) h[k] = k;
            Permute(h, n);

            auto start = chrono::high_resolution_clock::now();
            HeapSort(h, n);
            auto end = chrono::high_resolution_clock::now();

            double t1 = chrono::duration<double, milli>(end - start).count();
            if (t1 > maxTime) maxTime = t1;

            delete[] h;
        }

        cout << "n=" << n << "  HeapSort worst-case: " << maxTime << " ms" << endl;
    }

    return 0;
}
#include <iostream>
#include <chrono>

using namespace std;

void insertionSort(int arr[], int n)
{
    // 從第二個元素開始
    for (int i = 1; i < n; i++)
    {
        // 儲存目前要的值
        int key = arr[i];

        // j 指向前一個元素
        int j = i - 1;

        // 將比key大的元素往右移
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }
}

int main()
{
    int sizes[] = { 500, 1000, 2000, 3000, 4000, 5000 };

    for (int s = 0; s < 6; s++)
    {
        int n = sizes[s];
        int* arr = new int[n];
        // 倒序的測資
        for (int i = 0; i < n; i++)
            arr[i] = n - i;

        auto start = chrono::high_resolution_clock::now();
        insertionSort(arr, n);
        auto end = chrono::high_resolution_clock::now();

        double ms = chrono::duration<double, milli>(end - start).count();

        cout << "n = " << n << " InsertionSort worst-case: " << ms << " ms" << endl;

        bool sorted = true;
        for (int i = 1; i < n; i++)
            if (arr[i - 1] > arr[i]) sorted = false;

        delete[] arr;
    }

    return 0;
}

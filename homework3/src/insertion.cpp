#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

//Insertion Sort 函式
void insertionSort(int arr[], int n)
{
    for (int i = 1; i < n; i++)
    {
        int key = arr[i];
        int j = i - 1;

        // 在最差狀況（完全逆序）下，這個 while 迴圈每一次都要退到最前面（比較次數最大化）
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

//主程式
int main()
{
    // 設定需要測試的 N 值目標
    vector<int> sizes = { 500, 1000, 2000, 3000, 4000, 5000 };
    vector<long long> res_worst_time;

    for (int n : sizes) {
        //使用 dynamic array 配置記憶體，預防大資料時 stack overflow
        int* arr = new int[n];

        //產生 Worst Case 資料（從 n 倒數到 1 的完全逆序數列）
        for (int i = 0; i < n; i++)
        {
            arr[i] = n - i;
        }

        //記錄排序開始時間
        auto start = chrono::high_resolution_clock::now();

        //呼叫 Insertion Sort
        insertionSort(arr, n);

        //記錄排序結束時間
        auto end = chrono::high_resolution_clock::now();

        //計算並記錄微秒 (us) 數據
        res_worst_time.push_back(chrono::duration_cast<chrono::microseconds>(end - start).count());

        //釋放動態記憶體
        delete[] arr;
    }

    //輸出實驗數據結果
    cout << "--- Insertion Sort Worst Case Runtimes (us) ---\n";
    cout << "N Size:   500,  1000,  2000,  3000,  4000,  5000\n";
    cout << "Runtime:  "
        << res_worst_time[0] << ", "
        << res_worst_time[1] << ", "
        << res_worst_time[2] << ", "
        << res_worst_time[3] << ", "
        << res_worst_time[4] << ", "
        << res_worst_time[5] << " us\n";

    return 0;
}

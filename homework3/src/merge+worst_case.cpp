#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

//  Merge Sort
void merge(int arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];

    for (int i = 0; i < n2; i++)
        R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void iterativeMergeSort(int arr[], int n)
{
    for (int size = 1; size < n; size *= 2)
    {
        for (int left = 0; left < n - 1; left += 2 * size)
        {
            int mid = min(left + size - 1, n - 1);
            int right = min(left + 2 * size - 1, n - 1);

            if (mid < right)
                merge(arr, left, mid, right);
        }
    }
}

// 2. Merge Sort 最差狀況
void generateWorstCase(vector<int>& arr, int left, int right)
{
    if (right - left <= 1)
        return;

    vector<int> temp;

    // 將奇數索引與偶數索引分開，逆推出會讓 Merge 執行最多比較次數的排列
    for (int i = left; i < right; i += 2)
        temp.push_back(arr[i]);

    for (int i = left + 1; i < right; i += 2)
        temp.push_back(arr[i]);

    for (int i = left; i < right; i++)
        arr[i] = temp[i - left];

    int mid = (left + right) / 2;

    generateWorstCase(arr, left, mid);
    generateWorstCase(arr, mid, right);
}

//主程式 
int main()
{
    // 設定需要測試的 N 值目標
    vector<int> sizes = { 500, 1000, 2000, 3000, 4000, 5000 };
    vector<long long> res_worst_time;

    for (int n : sizes) {
        // 1. 建立 1 ~ N 的初始依序數列
        vector<int> worstCase;
        for (int i = 1; i <= n; i++)
            worstCase.push_back(i);

        // 2. 透過逆推演算法，打亂成 Merge Sort 的最差狀況排列
        generateWorstCase(worstCase, 0, n);

        // 3. 將資料轉移至原生動態陣列
        int* arr = new int[n];
        for (int i = 0; i < n; i++)
            arr[i] = worstCase[i];

        // 4. 精確計時這組最差狀況資料的排序耗時
        auto start = chrono::high_resolution_clock::now();

        iterativeMergeSort(arr, n);

        auto end = chrono::high_resolution_clock::now();

        // 5. 記錄微秒 (us) 數據
        res_worst_time.push_back(chrono::duration_cast<chrono::microseconds>(end - start).count());

        delete[] arr;
    }

    //輸出實驗數據結果
    cout << "--- Merge Sort Worst Case Runtimes (us) ---\n";
    cout << "N Size:   500,  1000,  2000,  3000,  4000,  5000\n";
    cout << "Runtime:  "
        << res_worst_time[0] << ", "
        << res_worst_time[1] << ", "
        << res_worst_time[2] << ", "
        << res_worst_time[3] << ", "
        << res_worst_time[4] << ", "
        << res_worst_time[5];

    return 0;
}

#include <iostream>
#include <vector>
using namespace std;


// Merge Function
void merge(int arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1;

    int n2 = right - mid;

    // 建立暫存陣列
    int* L = new int[n1];
    int* R = new int[n2];

    // 複製左半部資料
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];

    // 複製右半部資料
    for (int i = 0; i < n2; i++)
        R[i] = arr[mid + 1 + i];

    // i -> 左半部索引 j -> 右半部索引 k -> 原陣列索引
    int i = 0, j = 0, k = left;

    // 比較左右元素並合併
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    // 左半部剩餘元素
    while (i < n1)
        arr[k++] = L[i++];

    // 右半部剩餘元素
    while (j < n2)
        arr[k++] = R[j++];

    // 釋放記憶體
    delete[] L;
    delete[] R;
}

// Iterative Merge Sort
void iterativeMergeSort(int arr[], int n)
{
    // size 為每次合併區塊大小
    for (int size = 1; size < n; size *= 2)
    {
        // 每次處理兩個區塊
        for (int left = 0; left < n - 1; left += 2 * size)
        {
            // 中間位置
            int mid = min(left + size - 1, n - 1);

            // 右邊界
            int right = min(left + 2 * size - 1, n - 1);

            // 若可合併則進行 merge
            if (mid < right)
                merge(arr, left, mid, right);
        }
    }
}

// Merge Sort Worst Case Generator
void generateWorstCase(vector<int>& arr, int left, int right)
{
    // 若區間大小 <= 1 則停止
    if (right - left <= 1)
        return;

    // 暫存陣列
    vector<int> temp;

    // 先放偶數位置元素
    for (int i = left; i < right; i += 2)
        temp.push_back(arr[i]);

    // 再放奇數位置元素
    for (int i = left + 1; i < right; i += 2)
        temp.push_back(arr[i]);

    // 複製回原陣列
    for (int i = left; i < right; i++)
        arr[i] = temp[i - left];

    // 中間位置
    int mid = (left + right) / 2;

    // 遞迴處理左半部
    generateWorstCase(arr, left, mid);

    // 遞迴處理右半部
    generateWorstCase(arr, mid, right);
}

int main()
{
    // 建立 Worst Case 測資

    int n = 16;

    vector<int> worstCase;

    // 放入 1 ~ n
    for (int i = 1; i <= n; i++)
        worstCase.push_back(i);

    // 產生 Merge Sort Worst Case
    generateWorstCase(worstCase, 0, n);

    cout << "Worst Case Data:\n";

    for (int x : worstCase)
        cout << x << " ";

    cout << "\n\n";

    // 將 vector 轉成陣列

    int* arr = new int[n];

    for (int i = 0; i < n; i++)
        arr[i] = worstCase[i];

    // 執行 Iterative Merge Sort

    iterativeMergeSort(arr, n);

    cout << "After Merge Sort:\n";

    for (int i = 0; i < n; i++)
        cout << arr[i] << " ";


    delete[] arr;

    return 0;
}